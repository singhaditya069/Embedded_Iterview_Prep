# YouTube Video Sharing & Discovery Platform

## Overview
YouTube: 2+ billion users, 500+ hours of video uploaded per minute, billions of views per day, globally distributed video streaming platform.

## Key Requirements
- **Scale**: 2B users, 500+ hours/minute uploads
- **Throughput**: Billions of video views/day
- **Search**: Instant search across billions of videos
- **Recommendations**: Real-time personalized suggestions
- **Latency**: <50ms search results, <100ms video load
- **Storage**: Exabytes of video data

---

## System Architecture

```
┌────────────────────────────────────────────────────────────────┐
│                    YouTube Architecture                        │
└────────────────────────────────────────────────────────────────┘

CLIENT LAYER:
┌────────────────────────────────────────────────────────────────┐
│ Web Browser / Mobile App / Smart TV                            │
└────────────────────────────────────────────────────────────────┘
                          │
                          ▼
CDN & API GATEWAY:
┌────────────────────────────────────────────────────────────────┐
│ Global Load Balancer (Anycast)                                 │
│ - Route to nearest edge                                        │
│ - Request filtering, rate limiting                             │
└────────────────────────────────────────────────────────────────┘
                          │
        ┌─────────────────┼─────────────────┐
        ▼                 ▼                 ▼
    ┌─────────────┐  ┌──────────────┐  ┌──────────────┐
    │ Search      │  │ Video Info   │  │ Recommend    │
    │ Service     │  │ Service      │  │ Service      │
    └─────────────┘  └──────────────┘  └──────────────┘
        │                    │                  │
        ▼                    ▼                  ▼
    ┌─────────────────────────────────────────────────────────┐
    │         Microservices Layer                             │
    ├─────────────────────────────────────────────────────────┤
    │                                                         │
    │ ┌────────────────┐  ┌────────────────┐                │
    │ │Video Upload    │  │Video Encoding  │                │
    │ │Service         │  │Service         │                │
    │ │- Receive video │  │- Transcode to  │                │
    │ │- Store in S3   │  │  multiple fmt  │                │
    │ │- Queue job     │  │- Generate      │                │
    │ └────────────────┘  │  thumbnails    │                │
    │                     └────────────────┘                │
    │                                                         │
    │ ┌────────────────┐  ┌────────────────┐                │
    │ │Video Storage   │  │Video Streaming │                │
    │ │Service         │  │Service         │                │
    │ │- Metadata      │  │- Manifest gen  │                │
    │ │- Encoding info │  │- Quality select│                │
    │ │- Captions      │  │- DRM encryption│                │
    │ └────────────────┘  └────────────────┘                │
    │                                                         │
    │ ┌────────────────┐  ┌────────────────┐                │
    │ │Search Service  │  │Recommendation  │                │
    │ │- Full-text idx │  │- ML ranking    │                │
    │ │- Filters       │  │- Watch history │                │
    │ │- Suggestions   │  │- Trending calc │                │
    │ └────────────────┘  └────────────────┘                │
    │                                                         │
    │ ┌────────────────┐  ┌────────────────┐                │
    │ │Comments &      │  │User Service    │                │
    │ │Interactions    │  │- Profiles      │                │
    │ │- Likes         │  │- Subscriptions │                │
    │ │- Comments      │  │- Watch lists   │                │
    │ │- Shares        │  │- Preferences   │                │
    │ └────────────────┘  └────────────────┘                │
    │                                                         │
    └─────────────────────────────────────────────────────────┘
                          │
    ┌─────────────────────┼─────────────────────┐
    ▼                     ▼                     ▼
┌────────────────┐  ┌────────────────┐  ┌────────────────┐
│ Cache Layer    │  │ Search Infra   │  │ Data Warehouse │
│ - Redis        │  │ - Elasticsearch│  │ - Analytics    │
│ - Memcached    │  │ - Autocomplete │  │ - ML training  │
│ - CDN          │  │ - Index shards │  │ - Historical   │
└────────────────┘  └────────────────┘  └────────────────┘
    │                     │                     │
    ▼                     ▼                     ▼
┌────────────────┐  ┌────────────────┐  ┌────────────────┐
│ Primary DB     │  │ Video Storage  │  │ Message Queue  │
│ - MySQL        │  │ - S3 / GCS     │  │ - Kafka        │
│ - User data    │  │ - Multi-region │  │ - Events       │
│ - Metadata     │  │ - Replication  │  │ - Analytics    │
└────────────────┘  └────────────────┘  └────────────────┘
```

---

## Video Lifecycle Flow

```
VIDEO UPLOAD:
═════════════

User uploads video file (MP4, MOV, etc):
    │
    ▼
┌────────────────────────────────────┐
│ Upload Service                     │
│ - Receive file chunks              │
│ - Verify file integrity            │
│ - Store in S3 (temporary)          │
│ - Create VideoID                   │
└────────────────────────────────────┘
    │
    ▼
┌────────────────────────────────────┐
│ Transcoding Job Queue              │
│ - Prioritize by upload time        │
│ - Distribute to workers            │
└────────────────────────────────────┘
    │
    ▼
┌────────────────────────────────────┐
│ Encoding Service (Parallel)        │
│ - 1080p, 720p, 480p, 360p, 240p    │
│ - Multiple encoders per video      │
│ - ~1-2 hours encoding time         │
└────────────────────────────────────┘
    │
    ├──────────┬──────────┬──────────┬──────────┐
    ▼          ▼          ▼          ▼          ▼
Manifest   Thumbnails  Captions  Analytics  CDN Push
Generator  Generation  Etractor  Index
    │          │          │          │          │
    └──────────┴──────────┴──────────┴──────────┘
              │
              ▼
    ┌───────────────────────┐
    │ Video Ready           │
    │ - Searchable          │
    │ - Streamable          │
    │ - Available for       │
    │   recommendations     │
    └───────────────────────┘


VIDEO STREAMING:
════════════════

User clicks video:
    │
    ▼
┌────────────────────────────────────┐
│ Video Info Service                 │
│ - Get video metadata               │
│ - Check licensing                  │
│ - Verify user access               │
└────────────────────────────────────┘
    │
    ▼
┌────────────────────────────────────┐
│ Streaming Manifest Generation      │
│ - Select available encodings       │
│ - Generate HLS/DASH playlist       │
│ - Include ad insertion points      │
└────────────────────────────────────┘
    │
    ▼
┌────────────────────────────────────┐
│ Client Player                      │
│ - Parse manifest                   │
│ - Detect device capability         │
│ - Select best quality              │
│ - Start buffer                     │
└────────────────────────────────────┘
    │
    ▼
┌────────────────────────────────────┐
│ CDN Video Streaming                │
│ - Edge location cache              │
│ - Adaptive quality                 │
│ - Network-aware delivery           │
│ - <100ms initial load              │
└────────────────────────────────────┘
    │
    ▼
Video playback with:
• Real-time quality adjustment
• Ad insertion
• Analytics events
• User engagement tracking
```

---

## Search & Discovery System

```
SEARCH ARCHITECTURE:
════════════════════

User enters search query:
    │
    ▼
┌──────────────────────────────────────┐
│ Search API                           │
│ - Receive query                      │
│ - Rate limiting                      │
│ - Abuse detection                    │
└──────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────┐
│ Query Expansion                      │
│ - Spelling correction                │
│ - Synonym expansion                  │
│ - Intent classification              │
│ - Safety filtering                   │
└──────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────┐
│ Elasticsearch Cluster                │
│ - Sharded indices (~1000s)           │
│ - Replicated for availability        │
│ - Real-time indexing                 │
│ - <50ms latency                      │
└──────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────┐
│ Initial Results (Candidates)         │
│ - ~1000 matching videos              │
│ - Sorted by relevance                │
│ - Safety filtered                    │
└──────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────┐
│ Ranking Service                      │
│ - ML-based ranking model             │
│ - Features: clicks, watch time,      │
│           views, freshness,          │
│           channel authority          │
│ - Top 30 selected                    │
└──────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────┐
│ Result Hydration                     │
│ - Fetch full video info              │
│ - Thumbnails, metadata               │
│ - User metadata (subscribers, etc)   │
└──────────────────────────────────────┘
    │
    ▼
┌──────────────────────────────────────┐
│ Personalization                      │
│ - Apply user preferences             │
│ - Filter unwanted channels           │
│ - Language filtering                 │
│ - Content rating filters             │
└──────────────────────────────────────┘
    │
    ▼
Client receives results in ~50-200ms
```

---

## Recommendation Engine

```
RECOMMENDATION SYSTEM:
══════════════════════

┌───────────────────────────────────────┐
│ User Behavior Collection              │
│ - Watch history (all videos viewed)   │
│ - Search queries                      │
│ - Clicks, impressions                 │
│ - Watch time by content type          │
│ - Shares, likes, subscriptions        │
└───────────────────────────────────────┘
            │
            ▼
┌───────────────────────────────────────┐
│ Feature Generation (Offline)          │
│ - Recent watched videos               │
│ - Content categories                  │
│ - Channel affinity                    │
│ - Temporal features                   │
│ - Social features                     │
└───────────────────────────────────────┘
            │
            ▼
┌───────────────────────────────────────┐
│ ML Models (Multiple)                  │
│                                       │
│ 1. Collaborative Filtering            │
│    - Video-to-video similarity        │
│    - User-to-user similarity          │
│                                       │
│ 2. Deep Learning                      │
│    - RNN for sequence modeling        │
│    - Transformer for context          │
│                                       │
│ 3. Rule-based Engine                  │
│    - Subscribe to channel logic       │
│    - Series continuation              │
│    - Trending videos                  │
└───────────────────────────────────────┘
            │
            ▼
┌───────────────────────────────────────┐
│ Online Ranking                        │
│ - Score candidates                    │
│ - Diversify results                   │
│ - Apply business rules                │
│ - A/B test different models           │
└───────────────────────────────────────┘
            │
            ▼
Homepage recommendations personalized
for each user in real-time
```

---

## Key Technologies

| Component | Technology | Scale |
|-----------|-----------|-------|
| **Video Storage** | Google Cloud Storage / S3 | Exabytes |
| **Streaming** | HTTP/2, HLS, DASH | Billions of QPS |
| **Search** | Elasticsearch | 1000s of shards |
| **Cache** | Memcached, Redis | Petabytes of data |
| **Database** | H Table, BigTable | Billions of rows |
| **ML** | TensorFlow, PyTorch | Real-time inference |
| **Transcoding** | FFmpeg, Custom | 500+ hrs/min |
| **CDN** | Google CDN | 300+ edge locations |
| **Messaging** | Kafka, Pub/Sub | Real-time events |

---

## Scalability Strategies

1. **Video Upload Processing**
   - Queue-based transcoding
   - Distribute to thousands of workers
   - Parallel encoding of multiple formats
   - Estimated ~1-2 hours for full transcoding

2. **Search Scaling**
   - Elasticsearch sharding by video ID
   - Distributed indexing
   - Caching hot queries
   - Query result paging

3. **Video Delivery**
   - Edge caching in 300+ locations
   - Adaptive bitrate selection
   - TCP connection optimization
   - Regional redundancy

4. **Recommendation Scaling**
   - Batch model training offline
   - Real-time scoring service
   - Millions of QPS inference
   - GPU-accelerated serving

---

## Interview Questions

### Q1: Design YouTube video upload & processing
**Key points:**
- Handle large file uploads reliably
- Parallel transcoding to multiple formats
- Storage strategy (S3, cloud storage)
- Failure handling & retry logic
- User progress feedback

### Q2: Implement YouTube home page recommendations
**Key points:**
- Collaborative filtering approaches
- Deep learning models
- Real-time scoring
- Diversity & serendipity
- A/B testing framework

### Q3: Build efficient video search
**Key points:**
- Elasticsearch sharding strategy
- Indexing ~billions of videos
- Query expansion & understanding
- Ranking algorithm
- Personalization in results

