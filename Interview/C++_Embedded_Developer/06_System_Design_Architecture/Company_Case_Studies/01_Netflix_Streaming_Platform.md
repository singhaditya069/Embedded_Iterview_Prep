# Netflix System Design - Streaming Platform Architecture

## Overview
Netflix is one of the world's largest streaming platforms serving ~250 million subscribers globally with low-latency video streaming.

## Key Requirements
- **Scalability**: Handle millions of concurrent viewers
- **Latency**: <100ms latency for video playback
- **Availability**: 99.99% uptime SLA
- **Global Distribution**: Serve content from nearest region
- **Quality**: Adaptive bitrate streaming based on bandwidth
- **Analytics**: Track user behavior, recommendations in real-time

---

## Architecture Overview

```
┌─────────────────────────────────────────────────────────────────────┐
│                        Netflix System Architecture                   │
└─────────────────────────────────────────────────────────────────────┘

┌──────────────────────────────────────────────────────────────────────┐
│                         CLIENT LAYER                                 │
│  ┌─────────────┐  ┌────────────────┐  ┌──────────────┐              │
│  │   Web App   │  │  Mobile Client │  │  Smart TV    │              │
│  │  (React)    │  │  (iOS/Android) │  │  (Netflix OS)│              │
│  └─────────────┘  └────────────────┘  └──────────────┘              │
└──────────────────────────────────────────────────────────────────────┘
                              │
                              │ HTTPS
                              ▼
┌──────────────────────────────────────────────────────────────────────┐
│                      CDN LAYER (CloudFront)                          │
│  ┌──────────────────────────────────────────────────────────────┐   │
│  │  Edge Locations (300+ globally)                              │   │
│  │  - Caches popular content                                    │   │
│  │  - Reduces latency to <50ms                                  │   │
│  │  - Serves 95% of traffic                                     │   │
│  └──────────────────────────────────────────────────────────────┘   │
└──────────────────────────────────────────────────────────────────────┘
                              │
                              │
                              ▼
┌──────────────────────────────────────────────────────────────────────┐
│                    API GATEWAY & LOAD BALANCER                       │
│  ┌────────────────────────────────────────────────────────────────┐  │
│  │  Route requests to appropriate microservices                   │  │
│  │  Rate limiting, authentication, request validation            │  │
│  └────────────────────────────────────────────────────────────────┘  │
└──────────────────────────────────────────────────────────────────────┘
        │          │         │         │          │         │
        ▼          ▼         ▼         ▼          ▼         ▼
    ┌────────┐ ┌──────┐ ┌────────┐ ┌────────┐ ┌────────┐ ┌────────┐
    │ User   │ │Video │ │Search &│ │Payment │ │Content │ │Play-  │
    │Service │ │Service│Recommend│ │Service │ │Manager │ │back   │
    └────────┘ └──────┘ └────────┘ └────────┘ └────────┘ └────────┘

┌──────────────────────────────────────────────────────────────────────┐
│                    MICROSERVICES LAYER                               │
│                                                                      │
│  ┌────────────────────┐  ┌────────────────────────┐                 │
│  │ Video Streaming    │  │ User Recommendation   │                 │
│  │ - Adaptive bitrate │  │ - ML-based rec engine  │                 │
│  │ - DRM protection   │  │ - Real-time analytics  │                 │
│  │ - Quality metrics  │  │ - A/B testing          │                 │
│  └────────────────────┘  └────────────────────────┘                 │
│                                                                      │
│  ┌────────────────────┐  ┌────────────────────────┐                 │
│  │ Authentication     │  │ Billing & Payments     │                 │
│  │ - OAuth 2.0        │  │ - Subscription mgmt    │                 │
│  │ - JWT tokens       │  │ - Multi-currency       │                 │
│  │ - Device mgmt      │  │ - Fraud detection      │                 │
│  └────────────────────┘  └────────────────────────┘                 │
└──────────────────────────────────────────────────────────────────────┘
        │          │          │           │          │
        ▼          ▼          ▼           ▼          ▼
┌──────────────────────────────────────────────────────────────────────┐
│                    DATA LAYER                                        │
│  ┌────────────────┐  ┌──────────────┐  ┌────────────────┐           │
│  │ Primary DB     │  │ Cache Layer  │  │ Message Queue  │           │
│  │ (MySQL/Psql)   │  │ (Redis,      │  │ (Kafka)        │           │
│  │ - User data    │  │  Memcached)  │  │ - Events       │           │
│  │ - Subscriptions│  │ - Sessions   │  │ - Analytics    │           │
│  │ - Watch history│  │ - Hot data   │  │ - Playback logs│           │
│  └────────────────┘  └──────────────┘  └────────────────┘           │
│                                                                      │
│  ┌────────────────┐  ┌──────────────┐  ┌────────────────┐           │
│  │ Analytics DB   │  │ Search Index │  │ Object Storage │           │
│  │ (ClickHouse,   │  │ (Elasticsearch) │ (S3)          │           │
│  │  Parquet)      │  │ - Catalog    │  │ - Video files  │           │
│  │ - Viewing data │  │ - Metadata   │  │ - Thumbnails   │           │
│  │ - User actions │  │ - Full-text  │  │ - Artwork      │           │
│  └────────────────┘  └──────────────┘  └────────────────┘           │
└──────────────────────────────────────────────────────────────────────┘
```

---

## Component Deep Dive

### 1. Video Streaming Service

```
Video Streaming Flow:

┌─────────────────────────────────────────────────────┐
│  User Requests Video (Stream)                       │
└─────────────────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────┐
│  Playback Service                                   │
│  - Gets user's device profile                       │
│  - Determines available bitrates                    │
│  - Returns manifest with stream URLs                │
└─────────────────────────────────────────────────────┘
                    │
                    ▼
┌─────────────────────────────────────────────────────┐
│  Adaptive Bitrate Selection                         │
│  Algorithm: ABSL (Adaptive Bitrate Streaming Logic) │
│  - Monitor network bandwidth                        │
│  - Monitor device CPU/memory                        │
│  - Adjust stream quality dynamically                │
│  - Minimize rebuffering                             │
└─────────────────────────────────────────────────────┘
                    │
        ┌───────────┼───────────┐
        ▼           ▼           ▼
    4K Stream  720p Stream 480p Stream
   (25 Mbps)  (5 Mbps)    (2.5 Mbps)
        │           │           │
        └───────────┼───────────┘
                    ▼
        ┌──────────────────────────┐
        │  CDN Selection           │
        │  - Geo-location aware    │
        │  - Latency-based routing │
        │  - Load balancing        │
        └──────────────────────────┘
                    │
                    ▼
        ┌──────────────────────────┐
        │  Streaming Delivery      │
        │  - HTTP/2 or HTTP/3      │
        │  - DRM encryption (Widevine)
        │  - Manifest updates      │
        └──────────────────────────┘
```

### 2. Recommendation Engine

```
Recommendation System Architecture:

┌────────────────────────────────────────────────────┐
│  User Watch History & Behavior                     │
│  - Watch time, completion rate                     │
│  - Search queries, ratings                         │
│  - Device type, time of day                        │
└────────────────────────────────────────────────────┘
        │
        ▼
┌────────────────────────────────────────────────────┐
│  Real-time Feature Processing                      │
│  - Apache Spark / Flink                            │
│  - Extract features from events                    │
│  - Update user profiles                            │
│  - ~1 billion features per day                     │
└────────────────────────────────────────────────────┘
        │
        ▼
┌────────────────────────────────────────────────────┐
│  ML Models (Offline Training)                      │
│  ┌──────────────────────────────────────┐          │
│  │ Matrix Factorization / Collaborative │          │
│  │ Filtering model                      │          │
│  │ - Trained on historical data         │          │
│  │ - ~10B parameters                    │          │
│  │ - Updated daily                      │          │
│  └──────────────────────────────────────┘          │
│  ┌──────────────────────────────────────┐          │
│  │ Deep Learning Model (RNN/Transformer)│          │
│  │ - Sequence prediction                │          │
│  │ - Context-aware ranking              │          │
│  └──────────────────────────────────────┘          │
│  ┌──────────────────────────────────────┐          │
│  │ Content-based Filtering              │          │
│  │ - Metadata similarity                │          │
│  │ - Genre, cast, director              │          │
│  └──────────────────────────────────────┘          │
└────────────────────────────────────────────────────┘
        │
        ▼
┌────────────────────────────────────────────────────┐
│  Online Inference Service                          │
│  - Real-time prediction                            │
│  - <50ms latency requirement                       │
│  - Serving millions of QPS                         │
│  - Model serving platform (TensorFlow Serving)     │
└────────────────────────────────────────────────────┘
        │
        ▼
┌────────────────────────────────────────────────────┐
│  Home Page Personalization                         │
│  - Row-based recommendations                       │
│  - Rows: Trending, Similar to watched, etc.        │
│  - A/B testing different models                    │
│  - Ranking & diversity algorithms                  │
└────────────────────────────────────────────────────┘
```

---

## Data Flow Diagram

```
┌──────────────┐
│ User Device  │
└──────┬───────┘
       │ 1. Request stream
       ▼
┌──────────────────────────────────────┐
│ CDN / API Gateway                    │
├──────────────────────────────────────┤
│ 2. Route to nearest edge location    │
└──────┬───────────────────────────────┘
       │ 3. Manifest request
       ▼
┌──────────────────────────────────────┐
│ Playback Service (Microservice)      │
├──────────────────────────────────────┤
│ 4. Check licensing, DRM              │
│ 5. Query Redis for device state      │
│ 6. Return manifest with URLs         │
└──────┬───────────────────────────────┘
       │ 7. Manifest response
       │ 8. Start fetching video chunks
       ▼
┌──────────────────────────────────────┐
│ CDN Edge Location                    │
├──────────────────────────────────────┤
│ 9. Serve cached chunks OR            │
│ 10. Fetch from origin (other CDN)    │
└──────┬───────────────────────────────┘
       │ 11. Video chunks
       ▼
┌──────────────────────────────────────┐
│ Client Video Player                  │
├──────────────────────────────────────┤
│ 12. Adjust bitrate based on:         │
│     - Available bandwidth            │
│     - CPU load                       │
│     - Rebuffering events             │
│ 13. Emit telemetry events            │
└──────┬───────────────────────────────┘
       │ 14. Quality metrics, events
       ▼
┌──────────────────────────────────────┐
│ Kafka Event Stream                   │
├──────────────────────────────────────┤
│ 15. Real-time analytics pipeline     │
│ 16. Clickhouse for analytics         │
│ 17. ML feature generation            │
└──────────────────────────────────────┘
```

---

## Key Technologies Used

| Component | Technology | Purpose |
|-----------|-----------|---------|
| **CDN** | CloudFront | Content delivery at 300+ edge locations |
| **Streaming** | HTTP/2, HTTP/3 | Low-latency delivery |
| **Cache** | Redis, Memcached | Session, catalog, hot data |
| **Database** | MySQL, PostgreSQL | User, subscription data |
| **Analytics** | Kafka, Spark, ClickHouse | Real-time event processing |
| **Search** | Elasticsearch | Catalog search & metadata |
| **Storage** | S3 | Video, metadata storage |
| **ML** | TensorFlow, Spark MLlib | Recommendations |
| **Messaging** | Kafka | Event streaming |
| **Monitoring** | Prometheus, Grafana | System observability |
| **Containers** | Kubernetes, Docker | Microservice orchestration |

---

## Scalability Considerations

### Horizontal Scaling
- **Stateless services**: Multiple instances of playback service
- **Database sharding**: User data sharded by region
- **CDN edge caching**: Reduces origin traffic by 95%+
- **Microservice instances**: Auto-scaling based on load

### Performance Optimizations
1. **Connection pooling**: Reuse DB connections
2. **Caching layers**: Multi-tier caching (CDN → Redis → DB)
3. **Asynchronous processing**: Events via Kafka
4. **Batch operations**: Analytics in batches
5. **Query optimization**: Minimized DB round trips

### High Availability
- **Multi-region deployment**: Active-active setup
- **Failover mechanisms**: Automatic failover on outage
- **Redundancy**: No single point of failure
- **Circuit breakers**: Prevent cascade failures
- **Health checks**: Continuous monitoring

---

## Interview Questions for Netflix System Design

### Q1: How would you handle adaptive bitrate streaming?
**Key points:**
- Network bandwidth detection
- Device capability assessment
- Quality prediction algorithms
- Rebuffering prevention strategies
- User preference balancing

### Q2: Design the recommendation system
**Key points:**
- Collaborative filtering
- Content-based filtering
- Cold start problem solutions
- Real-time vs batch processing
- A/B testing framework

### Q3: How to maintain 99.99% uptime?
**Key points:**
- Multi-region architecture
- Database replication
- Caching strategies
- Circuit breaker patterns
- Graceful degradation

### Q4: Optimize for low-latency streaming
**Key points:**
- CDN edge locations
- Connection optimization
- Protocol choice (HTTP/3)
- Manifest prefetching
- Predictive buffering

---

## Design Trade-offs

| Decision | Benefit | Trade-off |
|----------|---------|-----------|
| **CDN caching** | Reduce latency to <50ms | Higher infrastructure cost |
| **Adaptive bitrate** | Better UX for varying networks | Complex algorithm, CPU overhead |
| **Real-time recommendations** | Personalized experience | Lower computation accuracy vs batch |
| **Multiple regions** | High availability | Data consistency challenges |
| **Kafka event streaming** | Scalable analytics | Operational complexity |

