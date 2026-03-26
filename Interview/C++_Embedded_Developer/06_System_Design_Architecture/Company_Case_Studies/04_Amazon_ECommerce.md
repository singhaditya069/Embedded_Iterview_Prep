# Amazon E-Commerce Platform Architecture

## Overview
Amazon: 300+ million users, 100+ million SKUs, 2M+ orders/day, globally distributed infrastructure.

## Key Requirements
- **Scale**: 300M users, 100M+ products
- **Orders**: 2M+ orders per day (peaks during sales)
- **Latency**: <100ms product search, <50ms add-to-cart
- **Availability**: 99.99% uptime SLA
- **Search**: Instant search across billions of products
- **Payment**: PCI-DSS compliance
- **Inventory**: Real-time stock levels

---

## High-Level Architecture

```
┌──────────────────────────────────────────────────────────────────┐
│                    Amazon E-Commerce Architecture                │
└──────────────────────────────────────────────────────────────────┘

CLIENT LAYER:
┌──────────────────────────────────────────────────────────────────┐
│ Web Browser / Mobile App / Checkout                              │
└──────────────────────────────────────────────────────────────────┘
            │
            ▼
┌──────────────────────────────────────────────────────────────────┐
│ CloudFront CDN / API Gateway                                     │
│ - Global distribution (200+ edge locations)                      │
│ - Request routing, rate limiting                                 │
└──────────────────────────────────────────────────────────────────┘
            │
    ┌───────┼───────┬───────┬───────┐
    ▼       ▼       ▼       ▼       ▼
┌─────┐ ┌──────┐ ┌────┐ ┌──────┐ ┌────────┐
│Product│Search │Cart │Orders │Payment │
│Service│Service│Svc  │Svc    │Service │
└─────┘ └──────┘ └────┘ └──────┘ └────────┘

MICROSERVICES LAYER:
┌──────────────────────────────────────────────────────────────────┐
│                                                                  │
│ ┌────────────────────────┐  ┌────────────────────────┐          │
│ │ Product Catalog Service│  │ Inventory Service      │          │
│ │ - Product metadata     │  │ - Stock levels         │          │
│ │ - Pricing              │  │ - Warehouse tracking   │          │
│ │ - Descriptions         │  │ - Reorder points       │          │
│ │ - Images & reviews     │  │ - Multi-warehouse sync │          │
│ └────────────────────────┘  └────────────────────────┘          │
│                                                                  │
│ ┌────────────────────────┐  ┌────────────────────────┐          │
│ │ Shopping Cart Service  │  │ Order Management Svc   │          │
│ │ - Persistent cart      │  │ - Order creation       │          │
│ │ - Session management   │  │ - Order tracking       │          │
│ │ - Recommendations      │  │ - Status updates       │          │
│ │ - Cart recovery        │  │ - Cancellation mgmt    │          │
│ └────────────────────────┘  └────────────────────────┘          │
│                                                                  │
│ ┌────────────────────────┐  ┌────────────────────────┐          │
│ │ Payment Processing     │  │ Shipping & Logistics   │          │
│ │ - PCI compliance       │  │ - Carrier integration  │          │
│ │ - Multiple payment     │  │ - Route optimization   │          │
│ │   methods              │  │ - Tracking             │          │
│ │ - Fraud detection      │  │ - Delivery estimates   │          │
│ └────────────────────────┘  └────────────────────────┘          │
│                                                                  │
└──────────────────────────────────────────────────────────────────┘

DATA LAYER:
┌──────────────────────────────────────────────────────────────────┐
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐     │
│  │Product DB      │  │Cache Layer     │  │Message Queue   │     │
│  │(RDS/Aurora)    │  │(ElastiCache)   │  │(SQS/SNS)       │     │
│  │- SKUs          │  │- Hot products  │  │- Orders        │     │
│  │- Pricing       │  │- User sessions │  │- Inventory     │     │
│  │- Descriptions  │  │- Carts         │  │- Events        │     │
│  └────────────────┘  └────────────────┘  └────────────────┘     │
│                                                                  │
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐     │
│  │Search Index    │  │Warehouse DB    │  │Analytics DW    │     │
│  │(Elasticsearch) │  │(DynamoDB)      │  │(Redshift)      │     │
│  │- Products      │  │- Stock by      │  │- Sales trends  │     │
│  │- Facets        │  │  warehouse     │  │- User behavior │     │
│  │- Real-time     │  │- Fulfillment   │  │- Recommendations│     │
│  └────────────────┘  └────────────────┘  └────────────────┘     │
│                                                                  │
│  ┌────────────────┐  ┌────────────────┐  ┌────────────────┐     │
│  │Object Storage  │  │Recommendation  │  │User Profiles   │     │
│  │(S3)            │  │DB (DynamoDB)   │  │(DynamoDB)      │     │
│  │- Product images│  │- User history  │  │- Preferences   │     │
│  │- Reviews       │  │- Affinities    │  │- Wishlist      │     │
│  │- Documents     │  │- Scores        │  │- Subscriptions │     │
│  └────────────────┘  └────────────────┘  └────────────────┘     │
└──────────────────────────────────────────────────────────────────┘
```

---

## Product Search Data Flow

```
PRODUCT SEARCH FLOW:
════════════════════

User searches "USB-C Cable":
    │
    ▼
┌─────────────────────────────────────┐
│ Search API                          │
│ - Receive query                     │
│ - Input validation                  │
│ - Rate limiting                     │
└─────────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────────┐
│ Query Processing                    │
│ - Spell check                       │
│ - Synonym expansion                 │
│ - Stop word removal                 │
│ - Categorization                    │
└─────────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────────┐
│ Elasticsearch Search                │
│ - Sharded indices (~100s)           │
│ - Filters by category               │
│ - ~10,000 matching products         │
└─────────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────────┐
│ Ranking & Filtering                 │
│ - Relevance scoring                 │
│ - Availability filter               │
│ - Price filters (user set)          │
│ - Rating filters                    │
│ - Prime eligibility                 │
│ - Top 100 candidates                │
└─────────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────────┐
│ Real-time Pricing & Availability    │
│ - Query inventory service           │
│ - Current pricing (dynamic)         │
│ - Stock status                      │
└─────────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────────┐
│ Personalization                     │
│ - User browsing history             │
│ - Wishlist items                    │
│ - Recommended products              │
│ - Personalized pricing              │
└─────────────────────────────────────┘
    │
    ▼
┌─────────────────────────────────────┐
│ Results Ranking                     │
│ - ML-based ranking                  │
│ - Sponsored products placement      │
│ - A/B testing different orders      │
│ - CTR prediction                    │
└─────────────────────────────────────┘
    │
    ▼
Client displays results (50-100ms)
```

---

## Order Processing Flow

```
ORDER WORKFLOW:
═══════════════

1. ADD TO CART:
   ┌──────────────────────────────────────┐
   │ Cart Service                         │
   │ - Check inventory                    │
   │ - Persist to DynamoDB                │
   │ - Update cache                       │
   │ - Calculate price                    │
   └──────────────────────────────────────┘
                │
                ▼
   ~10ms response (cached)

2. CHECKOUT:
   ┌──────────────────────────────────────┐
   │ Checkout Service                     │
   │ - Validate cart items                │
   │ - Check current prices               │
   │ - Verify inventory                   │
   │ - Apply promotions                   │
   │ - Calculate shipping                 │
   │ - Generate order preview             │
   └──────────────────────────────────────┘

3. PAYMENT:
   ┌──────────────────────────────────────┐
   │ Payment Service                      │
   │ - Payment detail encryption          │
   │ - Fraud detection ML model           │
   │ - Call payment processor             │
   │ - Verify transaction                 │
   │ - Store receipt                      │
   └──────────────────────────────────────┘
                │
        ┌───────┴───────┐
        ▼               ▼
   Success        Failure
   │              │
   ▼              ▼
   │         Handle fraud/
   │         retry logic

4. ORDER CREATION:
   ┌──────────────────────────────────────┐
   │ Order Service                        │
   │ - Create order record                │
   │ - Get order ID                       │
   │ - Reserve inventory                  │
   │ - Send confirmation email            │
   │ - Publish order event                │
   └──────────────────────────────────────┘
                │
                ▼
   ┌──────────────────────────────────────┐
   │ SQS/SNS Events                       │
   ├──────────────────────────────────────┤
   │ - Warehouse fulfillment system       │
   │ - Inventory decrement                │
   │ - Shipping system                    │
   │ - Analytics pipeline                 │
   │ - Recommendation engine update       │
   └──────────────────────────────────────┘

5. FULFILLMENT:
   ┌──────────────────────────────────────┐
   │ Warehouse Management System          │
   │ - Assign to warehouse                │
   │ - Pick items                         │
   │ - Pack                               │
   │ - Label with tracking                │
   │ - Handoff to carrier                 │
   └──────────────────────────────────────┘
                │
                ▼
   ┌──────────────────────────────────────┐
   │ Shipping Integration                 │
   │ - Call carrier API                   │
   │ - Get tracking number                │
   │ - Update order status                │
   │ - Send tracking email                │
   └──────────────────────────────────────┘
                │
                ▼
   User receives package
   Marks as delivered
```

---

## Key Technologies

| Component | Tech | Purpose |
|-----------|------|---------|
| **Frontend** | React, Vue | Client-side rendering |
| **API** | REST, GraphQL | Backend APIs |
| **Compute** | EC2, Lambda | Microservices |
| **Database** | RDS, DynamoDB | Data storage |
| **Search** | Elasticsearch | Product search |
| **Cache** | ElastiCache | Performance |
| **Storage** | S3 | Object storage |
| **Messaging** | SQS, SNS | Async events |
| **Data Warehouse** | Redshift | Analytics |
| **ML** | SageMaker | Recommendations |
| **Payment** | Custom PCI-DSS | Secure payments |
| **Monitoring** | CloudWatch | Observability |

---

## Scalability Mechanisms

1. **Product Catalog**
   - Elasticsearch sharding by category
   - ~100M+ SKUs indexed
   - Real-time indexing

2. **Checkout Process**
   - Stateless order service
   - Distributed inventory locking
   - Payment processor parallelization

3. **Order Processing**
   - SQS queue for queuing
   - Lambda for processing jobs
   - Retry logic with exponential backoff

4. **Inventory Management**
   - Cache warehouse stock locally
   - Event-driven inventory updates
   - Predictive reordering

---

## Interview Questions

### Q1: Design Amazon product search
**Key considerations:**
- Index hundreds of millions of products
- <100ms search latency
- Real-time inventory integration
- Ranking algorithm (relevance + business metrics)
- Faceted search and filtering

### Q2: Implement order processing system
**Key considerations:**
- ACID transactions for payments
- Inventory reservation strategy
- Failure handling & idempotency
- Notification system
- Analytics integration

### Q3: Scale to handle peak traffic (cyber sales)
**Key considerations:**
- Cache strategy
- Load balancing
- Database optimization
- Queue management
- Graceful degradation

