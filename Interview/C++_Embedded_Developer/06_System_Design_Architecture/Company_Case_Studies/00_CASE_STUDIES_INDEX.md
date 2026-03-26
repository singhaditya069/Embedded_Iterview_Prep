# Company Case Studies Index

## Overview
This directory contains comprehensive system design case studies from top technology companies. Each case study includes architecture diagrams, data flow analysis, scalability techniques, and interview questions.

**Total Companies Covered**: 12 major case studies
**Coverage**: Streaming, Social Media, Video Platforms, E-Commerce, AI/ML, Booking, Search, Ride-sharing, Marketplace, Real-time Messaging, Voice/Video, Workflow

---

## All Case Studies (12 Companies)

### 1. Netflix Streaming Platform
**File**: [01_Netflix_Streaming_Platform.md](01_Netflix_Streaming_Platform.md)

**Focus**: Video streaming at scale (250M+ users)
- **Key Systems**: 
  - Video streaming with adaptive bitrate (ABSL algorithm)
  - ML-based recommendation engine
  - Multi-tier caching architecture
  - DRM & content protection
- **Scale**: 250M+ users, billions of streams/day
- **Latency**: <100ms video load time
- **Technologies**: CloudFront, Redis, Kafka, TensorFlow
- **Interview Questions**: 4 questions covering streaming, recommendations, scalability
- **Diagrams**: Architecture, video streaming flow, recommendation engine, data flow (11-step)

---

### 2. Meta Social Network
**File**: [02_Meta_Social_Network.md](02_Meta_Social_Network.md)

**Focus**: Social network at massive scale (3B+ users)
- **Key Systems**:
  - Feed generation with ML-based ranking
  - Real-time messaging (WebSocket)
  - Social graph storage (TAO)
  - Multi-tier caching strategy
- **Scale**: 3B+ users, billions of posts/day
- **Latency**: <100ms feed response
- **Technologies**: GraphQL, Kafka, Memcached, MySQL, TAO, Lucene
- **Interview Questions**: 3 questions covering news feeds, messaging, scale
- **Diagrams**: Architecture, posting flow, feed generation (12-step), real-time messaging

---

### 3. YouTube Video Platform
**File**: [03_YouTube_Video_Platform.md](03_YouTube_Video_Platform.md)

**Focus**: Video search and discovery (2B+ users)
- **Key Systems**:
  - Video upload & transcoding (500+ hours/min)
  - Full-text search (billions of videos)
  - Recommendation engine
  - Adaptive quality streaming
- **Scale**: 2B+ users, 500+ hours uploaded/min
- **Latency**: <50ms search results, <100ms video load
- **Technologies**: Elasticsearch, S3/GCS, HLS/DASH, TensorFlow
- **Interview Questions**: 3 questions covering upload, search, recommendations
- **Diagrams**: Architecture, video lifecycle, search flow, recommendation flow

---

### 4. Amazon E-Commerce
**File**: [04_Amazon_ECommerce.md](04_Amazon_ECommerce.md)

**Focus**: E-commerce at global scale (300M+ users)
- **Key Systems**:
  - Product search (100M+ SKUs)
  - Shopping cart management
  - Order processing & fulfillment
  - Payment processing (PCI-DSS)
- **Scale**: 300M users, 2M+ orders/day
- **Latency**: <100ms search, <50ms add-to-cart
- **Technologies**: RDS, DynamoDB, Elasticsearch, Lambda, Kafka
- **Interview Questions**: 3 questions covering search, ordering, scaling
- **Diagrams**: Architecture, product search flow, order processing flow

---

### 5. Anthropic AI Infrastructure
**File**: [05_Anthropic_AI_Infrastructure.md](05_Anthropic_AI_Infrastructure.md)

**Focus**: LLM inference at scale (millions of requests/day)
- **Key Systems**:
  - GPU inference serving
  - Token generation pipeline
  - KV cache management
  - Request batching & scheduling
- **Scale**: Millions of inference requests/day
- **Latency**: <2 seconds per request
- **Technologies**: PyTorch, CUDA, vLLM, H100/A100 GPUs
- **Interview Questions**: 3 questions covering inference, pipelines, scaling
- **Diagrams**: Architecture, inference flow, GPU memory layout, multi-GPU sharding

---

### 6. IRCTC Indian Railways
**File**: [05_IRCTC_Railway_Booking.md](05_IRCTC_Railway_Booking.md)

**Focus**: High-traffic booking system (50M+ users)
- **Key Systems**:
  - Train search & inventory
  - Seat reservation with virtual queue
  - Payment integration (multiple gateways)
  - PNR & ticket management
- **Scale**: 50M users, 100K+ peak concurrent, 1M+ bookings/day
- **Peak**: 10x traffic spikes during ticket releases
- **Latency**: <2 seconds seat search
- **Technologies**: Redis, MySQL (sharded), Kafka, Multiple payment gateways
- **Interview Questions**: 3 questions covering inventory, queuing, peak handling
- **Diagrams**: Architecture, seat booking flow, peak load management

---

### 9. Airbnb Marketplace
**File**: [08_Airbnb_Marketplace.md](08_Airbnb_Marketplace.md)

**Focus**: Marketplace booking at global scale (7M+ listings)
- **Key Systems**:
  - Geospatial search (multi-region)
  - Availability calendar management
  - Booking with concurrent reservations
  - International payments (multi-currency)
- **Scale**: 7M+ listings, 150M+ users, 2M+ bookings/day
- **Latency**: <100ms search, <50ms booking
- **Technologies**: Elasticsearch (geospatial), PostgreSQL, DynamoDB, Redis
- **Interview Questions**: 3 questions covering search, concurrent bookings, payments
- **Diagrams**: Architecture, search flow, booking flow

---

### 10. Twitter Real-Time Platform
**File**: [09_Twitter_RealTime.md](09_Twitter_RealTime.md)

**Focus**: Real-time social feed at massive scale (500M MAU)
- **Key Systems**:
  - Tweet fanout (PUSH vs PULL strategy)
  - Timeline feed generation with ML ranking
  - Real-time engagement tracking
  - Trending topics calculation
- **Scale**: 500M MAU, 500M tweets/day, 100K+ tweets/sec peak
- **Latency**: <1 second timeline, <500ms tweet post
- **Technologies**: HBase, Cassandra, Elasticsearch, Kafka, WebSocket
- **Interview Questions**: 3 questions covering fanout, timeline, trending
- **Diagrams**: Architecture, posting/fanout flow, timeline generation

---

### 11. Slack Real-Time Messaging
**File**: [10_Slack_Messaging.md](10_Slack_Messaging.md)

**Focus**: Enterprise messaging at scale (700K+ teams)
- **Key Systems**:
  - Real-time message delivery (WebSocket/RTM)
  - Full-text search across all messages
  - Presence tracking
  - Thread management
- **Scale**: 700K+ teams, 20M+ DAU, 100M+ messages/day
- **Latency**: Sub-second message delivery, <200ms search
- **Technologies**: Cassandra, Elasticsearch, Redis, Kafka, WebSocket
- **Interview Questions**: 3 questions covering messaging, search, presence
- **Diagrams**: Architecture, message delivery flow, search flow

---

### 12. Discord Voice & Text at Scale
**File**: [11_Discord_Voice.md](11_Discord_Voice.md)

**Focus**: Real-time voice/text for massive concurrent users (150M MAU)
- **Key Systems**:
  - Voice streaming (sub-50ms latency)
  - Audio mixing for 1000+ user channels
  - Presence tracking at scale
  - Real-time message synchronization
- **Scale**: 150M MAU, 6M+ active servers, 10M+ concurrent users
- **Latency**: <50ms voice, <100ms text
- **Technologies**: RTP/UDP, Opus codec, Redis (presence), DynamoDB, WebSocket
- **Interview Questions**: 3 questions covering voice channels, presence, messaging
- **Diagrams**: Architecture, voice connection flow, presence tracking

---

## Quick Reference Table

| Company | Focus | Users/Scale | Key Challenge | Tech Stack |
|---------|-------|-------------|----------------|-----------|
| Netflix | Video Streaming | 250M | Streaming quality + recommendations | CDN, Redis, ML |
| Meta | Social Network | 3B | Feed ranking + messaging | TAO, Kafka, GraphQL |
| YouTube | Video Discovery | 2B | Upload + search + recommendations | Elasticsearch, Transcoding |
| Amazon | E-Commerce | 300M | Search + ordering at scale | DynamoDB, Lambda |
| Anthropic | AI/ML Inference | millions/day | GPU efficiency + latency | CUDA, H100 |
| IRCTC | Ticket Booking | 50M | Peak load handling | Redis, MySQL (sharded) |
| Google | Search Engine | 8.5B searches/day | Index scale + relevance | BigTable, PageRank |
| Uber | Ride-Sharing | 20M trips/day | Real-time matching + pricing | Geohashing, Kafka |
| Airbnb | Marketplace | 7M listings | Geospatial search + booking | Elasticsearch, PostgreSQL |
| Twitter | Social Feed | 500M MAU | Fanout at scale + trending | HBase, Cassandra, Kafka |
| Slack | Messaging | 700K teams | Real-time chat + search | Cassandra, Elasticsearch |
| Discord | Voice/Text | 150M MAU | Audio mixing + voice latency | RTP/UDP, Opus, Redis |

---

## Study Path by Role

### For Backend Engineers
1. Start with **Amazon E-Commerce** - scalable distributed systems
2. Move to **Meta Social Network** - complex data flows and consistency
3. Deep dive into **Netflix Streaming** - real-time performance requirements
4. Explore **IRCTC Booking** - high-concurrency challenges

### For ML/AI Engineers
1. Start with **Anthropic AI** - inference infrastructure design
2. Move to **Netflix Recommendations** - ML system in production
3. Explore **YouTube Recommendations** - large-scale ML recommendations
4. Study **Meta Feed Generation** - real-time ML ranking

### For Systems Designers
1. **IRCTC** - queuing and concurrency management
2. **Amazon** - distributed database design
3. **Netflix** - CDN and edge computing
4. **Anthropic** - GPU cluster orchestration

---

## Architecture Patterns Found Across Companies

### 1. Caching Hierarchies
- **Netflix**: CDN → Redis → Database
- **Meta**: CDN → Memcached (multi-tier)
- **Amazon**: CloudFront → ElastiCache → RDS
- **Lesson**: Multi-layer caching reduces database load dramatically

### 2. Distributed Search
- **YouTube**: Elasticsearch with 1000s of shards
- **Amazon**: Elasticsearch with 100s of shards
- **Lesson**: Sharding by entity ID enables massive scale

### 3. Queue-Based Processing
- **Netflix**: Encoding queue for video processing
- **IRCTC**: Virtual queue for booking system
- **Meta**: Kafka for asynchronous events
- **Lesson**: Decoupling producers from consumers with queues

### 4. Real-Time Features
- **Netflix**: Recommendations updated continuously
- **Meta**: Real-time WebSocket messaging
- **YouTube**: Live streaming support
- **Lesson**: Requires separate real-time infrastructure

### 5. Payment Processing
- **Amazon**: Multiple payment gateways, reconciliation
- **IRCTC**: Multiple payment methods with fraud detection
- **Lesson**: Idempotent design critical for correctness

---

## Common Interview Patterns

### System Design Focus Areas
1. **Scale & Throughput**: How to handle millions of users/requests
2. **Latency**: Keeping response times under requirements
3. **Consistency**: Managing eventual vs strong consistency
4. **Reliability**: Handling failures gracefully
5. **Cost**: Operating at global scale efficiently

### Typical Questions Asked
1. "How would you design X system?"
2. "How would you handle 10x traffic increase?"
3. "What happens when a component fails?"
4. "How do you ensure data consistency?"
5. "What are the trade-offs you made?"

### What Interviewers Look For
- Understanding of trade-offs
- Knowledge of real-world constraints
- Scalability thinking
- Failure mode analysis
- Cost awareness

---

## Tips for Interview Preparation

### Before Each Case Study
1. **Understand the problem**: What is the business trying to accomplish?
2. **Know the scale**: How many users, requests, data size?
3. **Identify bottlenecks**: What would break first at high scale?

### While Studying
1. **Draw diagrams**: Understand the architecture visually
2. **Trace a request**: Follow a request through the system
3. **Identify each component's role**: Why is each technology chosen?
4. **Think about failure modes**: What breaks? How to recover?

### During Interviews
1. **Start simple**: Begin with basic architecture
2. **Show your thinking**: Explain trade-offs explicitly
3. **Ask clarifying questions**: Don't assume requirements
4. **Use examples**: Reference real companies when applicable
5. **Discuss alternatives**: Why not use technology X instead?

---

## Related Topics to Study

### Complementary System Design Topics
- Distributed consensus (Raft, Paxos)
- Database replication strategies
- Load balancing algorithms
- Message queue patterns
- Caching patterns
- API design considerations
- Monitoring & observability

### Technologies Frequently Mentioned
- MySQL, PostgreSQL (relational databases)
- Redis (in-memory cache)
- Kafka (message queue)
- Elasticsearch (search)
- DynamoDB, HBase (NoSQL)
- Kubernetes (orchestration)
- Docker (containerization)

### Design Patterns Covered
- Microservices architecture
- API Gateway pattern
- CQRS (Command Query Responsibility Segregation)
- Eventual consistency
- Sharding & partitioning
- Circuit breaker & retry patterns
- Bulkhead isolation

---

## How to Use This Directory

### Quick Lookup
Use this index to find case studies by company, technology, or use case.

### Deep Dive
Each case study file contains:
1. Architecture overview with ASCII diagrams
2. Detailed dataflow for key operations
3. Technology stack explanation
4. Scalability strategies
5. Real interview questions with key points

### Practice
After reading each case study:
1. Draw the architecture from memory
2. Design a similar system with different scale
3. Answer the interview questions
4. Explain the system to someone else

### Compare
Look for patterns across companies:
- How do they handle similar problems differently?
- Which technologies appear frequently?
- What trade-offs seem common?

---

## Future Additions

Other companies often asked in system design interviews:
- **Spotify**: Music streaming, personalized playlists, recommendations, offline sync
- **TikTok**: Video recommendation at massive scale, viral algorithms, FYP algorithm
- **LinkedIn**: Social graph, job search, feed ranking, connection graph
- **Dropbox**: File sync, storage optimization, collaborative editing, version control
- **Pinterest**: Image discovery, recommendation engine, visual search
- **Stripe**: Payment processing, fraud detection, webhooks, reconciliation
- **Zoom**: Real-time video conferencing, WebRTC, latency optimization
- **WhatsApp**: End-to-end encryption at scale, message delivery, group chats
- **Instagram**: Photo sharing, infinite scroll, personalized feed, stories
- **Twitch**: Live streaming, real-time chat moderation, latency optimization
- **Notion**: Collaborative document editing, real-time sync, block editing
- **Figma**: Collaborative design tool, real-time drawing, multiplayer cursors

