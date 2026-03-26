# Twitter Real-Time Platform

## Overview
Twitter/X: 500M+ monthly active users, 500M+ tweets/day, real-time fanout, handling massive concurrent reads/writes.

## Key Requirements
- **Scale**: 500M MAU, 500M tweets/day
- **Timeline**: <1 second for home feed refresh
- **Throughput**: 100K+ tweets/second at peak
- **Latency**: <500ms for tweet composing and posting
- **Fanout**: Deliver new tweets to followers in real-time
- **Search**: Instant search across billions of tweets
- **Trending**: Live trending topics globally

---

## High-Level Architecture

```
┌──────────────────────────────────────────────────────────────┐
│          Twitter Real-Time Infrastructure                    │
└──────────────────────────────────────────────────────────────┘

CLIENT LAYER:
┌──────────────────────────────────────────────────────────────┐
│ Web Browser / Mobile App / Desktop Client                    │
│ - Compose tweets, view timeline                              │
│ - Real-time updates via WebSocket                            │
│ - Notifications, conversations                               │
└──────────────────────────────────────────────────────────────┘
            │
            ▼
┌──────────────────────────────────────────────────────────────┐
│ WebSocket Gateway (Long polling fallback)                    │
│ - Maintain connection with client                            │
│ - Bi-directional communication                               │
│ - Route to appropriate server                                │
└──────────────────────────────────────────────────────────────┘
            │
    ┌───────┼───────┬────────┬────────┐
    ▼       ▼       ▼        ▼        ▼
┌──────────────────────────────────────────────────┐
│ Microservices Layer                              │
├──────────────────────────────────────────────────┤
│                                                  │
│ ┌────────────────┐  ┌────────────────┐          │
│ │Tweet Service   │  │Timeline Service│          │
│ │- Compose tweet │  │- Home timeline │          │
│ │- Post tweet    │  │- Profile timeline        │
│ │- Delete tweet  │  │- Push new tweets        │
│ │- Metrics       │  │- Cache results  │        │
│ └────────────────┘  └────────────────┘          │
│                                                  │
│ ┌────────────────┐  ┌────────────────┐          │
│ │Fanout Service  │  │Search Service  │          │
│ │- Deliver tweets│  │- Tweet search  │          │
│ │  to followers  │  │- User search   │          │
│ │- Push notif    │  │- Hashtag search│          │
│ │- Handle delays │  │- Trending calc │          │
│ └────────────────┘  └────────────────┘          │
│                                                  │
│ ┌────────────────┐  ┌────────────────┐          │
│ │Engagement Srv  │  │User Service    │          │
│ │- Likes, retweet│  │- Follow/unfollow        │
│ │- Count updates │  │- User profiles │          │
│ │- Real-time    │  │- Blocking      │          │
│ │  engagement    │  │- Muting        │          │
│ └────────────────┘  └────────────────┘          │
│                                                  │
│ ┌────────────────┐  ┌────────────────┐          │
│ │Notification    │  │Conversation    │          │
│ │Service         │  │Service         │          │
│ │- Like notif    │  │- DMs           │          │
│ │- Reply notif   │  │- Reply threads │          │
│ │- Follow notif  │  │- Mention notif │          │
│ └────────────────┘  └────────────────┘          │
│                                                  │
└──────────────────────────────────────────────────┘

DATA LAYER:
┌──────────────────────────────────────────────────────────────┐
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐ │
│  │Tweet Service DB│  │Timeline Cache  │  │Social Graph DB │ │
│  │(HBase/Cassand) │  │(Redis)         │  │(Graph DB)      │ │
│  │- Tweets data   │  │- User timelines│  │- Relationships │ │
│  │- Metadata      │  │- Hot tweets    │  │- Follow pairs  │ │
│  │- User metrics  │  │- Recent posts  │  │- Blocks        │ │
│  └────────────────┘  └────────────────┘  └────────────────┘ │
│                                                              │
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐ │
│  │Search Index    │  │Real-time Queue │  │Analytics DW    │ │
│  │(Elasticsearch) │  │(Kafka, Redis)  │  │(Hadoop/BigQ)   │ │
│  │- Searchable    │  │- Tweet events  │  │- Trends        │ │
│  │  tweets        │  │- Engagement evt│  │- User behavior │ │
│  │- Trending calc │  │- Fanout jobs   │  │- Demographics  │ │
│  └────────────────┘  └────────────────┘  └────────────────┘ │
│                                                              │
└──────────────────────────────────────────────────────────────┘
```

---

## Tweet Posting & Fanout Flow

```
TWEET POSTING PROCESS:
══════════════════════

User composes tweet: "Just launched new feature! 🚀"

1. COMPOSE & POST:
   ┌───────────────────────────────────┐
   │ Tweet Composition Service         │
   │ - Validate tweet content          │
   │ - Length check (<280 chars)       │
   │ - Media attachment (if any)       │
   │ - URL shortening (t.co)           │
   │ - Entity extraction (mentions,    │
   │   hashtags, URLs)                 │
   └───────────────────────────────────┘
                │
                ▼
   ┌───────────────────────────────────┐
   │ Tweet Service (Primary)           │
   │ - Store tweet metadata            │
   │ - Assign tweet ID                 │
   │ - Create record in HBase          │
   │ - Store media references          │
   │ - Return tweet ID to client       │
   └───────────────────────────────────┘
                │
                ▼
   ┌───────────────────────────────────┐
   │ Publish Event to Kafka            │
   │ Event: {                          │
   │   tweet_id: 12345,                │
   │   user_id: 67890,                 │
   │   follower_count: 50000,          │
   │   content: "Just launched...",    │
   │   timestamp: NOW,                 │
   │   media: [...]                    │
   │ }                                 │
   └───────────────────────────────────┘

2. FANOUT TO FOLLOWERS:
   ┌───────────────────────────────────┐
   │ Fanout Service (Reads from Kafka) │
   │ For each user, determine fanout   │
   │ strategy:                         │
   │                                   │
   │ Low follower count (<10K):        │
   │ - PUSH fanout (push to all)       │
   │ - 100ms to deliver to all         │
   │                                   │
   │ High follower count (10K-1M):     │
   │ - PULL fanout (users fetch)       │
   │ - Timeline queries fetch on-demand│
   │                                   │
   │ Mega celebrity (>1M followers):   │
   │ - HYBRID (prioritize active users)│
   │ - Push to online users            │
   │ - Pull available to others        │
   └───────────────────────────────────┘

3. PUSH FANOUT PROCESS:
   ┌───────────────────────────────────┐
   │ For followers with enabled push:  │
   │                                   │
   │ Get 50K followers from DB         │
   │ Queue job: process in batches     │
   │ - Batch size: 1000 followers      │
   │ - ~50 batches total               │
   │                                   │
   │ For each batch:                   │
   │ - Check mute/block status         │
   │ - Apply privacy filters           │
   │ - Add to their timeline cache     │
   │ - Update Redis timeline list      │
   │ - Send push notification          │
   └───────────────────────────────────┘

4. PUSH NOTIFICATION SERVICE:
   ┌───────────────────────────────────┐
   │ Via FCM / APNs                    │
   │ - Send notification (if opt-in)   │
   │ - Update app badge count          │
   │ - Open directly to tweet          │
   │ - WebSocket push for web clients  │
   │ - ~1 sec to reach most users      │
   └───────────────────────────────────┘

5. TIMELINE UPDATES:
   ┌───────────────────────────────────┐
   │ User timelines updated:           │
   │ - Redis list: user:123:timeline   │
   │ - Prepend new tweet ID            │
   │ - Keep top 500 tweets cached      │
   │ - Rest fetched on demand          │
   │ - TTL: 24 hours                   │
   └───────────────────────────────────┘

6. ENGAGEMENT TRACKING:
   ┌───────────────────────────────────┐
   │ Metrics Service                   │
   │ - Impression count                │
   │ - Click through rate              │
   │ - Like count (real-time)          │
   │ - Retweet count                   │
   │ - Reply tracking                  │
   │ - Store in HBase for analytics    │
   └───────────────────────────────────┘
```

---

## Timeline Feed Generation

```
HOME TIMELINE REFRESH:
══════════════════════

User opens Twitter app and pulls to refresh:

1. REQUEST:
   ┌───────────────────────────────────┐
   │ Timeline API                      │
   │ GET /timeline?user_id=123&limit=20
   └───────────────────────────────────┘

2. CACHE CHECK:
   ┌───────────────────────────────────┐
   │ Redis Cache Lookup                │
   │ Key: "user:123:timeline"          │
   │ - If exists & fresh: return       │
   │ - Otherwise: proceed to pull      │
   └───────────────────────────────────┘

3. FETCH STRATEGY (PULL):
   ┌───────────────────────────────────┐
   │ If cache miss:                    │
   │ - Get user's followers list       │
   │ - Last 500k tweets from followers │
   │ - Score by recency & engagement   │
   │ - Top 20 selected                 │
   └───────────────────────────────────┘

4. RANKING:
   ┌───────────────────────────────────┐
   │ ML Ranking Model                  │
   │ Factors:                          │
   │ - Recency (newer preferred)       │
   │ - Engagement (likes, RTs)         │
   │ - Social graph distance           │
   │ - User interests (topics)         │
   │ - Tweet diversity                 │
   │ - Conversation threads            │
   └───────────────────────────────────┘

5. HYDRATION:
   ┌───────────────────────────────────┐
   │ Fetch full tweet data             │
   │ - Text content                    │
   │ - Author info (name, handle)      │
   │ - Media thumbs                    │
   │ - Engagement counts               │
   │ - Whether you liked it            │
   └───────────────────────────────────┘

6. RESPONSE:
   ┌───────────────────────────────────┐
   │ Return feed in ~500-800ms         │
   │ - 20 tweets                       │
   │ - Push WebSocket update           │
   │ - Cache result for 5 min          │
   │ - Ready for next page scroll      │
   └───────────────────────────────────┘
```

---

## Key Technologies

| Component | Tech | Purpose |
|-----------|------|---------|
| **Storage** | HBase, Cassandra | Tweet data |
| **Timeline** | Redis | Feed caching |
| **Search** | Elasticsearch | Tweet search |
| **Graph** | Neo4j | Social graph |
| **Fanout** | Kafka | Event streaming |
| **Real-time** | WebSocket | Live updates |
| **Messaging** | Kafka | Async events |
| **Analytics** | Hadoop | Big data processing |

---

## Scalability Mechanisms

1. **Tweet Fanout**
   - PUSH vs PULL strategy based on follower count
   - Batch processing for bulk delivery
   - Eventual consistency acceptable

2. **Timeline Caching**
   - Redis for hot timelines
   - Multi-tier caching strategy
   - Cache invalidation on new tweets

3. **Search Scaling**
   - Elasticsearch sharding by time/topic
   - Real-time indexing of new tweets
   - Cached trending queries

4. **Real-time Updates**
   - WebSocket for bi-directional comms
   - Long polling fallback
   - Notification batching

---

## Interview Questions

### Q1: Design tweet fanout system
**Key points:**
- Push vs pull fanout strategies
- Handling high-follower users
- Eventual consistency trade-offs
- Batch job processing
- Performance optimization

### Q2: Implement home timeline feed
**Key points:**
- Selecting relevant tweets
- Ranking algorithm
- Caching strategy
- Real-time updates
- Handling infinite scroll

### Q3: Scale trending topics calculation
**Key points:**
- Aggregating hashtags globally
- Real-time vs batch calculation
- Geographic variation
- Spam detection
- Update frequency

