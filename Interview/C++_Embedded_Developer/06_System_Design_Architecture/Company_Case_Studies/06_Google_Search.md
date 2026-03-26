# Google Search & Infrastructure

## Overview
Google: 8.5+ billion searches/day, 99.9%+ uptime, serves results in <100ms latency, needs to handle unprecedented scale in information retrieval.

## Key Requirements
- **Scale**: 8.5B+ searches/day, 100K+ QPS
- **Index Size**: Trillions of web pages
- **Latency**: <100ms for search results
- **Availability**: 99.99% uptime SLA
- **Freshness**: Index updates within hours/days
- **Ranked Results**: Relevant result ordering crucial
- **Language Support**: 100+ languages

---

## High-Level Architecture

```
┌──────────────────────────────────────────────────────────────────┐
│            Google Search Infrastructure                          │
└──────────────────────────────────────────────────────────────────┘

CLIENT LAYER:
┌──────────────────────────────────────────────────────────────────┐
│ Search Box, Browser, Mobile, Voice Assistant                     │
└──────────────────────────────────────────────────────────────────┘
            │
            ▼
┌──────────────────────────────────────────────────────────────────┐
│ Global Load Balancer (Geo-distributed Edge)                      │
│ - Route to nearest search cluster                                │
│ - Query deduplication                                            │
│ - SafeSearch filtering                                           │
└──────────────────────────────────────────────────────────────────┘
            │
        ┌───┴───────────┬──────────┐
        ▼               ▼          ▼
    ┌─────────────┐  ┌──────────┐  ┌────────────┐
    │Query Parser │  │Spell     │  │Language    │
    │- Tokenize   │  │Corrector │  │Detector    │
    │- Intent     │  │- Typos   │  │- 100+ langs│
    │- Operators  │  │- Suggest │  │            │
    └─────────────┘  └──────────┘  └────────────┘
            │
            ▼
    ┌─────────────────────────────────────┐
    │ Index Query Service                 │
    │ - Distributed index lookup          │
    │ - Multiple shards (1000s)           │
    │ - Merge results from shards         │
    │ - Apply ranking algorithms          │
    └─────────────────────────────────────┘
            │
    ┌───────┼───────┬───────┐
    ▼       ▼       ▼       ▼
 ┌───────────────────────────────────────┐
 │ Specialized Search Services           │
 ├───────────────────────────────────────┤
 │ - News search (real-time index)       │
 │ - Image search (visual index)         │
 │ - Video search (YouTube integration)  │
 │ - Maps integration                    │
 │ - Knowledge graph (entity search)     │
 │ - Shopping search (product index)     │
 │ - Scholar search (academic papers)    │
 └───────────────────────────────────────┘

RANKING LAYER:
┌──────────────────────────────────────────────────────────────────┐
│                                                                  │
│ ┌────────────────────────────────────────────────┐              │
│ │ Ranking Service                                │              │
│ │ - PageRank algorithm (link analysis)           │              │
│ │ - RankBrain (ML-based ranking)                 │              │
│ │ - E-A-T signals (Expertise, Authoritativeness)│              │
│ │ - Link popularity, domain authority            │              │
│ │ - Click-through rate from search history       │              │
│ │ - BERT NLP understanding                       │              │
│ └────────────────────────────────────────────────┘              │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘
            │
            ▼
┌──────────────────────────────────────────────────────────────────┐
│ Result Hydration & Formatting                                    │
│ - Snippets generation                                            │
│ - Rich snippets (star ratings, prices, etc)                      │
│ - URL display optimization                                       │
│ - Knowledge box assembly                                         │
│ - Ad integration                                                 │
└──────────────────────────────────────────────────────────────────┘
            │
            ▼
Client receives 10 results in ~50-100ms
```

---

## Web Crawling & Indexing Pipeline

```
CRAWLING & INDEXING FLOW:
═════════════════════════

1. SEED URL COLLECTION:
   ┌───────────────────────────────────┐
   │ URL Frontier                      │
   │ - Known URLs from previous crawl  │
   │ - User submissions                │
   │ - XML sitemaps                    │
   │ - RSS feeds                       │
   │ Priority queue by:                │
   │ - PageRank                        │
   │ - Update frequency                │
   │ - Importance                      │
   └───────────────────────────────────┘

2. DISTRIBUTED CRAWLING:
   ┌───────────────────────────────────┐
   │ Crawler Service (1000s of nodes)  │
   │ - Fetch pages (respecting robots) │
   │ - Extract links                   │
   │ - Detect language                 │
   │ - Store raw HTML                  │
   │ - Parallel fetching               │
   │ - 100+ billion pages/day          │
   └───────────────────────────────────┘
                │
                ▼
   ┌───────────────────────────────────┐
   │ Content Processing                │
   │ - Parse HTML                      │
   │ - Extract text                    │
   │ - Remove boilerplate              │
   │ - Identify title, description     │
   │ - Extract structured data         │
   └───────────────────────────────────┘

3. INDEXING:
   ┌───────────────────────────────────┐
   │ Indexer Service                   │
   │ - Tokenize content                │
   │ - Build inverted index            │
   │ - Compute PageRank                │
   │ - Generate snippets               │
   │ - Extract entities (NER)          │
   └───────────────────────────────────┘
                │
    ┌───────────┼───────────┐
    ▼           ▼           ▼
 ┌─────────┐ ┌──────────┐ ┌──────────┐
 │Forward  │ │Inverted  │ │Link      │
 │Index    │ │Index     │ │Graph     │
 │         │ │          │ │          │
 │docID →  │ │word →    │ │Link      │
 │words    │ │docIDs    │ │analysis  │
 └─────────┘ └──────────┘ └──────────┘

4. INDEX SHARDING:
   ┌───────────────────────────────────┐
   │ ~1000 Shards by docID             │
   │ - Each shard: 1-2 billion docs    │
   │ - Replicated 3x for availability  │
   │ - Distributed geographically      │
   │ - Query all shards in parallel    │
   └───────────────────────────────────┘

5. RANKING COMPUTATION (Offline):
   ┌───────────────────────────────────┐
   │ PageRank & ML Scoring             │
   │ - Compute PageRank (weeks job)    │
   │ - Train ML ranking models         │
   │ - Store scores in index           │
   │ - Update periodically             │
   └───────────────────────────────────┘
```

---

## Search Query Processing

```
QUERY FLOW:
═══════════

User types: "best restaurants near me"

1. AUTO-COMPLETION:
   ┌──────────────────────────────────┐
   │ Suggestion Service (cached)      │
   │ - Popular queries                │
   │ - User search history            │
   │ - Trending terms                 │
   │ - Personalized suggestions       │
   │ - <50ms latency                  │
   └──────────────────────────────────┘

2. QUERY UNDERSTANDING:
   ┌──────────────────────────────────┐
   │ NLP & Intent Recognition         │
   │ - Entity extraction:             │
   │   "restaurants" = ENTITY.PLACE   │
   │   "near me" = LOCATION.CURRENT   │
   │ - Intent: LOCAL_SEARCH           │
   │ - Query expansion:               │
   │   - synonyms: "cafe", "eatery"   │
   │   - related: cuisine type        │
   └──────────────────────────────────┘

3. DISTRIBUTED SEARCH:
   ┌──────────────────────────────────┐
   │ Launch queries to 1000 shards    │
   │ - Each shard searches locally    │
   │ - Returns top 100 results        │
   │ - Parallel timeout: 80ms         │
   └──────────────────────────────────┘
                │
    ┌───────────┼───────────┬───────┐
    ▼           ▼           ▼       ▼
 100 results 100 results 100 results
 from shard1 from shard2 from shard3

4. RESULT MERGING:
   ┌──────────────────────────────────┐
   │ Merge Service                    │
   │ - Deduplicate results            │
   │ - Merge-sort by score            │
   │ - Keep top 1000 candidates       │
   └──────────────────────────────────┘

5. RE-RANKING:
   ┌──────────────────────────────────┐
   │ RankBrain (ML Model)             │
   │ - Neural ranking model           │
   │ - Considers: user context,       │
   │              search history,     │
   │              page quality,       │
   │              freshness           │
   │ - Top 10 selected                │
   └──────────────────────────────────┘

6. RESULT ENRICHMENT:
   ┌──────────────────────────────────┐
   │ Knowledge Services               │
   │ - Local search integration       │
   │ - Maps/directions                │
   │ - Reviews from Google Maps       │
   │ - Image thumbnails               │
   │ - Rich snippets                  │
   │ - Ads insertion                  │
   └──────────────────────────────────┘

7. RESPONSE:
   ┌──────────────────────────────────┐
   │ Format for client                │
   │ - JSON response                  │
   │ - Snippet generation             │
   │ - Tracking URLs                  │
   │ - Sent in ~100-150ms             │
   └──────────────────────────────────┘
```

---

## Key Technologies

| Component | Tech | Purpose |
|-----------|------|---------|
| **Crawling** | Custom spider | 100B+ pages/day |
| **Storage** | BigTable, Spanner | Index storage |
| **Search** | Custom engine | Distributed queries |
| **Ranking** | PageRank, ML | Result ordering |
| **Language Processing** | BERT, NLP | Understanding |
| **Caching** | Memcached | Query results cache |
| **Analytics** | BigQuery | Query analysis |
| **ML** | TensorFlow | RankBrain models |

---

## Scalability Mechanisms

1. **Index Sharding**
   - 1000s of shards distributed globally
   - Each shard independent
   - Parallel query fanout

2. **Crawling at Scale**
   - 1000s of crawlers
   - Respect robots.txt
   - Polite delays per domain
   - 100B+ pages crawled daily

3. **Caching Strategy**
   - Cache popular query results
   - Cache in edge locations
   - User-personalized cache

4. **Real-time Index Updates**
   - Fresh content index (minutes)
   - Regular index (days)
   - Mobile-first indexing

---

## Interview Questions

### Q1: Design web search engine
**Key points:**
- Crawling strategy
- Index structure & sharding
- Query processing pipeline
- Ranking algorithm
- Distributed scoring

### Q2: Implement PageRank algorithm
**Key points:**
- Link graph representation
- Iterative computation
- Convergence detection
- Scalability at billions of pages

### Q3: Handle real-time search results
**Key points:**
- Fresh content indexing
- Eventual consistency
- Cache invalidation
- Query latency optimization

