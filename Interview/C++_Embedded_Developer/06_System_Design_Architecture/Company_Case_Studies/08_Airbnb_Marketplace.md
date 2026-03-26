# Airbnb Marketplace Platform

## Overview
Airbnb: 7M+ listings across 220+ countries, 150M+ active users, handling booking, payments, and trust/safety at global scale.

## Key Requirements
- **Scale**: 7M+ listings, 150M+ users
- **Bookings**: 2M+ reservations/day
- **Latency**: <100ms search, <50ms booking confirmation
- **Payments**: Multi-currency, multiple payment methods
- **Trust**: Verification, reviews, ratings system
- **Availability**: 99.9% uptime SLA
- **Content**: 10+ photos per listing, high-quality images

---

## High-Level Architecture

```
┌────────────────────────────────────────────────────────────────┐
│              Airbnb Marketplace Architecture                   │
└────────────────────────────────────────────────────────────────┘

CLIENT LAYER:
┌────────────────────────────────────────────────────────────────┐
│ Web Browser / Mobile App / Host Dashboard                      │
│ - Search listings, filter by criteria                          │
│ - Book stays, manage payments                                  │
│ - Host listing management                                      │
└────────────────────────────────────────────────────────────────┘
            │
            ▼
┌────────────────────────────────────────────────────────────────┐
│ Global Load Balancer                                           │
│ - Geo-routing to nearest region                                │
│ - Rate limiting, DDoS protection                               │
└────────────────────────────────────────────────────────────────┘
            │
    ┌───────┼───────┬────────┬────────┐
    ▼       ▼       ▼        ▼        ▼
┌──────────────────────────────────────────────┐
│ Microservices Layer                          │
├──────────────────────────────────────────────┤
│                                              │
│ ┌──────────────────┐  ┌──────────────────┐  │
│ │Search Service    │  │Listing Service   │  │
│ │- Search listings │  │- Create/Update   │  │
│ │- Geospatial      │  │- Photo mgmt      │  │
│ │- Filters & sort  │  │- Pricing rules   │  │
│ │- Recommendations │  │- Availability    │  │
│ └──────────────────┘  └──────────────────┘  │
│                                              │
│ ┌──────────────────┐  ┌──────────────────┐  │
│ │Booking Service   │  │Payment Service   │  │
│ │- Reserve dates   │  │- Process payment │  │
│ │- Instant book    │  │- Multi-currency  │  │
│ │- Flexible cancel │  │- Multiple methods│  │
│ │- Hold period     │  │- Payout to hosts │  │
│ └──────────────────┘  └──────────────────┘  │
│                                              │
│ ┌──────────────────┐  ┌──────────────────┐  │
│ │Review Service    │  │Trust & Safety    │  │
│ │- Post reviews    │  │- Host/Guest ver  │  │
│ │- Ratings system  │  │- ID verification │  │
│ │- Host response   │  │- Fraud detection │  │
│ │- Review moderation│ │- Dispute handling │  │
│ └──────────────────┘  └──────────────────┘  │
│                                              │
│ ┌──────────────────┐  ┌──────────────────┐  │
│ │Messaging Service │  │Notification Svc  │  │
│ │- Host-guest chat │  │- Booking updates │  │
│ │- Real-time msg   │  │- Review reminders│  │
│ │- Auto-responses  │  │- Price alerts    │  │
│ └──────────────────┘  └──────────────────┘  │
│                                              │
└──────────────────────────────────────────────┘

DATA LAYER:
┌────────────────────────────────────────────────────────────────┐
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐   │
│  │Listing DB      │  │Booking DB      │  │Search Index    │   │
│  │(PostgreSQL)    │  │(DynamoDB)      │  │(Elasticsearch) │   │
│  │- 7M listings   │  │- Reservations  │  │- Searchable    │   │
│  │- Host info     │  │- Dates held    │  │  listings      │   │
│  │- Amenities     │  │- Payment info  │  │- Geospatial    │   │
│  │- Pricing       │  │- Guests        │  │  queries       │   │
│  └────────────────┘  └────────────────┘  └────────────────┘   │
│                                                                │
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐   │
│  │Review DB       │  │Image Storage   │  │Cache Layer     │   │
│  │(DynamoDB)      │  │(S3 + CDN)      │  │(Redis)         │   │
│  │- Reviews data  │  │- 10+ photos    │  │- Listing cache │   │
│  │- Ratings       │  │  per listing   │  │- User sessions │   │
│  │- Verified      │  │- Thumbnails    │  │- Search results│   │
│  └────────────────┘  └────────────────┘  └────────────────┘   │
│                                                                │
│  ┌────────────────┐  ┌────────────────┐                       │
│  │Analytics DW    │  │Message Queue   │                       │
│  │(BigQuery)      │  │(Kafka)         │                       │
│  │- Booking trends│  │- Booking events│                       │
│  │- Host revenue  │  │- Payment evt   │                       │
│  │- User behavior │  │- Review events │                       │
│  └────────────────┘  └────────────────┘                       │
│                                                                │
└────────────────────────────────────────────────────────────────┘
```

---

## Search & Discovery Flow

```
AIRBNB SEARCH PROCESS:
══════════════════════

User searches: "Paris, France" for March 15-20

1. QUERY PARSING:
   ┌───────────────────────────────────┐
   │ Extract parameters:               │
   │ - Location: Paris                 │
   │ - Check-in: Mar 15                │
   │ - Check-out: Mar 20               │
   │ - Guests: 2 adults                │
   │ - Room type: Entire home          │
   │ - Price range: ₹3000-10000/night  │
   └───────────────────────────────────┘

2. AVAILABILITY CHECK:
   ┌───────────────────────────────────┐
   │ Query Booking DB                  │
   │ - Find available dates            │
   │ - Filter booked dates             │
   │ - Check instant-book policy       │
   │ - Exclude host blackouts          │
   │ Result: ~15,000 available         │
   └───────────────────────────────────┘

3. GEOSPATIAL SEARCH:
   ┌───────────────────────────────────┐
   │ Elasticsearch Geo-Query           │
   │ - Center: Paris (lat/long)        │
   │ - Radius: configurable (5-50km)   │
   │ - Locate listings in region       │
   │ - Result: ~8,000 listings         │
   └───────────────────────────────────┘

4. FILTERING:
   ┌───────────────────────────────────┐
   │ Apply user filters                │
   │ - Amenities (WiFi, Pool, etc)     │
   │ - Room type (Entire/Private room) │
   │ - Accommodates (2+ guests)        │
   │ - Host reviews (4.5+ stars)       │
   │ - Superhost status (optional)     │
   │ - Price range                     │
   │ Result: ~3,000 candidates         │
   └───────────────────────────────────┘

5. RANKING:
   ┌───────────────────────────────────┐
   │ ML Ranking Model                  │
   │ Factors:                          │
   │ - Relevance (query match)         │
   │ - Host rating/reviews             │
   │ - Booking count (popularity)      │
   │ - Price (medium preferred)        │
   │ - Response time                   │
   │ - Photos quality                  │
   │ - User click history              │
   │ Result: Top 30 recommendations    │
   └───────────────────────────────────┘

6. HYDRATION:
   ┌───────────────────────────────────┐
   │ Fetch full data:                  │
   │ - Listing details                 │
   │ - Host info (verified check)      │
   │ - Reviews summary                 │
   │ - Photo thumbnails                │
   │ - Availability calendar           │
   │ - Pricing breakdown               │
   └───────────────────────────────────┘

7. RESPONSE:
   ┌───────────────────────────────────┐
   │ Display results in ~80-150ms      │
   │ - List of 30 listings             │
   │ - Map view with pins              │
   │ - Apply A/B testing               │
   │ - Track impressions               │
   └───────────────────────────────────┘
```

---

## Booking Flow

```
AIRBNB BOOKING PROCESS:
═══════════════════════

User clicks "Book": Luxury Apt in Paris

1. BOOKING REQUEST:
   ┌───────────────────────────────────┐
   │ Create booking request            │
   │ - Listing ID                      │
   │ - Check-in/out dates              │
   │ - Guest count                     │
   │ - Guest info (name, email, phone) │
   └───────────────────────────────────┘

2. VERIFICATION:
   ┌───────────────────────────────────┐
   │ Pre-booking checks                │
   │ - Verify guest account (age, ID)  │
   │ - Verify host account             │
   │ - Check for fraud flags           │
   │ - Dates still available?          │
   │ - Guest meets host requirements?  │
   └───────────────────────────────────┘

3. HOLD DATES:
   ┌───────────────────────────────────┐
   │ Lock dates in Booking DB          │
   │ - Set hold period: 10 minutes     │
   │ - Mark as "pending_payment"       │
   │ - If hold expires: release dates  │
   └───────────────────────────────────┘

4. CALCULATE PRICING:
   ┌───────────────────────────────────┐
   │ Breakdown:                        │
   │ - Base: ₹5000/night × 5 = ₹25000 │
   │ - Cleaning fee: ₹1000             │
   │ - Service fee (3%): ₹780          │
   │ - Local taxes (10%): ₹2680        │
   │ - Discount code: -₹1000           │
   │ - Total: ₹28460                   │
   │ - Host gets: ₹24200 (after cut)  │
   └───────────────────────────────────┘

5. PAYMENT PROCESSING:
   ┌───────────────────────────────────┐
   │ Process payment                   │
   │ - Support credit card             │
   │ - Support UPI/Digital wallets     │
   │ - Support PayPal                  │
   │ - Currency conversion             │
   │ - Fraud detection                 │
   │ - 3D secure if needed             │
   └───────────────────────────────────┘
                │
        ┌───────┴───────┐
        ▼               ▼
    Success        Failure
    │              │
    ▼              ▼
    │         Release hold,
    │         try another card

6. BOOKING CONFIRMATION:
   ┌───────────────────────────────────┐
   │ Finalize booking                  │
   │ - Create DynamoDB record          │
   │ - Generate booking ID             │
   │ - Send confirmation email         │
   │ - Notify host (instant or req)    │
   │ - Update calendar (auto)          │
   │ - Publish booking event (Kafka)   │
   └───────────────────────────────────┘

7. COMMUNICATIONS:
   ┌───────────────────────────────────┐
   │ Messaging Service                 │
   │ - Pre-filled welcome msg from host│
   │ - Guest can ask questions         │
   │ - Host can set expectations       │
   │ - Auto-translate messages         │
   └───────────────────────────────────┘
```

---

## Key Technologies

| Component | Tech | Purpose |
|-----------|------|---------|
| **Search** | Elasticsearch | Listing search |
| **Geospatial** | PostGIS | Location queries |
| **Listings** | PostgreSQL | Persistent storage |
| **Bookings** | DynamoDB | Reservation data |
| **Photos** | S3 + CloudFront | Image delivery |
| **Cache** | Redis | Session/listing cache |
| **Messaging** | Kafka | Event streaming |
| **Analytics** | BigQuery | Data warehouse |
| **ML** | TensorFlow | Search ranking |

---

## Scalability Mechanisms

1. **Search Scaling**
   - Elasticsearch sharding by region
   - Caching popular searches
   - Geospatial indexing optimization

2. **Booking Concurrency**
   - Lock-free date reservation
   - DynamoDB strong consistency
   - Hold period management

3. **Content Delivery**
   - CDN for images (10+ per listing)
   - Image optimization & thumbnails
   - Lazy loading on client

4. **Multi-region**
   - Regional booking databases
   - Data replication for consistency
   - Regional search indexes

---

## Interview Questions

### Q1: Design Airbnb search system
**Key points:**
- Geospatial indexing for location
- Availability filtering
- ML-based ranking
- Caching strategy
- Scaling to millions of listings

### Q2: Implement booking system with concurrent reservations
**Key points:**
- Date locking mechanism
- Payment integration
- Atomicity guarantees
- Hold period management
- Overbooking prevention

### Q3: Handle international payments & currencies
**Key points:**
- Multi-currency support
- Payment gateway integration
- Host payout mechanism
- Transaction fees
- Currency conversion rates

