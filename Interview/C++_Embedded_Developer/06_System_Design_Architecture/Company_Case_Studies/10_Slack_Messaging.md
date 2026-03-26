# Slack Real-Time Messaging Platform

## Overview
Slack: 700K+ active teams, 20M+ daily active users, real-time messaging, file sharing, search across millions of messages.

## Key Requirements
- **Scale**: 700K+ teams, 20M+ DAU
- **Messages**: 100M+ messages/day
- **Latency**: Sub-second message delivery
- **Search**: Instant full-text search across all messages
- **Reliability**: 99.99% uptime SLA
- **Storage**: Years of message history
- **Scalability**: Handle spikes during work hours

---

## High-Level Architecture

```
┌────────────────────────────────────────────────────────────┐
│          Slack Real-Time Architecture                      │
└────────────────────────────────────────────────────────────┘

CLIENT LAYER:
┌────────────────────────────────────────────────────────────┐
│ Web App / Desktop / Mobile / API Clients                   │
│ - Send/receive messages                                    │
│ - Real-time updates (WebSocket/RTM)                        │
│ - Search messages                                          │
│ - File sharing                                             │
└────────────────────────────────────────────────────────────┘
            │
            ▼
┌────────────────────────────────────────────────────────────┐
│ RTM (Real-Time Messaging) Gateway                          │
│ - WebSocket connections                                    │
│ - Bi-directional communication                             │
│ - Connection pooling                                       │
│ - Presence tracking                                        │
└────────────────────────────────────────────────────────────┘
            │
    ┌───────┼───────┬────────┬────────┐
    ▼       ▼       ▼        ▼        ▼
┌────────────────────────────────────────────┐
│ Microservices Layer                        │
├────────────────────────────────────────────┤
│                                            │
│ ┌──────────────────┐  ┌──────────────────┐ │
│ │Messaging Service │  │Channel Service   │ │
│ │- Send message    │  │- Create channel  │ │
│ │- Thread replies  │  │- Member manage   │ │
│ │- Edit messages   │  │- Topic/desc      │ │
│ │- Delete messages │  │- Policies        │ │
│ └──────────────────┘  └──────────────────┘ │
│                                            │
│ ┌──────────────────┐  ┌──────────────────┐ │
│ │Search Service    │  │File Service      │ │
│ │- Full-text search│  │- Upload files    │ │
│ │- Filter by user  │  │- Preview gen     │ │
│ │- Filter by date  │  │- Sharing perms   │ │
│ │- Filter by chan  │  │- Virus scanning  │ │
│ └──────────────────┘  └──────────────────┘ │
│                                            │
│ ┌──────────────────┐  ┌──────────────────┐ │
│ │Notification Svc  │  │User Service      │ │
│ │- Direct message  │  │- Profiles        │ │
│ │- Mention notifs  │  │- Presence        │ │
│ │- Thread notifs   │  │- Status updates  │ │
│ │- Settings        │  │- Permissions     │ │
│ └──────────────────┘  └──────────────────┘ │
│                                            │
│ ┌──────────────────┐  ┌──────────────────┐ │
│ │Workspace Svc     │  │Integration Svc   │ │
│ │- Org hierarchy   │  │- Webhooks        │ │
│ │- Admin perms     │  │- Bot API         │ │
│ │- Auditing        │  │- Third-party app │ │
│ │- Retention       │  │- Custom bots     │ │
│ └──────────────────┘  └──────────────────┘ │
│                                            │
└────────────────────────────────────────────┘

DATA LAYER:
┌────────────────────────────────────────────────────────────┐
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐ │
│  │Message DB      │  │Channel DB      │  │Search Index    │ │
│  │(Cassandra/HBase)│ │(PostgreSQL)    │  │(Elasticsearch) │ │
│  │- Messages      │  │- Channels      │  │- Messages      │ │
│  │- Threads       │  │- Membership    │  │- Full-text idx │ │
│  │- Timestamps    │  │- Settings      │  │- Aggregated    │ │
│  └────────────────┘  └────────────────┘  └────────────────┘ │
│                                                              │
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐ │
│  │File Storage    │  │Cache Layer     │  │Analytics DW    │ │
│  │(S3)            │  │(Redis)         │  │(Hadoop/BigQ)   │ │
│  │- User uploads  │  │- Presence      │  │- Usage trends  │ │
│  │- Previews      │  │- Recent msgs   │  │- User behavior │ │
│  │- Expiration    │  │- User prefs    │  │- File access   │ │
│  └────────────────┘  └────────────────┘  └────────────────┘ │
│                                                              │
│  ┌────────────────┐  ┌────────────────┐                     │
│  │Message Queue   │  │Graph DB        │                     │
│  │(Kafka)         │  │(Neo4j)         │                     │
│  │- Message evt   │  │- User relation │                     │
│  │- Search events │  │- App graph     │                     │
│  │- File events   │  │- Org structure │                     │
│  └────────────────┘  └────────────────┘                     │
│                                                              │
└────────────────────────────────────────────────────────────┘
```

---

## Message Delivery Flow

```
SLACK MESSAGE DELIVERY:
═══════════════════════

User A types in #general: "Let's ship it!"

1. COMPOSE & SEND:
   ┌──────────────────────────────────┐
   │ Client (Web/Mobile/Desktop)      │
   │ - User types message             │
   │ - Format: markdown, emojis       │
   │ - Mentions user detection        │
   │ - Sends via WebSocket            │
   └──────────────────────────────────┘

2. MESSAGE SERVICE:
   ┌──────────────────────────────────┐
   │ Validate & Process               │
   │ - Valid channel/user?            │
   │ - Permission check               │
   │ - Spam/profanity filter          │
   │ - Extract mentions               │
   │ - Extract links                  │
   │ - Assign message ID              │
   └──────────────────────────────────┘

3. PERSIST TO DB:
   ┌──────────────────────────────────┐
   │ Cassandra (distributed)          │
   │ - Partitioned by: team_id        │
   │ - Sorted by: timestamp           │
   │ - Replicated: 3x                 │
   │ - TTL: based on plan             │
   │                                  │
   │ Record:                          │
   │ {                                │
   │   msg_id: uuid,                  │
   │   channel_id: xyz,               │
   │   user_id: abc,                  │
   │   text: "Let's ship it!",        │
   │   ts: 1703123456.123             │
   │ }                                │
   └──────────────────────────────────┘

4. PUBLISH TO KAFKA:
   ┌──────────────────────────────────┐
   │ Event: MessagePosted             │
   │ - Message ID                     │
   │ - Channel ID                     │
   │ - User ID                        │
   │ - Timestamp                      │
   │ - Broadcast to subscribers       │
   └──────────────────────────────────┘

5. FANOUT TO RECIPIENTS:
   ┌──────────────────────────────────┐
   │ For each channel member:         │
   │ - Get active RTM connection      │
   │ - Send via WebSocket             │
   │ (if offline: store & deliver     │
   │  when comes online)              │
   │                                  │
   │ Delivery: <50ms to connected     │
   └──────────────────────────────────┘

6. CLIENT RECEIVES:
   ┌──────────────────────────────────┐
   │ User B's client gets message     │
   │ - Parse JSON payload             │
   │ - Update channel view            │
   │ - Scroll to latest               │
   │ - Play notification sound        │
   │ - Update unread count            │
   │ - Fetch user profile if needed   │
   └──────────────────────────────────┘

7. INDEXING FOR SEARCH:
   ┌──────────────────────────────────┐
   │ Search Service (via Kafka event) │
   │ - Index in Elasticsearch         │
   │ - Tokenize text                  │
   │ - Index mentions, links          │
   │ - Make searchable instantly      │
   │ - TTL: full message retention    │
   └──────────────────────────────────┘
```

---

## Message Search

```
SLACK SEARCH QUERY:
═══════════════════

User searches: "when deadline?"

1. QUERY PARSING:
   ┌──────────────────────────────────┐
   │ Parse query syntax               │
   │ - Keywords: "when deadline"      │
   │ - Filters: channel, user, date   │
   │ - Modifiers: in:, from:, after:  │
   │                                  │
   │ Example: "from:alice after:2024" │
   └──────────────────────────────────┘

2. ELASTICSEARCH SEARCH:
   ┌──────────────────────────────────┐
   │ Multi-shard search               │
   │ - Search across all channels     │
   │ - Full-text matching             │
   │ - Fuzzy matching for typos       │
   │ - Result: ~5000 messages         │
   └──────────────────────────────────┘

3. FILTERING & RANKING:
   ┌──────────────────────────────────┐
   │ Apply user permissions           │
   │ - Can user see message?          │
   │ - Private channel check          │
   │ - Archived message check         │
   │                                  │
   │ Rank by:                         │
   │ - Relevance score                │
   │ - Recency (recent preferred)    │
   │ - Engagement (reactions, replies)│
   │ - Result: Top 50                 │
   └──────────────────────────────────┘

4. HYDRATION:
   ┌──────────────────────────────────┐
   │ Fetch full message data          │
   │ - User profile (sender)          │
   │ - Message context (thread)       │
   │ - Reactions/emoji               │
   │ - File attachments               │
   └──────────────────────────────────┘

5. RESPONSE:
   ┌──────────────────────────────────┐
   │ Display results in ~200-400ms    │
   │ - Top 50 matching messages       │
   │ - With context (before/after)    │
   │ - Highlight matching terms       │
   │ - Jump to message in channel     │
   └──────────────────────────────────┘
```

---

## Key Technologies

| Component | Tech | Purpose |
|-----------|------|---------|
| **Messaging** | Cassandra | Message storage |
| **Real-time** | WebSocket, RTM | Live delivery |
| **Search** | Elasticsearch | Message search |
| **Cache** | Redis | Presence, recent |
| **Queue** | Kafka | Event streaming |
| **Storage** | S3 | File uploads |
| **Graph** | Neo4j | Org structure |
| **Analytics** | Hadoop | Usage tracking |

---

## Scalability Mechanisms

1. **Message Storage**
   - Cassandra sharding by team_id
   - Time-series partitioning
   - Multi-region replication

2. **Real-time Delivery**
   - Connection pooling per user
   - WebSocket for bi-directional
   - Graceful fallback to polling

3. **Search Scaling**
   - Elasticsearch sharded by team
   - Incremental indexing via Kafka
   - Search result caching

4. **Presence Tracking**
   - Redis for online status
   - Heartbeat mechanism
   - Update frequency: 10-30 seconds

---

## Interview Questions

### Q1: Design real-time messaging system
**Key points:**
- WebSocket connection management
- Message delivery guarantees
- Offline message handling
- Distributed message queue
- Notification strategy

### Q2: Implement message search across billions
**Key points:**
- Elasticsearch indexing strategy
- Permission-aware search results
- Multi-team isolation
- Search performance optimization
- Index freshness

### Q3: Handle message archive & retention
**Key points:**
- Storage optimization over time
- Archival strategy
- Search on archived messages
- Deletion policies
- Compliance requirements

