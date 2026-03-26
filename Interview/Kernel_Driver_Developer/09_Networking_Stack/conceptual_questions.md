# Networking Stack - Conceptual Questions

## OSI Model in Linux
1. How Linux implements OSI layers.
2. Protocol families and socket types.
3. Network subsystem architecture.

## Socket Programming
1. Socket creation and binding.
2. TCP and UDP sockets.
3. Blocking vs. non-blocking operations.
4. Asynchronous I/O (select, poll, epoll).

## TCP/IP Stack
1. IP layer (routing, fragmentation).
2. TCP (connection, flow control, congestion).
3. UDP (connectionless, best-effort).
4. ICMP (ping, unreachable messages).

## Network Driver Interface
1. Network device operations.
2. Packet transmission and reception.
3. DMA for packet buffers.
4. Interrupt handling in network drivers.

## QoS (Quality of Service)
1. Traffic shaping and policing.
2. Priority queuing.
3. Bandwidth management.

## Real-time Network Features
1. TSN (Time Sensitive Networking).
2. Real-time Ethernet protocols.
3. Deterministic packet delivery.

## Real-world Scenario Questions

### Scenario 1: Network Performance Bottleneck
**Context**: Network throughput is lower than expected.

**Questions**:
1. How would you profile network stack?
2. Where is the bottleneck?
3. How would you optimize?

### Scenario 2: Real-time Network Requirements
**Context**: Application needs bounded latency for network operations.

**Questions**:
1. What techniques ensure determinism?
2. How would you implement TSN?

