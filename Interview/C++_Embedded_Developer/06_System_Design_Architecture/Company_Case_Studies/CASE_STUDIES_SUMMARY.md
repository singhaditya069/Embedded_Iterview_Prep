# Company Case Studies Complete List

## 12 Comprehensive System Design Case Studies

This directory now contains **12 detailed case studies** from top technology companies, organized by industry and architectural complexity.

---

## Case Studies by Category

### 🎬 **Media & Streaming**
1. **Netflix** - Video streaming platform (01_Netflix_Streaming_Platform.md)
   - Focus: Adaptive bitrate streaming, CDN optimization, ML recommendations
   - Scale: 250M users, billions of streams/day
   - Key tech: CloudFront, Redis, Kafka, TensorFlow

2. **YouTube** - Video platform with discovery (03_YouTube_Video_Platform.md)
   - Focus: Video search, upload/transcoding, recommendations
   - Scale: 2B users, 500+ hours uploaded/min
   - Key tech: Elasticsearch, GCS, TensorFlow

3. **Slack** - Enterprise messaging (10_Slack_Messaging.md)
   - Focus: Real-time chat, full-text search, presence
   - Scale: 700K teams, 20M DAU, 100M messages/day
   - Key tech: Cassandra, Elasticsearch, Redis, WebSocket

### 🎮 **Real-Time Communication**
4. **Discord** - Voice & text platform (11_Discord_Voice.md)
   - Focus: Voice streaming, audio mixing, presence at scale
   - Scale: 150M MAU, 6M servers, 10M concurrent users
   - Key tech: RTP/UDP, Opus codec, Redis, WebSocket

5. **Twitter** - Real-time social feed (09_Twitter_RealTime.md)
   - Focus: Tweet fanout, timeline generation, trending topics
   - Scale: 500M MAU, 500M tweets/day
   - Key tech: HBase, Cassandra, Elasticsearch, Kafka

### 📱 **Social & Networks**
6. **Meta** - Social network platform (02_Meta_Social_Network.md)
   - Focus: Feed ranking, messaging, social graph
   - Scale: 3B users, billions of posts/day
   - Key tech: TAO (graph DB), Memcached, Kafka, GraphQL

7. **Airbnb** - Marketplace platform (08_Airbnb_Marketplace.md)
   - Focus: Geospatial search, booking with concurrent reservations, payments
   - Scale: 7M listings, 150M users, 2M bookings/day
   - Key tech: Elasticsearch (geo), PostgreSQL, DynamoDB

### 🛒 **Commerce & Transactions**
8. **Amazon** - E-commerce platform (04_Amazon_ECommerce.md)
   - Focus: Product search, order processing, distributed systems
   - Scale: 300M users, 100M+ SKUs, 2M+ orders/day
   - Key tech: RDS, DynamoDB, Elasticsearch, Lambda

9. **IRCTC** - High-traffic ticketing (05_IRCTC_Railway_Booking.md)
   - Focus: Seat inventory, virtual queue, peak load (10x spikes)
   - Scale: 50M users, 100K+ peak concurrent
   - Key tech: Redis, MySQL (sharded), Kafka

### 🚗 **Location & Real-Time**
10. **Uber** - Ride-sharing platform (07_Uber_RideSharing.md)
    - Focus: Real-time matching, location tracking, dynamic pricing
    - Scale: 20M trips/day, 5M+ drivers
    - Key tech: Geohashing, WebSocket, Kafka, Google Maps API

### 🔍 **Search & Information**
11. **Google** - Search engine (06_Google_Search.md)
    - Focus: Web crawling, indexing, PageRank, ML ranking
    - Scale: 8.5B+ searches/day, trillions of pages
    - Key tech: BigTable, custom crawler, PageRank, TensorFlow

### 🤖 **AI & Machine Learning**
12. **Anthropic** - LLM inference (05_Anthropic_AI_Infrastructure.md)
    - Focus: GPU inference serving, token generation, KV cache
    - Scale: Millions of inference requests/day
    - Key tech: PyTorch, CUDA, vLLM, H100/A100 GPUs

---

## Key Statistics

| Metric | Range | Example |
|--------|-------|---------|
| **Daily Active Users** | 20M - 3B | Meta: 3B, Twitter: 500M, Discord: 150M |
| **Requests/Second** | 100K - 1M+ | Google: 100K+, Amazon: 200K+ |
| **Data Stored** | Terabytes - Exabytes | YouTube: Exabytes, Google: Exabytes |
| **Latency Target** | <10ms - <2sec | Voice: <50ms, Search: <100ms, LLM: <2sec |
| **Uptime SLA** | 99.9% - 99.99% | Most: 99.99%, some 99.95% |

---

## Technologies Overview

### Most Frequently Used
- **Databases**: PostgreSQL, DynamoDB, Cassandra, HBase
- **Cache**: Redis, Memcached
- **Search**: Elasticsearch
- **Messaging**: Kafka, Redis
- **Real-time**: WebSocket
- **CDN**: CloudFront, Google CDN

### Company-Specific Choices
- **Meta**: TAO (custom graph database)
- **Netflix**: CloudFront CDN + custom optimization
- **Anthropic**: CUDA for GPU computing
- **Google**: BigTable for massive scale
- **Amazon**: DynamoDB + Lambda (cost optimization)

---

## Architecture Patterns Discovered

### 1. **Multi-Tier Caching**
Netflix: CDN → Redis → Database
Pattern used by: Netflix, Meta, Amazon, YouTube

### 2. **Sharding & Partitioning**
Partition by: user_id, time, geographic region, entity_id
Used by: All companies at massive scale

### 3. **Eventual Consistency**
Accept delayed consistency for availability
Examples: Meta (social graph), IRCTC (inventory)

### 4. **Event-Driven Architecture**
Kafka for decoupling producers from consumers
Used by: Meta, Netflix, Uber, IRCTC, Twitter

### 5. **Geospatial Indexing**
Geohashing for location-based queries
Used by: Uber, Airbnb, Google Maps

### 6. **PUSH vs PULL Fanout**
Twitter: PUSH for low followers, PULL for celebrities
Discord: PUSH for online, PULL for offline

### 7. **Real-Time Streaming**
WebSocket for bi-directional communication
Used by: Discord, Twitter/X, Slack, Uber

### 8. **ML-Based Ranking**
ML models for recommendations/ranking
Used by: Netflix, YouTube, Meta, Google, Amazon

---

## Interview Preparation Roadmap

### Week 1-2: Foundation
- Read: Amazon, Netflix, Meta
- Focus: Distributed systems basics
- Topics: Sharding, replication, caching

### Week 3-4: Scale & Distribution
- Read: Google, YouTube, Uber
- Focus: Massive scale systems
- Topics: Geohashing, CDN, search

### Week 5-6: Real-Time Systems
- Read: Twitter, Discord, Slack
- Focus: Real-time requirements
- Topics: WebSocket, presence, messaging

### Week 7-8: Specialized Systems
- Read: Anthropic, IRCTC, Airbnb
- Focus: Domain-specific challenges
- Topics: ML infrastructure, ticketing, marketplace

### Week 9: Practice & Design
- Design similar systems with different scale
- Answer interview questions
- Explain systems to peers

---

## Common Interview Questions Across All

### Q1: How to design system X?
**Approach**: Clarify requirements → Design high-level → Deep dive into components → Discuss trade-offs

### Q2: How to handle 10x traffic increase?
**Answer**: Multi-layer caching, database sharding, CDN, horizontal scaling, graceful degradation

### Q3: How to ensure data consistency?
**Answer**: Depends on requirements - strong consistency (transactions) vs eventual (high availability)

### Q4: What happens when component fails?
**Answer**: Redundancy, failover strategies, graceful degradation, monitoring/alerting

### Q5: How to optimize cost?
**Answer**: Use appropriate tech (serverless vs containers), data compression, regional routing

---

## File Organization

```
Company_Case_Studies/
├── 00_CASE_STUDIES_INDEX.md           # Main index (this file)
├── 01_Netflix_Streaming_Platform.md    # #1
├── 02_Meta_Social_Network.md           # #2
├── 03_YouTube_Video_Platform.md        # #3
├── 04_Amazon_ECommerce.md              # #4
├── 05_Anthropic_AI_Infrastructure.md   # #5
├── 05_IRCTC_Railway_Booking.md         # #6 (Note: Shared 05 prefix)
├── 06_Google_Search.md                 # #7
├── 07_Uber_RideSharing.md              # #8
├── 08_Airbnb_Marketplace.md            # #9
├── 09_Twitter_RealTime.md              # #10
├── 10_Slack_Messaging.md               # #11
├── 11_Discord_Voice.md                 # #12
└── CASE_STUDIES_SUMMARY.md             # This file
```

---

## How to Use These Materials

### For Interview Preparation
1. Read one case study completely
2. Draw architecture from memory
3. Answer the interview questions
4. Compare with other companies

### For System Design Learning
1. Understand why each technology was chosen
2. Identify trade-offs made
3. Think about alternative approaches
4. Consider what you would do differently

### For Technical Interviews
1. Reference real examples
2. Show knowledge of scalability
3. Discuss trade-offs thoughtfully
4. Ask clarifying questions

---

## Next Steps

### Recommended Reading Order
1. Start with **one** you find most interesting
2. Deep dive into **all 12** for breadth
3. Focus on **3-4** that match your target role
4. Practice designing **similar** systems

### Additional Topics to Explore
- Distributed consensus (Raft, Paxos)
- Distributed tracing (Jaeger, Honeycomb)
- Circuit breakers & resilience patterns
- Database indexing strategies
- API design patterns (REST, GraphQL, gRPC)

### Community Resources
- Interview preparation communities
- System design Discord servers
- LeetCode system design problems
- Grokking System Design interviews

---

## Final Notes

These 12 case studies cover a wide breadth of system design challenges from companies operating at global scale. Each represents years of engineering decisions and trade-offs.

**Key Takeaway**: There's rarely a "one size fits all" solution. Success comes from understanding your specific constraints (scale, latency, cost, reliability) and making informed trade-offs.

Good luck with your interview preparation! 🚀

