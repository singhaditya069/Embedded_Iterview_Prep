# Discord Voice & Text at Scale

## Overview
Discord: 150M+ monthly active users, 6M+ active servers, real-time voice/text, handling sustained concurrent connections at massive scale.

## Key Requirements
- **Scale**: 150M MAU, 6M+ servers
- **Concurrency**: 10M+ simultaneous active users
- **Voice**: Sub-50ms latency for voice channels
- **Messages**: Billions of messages/day
- **Reliability**: 99.99% uptime SLA
- **Bandwidth**: Massive video/voice streaming
- **Cost**: Expensive infrastructure (especially voice)

---

## High-Level Architecture

```
┌────────────────────────────────────────────────────────────┐
│          Discord Voice & Text Architecture                 │
└────────────────────────────────────────────────────────────┘

CLIENT LAYER:
┌────────────────────────────────────────────────────────────┐
│ Desktop App / Mobile / Browser / Console Clients           │
│ - Text messaging (channels)                                │
│ - Voice channels (1000+ users)                             │
│ - Video streaming (camera, screen share)                   │
│ - Real-time presence                                       │
└────────────────────────────────────────────────────────────┘
            │
            ▼
┌────────────────────────────────────────────────────────────┐
│ Distributed Gateway (Edge Servers)                         │
│ - Geo-routed connections                                   │
│ - WebSocket for text/presence                              │
│ - RTP for voice/video                                      │
│ - Connection pooling & multiplexing                        │
└────────────────────────────────────────────────────────────┘
            │
    ┌───────┼───────┬────────┬────────┐
    ▼       ▼       ▼        ▼        ▼
┌────────────────────────────────────────────┐
│ Microservices Layer                        │
├────────────────────────────────────────────┤
│                                            │
│ ┌──────────────────┐  ┌──────────────────┐ │
│ │Message Service   │  │Voice Service     │ │
│ │- Send/receive    │  │- Join voice chan │ │
│ │- Thread replies  │  │- Codec negotiat  │ │
│ │- React with emoji│  │- Mixer process   │ │
│ │- Pin messages    │  │- Quality adapt   │ │
│ └──────────────────┘  └──────────────────┘ │
│                                            │
│ ┌──────────────────┐  ┌──────────────────┐ │
│ │Video Service     │  │Gateway Service   │ │
│ │- Stream screen   │  │- Manage channels │ │
│ │- Camera feed     │  │- Server manage   │ │
│ │- Transcoding     │  │- Role manage     │ │
│ │- Bandwidth adapt │  │- Permissions     │ │
│ └──────────────────┘  └──────────────────┘ │
│                                            │
│ ┌──────────────────┐  ┌──────────────────┐ │
│ │Presence Service  │  │Notification Svc  │ │
│ │- Online/offline  │  │- Direct messages │ │
│ │- Activity status │  │- @mentions       │ │
│ │- Game streaming  │  │- Friend requests │ │
│ │- Rich activity   │  │- Server updates  │ │
│ └──────────────────┘  └──────────────────┘ │
│                                            │
│ ┌──────────────────┐  ┌──────────────────┐ │
│ │User Service      │  │Analytics Service │ │
│ │- Profiles        │  │- Server stats    │ │
│ │- Relationships   │  │- User behavior   │ │
│ │- Settings        │  │- Voice quality   │ │
│ │- Connections     │  │- Performance MP  │ │
│ └──────────────────┘  └──────────────────┘ │
│                                            │
└────────────────────────────────────────────┘

DATA LAYER:
┌────────────────────────────────────────────────────────────┐
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐ │
│  │Message DB      │  │Voice DB        │  │Presence Cache  │ │
│  │(DynamoDB)      │  │(Redis cluster) │  │(Redis)         │ │
│  │- Messages      │  │- VoiceState    │  │- Online status │ │
│  │- Channel refs  │  │- Channel stream│  │- Activity      │ │
│  │- Delete history│  │- Session info  │  │- Last seen     │ │
│  └────────────────┘  └────────────────┘  └────────────────┘ │
│                                                              │
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐ │
│  │User DB         │  │Server Metadata │  │Connection Pool │ │
│  │(PostgreSQL)    │  │(PostgreSQL)    │  │(Redis)         │ │
│  │- User profiles │  │- Server config │  │- Gateway conns │ │
│  │- Relationships │  │- Members list  │  │- Established   │ │
│  │- Settings      │  │- Roles/perms   │  │- Tracking      │ │
│  └────────────────┘  └────────────────┘  └────────────────┘ │
│                                                              │
│  ┌────────────────┐  ┌────────────────┐                     │
│  │Event Queue     │  │Analytics DW    │                     │
│  │(Kafka)         │  │(Redshift)      │                     │
│  │- Message events│  │- Server growth │                     │
│  │- Voice events  │  │- User activity │                     │
│  │- Presence evt  │  │- Network perf  │                     │
│  └────────────────┘  └────────────────┘                     │
│                                                              │
└────────────────────────────────────────────────────────────┘
```

---

## Voice Channel Connection Flow

```
DISCORD VOICE CONNECTION:
═════════════════════════

User clicks "Join Voice Channel":

1. REQUEST JOIN:
   ┌──────────────────────────────────┐
   │ Client → Websocket Gateway       │
   │ Event: VOICE_STATE_UPDATE        │
   │ {                                │
   │   guild_id: server_id,           │
   │   channel_id: voice_channel_id,  │
   │   self_mute: false,              │
   │   self_deaf: false               │
   │ }                                │
   └──────────────────────────────────┘

2. GATEWAY VALIDATION:
   ┌──────────────────────────────────┐
   │ Validate permissions             │
   │ - User in server?                │
   │ - Can access channel?            │
   │ - Server capacity? (10K+ users)  │
   │ - User not banned?               │
   └──────────────────────────────────┘

3. VOICE SERVER ALLOCATION:
   ┌──────────────────────────────────┐
   │ Select best voice server:        │
   │ - Geographic proximity           │
   │ - Current load                   │
   │ - Codec support                  │
   │ - Network quality                │
   │                                  │
   │ Send voice_server_update to      │
   │ client with:                     │
   │ - Server endpoint IP             │
   │ - Session token                  │
   │ - Expires in: 15 seconds         │
   └──────────────────────────────────┘

4. UDP ENCRYPTION SETUP:
   ┌──────────────────────────────────┐
   │ Discord Voice Protocol           │
   │ - Establish UDP connection       │
   │ - IP Discovery (get public IP)   │
   │ - Generate Nonce                 │
   │ - Encrypt connection (ChaCha20)  │
   │ - Ready to send voice            │
   └──────────────────────────────────┘

5. VOICE STATE BROADCAST:
   ┌──────────────────────────────────┐
   │ Notify all channel members       │
   │ Event: VOICE_STATE_UPDATE        │
   │ {                                │
   │   user_id: new_member_id,        │
   │   channel_id: voice_channel_id,  │
   │   session_id: xyz                │
   │ }                                │
   │                                  │
   │ All connected members see:       │
   │ "New user joined voice channel"  │
   └──────────────────────────────────┘

6. VOICE STREAMING:
   ┌──────────────────────────────────┐
   │ Real-time voice RTP packets      │
   │ - Codec: Opus (20ms frames)      │
   │ - Bitrate: 16-128 kbps           │
   │ - Voice Activity Detection       │
   │ - Echo cancellation              │
   │ - Noise suppression              │
   │ - Latency: <50ms target          │
   └──────────────────────────────────┘

7. AUDIO MIXING:
   ┌──────────────────────────────────┐
   │ For each user in channel:        │
   │ - Receive voice packets          │
   │ - Decompress Opus                │
   │ - Mix together (sum audio)       │
   │ - Compress to Opus               │
   │ - Send to all members            │
   │                                  │
   │ Fallback for 1000+ users:        │
   │ - Broadcast mode (no mixing)     │
   │ - Users hear selective stream    │
   │ - Reduces CPU load               │
   └──────────────────────────────────┘
```

---

## Presence & Online Status

```
PRESENCE TRACKING:
═══════════════════

User connects to Discord:

1. SEND PRESENCE:
   ┌──────────────────────────────────┐
   │ Client → WebSocket Gateway       │
   │ Event: PRESENCE_UPDATE           │
   │ {                                │
   │   status: "online",              │
   │   activities: [{                 │
   │     type: "game",                │
   │     name: "Elden Ring"           │
   │   }]                             │
   │ }                                │
   └──────────────────────────────────┘

2. STORE IN CACHE:
   ┌──────────────────────────────────┐
   │ Redis cluster               │
   │ Key: "presence:user_id"     │
   │ Value: {status, activity}   │
   │ TTL: 5 minutes              │
   │ (Heartbeat resets)          │
   └──────────────────────────────────┘

3. BROADCAST TO FRIENDS:
   ┌──────────────────────────────────┐
   │ Get user's friends list          │
   │ For each friend connection:      │
   │ - If online: send update via     │
   │   WebSocket                      │
   │                                  │
   │ Friend sees:                     │
   │ "User is now playing Elden Ring" │
   └──────────────────────────────────┘

4. OPTIMIZATION FOR SCALE:
   ┌──────────────────────────────────┐
   │ For 150M users:                  │
   │ - Batch presence updates         │
   │ - Don't send every heartbeat     │
   │ - Aggregate deltas every 5 sec   │
   │ - Compress presence changes      │
   │ - Send only changes (not full)   │
   └──────────────────────────────────┘

5. OFFLINE DETECTION:
   ┌──────────────────────────────────┐
   │ If heartbeat not received:       │
   │ - TTL expires from Redis         │
   │ - Mark as offline                │
   │ - Broadcast offline to friends   │
   │ - Clean up session               │
   └──────────────────────────────────┘
```

---

## Key Technologies

| Component | Tech | Purpose |
|-----------|------|---------|
| **Gateway** | Custom WebSocket | Real-time events |
| **Voice** | RTP/UDP | Voice streaming |
| **Codec** | Opus | Audio compression |
| **Database** | DynamoDB, PostgreSQL | Data storage |
| **Cache** | Redis cluster | Presence, state |
| **Queue** | Kafka | Event streaming |
| **Analytics** | Redshift | Usage tracking |
| **Encryption** | ChaCha20 | Voice security |

---

## Scalability Mechanisms

1. **Voice Server**
   - Geo-distributed voice servers
   - Load balancing per region
   - Packet loss recovery
   - Codec negotiation

2. **Presence Caching**
   - Redis cluster for global presence
   - TTL-based expiration
   - Batch updates every 5 seconds
   - Delta compression

3. **Message Storage**
   - DynamoDB sharding by server_id
   - Message history retention
   - Efficient pagination

4. **Bandwidth Optimization**
   - Opus codec (low bitrate)
   - Voice Activity Detection
   - Selective streaming for large channels
   - Packet loss concealment

---

## Interview Questions

### Q1: Design voice channel at scale
**Key points:**
- Audio codec selection
- Voice server allocation
- Mixing strategy for 1000+ users
- Bandwidth optimization
- Latency requirements

### Q2: Implement presence system for 150M users
**Key points:**
- Presence cache architecture
- Heartbeat mechanism
- Broadcasting updates
- TTL and expiration
- Friend list optimization

### Q3: Build real-time message synchronization
**Key points:**
- Message delivery guarantees
- Ordering semantics
- Offline sync
- Cache invalidation
- Scalability across millions

