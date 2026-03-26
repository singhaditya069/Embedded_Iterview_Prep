# IRCTC Indian Railways Booking System

## Overview
IRCTC: Indian Railways ticket booking platform, 50+ million users, peaks 100K+ concurrent users during booking, handles massive load during ticket releases.

## Key Requirements
- **Scale**: 50M+ users, 100K+ peak concurrent
- **Bookings**: 1M+ tickets/day during peaks
- **Latency**: <2 seconds for seat search
- **Availability**: High uptime during peak hours
- **Payment**: Multiple payment methods (UPI, Credit, Debit, Net-banking)
- **Seat Inventory**: Real-time seat availability
- **Scalability**: Handle 10x traffic spikes

---

## High-Level Architecture

```
┌──────────────────────────────────────────────────────────────┐
│         IRCTC Railway Booking System Architecture            │
└──────────────────────────────────────────────────────────────┘

CLIENT LAYER:
┌──────────────────────────────────────────────────────────────┐
│ Web App / Mobile / IRCTC Partner APIs                        │
│ - Desktop browser                                            │
│ - Android/iOS app                                            │
│ - OTA integration (MakeMyTrip, Cleartrip)                    │
└──────────────────────────────────────────────────────────────┘
            │
            ▼
┌──────────────────────────────────────────────────────────────┐
│ Global Load Balancer (Geo-routing)                           │
│ - Route to nearest regional data center                      │
│ - DDoS protection                                            │
│ - Rate limiting (200+ req/min per user)                      │
└──────────────────────────────────────────────────────────────┘
            │
        ┌───┴───────────┬──────────┐
        ▼               ▼          ▼
    ┌─────────────┐  ┌──────────┐  ┌────────────┐
    │Search Svc   │  │Booking   │  │Payment     │
    │- Trains     │  │Svc       │  │Svc         │
    │- Routes     │  │- Reserve │  │- HDFC      │
    │- Availability│ │- Confirm │  │- ICICI     │
    └─────────────┘  └──────────┘  └────────────┘

MICROSERVICES LAYER:
┌──────────────────────────────────────────────────────────────┐
│                                                              │
│ ┌────────────────────────────────────────────────┐          │
│ │ Train Search & Inventory Service               │          │
│ │ - Train schedule data (10K+ trains)            │          │
│ │ - Route information                            │          │
│ │ - Coach composition (15 coaches/train)         │          │
│ │ - Real-time seat availability                  │          │
│ └────────────────────────────────────────────────┘          │
│                                                              │
│ ┌────────────────────────────────────────────────┐          │
│ │ Booking Service                                │          │
│ │ - Reserve seats (60 second hold)               │          │
│ │ - Create provisional bookings                  │          │
│ │ - Generate TDS (Ticket Dispensing System)      │          │
│ │ - Confirm bookings                             │          │
│ └────────────────────────────────────────────────┘          │
│                                                              │
│ ┌────────────────────────────────────────────────┐          │
│ │ Payment Integration Service                    │          │
│ │ - Multiple gateway integration                 │          │
│ │ - UPI, Credit/Debit, Net-banking               │          │
│ │ - Fraud detection                              │          │
│ │ - Amount reconciliation                        │          │
│ └────────────────────────────────────────────────┘          │
│                                                              │
│ ┌────────────────────────────────────────────────┐          │
│ │ PNR & Ticket Management                        │          │
│ │ - Generate PNR (Passenger Name Record)         │          │
│ │ - Store ticket details                         │          │
│ │ - SMS/Email notifications                      │          │
│ │ - Ticket download (e-ticket)                   │          │
│ └────────────────────────────────────────────────┘          │
│                                                              │
│ ┌────────────────────────────────────────────────┐          │
│ │ Queue & Waitlist Management                    │          │
│ │ - Virtual queue system                         │          │
│ │ - Auto-upgrade from waitlist                   │          │
│ │ - Waitlist confirmation batching               │          │
│ └────────────────────────────────────────────────┘          │
│                                                              │
└──────────────────────────────────────────────────────────────┘

CACHING & STORAGE:
┌──────────────────────────────────────────────────────────────┐
│  ┌──────────────────┐  ┌──────────────────┐                 │
│  │Redis Cache       │  │MySQL DB (Seats)  │                 │
│  │- Train schedules │  │- 10K+ trains     │                 │
│  │- Seat inventory  │  │- Each: ~1000     │                 │
│  │- User sessions   │  │  seats            │                 │
│  │- Queue state     │  │- Real-time state  │                 │
│  └──────────────────┘  └──────────────────┘                 │
│                                                              │
│  ┌──────────────────┐  ┌──────────────────┐                 │
│  │Message Queue     │  │Booking DB        │                 │
│  │(RabbitMQ/Kafka)  │  │- Confirmations   │                 │
│  │- Booking events  │  │- PNR records     │                 │
│  │- Payment events  │  │- Waitlists       │                 │
│  │- Asynctasks      │  │- Transaction log  │                 │
│  └──────────────────┘  └──────────────────┘                 │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

---

## Seat Booking Flow

```
SEAT BOOKING PROCESS:
═════════════════════

User selects: Delhi → Mumbai, Date: 2024-02-14

1. TRAIN SEARCH:
   ┌────────────────────────────┐
   │ Query Redis Cache          │
   │ - Get train schedules      │
   │ - Available trains: 50+    │
   │ - Response: <100ms         │
   └────────────────────────────┘

2. AVAILABILITY CHECK:
   ┌────────────────────────────┐
   │ For each train:            │
   │ - Query seat inventory     │
   │ - Available seats count    │
   │ - Coach-wise breakdown     │
   │ - Fare calculation         │
   └────────────────────────────┘

3. USER ENTERS QUEUE:
   ┌────────────────────────────┐
   │ Virtual Queue Management   │
   │ - Assign queue position    │
   │ - Show position: #5234     │
   │ - User waits (~2-5 mins)   │
   │ - Priority: first-come     │
   └────────────────────────────┘

4. ENTER SEAT SELECTION:
   ┌────────────────────────────┐
   │ Seat Map Generation        │
   │ - Query current layout     │
   │ - 1A, 1B, 1C, ... (15/row) │
   │ - Green = Available        │
   │ - Red = Booked             │
   │ - Yellow = Being booked    │
   │ - 60 second hold on select │
   └────────────────────────────┘

5. SEAT RESERVATION:
   ┌────────────────────────────┐
   │ Book Seat to Cache         │
   │ - Mark in Redis: "hold"    │
   │ - 60 second timeout        │
   │ - Row lock (no conflicts)  │
   └────────────────────────────┘
                │
                ▼
   ┌────────────────────────────┐
   │ Reserve in MySQL           │
   │ - Lock row                 │
   │ - Insert holding record    │
   │ - TTL: 60 seconds          │
   │ - Free if not confirmed    │
   └────────────────────────────┘

6. PROCEED TO PAYMENT:
   ┌────────────────────────────┐
   │ Payment Service            │
   │ - Verify PAN/Email         │
   │ - Show amount: ~₹1200      │
   │ - Available gateways       │
   └────────────────────────────┘
                │
        ┌───────┼───────┐
        ▼       ▼       ▼
    HDFC    ICICI   Airtel
    Pay     Pay     Payments

   ┌────────────────────────────┐
   │ Payment Verification       │
   │ - Receive callback         │
   │ - Mark as Success/Failed   │
   └────────────────────────────┘

7. BOOKING CONFIRMATION:
   ┌────────────────────────────┐
   │ Confirm Booking            │
   │ - Remove hold              │
   │ - Insert confirmed record  │
   │ - Generate PNR: "1234567"  │
   │ - Seat: "S1-52A"           │
   │ - Transaction ID logged    │
   └────────────────────────────┘

8. SEND NOTIFICATIONS:
   ┌────────────────────────────┐
   │ Async Tasks (via Kafka)    │
   │ - SMS: PNR + details       │
   │ - Email: Receipt + TDS     │
   │ - E-ticket generation      │
   │ - Analytics update         │
   └────────────────────────────┘

Total time: 120-180 seconds
```

---

## Handling Peak Load (Ticket Release)

```
PEAK TRAFFIC MANAGEMENT:
════════════════════════

Normal traffic:        1000 QPS
Peak traffic:        10,000+ QPS (10x spike)

Strategy:

1. QUEUING SYSTEM:
   ┌──────────────────────────────────────┐
   │ Virtual Queue                        │
   │ - Limit concurrent users: 5000       │
   │ - Queue waiting: 50,000 additional   │
   │ - Serve FIFO                         │
   │ - Position update every 10 seconds   │
   └──────────────────────────────────────┘

2. RATE LIMITING:
   ┌──────────────────────────────────────┐
   │ Per user:                            │
   │ - Max 200 req/min                    │
   │ - Max 1 booking per 5 min            │
   │ - Prevent bot traffic                │
   │                                      │
   │ Per IP:                              │
   │ - Max 500 req/min                    │
   │ - Detect attack patterns             │
   └──────────────────────────────────────┘

3. DISTRIBUTED CACHING:
   ┌──────────────────────────────────────┐
   │ Replicate seat inventory:            │
   │ - 3-way replication in Redis         │
   │ - Eventual consistency acceptable    │
   │ - 99.9% accuracy on availability     │
   │ - Reconcile every 1 second           │
   └──────────────────────────────────────┘

4. DATABASE OPTIMIZATION:
   ┌──────────────────────────────────────┐
   │ MySQL Seat Tables:                   │
   │ - Sharded by train_id                │
   │ - ~50 shards                         │
   │ - Row-level locking                  │
   │ - Read replicas for searches         │
   │ - Write to master only               │
   └──────────────────────────────────────┘

5. GRACEFUL DEGRADATION:
   ┌──────────────────────────────────────┐
   │ When max capacity reached:           │
   │ - Stop new user entry                │
   │ - Serve existing queue               │
   │ - Show "Come back in 2 hours"        │
   │ - Prevent cascade failure            │
   └──────────────────────────────────────┘

6. HORIZONTAL SCALING:
   ┌──────────────────────────────────────┐
   │ Auto-scale components:               │
   │ - API servers: 100 → 500             │
   │ - Payment workers: 50 → 200          │
   │ - Redis replicas: 5 → 20             │
   │ - MySQL replicas: 10 → 30            │
   └──────────────────────────────────────┘
```

---

## Key Technologies

| Component | Tech | Purpose |
|-----------|------|---------|
| **Web Server** | Nginx, Apache | Request handling |
| **Cache** | Redis | Seat inventory, sessions |
| **Database** | MySQL | Persistent storage |
| **Message Queue** | Kafka, RabbitMQ | Async tasks |
| **Payment** | Multiple gateways | Transaction processing |
| **Search** | Elasticsearch | Train search |
| **Monitoring** | New Relic, Datadog | Observability |
| **Queue Mgmt** | Custom service | Virtual queuing |

---

## Reliability Mechanisms

1. **Seat Booking Consistency**
   - Row-level locking in MySQL
   - 60-second hold period
   - Auto-release on timeout

2. **Payment Reconciliation**
   - Idempotent payment APIs
   - Callback reconciliation
   - Manual verification for failed txns

3. **Booking Confirmation**
   - Transactional guarantees
   - PNR generation after payment confirmation
   - Backup PNR generation on retry

4. **Disaster Recovery**
   - Multi-region deployment
   - RTO: 5 minutes
   - RPO: 1 minute (Kafka replication)

---

## Interview Questions

### Q1: Design seat inventory management system
**Key points:**
- Cache vs database consistency
- Handling 100K concurrent users
- Seat locking mechanism
- Timeout handling
- Race condition prevention

### Q2: Implement booking queue system
**Key points:**
- Virtual queue priority
- FIFO ordering
- Position estimation
- Dynamic queue management
- Handle user drop-off

### Q3: Scale to handle 10x peak traffic
**Key points:**
- Rate limiting strategy
- Horizontal scaling
- Database sharding
- Cache replication
- Graceful degradation

