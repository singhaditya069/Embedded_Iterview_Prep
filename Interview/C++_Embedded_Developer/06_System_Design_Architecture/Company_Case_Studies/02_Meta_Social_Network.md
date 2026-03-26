# Meta (Facebook) Social Network Architecture

## Overview
Meta operates the world's largest social network with ~3 billion monthly active users, real-time messaging, and photo/video sharing at massive scale.

## Key Requirements
- **Scale**: 3B+ users, billions of posts/day
- **Latency**: <100ms for feed updates, <50ms for messages
- **Consistency**: Eventually consistent architecture
- **Throughput**: Millions of requests/second
- **Real-time**: Instant notifications, live updates
- **Storage**: Exabytes of data

---

## High-Level Architecture

```
┌────────────────────────────────────────────────────────────────────┐
│                    Meta System Architecture                        │
└────────────────────────────────────────────────────────────────────┘

┌────────────────────────────────────────────────────────────────────┐
│                        CLIENT LAYER                                │
│  ┌──────────────┐  ┌──────────────┐  ┌──────────────┐             │
│  │ Web Browser  │  │ Mobile App   │  │ Messenger    │             │
│  │  (React)     │  │ (iOS/Android)│  │  (WhatsApp)  │             │
│  └──────────────┘  └──────────────┘  └──────────────┘             │
└────────────────────────────────────────────────────────────────────┘
                            │
                            │ GraphQL/REST
                            ▼
┌────────────────────────────────────────────────────────────────────┐
│                   API LAYER & LOAD BALANCER                        │
│  ┌─────────────────────────────────────────────────────────────┐  │
│  │ Request routing, authentication, rate limiting              │  │
│  │ GraphQL query optimization, batching                        │  │
│  └─────────────────────────────────────────────────────────────┘  │
└────────────────────────────────────────────────────────────────────┘
        │          │         │         │          │
        ▼          ▼         ▼         ▼          ▼
    ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐
    │  Feed  │ │Messages│ │Profile │ │Notifi- │ │Search &│
    │Service │ │Service │ │Service │ │cations │ │Discovery│
    └────────┘ └────────┘ └────────┘ └────────┘ └────────┘

┌────────────────────────────────────────────────────────────────────┐
│                    MICROSERVICES LAYER                             │
│                                                                   │
│  ┌───────────────────────────────────────────────────────────┐   │
│  │ Feed Generation Service                                   │   │
│  │ - Fetch user's friends' posts                             │   │
│  │ - ML-based ranking algorithm                              │   │
│  │ - Personalized timeline                                   │   │
│  │ - Real-time with eventual consistency                     │   │
│  └───────────────────────────────────────────────────────────┘   │
│                                                                   │
│  ┌───────────────────────────────────────────────────────────┐   │
│  │ Post Storage & Retrieval                                  │   │
│  │ - Async writes to database                                │   │
│  │ - Distributed post storage                                │   │
│  │ - Versioning system                                       │   │
│  └───────────────────────────────────────────────────────────┘   │
│                                                                   │
│  ┌───────────────────────────────────────────────────────────┐   │
│  │ Messaging Service                                         │   │
│  │ - Real-time message delivery                              │   │
│  │ - WebSocket connections                                   │   │
│  │ - Message consistency                                     │   │
│  │ - Push notifications                                      │   │
│  └───────────────────────────────────────────────────────────┘   │
│                                                                   │
│  ┌───────────────────────────────────────────────────────────┐   │
│  │ Notification Service                                      │   │
│  │ - Likes, comments, mentions                               │   │
│  │ - Push notifications                                      │   │
│  │ - Notification preferences                                │   │
│  └───────────────────────────────────────────────────────────┘   │
└────────────────────────────────────────────────────────────────────┘
        │          │         │         │          │
        ▼          ▼         ▼         ▼          ▼
┌────────────────────────────────────────────────────────────────────┐
│                    DATA LAYER                                      │
│  ┌─────────────────┐  ┌──────────────┐  ┌─────────────────┐       │
│  │ Primary DB      │  │ Cache Layer  │  │ Message Queue   │       │
│  │ (MySQL)         │  │ (Memcached)  │  │ (Kafka)         │       │
│  │ - User profiles │  │ - Feed cache │  │ - Events        │       │
│  │ - Posts         │  │ - User data  │  │ - Notifications │       │
│  │ - Comments      │  │ - Hot data   │  │ - Analytics     │       │
│  └─────────────────┘  └──────────────┘  └─────────────────┘       │
│                                                                   │
│  ┌─────────────────┐  ┌──────────────┐  ┌─────────────────┐       │
│  │ Graph DB        │  │ Search Index │  │ Object Storage  │       │
│  │ (TAO/Memcached) │  │ (Lucene,     │  │ (S3)            │       │
│  │ - Relationships │  │  Sphinx)     │  │ - Photos        │       │
│  │ - Social graph  │  │ - People     │  │ - Videos        │       │
│  │ - Billions of   │  │ - Groups     │  │ - Other objects │       │
│  │  edges          │  │ - Pages      │  │                 │       │
│  └─────────────────┘  └──────────────┘  └─────────────────┘       │
│                                                                   │
│  ┌─────────────────┐  ┌──────────────────────────────────┐       │
│  │ Real-time DB    │  │ Analytics & DW                   │       │
│  │ (MySQL, Mongo)  │  │ (Presto, Hive, Spark)           │       │
│  │ - Messages      │  │ - User behavior analytics        │       │
│  │ - Sessions      │  │ - Engagement metrics             │       │
│  │ - Presence info │  │ - Recommendations training       │       │
│  └─────────────────┘  └──────────────────────────────────┘       │
└────────────────────────────────────────────────────────────────────┘
```

---

## Data Flow: Posting & Feed Generation

```
POSTING FLOW:
═════════════

User creates post (photo + caption + tags):
    │
    ▼
┌─────────────────────────────────┐
│ Upload Service                  │
│ - Store photo in S3             │
│ - Generate thumbnails           │
│ - Process image (tags, OCR)     │
└─────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────┐
│ Post Storage Service            │
│ - Insert into MySQL             │
│ - Async job to index            │
│ - Invalidate user's feed cache  │
└─────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────┐
│ Kafka Event: Post Created       │
│ - User ID, post ID, timestamp   │
└─────────────────────────────────┘
    │
    ├─────────┬──────────┬──────────┐
    ▼         ▼          ▼          ▼
Notification  Search    Analytics  Graph DB
Index update  Index     Pipeline   Updates
Async         Async     Batch      Async


FEED GENERATION FLOW:
═════════════════════

User requests feed:
    │
    ▼
┌─────────────────────────────────┐
│ Feed Service API                │
│ - User ID, timestamp            │
└─────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────┐
│ Check Cache (Memcached)         │
│ - Cached personalized feed      │
│ - TTL: 30 minutes               │
└─────────────────────────────────┘
    │
    ├─ HIT ──────┐
    │            │
    │            ▼ (Return cached)
    │       Client
    │
    └─ MISS ──────┐
                  ▼
        ┌──────────────────────────┐
        │ Social Graph Lookup (TAO)│
        │ - Get user's friends list│
        │ - ~5K friends per user   │
        └──────────────────────────┘
                  │
                  ▼
        ┌──────────────────────────┐
        │ Fetch Recent Posts       │
        │ - From friends' posts    │
        │ - Candidate set ~10K     │
        └──────────────────────────┘
                  │
                  ▼
        ┌──────────────────────────┐
        │ ML Ranking Model         │
        │ - Relevance scoring      │
        │ - Engagement prediction  │
        │ - Diversity algorithms   │
        │ - Top 30 posts selected  │
        └──────────────────────────┘
                  │
                  ▼
        ┌──────────────────────────┐
        │ Hydrate Posts            │
        │ - Get full post data     │
        │ - Fetch metadata         │
        │ - Get author info        │
        └──────────────────────────┘
                  │
                  ▼
        ┌──────────────────────────┐
        │ Cache Result             │
        │ - Store in Memcached     │
        │ - TTL: 30 minutes        │
        └──────────────────────────┘
                  │
                  ▼
              Client
```

---

## Real-Time Messaging Architecture

```
┌────────────────────────────────────────────────┐
│ Messenger Real-Time System                     │
└────────────────────────────────────────────────┘

┌────────────────────────────────────────────────┐
│ Client A                                       │
│ WebSocket Connection (Thrift)                  │
└────────────────────────────────────────────────┘
            │
            │ Long-lived connection
            ▼
┌────────────────────────────────────────────────┐
│ Connection Manager                             │
│ - Maintains WebSocket connections              │
│ - Routes messages to right service             │
└────────────────────────────────────────────────┘
            │
            ▼
┌────────────────────────────────────────────────┐
│ Message Service                                │
│ - Receives message                             │
│ - Stores in database                           │
│ - Emits event to Kafka                         │
└────────────────────────────────────────────────┘
            │
            ├─────────────────┬──────────────────┐
            ▼                 ▼                  ▼
    ┌────────────────┐ ┌────────────────┐ ┌────────────────┐
    │ Kafka Event    │ │ Real-time DB   │ │ Notification   │
    │ - Publish msg  │ │ - Store msg    │ │ Service        │
    │ - Other subs   │ │ - Presence     │ │ - Push notif   │
    └────────────────┘ └────────────────┘ └────────────────┘
            │
            ▼
    ┌────────────────┐
    │ Client B       │
    │ WebSocket Conn │
    └────────────────┘
            │
            ▼
        Receives message
        Updates UI in <100ms
```

---

## Architecture Design Patterns

### 1. Caching Strategy (Multi-Layer)

```
Request
  │
  ├─ L1: CDN Layer (Global edge caches)
  │  - Static assets, images
  │  - TTL: 1 hour
  │
  ├─ L2: Memcached Tier (Regional)
  │  - Feed cache, user data
  │  - TTL: 30 minutes
  │  - Millions of QPS
  │
  ├─ L3: Local In-Process Cache
  │  - Hot data, frequently accessed
  │  - TTL: Few minutes
  │
  └─ L4: Database
     – MySQL, primary storage
```

### 2. Eventual Consistency Model

```
Write Operations:
┌──────────────┐
│ Accept write │ ─────> Return success to client (fast)
└──────────────┘
       │
       ├──────> Synchronously write to primary DB
       │
       └──────> Async job queue
              ├─ Update cache
              ├─ Update search indexes
              ├─ Update graph database
              └─ Publish events
```

### 3. Service Communication

```
GraphQL API:
  │
  ├─ Query: Get user feed
  │  - Batches multiple requests
  │  - Sends single optimized query
  │
  ├─ Mutation: Create post
  │  - Transactional
  │  - Returns new post ID
  │
  └─ Subscription: Real-time updates
     - WebSocket maintained
     - Server pushes changes
```

---

## Key Technologies

| Layer | Tech | Purpose |
|-------|------|---------|
| **Frontend** | React, GraphQL | Client-side rendering |
| **API** | GraphQL, Thrift | Query language, RPC |
| **Messaging** | Kafka | Event streaming |
| **Cache** | Memcached, Redis | Multi-tier caching |
| **DB** | MySQL | Relational data |
| **Graph DB** | TAO, HBase | Social graph storage |
| **Search** | Lucene, Sphinx | Full-text search |
| **RT DB** | HBase, Cassandra | Real-time data |
| **Storage** | S3 | Object storage |
| **ML** | TensorFlow, Spark | Recommendations |
| **Monitoring** | ODS, Scuba | Analytics & debugging |

---

## Scalability Mechanisms

1. **Database Sharding**
   - Shard by user ID
   - Billions of users distributed
   - Read replicas for scale

2. **Cache Invalidation**
   - TTL-based expiration
   - Event-based invalidation
   - Lazy loading on miss

3. **Feed Ranking**
   - Candidate retrieval (fast)
   - ML ranking (accurate)
   - Diversity algorithms

4. **Connection Management**
   - Connection pooling
   - Long-lived WebSockets
   - Graceful reconnection

---

## Interview Questions

### Q1: Design Facebook's news feed
**Key considerations:**
- Generate personalized feed for billions
- Real-time post updates
- ML ranking algorithm
- Handle dynamic friend lists
- Cache strategy for hot users

### Q2: Implement real-time messaging
**Key considerations:**
- WebSocket persistence
- Message ordering guarantee
- Offline message storage
- Delivery confirmation
- Scaling to billions of messages

### Q3: Handle 3 billion users
**Key considerations:**
- Database sharding strategy
- Consistency models
- Regional deployment
- Disaster recovery
- Cost optimization

