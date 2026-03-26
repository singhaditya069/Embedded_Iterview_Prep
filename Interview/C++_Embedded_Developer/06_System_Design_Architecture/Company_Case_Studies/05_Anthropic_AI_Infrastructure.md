# Anthropic AI/ML Infrastructure

## Overview
Anthropic: AI research company, millions of inference requests/day, serving multi-billion parameter models, handling complex prompts with thousands of tokens.

## Key Requirements
- **Scale**: Millions of inference requests/day
- **Throughput**: Sub-second response times
- **Model Size**: Multi-billion parameters
- **Context**: Support up to 100K+ token contexts
- **Latency**: <2 seconds for typical requests
- **Availability**: 99.95% uptime for API
- **Cost**: GPU efficiency critical (expensive resource)

---

## High-Level Architecture

```
┌──────────────────────────────────────────────────────────────┐
│          Anthropic LLM Serving Infrastructure                │
└──────────────────────────────────────────────────────────────┘

CLIENT LAYER:
┌──────────────────────────────────────────────────────────────┐
│ Web API / SDK / Third-party Apps                             │
│ - REST endpoints / gRPC                                      │
│ - WebSocket for streaming                                   │
└──────────────────────────────────────────────────────────────┘
            │
            ▼
┌──────────────────────────────────────────────────────────────┐
│ API Gateway & Load Balancer                                  │
│ - Rate limiting (tokens/min, requests/min)                   │
│ - Auth & API key validation                                  │
│ - Request queuing                                            │
│ - Cost tracking                                              │
└──────────────────────────────────────────────────────────────┘
            │
        ┌───┴───┬───┬───┐
        ▼       ▼   ▼   ▼
    Processing  Batch Routing
    Queue       Queue  Engine
        │       │      │
        ▼       ▼      ▼
    ┌─────────────────────────────┐
    │ Request Router Service      │
    │ - Route by model size       │
    │ - Route by priority         │
    │ - Load balancing            │
    │ - Batch vs streaming        │
    └─────────────────────────────┘
            │
    ┌───────┼───────┬───────┐
    ▼       ▼       ▼       ▼
    ┌────────────────────────────────────────┐
    │   GPU Inference Clusters               │
    ├────────────────────────────────────────┤
    │                                        │
    │ ┌──────────────────────────────────┐  │
    │ │ Model Server A (70B params)      │  │
    │ │ - Load 70B model into GPU VRAM   │  │
    │ │ - Batch inference (16-64 reqs)   │  │
    │ │ - Token generation loop          │  │
    │ │ - Multi-GPU sharding             │  │
    │ └──────────────────────────────────┘  │
    │                                        │
    │ ┌──────────────────────────────────┐  │
    │ │ Model Server B (13B params)      │  │
    │ │ - Load 13B model                 │  │
    │ │ - Higher throughput              │  │
    │ │ - Lower latency                  │  │
    │ └──────────────────────────────────┘  │
    │                                        │
    │ ┌──────────────────────────────────┐  │
    │ │ Token Buffer Management          │  │
    │ │ - KV cache                       │  │
    │ │ - Attention mask computation     │  │
    │ │ - Multi-instance paging          │  │
    │ └──────────────────────────────────┘  │
    │                                        │
    └────────────────────────────────────────┘
            │
    ┌───────┼───────┐
    ▼       ▼       ▼
┌────────────────────────────────────┐
│ Monitoring & Observability         │
│ - GPU utilization tracking         │
│ - Token generation metrics         │
│ - Latency percentiles              │
│ - Error rates                      │
└────────────────────────────────────┘
```

---

## Inference Request Flow

```
INFERENCE REQUEST LIFECYCLE:
═════════════════════════════

1. CLIENT REQUEST:
   ┌─────────────────────────────────────────┐
   │ User sends prompt via API                │
   │ "What is machine learning?"              │
   └─────────────────────────────────────────┘
                │
                ▼
   ┌─────────────────────────────────────────┐
   │ API Validation                          │
   │ - Check API key                         │
   │ - Validate request format               │
   │ - Enforce rate limits                   │
   │ - Estimate token cost                   │
   └─────────────────────────────────────────┘

2. TOKENIZATION & QUEUING:
   ┌─────────────────────────────────────────┐
   │ Pre-processing Service                  │
   │ - Tokenize prompt                       │
   │ - Apply system instructions             │
   │ - Count input tokens                    │
   │ - Extract model parameters              │
   │   (temperature, max_tokens, etc)        │
   └─────────────────────────────────────────┘
                │
                ▼
   ┌─────────────────────────────────────────┐
   │ Priority Queue                          │
   │ - Priority: GPT-3 > GPT-4               │
   │ - FIFO within priority                  │
   │ - Max queue size enforcement            │
   └─────────────────────────────────────────┘

3. BATCHING LOGIC:
   ┌─────────────────────────────────────────┐
   │ Request Batching Engine                 │
   │ - Collect 16-64 requests                │
   │ - Wait up to 50ms for batch             │
   │ - Pad to uniform length                 │
   │ - Create attention masks                │
   │ - Allocate KV cache                     │
   └─────────────────────────────────────────┘

4. INFERENCE (on GPU):
   ┌─────────────────────────────────────────┐
   │ Token Generation Loop                   │
   │                                         │
   │ Position 0 (first token):               │
   │ - Load model weights (if first batch)   │
   │ - Forward pass: embed + transformer     │
   │ - Cache KV for next position            │
   │ - Logits to probability dist            │
   │ - Sample next token (temp, top_k)       │
   │                                         │
   │ Position 1..N (subsequent tokens):      │
   │ - Reuse KV cache                        │
   │ - Only attend to new position           │
   │ - Much faster than position 0           │
   │                                         │
   │ Stop when: EOS token OR max length      │
   └─────────────────────────────────────────┘

5. STREAMING TO CLIENT:
   ┌─────────────────────────────────────────┐
   │ Response Streaming                      │
   │ - Stream tokens as generated            │
   │ - Send via WebSocket/Server-Sent-Events│
   │ - Client sees real-time output          │
   │ - ~50-100ms chunks                      │
   └─────────────────────────────────────────┘

Example token by token:
"What" → "is" → "machine" → "learning?"
→ "Machine..." → "learning..." → "is..."

6. POST-PROCESSING:
   ┌─────────────────────────────────────────┐
   │ Response Completion                     │
   │ - Count output tokens                   │
   │ - Calculate cost                        │
   │ - Log to analytics                      │
   │ - Send final metadata                   │
   └─────────────────────────────────────────┘
```

---

## Model Serving Architecture

```
GPU CLUSTER ORGANIZATION:
═════════════════════════

┌────────────────────────────────────────────┐
│ Single GPU Node (Hardware)                 │
├────────────────────────────────────────────┤
│                                            │
│ GPU Memory Layout:                         │
│ ┌──────────────────────────────────────┐   │
│ │ Model Weights: 140GB (70B params)    │   │
│ │ (~2 bytes per parameter)             │   │
│ └──────────────────────────────────────┘   │
│ ┌──────────────────────────────────────┐   │
│ │ KV Cache (Batch of 64)               │   │
│ │ - Keys: (seq_len, batch, d_model)    │   │
│ │ - Values: (seq_len, batch, d_model)   │   │
│ │ Total: ~25GB for 100k tokens         │   │
│ └──────────────────────────────────────┘   │
│ ┌──────────────────────────────────────┐   │
│ │ Activations & Temp Buffers           │   │
│ │ - During forward pass                │   │
│ │ - ~5-10GB                            │   │
│ └──────────────────────────────────────┘   │
│                                            │
│ Total: 320GB (8x 40GB H100 GPUs)          │
│                                            │
└────────────────────────────────────────────┘

MULTI-GPU SHARDING:
═══════════════════

For 70B parameter model on 8 H100s:

GPU 0: Layers 0-8    (Transformer blocks)
GPU 1: Layers 9-17   (Transformer blocks)
GPU 2: Layers 18-26  (Transformer blocks)
GPU 3: Layers 27-35  (Transformer blocks)
GPU 4: Layers 36-44  (Transformer blocks)
GPU 5: Layers 45-53  (Transformer blocks)
GPU 6: Layers 54-62  (Transformer blocks)
GPU 7: Layers 63-70 + Output layer

All-reduce operations for hidden states:
GPU0 → GPU1 (pass hidden state)
GPU1 → GPU2 (pass hidden state)
... (pipeline parallelism)

TENSOR PARALLELISM (Column sharding):
Transformer layer split across GPUs:
Matrix ops: (seq_len, d_model) x (d_model, d_ff)
GPU0 handles d_model//8 columns
GPU1 handles next d_model//8 columns
All-reduce to combine outputs
```

---

## Key Technologies

| Component | Tech | Purpose |
|-----------|------|---------|
| **Framework** | PyTorch, JAX | Model development |
| **Server** | vLLM, TensorRT-LLM | Inference optimization |
| **Hardware** | H100, A100 | GPU compute |
| **Scheduling** | Ray, Kubernetes | Request distribution |
| **Monitoring** | Prometheus, Grafana | Performance tracking |
| **Caching** | Redis, local KV | Response caching |
| **Database** | DynamoDB, PostgreSQL | Logs & metrics |
| **Inference** | CUDA, cuBLAS | Low-level ops |

---

## Optimization Techniques

1. **KV Cache Reuse**
   - Avoid recomputing attention for previous tokens
   - 10-80x throughput improvement
   - Memory management critical

2. **Batching**
   - Amortize model load time
   - GPU utilization: single request = 5-10%, batch = 80-95%
   - Tiny batches increase latency

3. **Quantization**
   - INT8/INT4 weights
   - Reduce model size by 4-8x
   - ~10-20% accuracy loss (often acceptable)

4. **Speculative Decoding**
   - Draft small model, verify with large model
   - Skip small model tokens via large model
   - ~2-3x throughput improvement

---

## Interview Questions

### Q1: Design LLM inference infrastructure
**Key points:**
- GPU memory management
- KV cache strategy
- Batching algorithms
- Request scheduling
- Cost-efficiency optimization

### Q2: Implement token generation pipeline
**Key points:**
- Autoregressive decoding
- Temperature and sampling
- Multi-GPU coordination
- Streaming responses
- Error handling

### Q3: Scale to millions of requests/day
**Key points:**
- Load balancing strategy
- Queue management
- Priority scheduling
- Resource allocation
- Cost optimization (GPUs are expensive)

