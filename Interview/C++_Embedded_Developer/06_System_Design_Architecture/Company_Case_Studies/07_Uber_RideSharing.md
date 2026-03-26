# Uber Ride-Sharing Platform

## Overview
Uber: Operating in 70+ countries, serving millions of riders/drivers, real-time location tracking, dynamic pricing, handling 20M+ trips/day.

## Key Requirements
- **Scale**: 20M+ active drivers, 100M+ riders
- **Orders**: 20M+ trips/day globally
- **Real-time**: Sub-second location updates
- **Matching**: Match driver to rider in seconds
- **Pricing**: Dynamic pricing based on demand/supply
- **Reliability**: 99.95% uptime SLA
- **Latency**: <10 second ETA, real-time tracking

---

## High-Level Architecture

```
┌──────────────────────────────────────────────────────────────────┐
│                 Uber Ride-Sharing Platform                       │
└──────────────────────────────────────────────────────────────────┘

CLIENT LAYER:
┌──────────────────────────────────────────────────────────────────┐
│ Rider App / Driver App / Web                                     │
│ - Real-time location tracking                                    │
│ - Push notifications                                             │
│ - Trip management                                                │
└──────────────────────────────────────────────────────────────────┘
            │
            ▼
┌──────────────────────────────────────────────────────────────────┐
│ API Gateway & Load Balancer                                      │
│ - Geo-routing to nearest region                                  │
│ - Rate limiting per user                                         │
│ - Authentication                                                 │
└──────────────────────────────────────────────────────────────────┘
            │
    ┌───────┼───────┬───────┬───────┐
    ▼       ▼       ▼       ▼       ▼
┌─────────────────────────────────────────────────┐
│ Microservices Layer                             │
├─────────────────────────────────────────────────┤
│                                                 │
│ ┌──────────────┐  ┌──────────────┐             │
│ │Rider Request │  │Driver  Avail │             │
│ │Service       │  │Service       │             │
│ │- Create req  │  │- Location    │             │
│ │- Update      │  │- Status      │             │
│ │- Cancel      │  │- Availability│             │
│ └──────────────┘  └──────────────┘             │
│                                                 │
│ ┌──────────────┐  ┌──────────────┐             │
│ │Location & GIS│  │Matching      │             │
│ │Service       │  │Engine        │             │
│ │- Real-time   │  │- Driver-Rider│             │
│ │  tracking    │  │  matching    │             │
│ │- Geohashing  │  │- Optimal     │             │
│ │- Map service │  │  assignment  │             │
│ └──────────────┘  └──────────────┘             │
│                                                 │
│ ┌──────────────┐  ┌──────────────┐             │
│ │Trip Service  │  │Pricing Svc   │             │
│ │- Track trip  │  │- Calculate   │             │
│ │- Payment     │  │  dynamic fare│             │
│ │- History     │  │- Surge pricing           │
│ │- Ratings     │  │- Promo codes │             │
│ └──────────────┘  └──────────────┘             │
│                                                 │
│ ┌──────────────┐  ┌──────────────┐             │
│ │Notification  │  │Analytics     │             │
│ │Service       │  │Service       │             │
│ │- Real-time   │  │- Trip metrics│             │
│ │  updates     │  │- User events │             │
│ │- Push notifs │  │- Performance │             │
│ └──────────────┘  └──────────────┘             │
│                                                 │
└─────────────────────────────────────────────────┘

DATA LAYER:
┌──────────────────────────────────────────────────────────────────┐
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐     │
│  │Location Cache  │  │Trip Database   │  │Pricing Service │     │
│  │(Redis)         │  │(DynamoDB)      │  │(Real-time)     │     │
│  │- Driver locs   │  │- Active trips  │  │- Surge calc    │     │
│  │- Rider locs    │  │- Trip history  │  │- Base fare     │     │
│  │- GeoHashes     │  │- Payment info  │  │- Distance-based│     │
│  └────────────────┘  └────────────────┘  └────────────────┘     │
│                                                                  │
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐     │
│  │Message Queue   │  │Analytics DW    │  │Graph DB        │     │
│  │(Kafka)         │  │(BigQuery)      │  │(Neo4j)         │     │
│  │- Trip events   │  │- Trip analytics│  │- Driver graph  │     │
│  │- Payment evt   │  │- User behavior │  │- Relationships │     │
│  │- Async tasks   │  │- Revenue       │  │                │     │
│  └────────────────┘  └────────────────┘  └────────────────┘     │
└──────────────────────────────────────────────────────────────────┘
```

---

## Ride Request Matching Flow

```
UBER MATCHING ALGORITHM:
════════════════════════

1. RIDER REQUESTS RIDE:
   ┌───────────────────────────────────┐
   │ Rider sends search request        │
   │ - Pickup location (GPS)           │
   │ - Destination (if known)          │
   │ - Desired service (Uber X/Plus)   │
   └───────────────────────────────────┘

2. LOCATION SERVICE (GeoHashing):
   ┌───────────────────────────────────┐
   │ Convert lat/long to geohash       │
   │ - Pickup location: "ezs42"        │
   │ - Nearby geohashes (500m radius)  │
   │ - Query available drivers         │
   │ Result: ~50-200 nearby drivers    │
   └───────────────────────────────────┘

3. RANKING CANDIDATES:
   ┌───────────────────────────────────┐
   │ Filter & Score Drivers            │
   │ Factors:                          │
   │ - Distance (closer is better)     │
   │ - Driver ETA to pickup            │
   │ - Driver rating (minimum 4.6*)    │
   │ - Vehicle type match              │
   │ - Driver acceptance rate          │
   │ - Cancel rate                     │
   │ - Time on app (experienced)       │
   │                                   │
   │ Top candidates: 10-20 drivers     │
   └───────────────────────────────────┘

4. SEND OFFER TO DRIVERS (Parallel):
   ┌───────────────────────────────────┐
   │ Send trip offer to top 10 drivers │
   │ - Trip ID                         │
   │ - Pickup location                 │
   │ - Dropoff location                │
   │ - Estimated fare                  │
   │ - Start time: NOW                 │
   │                                   │
   │ Timeout: 60 seconds               │
   └───────────────────────────────────┘
   
   ┌─────────────────────────────────────┐
   │ Race between drivers                │
   │ - First to accept wins              │
   │ - Send push notification + alert sound │
   │ - Others: "Trip no longer available" │
   └─────────────────────────────────────┘

5. DRIVER ACCEPTED:
   ┌───────────────────────────────────┐
   │ Match Confirmed                   │
   │ - Lock driver to trip             │
   │ - Store in DynamoDB               │
   │ - Calculate ETA (Google Maps)     │
   │ - Notify rider: driver info, ETA  │
   │ - Notify driver: rider info, pickup
   └───────────────────────────────────┘

6. TRIP IN PROGRESS:
   ┌───────────────────────────────────┐
   │ Real-time Updates                 │
   │ Driver location updates:          │
   │ - Every 2-5 seconds               │
   │ - Via Kafka/WebSocket             │
   │ - Rider sees live tracking        │
   │ - Google Maps API for routing     │
   │ - ETA continuously updated        │
   └───────────────────────────────────┘

7. TRIP COMPLETION:
   ┌───────────────────────────────────┐
   │ End Trip                          │
   │ - Calculate actual distance       │
   │ - Calculate fare                  │
   │ - Process payment                 │
   │ - Request ratings                 │
   │ - Store trip history              │
   │ - Update analytics                │
   └───────────────────────────────────┘
```

---

## Real-Time Location Tracking

```
LOCATION UPDATE FLOW:
═════════════════════

Driver App every 2-5 seconds:
    │
    ▼
┌──────────────────────────────────┐
│ Location Service                 │
│ - Receive lat/long + timestamp   │
│ - Validate (not too fast/slow)   │
│ - Convert to geohash             │
└──────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────┐
│ Redis Cache Update               │
│ - Key: "driver:12345:location"   │
│ - Value: {lat, long, ts}         │
│ - TTL: 30 seconds                │
└──────────────────────────────────┘
    │
    ├─────────────────┬──────────────┐
    ▼                 ▼              ▼
 Rider App        Analytics     Matching
 Gets Update      Pipeline      Service
 (WebSocket)      (Kafka)       Updates
    │
    ▼
Rider sees driver on map
moving in real-time
```

---

## Dynamic Pricing Engine

```
SURGE PRICING CALCULATION:
═══════════════════════════

Real-time Metrics:
┌────────────────────────────────────┐
│ For each city/zone (every minute): │
│ - Active riders: 50,000            │
│ - Active drivers: 8,500            │
│ - Demand rate: 300 req/min         │
│ - Supply rate: 250 accept/min      │
│ - Wait time: 8 minutes (median)    │
│ - Acceptance rate: 83%             │
└────────────────────────────────────┘

Surge Ratio Calculation:
┌────────────────────────────────────┐
│ Demand / Supply Ratio              │
│                                    │
│ Ratio = Pending Requests /         │
│         Available Drivers          │
│                                    │
│ Ratio 1.0 = No surge              │
│ Ratio 1.5 = 50% surge             │
│ Ratio 2.0 = 100% surge (2x)       │
│ Ratio 3.0+ = 3x or more surge     │
│ Max cap: 4x surge pricing          │
└────────────────────────────────────┘

Update Frequency:
┌────────────────────────────────────┐
│ Every 1-5 minutes (event driven)   │
│ - Supply/demand change             │
│ - External events (weather, etc)   │
│ - Time of day factoring            │
└────────────────────────────────────┘

Example Calculation:
Base Fare: ₹40
Distance: 5 km @ ₹8/km = ₹40
Surge: 1.5x (₹120 * 1.5) = ₹180
Final Fare: ₹40 + ₹40 + ₹40 surge = ₹120
```

---

## Key Technologies

| Component | Tech | Purpose |
|-----------|------|---------|
| **Location** | Geohashing | Spatial indexing |
| **Matching** | Custom algo | Driver-rider matching |
| **Mapping** | Google Maps API | Navigation, ETA |
| **Real-time** | WebSocket, Kafka | Location updates |
| **Database** | DynamoDB, Cassandra | Trip storage |
| **Cache** | Redis | Driver locations |
| **Analytics** | Kafka, BigQuery | Metrics |
| **Notification** | FCM, APNS | Push notifications |

---

## Scalability Mechanisms

1. **Location Indexing**
   - Geohashing for spatial queries
   - Redis cache for hot data
   - Update every 2-5 seconds

2. **Matching Algorithm**
   - Parallel scoring
   - Top 10 candidates
   - Fast decision (60s timeout)

3. **Real-time Processing**
   - WebSocket for bi-directional updates
   - Kafka for asynchronous events
   - Low-latency notification delivery

4. **Pricing Calculation**
   - Aggregate metrics per zone
   - Update every 1-5 minutes
   - Applied at request time

---

## Interview Questions

### Q1: Design ride-sharing matching algorithm
**Key points:**
- Geospatial indexing for nearby drivers
- Ranking criteria (distance, rating, ETA)
- Concurrency handling
- Fallback strategy if no drivers available

### Q2: Implement real-time location tracking
**Key points:**
- Location update frequency
- WebSocket vs polling
- Cache invalidation
- Moving map overlay

### Q3: Build dynamic pricing system
**Key points:**
- Demand/supply metrics
- Surge ratio calculation
- Update frequency
- Market fairness

