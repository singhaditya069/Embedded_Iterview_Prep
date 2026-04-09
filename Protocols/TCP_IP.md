# TCP/IP Interview Questions and Answers

## Table of Contents
1. [Beginner Level](#beginner-level)
2. [Intermediate Level](#intermediate-level)
3. [Advanced Level](#advanced-level)

---

## Beginner Level

### Q1. What is TCP/IP?
**Answer:**
TCP/IP (Transmission Control Protocol/Internet Protocol) is a suite of communication protocols used for exchanging data across networks. It provides a framework for how data is formatted, transmitted, and received over the internet. The protocol suite consists of multiple layers that work together to enable network communication.

**Key Points:**
- TCP handles data delivery and ensures packets arrive in order
- IP handles routing and addressing
- Forms the foundation of the modern internet
- Open standard developed by IETF (Internet Engineering Task Force)

### Q2. What are the layers in the TCP/IP model?
**Answer:**
The TCP/IP model typically consists of 4 layers:

1. **Application Layer** - Handles user applications and services (HTTP, HTTPS, FTP, SMTP, DNS, SSH)
2. **Transport Layer** - Manages end-to-end communication (TCP, UDP)
3. **Internet Layer** - Handles routing and logical addressing (IP, ICMP, ARP, IGMP)
4. **Link Layer** - Manages physical transmission (Ethernet, PPP, Wi-Fi)

### Q3. Difference between TCP and UDP?

| Feature | TCP | UDP |
|---------|-----|-----|
| Connection | Connection-oriented | Connectionless |
| Reliability | Reliable delivery | Unreliable delivery |
| Ordering | Maintains packet order | No ordering guarantee |
| Speed | Slower (setup required) | Faster |
| Error Checking | Extensive | Minimal |
| Use Case | Email, FTP, HTTP, Telnet | DNS, DHCP, Streaming, Gaming |

### Q4. What is an IP address?
**Answer:**
An IP address is a unique numerical identifier assigned to every device connected to a network that uses Internet Protocol for communication.

**Types:**
- **IPv4** - 32-bit address (e.g., 192.168.1.1), allows ~4.3 billion unique addresses
- **IPv6** - 128-bit address (e.g., 2001:0db8:85a3:0000:0000:8a2e:0370:7334), supports far more addresses

**Format:**
- IPv4: Four octets separated by dots (e.g., 192.168.0.1)
- IPv6: Eight groups of 16-bit hexadecimal values

### Q5. What is a port?
**Answer:**
A port is a virtual endpoint of network communication that identifies a specific process or service on a device. It's a 16-bit number (0-65535) that works with IP addresses to establish connections.

**Port Ranges:**
- **Well-Known Ports** (0-1023) - Reserved for system services (HTTP=80, HTTPS=443, SSH=22)
- **Registered Ports** (1024-49151) - For user applications
- **Dynamic/Private Ports** (49152-65535) - Temporary ports assigned by OS

### Q6. What is a MAC address?
**Answer:**
A MAC (Media Access Control) address is a physical address uniquely identifying a device on a local network. It operates at Layer 2 (Link Layer) and is used for communication within the same local network segment.

**Format:**
- 48-bit address usually represented as six pairs of hexadecimal digits separated by colons (e.g., 00:1A:2B:3C:4D:5E)
- First 24 bits represent the manufacturer (OUI)
- Last 24 bits represent the device-specific address

### Q7. What is ARP (Address Resolution Protocol)?
**Answer:**
ARP is a protocol that maps IP addresses to MAC addresses on a local network. When a device needs to communicate with another device on the same network segment, it uses ARP to discover the MAC address associated with an IP address.

**Process:**
1. Host broadcasts ARP request with target IP address
2. Device with matching IP responds with its MAC address
3. Response is cached in ARP table for future use
4. Communication can now proceed using MAC address

### Q8. What is DHCP?
**Answer:**
DHCP (Dynamic Host Configuration Protocol) automatically assigns IP addresses and other network configuration parameters to devices on a network, eliminating the need for manual IP configuration.

**Process:**
1. **DISCOVER** - Client broadcasts DHCP discovery message
2. **OFFER** - Server responds with available IP configuration
3. **REQUEST** - Client requests the offered configuration
4. **ACK** - Server acknowledges and assigns the IP

**Benefits:**
- Reduces manual configuration effort
- Prevents IP address conflicts
- Efficient use of IP address pool
- Easy network management

### Q9. What is DNS?
**Answer:**
DNS (Domain Name System) translates human-readable domain names into IP addresses, enabling users to access websites using names instead of numerical IP addresses.

**Resolution Process:**
1. User enters domain name in browser
2. Query sent to recursive resolver (usually ISP's DNS)
3. Resolver queries root nameserver
4. Root server directs to TLD nameserver
5. TLD server directs to authoritative nameserver
6. Authoritative server returns IP address
7. IP address cached and returned to user

**Record Types:**
- **A** - Maps domain to IPv4 address
- **AAAA** - Maps domain to IPv6 address
- **MX** - Mail exchange records
- **CNAME** - Canonical name (alias)
- **TXT** - Text records
- **NS** - Nameserver records

### Q10. What is the three-way handshake in TCP?
**Answer:**
The three-way handshake is the process used to establish a TCP connection. It ensures both client and server are ready for communication.

**Steps:**
1. **SYN** - Client sends SYN packet to server with sequence number (SYN_SENT state)
2. **SYN-ACK** - Server responds with SYN-ACK packet (SYN_RECEIVED state)
3. **ACK** - Client sends ACK packet to acknowledge server's response (ESTABLISHED state)

**Result:** Connection is established and data transfer can begin

---

## Intermediate Level

### Q11. Explain the TCP/IP packet structure.
**Answer:**
A TCP/IP packet consists of multiple headers stacked together, each layer adding its own header.

**Structure (Top-Down):**
```
┌─────────────────────────────────────┐
│    Application Layer Data           │ (Payload)
├─────────────────────────────────────┤
│    TCP/UDP Header                   │ (Port info, checksums, flags)
├─────────────────────────────────────┤
│    IP Header                        │ (Source/Dest IP, TTL)
├─────────────────────────────────────┤
│    Ethernet/Link Layer Header       │ (MAC addresses)
├─────────────────────────────────────┤
│    Data Frame/Packet                │
└─────────────────────────────────────┘
```

**TCP Header Fields:**
- Source Port (16 bits)
- Destination Port (16 bits)
- Sequence Number (32 bits)
- Acknowledgment Number (32 bits)
- Flags (SYN, ACK, FIN, RST, PSH, URG)
- Window Size (16 bits)
- Checksum (16 bits)

**IP Header Fields:**
- Version (4 bits)
- IHL - Header Length (4 bits)
- ToS - Type of Service (8 bits)
- Total Length (16 bits)
- Identification (16 bits)
- TTL - Time to Live (8 bits)
- Protocol (8 bits)
- Header Checksum (16 bits)
- Source IP Address (32 bits)
- Destination IP Address (32 bits)

### Q12. What is the difference between MAC address and IP address?

| Aspect | MAC Address | IP Address |
|--------|-------------|-----------|
| **Layer** | Layer 2 (Data Link) | Layer 3 (Network) |
| **Scope** | Local network only | Global/Routed networks |
| **Format** | 48-bit (6 octets in hex) | 32-bit or 128-bit (decimal) |
| **Assignment** | Assigned by manufacturer | Assigned by network admin/DHCP |
| **Uniqueness** | Unique per device | Unique per device on network |
| **Purpose** | Local delivery on LAN | Routing across networks |
| **Example** | 00:1A:2B:3C:4D:5E | 192.168.1.100 |

### Q13. What happens during connection termination in TCP?
**Answer:**
TCP connection termination uses a four-way handshake called the "FIN-ACK" process:

**Steps:**
1. **FIN** - First endpoint sends FIN packet (indicates no more data)
2. **ACK** - Opposing endpoint acknowledges the FIN
3. **FIN** - Opposing endpoint sends its own FIN packet
4. **ACK** - First endpoint acknowledges the second FIN

**States Involved:**
- ESTABLISHED → FIN_WAIT_1 → FIN_WAIT_2 → TIME_WAIT → CLOSED
- ESTABLISHED → CLOSE_WAIT → LAST_ACK → CLOSED

**TIME_WAIT State:**
- Waits for 2 × MSL (Maximum Segment Lifetime) before fully closing
- Ensures delayed packets are properly discarded
- Prevents confusion with new connections using same port

### Q14. What is NAT (Network Address Translation)?
**Answer:**
NAT is a technique that modifies IP address information in packet headers during transit, allowing devices with private IP addresses to communicate with external networks.

**Types:**
1. **Static NAT** - One-to-one mapping of private to public IPs
2. **Dynamic NAT** - Maps private IPs to pool of public IPs
3. **PAT (Port Address Translation)** - Maps multiple private IPs to single public IP using different ports

**Benefits:**
- Conserves public IP addresses
- Provides security by hiding internal network
- Enables multiple devices to share single public IP

**Drawbacks:**
- Adds complexity to troubleshooting
- Impacts performance with translation overhead
- Breaks end-to-end connectivity model
- Complicates peer-to-peer applications

### Q15. What is the difference between OSI model and TCP/IP model?

| Factor | OSI Model | TCP/IP Model |
|--------|-----------|--------------|
| **Layers** | 7 layers | 4-5 layers |
| **Development** | ISO standard | IETF standard |
| **Usage** | Theoretical/Academic | Practical/Implementation |
| **Layer Names** | Physical, Data Link, Network, Transport, Session, Presentation, Application | Link, Internet, Transport, Application |
| **Adoption** | Less practical | More widely used |
| **Flexibility** | More rigid | More flexible |

**Mapping:**
- OSI Layers 1-2 → TCP/IP Link Layer
- OSI Layer 3 → TCP/IP Internet Layer
- OSI Layer 4 → TCP/IP Transport Layer
- OSI Layers 5-7 → TCP/IP Application Layer

### Q16. Explain ICMP and its uses.
**Answer:**
ICMP (Internet Control Message Protocol) is used for sending error messages and diagnostic information about network conditions.

**Common Uses:**
1. **Echo Request/Reply** - Used by `ping` command to test connectivity
2. **Destination Unreachable** - Sent when host/network not reachable
3. **Time Exceeded** - Sent when TTL reaches zero
4. **Redirect** - Informs source of better route
5. **Parameter Problem** - Invalid header parameter detected

**Key Points:**
- Used for network diagnostics (ping, tracert/traceroute)
- Not used for data transfer
- Usually processed by network layer, not applications
- Can be filtered by firewalls for security

**Example:**
```bash
ping 8.8.8.8  # Sends ICMP Echo Request
# Receives ICMP Echo Reply with response time
```

### Q17. What is TTL (Time to Live)?
**Answer:**
TTL is a field in IP header that prevents packets from circulating indefinitely in networks with routing loops.

**How It Works:**
- Set to initial value (typically 64 or 128) when packet is sent
- Each router decrements TTL by 1 before forwarding
- When TTL reaches 0, packet is discarded
- Router sends ICMP "Time Exceeded" message back to source

**Purpose:**
- Prevents infinite loops in case of routing misconfiguration
- Limits packet propagation
- Used by `traceroute` command

**Example:**
```
Hop 1: TTL=64 → becomes 63
Hop 2: TTL=63 → becomes 62
...
Hop 64: TTL=1 → becomes 0 (packet discarded)
```

### Q18. What is subnet masking?
**Answer:**
A subnet mask is used to divide an IP address into network and host portions.

**Concept:**
- 32-bit number in IPv4 that identifies network portion of address
- Binary 1s identify network bits, 0s identify host bits
- Allows dividing large networks into smaller subnets

**Examples:**
```
IP Address:    192.168.1.100
Subnet Mask:   255.255.255.0
Network:       192.168.1.0
Broadcast:     192.168.1.255
Host Range:    192.168.1.1 to 192.168.1.254

CIDR Notation: 192.168.1.0/24
(24 bits for network, 8 bits for hosts)
```

**Common Subnet Masks:**
- /8 or 255.0.0.0 - Class A
- /16 or 255.255.0.0 - Class B
- /24 or 255.255.255.0 - Class C

### Q19. Explain the concept of socket.
**Answer:**
A socket is an endpoint of network communication identified by IP address, port, and protocol. It's an abstraction that represents one end of a network connection.

**Types:**
1. **Stream Socket (SOCK_STREAM)** - Uses TCP, connection-oriented, reliable
2. **Datagram Socket (SOCK_DGRAM)** - Uses UDP, connectionless, unreliable
3. **Raw Socket** - Allows direct IP header manipulation

**Socket Pair:**
Connection uniquely identified by:
- Source IP Address
- Source Port
- Destination IP Address
- Destination Port
- Protocol (TCP/UDP)

**Life Cycle:**
1. **Create** - Create socket
2. **Bind** - Associate with port
3. **Listen** (server) - Wait for connections
4. **Connect** (client) - Initiate connection
5. **Send/Receive** - Exchange data
6. **Close** - Terminate connection

### Q20. What is the difference between unicast, broadcast, and multicast?

| Type | Scope | Delivery | Use Case |
|------|-------|----------|----------|
| **Unicast** | One-to-one | Directed to single host | Regular communication, web browsing |
| **Broadcast** | One-to-all | Sent to all hosts on network segment | ARP requests, network announcements |
| **Multicast** | One-to-many | Sent to specific group of hosts | Video streaming, online games, IPTV |
| **Anycast** | One-to-nearest | Routed to nearest of multiple hosts | DNS, CDN load balancing |

**Example IP Ranges:**
- Unicast: 1.0.0.0 - 223.255.255.255
- Broadcast: 224.0.0.0 - 239.255.255.255 (Multicast)
- Special: 240.0.0.0 - 255.255.255.255 (Reserved/Experimental)

---

## Advanced Level

### Q21. Explain TCP congestion control mechanisms.
**Answer:**
TCP congestion control prevents network overload by adjusting transmission rate based on network conditions.

**Key Algorithms:**

**1. Slow Start:**
- Initial phase where transmission rate increases exponentially
- Congestion Window (CWND) starts at 1 MSS (Maximum Segment Size)
- CWND doubles with each ACK: 1 → 2 → 4 → 8...
- Continues until reaching Slow Start Threshold (SSThresh) or packet loss

**2. Congestion Avoidance:**
- Linear increase phase
- CWND increases by 1 MSS per RTT (Round Trip Time)
- More conservative than slow start
- Continues until packet loss detected

**3. Fast Retransmit:**
- Retransmits lost segment immediately after receiving 3 duplicate ACKs
- Doesn't wait for RTO (Retransmission TimeOut)
- Reduces latency of recovery

**4. Fast Recovery:**
- Follows fast retransmit
- Sets SSThresh to half current CWND
- Sets CWND to SSThresh + 3 MSS
- Increases CWND by 1 MSS for each duplicate ACK
- Exits on ACK of new data

**Modern Algorithms:**
- **CUBIC** - More aggressive in high-speed networks
- **BBR (Bottleneck Bandwidth and RTT)** - Google's algorithm for better performance

### Q22. What is TCP flow control?
**Answer:**
TCP flow control ensures sender doesn't overwhelm receiver with data faster than it can be processed.

**Mechanism:**
- Uses **Receive Window (RWnd)** field in TCP header
- Receiver advertises available buffer space to sender
- Sender adjusts transmission rate based on window size
- Prevents buffer overflow and packet loss

**Process:**
1. Receiver has 65KB buffer
2. Receiver advertises RWnd=65KB (available space)
3. Sender transmits up to 65KB without waiting for ACK
4. As receiver processes data, it frees buffer
5. Receiver updates RWnd in ACK packets
6. Sender adjusts transmission based on new window

**Zero Window:**
- When receiver buffer is full, it sends RWnd=0
- Sender stops transmitting
- Sender periodically sends Window Update requests
- When space becomes available, receiver sends new window size

**Difference from Congestion Control:**
- **Flow Control** - Receiver-driven, prevents receiver overload
- **Congestion Control** - Network-driven, prevents network overload

### Q23. Explain TCP sequence numbers and acknowledgments.
**Answer:**
Sequence and acknowledgment numbers ensure reliable, in-order delivery of TCP data.

**Sequence Numbers:**
- 32-bit counter starting with ISN (Initial Sequence Number)
- Randomly chosen to prevent attacks
- Each byte of data has unique sequence number
- Increments with each transmitted byte
- Wraps around after 2^32 bytes (232 bytes total)

**Acknowledgment Numbers:**
- Confirms receipt of data
- Contains sequence number of next expected byte
- Example: ACK=1001 means "I received up to byte 1000 and expect 1001 next"
- Sent in TCP header of packets in opposite direction

**Example:**
```
Client → Server: SYN, Seq=100, Payload=0 bytes
Server → Client: SYN-ACK, Seq=300, Ack=101 (expecting 101)
Client → Server: ACK, Seq=101, Ack=301, Payload="Hello"
Server → Client: ACK, Seq=301, Ack=106 (received up to 105)
```

**Out-of-Order Handling:**
- If segment arrives out-of-order, TCP buffers it
- Continues acknowledging last in-order sequence number
- Sender can detect loss via duplicate ACKs
- Resends lost segment

### Q24. What are TCP flags and their significance?
**Answer:**
TCP flags control connection behavior and are single-bit indicators in TCP header.

**Common Flags:**

| Flag | Name | Purpose |
|------|------|---------|
| **SYN** | Synchronize | Initiates connection, carries ISN |
| **ACK** | Acknowledgment | Acknowledges received data/flags |
| **FIN** | Finish | Indicates no more data to send |
| **RST** | Reset | Abruptly closes connection, clears buffers |
| **PSH** | Push | Immediately deliver data to application, don't buffer |
| **URG** | Urgent | Indicates urgent data, used with Urgent Pointer |

**Flag Combinations:**

```
SYN      - Connection initiation
SYN-ACK  - Server responding to connection request
ACK      - Acknowledging received data
FIN-ACK  - Graceful connection close
RST      - Immediate connection termination
PSH-ACK  - Push data and acknowledge
```

**Usage Examples:**
```
Client initiates: SYN flag set, Seq=X
Server responds: SYN-ACK flags set, Seq=Y, Ack=X+1
Data transfer: ACK flag set with data
Connection close: FIN flag set with remaining data
Forced close: RST flag set to abort
```

### Q25. Explain Path MTU Discovery (PMTUD).
**Answer:**
PMTUD is a technique to determine the largest MTU (Maximum Transmission Unit) between two hosts without fragmentation.

**How It Works:**
1. Sender assumes path MTU equals local interface MTU
2. Sends packet with Don't Fragment (DF) flag set
3. If packet too large for intermediate router, router sends ICMP "Fragmentation Needed"
4. ICMP message includes Next-Hop MTU value
5. Sender sets path MTU to received value and retransmits
6. Process repeats until packet passes through without fragmentation

**MTU Values:**
- Ethernet: 1500 bytes
- PPP: 1500 bytes
- Loopback: 65536 bytes
- Typical Path MTU: 1500 bytes

**Benefits:**
- Optimal packet size without fragmentation
- Reduces bandwidth waste from fragmentation headers
- Improves transmission efficiency

**Issues:**
- PMTUD black holes - Some routers silently drop packets instead of sending ICMP
- IPv4 fragmentation deprecated in IPv6
- Alternative: MSS (Maximum Segment Size) negotiation in TCP

### Q26. What is the Nagle's algorithm?
**Answer:**
Nagle's algorithm reduces TCP network traffic by combining small data segments into larger packets before transmission.

**Rules:**
1. If data less than MSS and there's unacknowledged data in flight, buffer it
2. Otherwise, send immediately
3. When ACK arrives, transmit buffered data

**Purpose:**
- Reduces number of small packets (inefficient)
- Prevents sending multiple small TCP segments
- Improves network efficiency for applications sending frequent small messages

**Effect:**
```
Without Nagle:  A, A, A, A, A (5 packets)
With Nagle:     AAAAA (1 packet after ACK received)
```

**When to Disable:**
- Interactive applications (SSH, telnet) - require low latency
- Real-time applications (VoIP, online games)
- Applications with explicit flush requests

**Controlled Via:**
```c
// Disable Nagle (TCP_NODELAY option)
int flag = 1;
setsockopt(socket, IPPROTO_TCP, TCP_NODELAY, &flag, sizeof(int));
```

### Q27. Explain TCP TIME_WAIT state and its importance.
**Answer:**
TIME_WAIT is a state where connection lingers after closing, waiting to ensure all packets are processed.

**Why TIME_WAIT Exists:**

1. **Handle Delayed Packets:**
   - Late packets from old connection might arrive
   - TIME_WAIT prevents them from being accepted by new connection
   - Duration: 2 × MSL (Maximum Segment Lifetime)
   - Default MSL: 30-120 seconds (varies by OS)

2. **Reliable Connection Closure:**
   - If final ACK is lost, server retransmits final FIN
   - Client in TIME_WAIT can retransmit ACK
   - Ensures graceful termination

3. **Prevent TCP Sequence Number Reuse:**
   - Ensures old sequence numbers don't conflict with new connections

**State Diagram:**
```
Active Close:
ESTABLISHED → FIN_WAIT_1 → FIN_WAIT_2 → TIME_WAIT → CLOSED

Passive Close:
ESTABLISHED → CLOSE_WAIT → LAST_ACK → CLOSED
```

**Problems:**
- Holds server socket resources
- Prevents rapid reconnection on same port
- Can exhaust ephemeral port pool under heavy load

**Solutions:**
- SO_REUSEADDR socket option
- Load balancing across multiple IPs/ports
- Increase ulimit for open file descriptors

### Q28. What is TCP Out-of-Order delivery and handling?
**Answer:**
TCP handles segments arriving out-of-order by buffering and waiting for missing segments before delivering to application.

**Process:**
```
Example:
- Sent: Seq 1000-1500, 1500-2000, 2000-2500
- Received: 2000-2500, 1000-1500, 1500-2000

1. Receive 2000-2500: Buffer it, ACK=1000 (last in-order)
2. Receive 1000-1500: This fills gap, deliver 1000-2000
3. Receive 1500-2000: Now gaps filled, deliver 1500-2500
4. Send ACK=2500 (all consumed)
```

**TCP Buffering:**
- Maintains receive buffer (TCP window size)
- Buffers out-of-order segments
- Delivers data to application only when complete sequence received

**Detection of Loss:**
- If sender doesn't receive ACK for segment
- After 3 duplicate ACKs received for previous segment
- Sender knows segment was lost

**Retransmission Strategies:**
```
Selective Repeat:
- Only retransmit lost segments
- More efficient, modern TCP

Go-Back-N:
- older approach
- Retransmit from lost segment onwards
```

### Q29. Explain half-open connection and detection.
**Answer:**
A half-open connection occurs when TCP connection is terminated unexpectedly on one end but not the other.

**Causes:**
- Network failure/cable disconnect
- Host crash without closing connections
- Firewall/NAT timeout
- Application crash without proper cleanup
- Mobile device losing connection

**Detection Methods:**

**1. Keep-Alive Probes:**
```c
int flag = 1;
setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, &flag, sizeof(int));
```
- Sends probe packets periodically on idle connections
- Detects dead connections early
- Prevents resource waste

**2. Read/Write Failure:**
- Attempt to send data on dead connection
- Receive RST packet or connection refused
- Application detects error

**3. Explicit Heartbeat:**
- Application-level keep-alive messages
- More reliable than TCP keep-alive
- Can be application-specific (ping/pong)

**Parameters:**
- TCP_KEEP_IDLE - Time before first probe (7200s)
- TCP_KEEP_INTVL - Interval between probes (75s)
- TCP_KEEP_CNT - Number of probes before giving up (9)

### Q30. What is the SYN flood attack and mitigation?
**Answer:**
SYN flood is a denial-of-service attack exploiting TCP three-way handshake mechanism.

**How Attack Works:**
```
1. Attacker sends SYN packets to target server
2. Server responds with SYN-ACK
3. Attacker doesn't complete handshake (no ACK sent)
4. Server allocates resources for half-open connection
5. Attacker repeats with many connections
6. Server's connection queue fills completely
7. Legitimate connections rejected
```

**Impact:**
- Server resources exhausted
- Service unavailable
- Can crash server

**Mitigation Techniques:**

**1. SYN Cookies:**
```
- Server encodes connection info in ISN (Initial Sequence Number)
- Doesn't allocate resources until ACK received
- Recovers info from cookie in ACK packet
- Transparent to legitimate clients
```

**2. Connection Timeout:**
- Aggressively timeout half-open connections
- Free resources quickly

**3. Firewall/IDS Protection:**
- Rate limiting at firewall
- Detect and block spoofed sources
- SYN throttling

**4. Load Balancing:**
- Distribute load across servers
- Bypass attacks to legitimate traffic

**5. Backlog Queue Management:**
- Increase server's SYN backlog queue
- Tune system parameters:
```bash
# Linux
sysctl -w net.ipv4.tcp_max_syn_backlog=4096
sysctl -w net.ipv4.tcp_synack_retries=3
```

### Q31. Explain IP fragmentation and reassembly.
**Answer:**
IP fragmentation breaks large packets into smaller fragments when exceeding MTU, reassembled at destination.

**When Fragmentation Occurs:**
- Packet size > Path MTU
- DF (Don't Fragment) flag not set
- Intermediate router performs fragmentation

**Fragment Header Fields:**
```
- Identification - Identifies fragments of same original packet
- Flags:
  - More Fragments (MF) flag - 0=last, 1=more fragments
  - Don't Fragment (DF) flag - 0=allow frag, 1=don't fragment
- Fragment Offset - Position of fragment in original packet (units of 8 bytes)
```

**Example:**
```
Original: 1500 byte packet, ID=100
MTU=500 bytes

Fragment 1: Offset=0, MF=1, ID=100, Data=500 bytes
Fragment 2: Offset=62, MF=1, ID=100, Data=500 bytes  (62*8=496)
Fragment 3: Offset=124, MF=0, ID=100, Data=500 bytes (124*8=992)
```

**Reassembly Process:**
- Destination collects fragments with same ID
- Reassembles in order using offset
- Forwards complete packet to transport layer
- Discards incomplete packets after timeout

**Issues:**
- Performance overhead
- Increases latency
- Packet loss affects entire reassembly
- Complicates firewall filtering

**Best Practices:**
- Use PMTUD to avoid fragmentation
- Set MSS appropriately
- Use IPSec which handles fragmentation better

### Q32. What is Explicit Congestion Notification (ECN)?
**Answer:**
ECN allows routers to signal network congestion without dropping packets, improving network efficiency.

**Traditional Approach:**
- Network congestion detected only by packet loss
- TCP interprets loss as signal to reduce rate
- High latency due to retransmission

**ECN Mechanism:**
- Uses 2 bits in IP ToS field (Explicit Congestion Experienced)
- Router marks packets instead of dropping them
- Receiver notifies sender of congestion
- Sender reduces rate without packet loss/retransmission

**How It Works:**
```
1. Sender sets ECT (ECN-Capable Transport) in ToS field
2. Network congestion detected by router
3. Router sets CE (Congestion Experienced) bit instead of dropping
4. Receiver sees CE bit and sets ECE flag in ACK
5. Sender reduces congestion window like packet loss occurred
6. Sender sends CWR flag in next packet
```

**Benefits:**
- Reduced packet retransmission
- Lower latency
- More efficient network utilization
- Better performance for latency-sensitive apps

**Negotiation:**
- Must be negotiated during TCP handshake
- Both sides must support ECN
- Can be disabled if problematic with network

### Q33. Explain TCP sequence number wrapping and handling.
**Answer:**
TCP sequence numbers wrap around after reaching 2^32, creating potential sequence number ambiguity.

**Wrapping Scenario:**
```
Sequence number range: 0 to 4,294,967,295 (2^32 - 1)
After 4GB of data, wraps back to 0
At high data rates, wrapping can happen in seconds
```

**Calculation:**
```
Time to wrap at 1 Gbps: 4,294,967,295 bytes / (1e9 bits/s / 8) ≈ 34 seconds
Time to wrap at 1 Mbps: ~9.1 hours
```

**Problems:**
- Ambiguity: Can't distinguish new packets from retransmissions
- Lost segments hard to detect
- Out-of-order delivery handling complicated

**Solutions:**

**1. PAWS (Protection Against Wrapped Sequences):**
- Uses TCP timestamp option
- 32-bit timestamp incremented every millisecond
- Discards segments with older timestamp
- Detects wrapped sequence numbers

**TCP Timestamp Option:**
```
Options: Kind=8, Length=10
TSval - Sender's timestamp (32-bit)
TSecr - Echo of receiver's timestamp (32-bit)
```

**2. RFC 1323 Extensions:**
- Window scaling for better flow control
- Explicit acknowledgments
- More robust handling

### Q34. What is TCP Options and SACK?
**Answer:**
TCP Options provide additional features negotiated during connection setup.

**Common TCP Options:**

**1. MSS (Maximum Segment Size):**
```
Kind=2, Length=4
Specifies max payload size sender willing to send
Negotiated during SYN exchange
```

**2. Window Scaling (RFC 1323):**
```
Kind=3, Length=3
Multiplier for advertised window (65535 * 2^factor)
Enables windows larger than 65KB
```

**3. Timestamp (RFC 1323):**
```
Kind=8, Length=10
Solves PAWS, improves RTT measurements
```

**4. SACK (Selective Acknowledgment - RFC 2018):**
```
Kind=4, Length=variable
Allows receiver to acknowledge non-contiguous segments
Reduces retransmission of correctly received data

Without SACK:
Send: 1-1000, 1001-2000, 2001-3000
Recv: 1-1000, 2001-3000 (loses middle)
ACK only 1-1000, must retransmit 1001-2000 again

With SACK:
ACK=1001, SACK=[2001-3000]
Sender knows 2001-3000 received, only retransmit 1001-2000
```

**Benefits of SACK:**
- Reduced unnecessary retransmissions
- Better performance over lossy links
- More efficient bandwidth usage

### Q35. Explain TCP state machine and transitions.
**Answer:**
TCP state machine defines all possible states and transitions during connection lifecycle.

**Connection States:**

```
CLOSED         - No connection
LISTEN         - Waiting for connection request (server)
SYN_SENT       - Client waiting for SYN-ACK
SYN_RECEIVED   - Server got SYN, sent SYN-ACK
ESTABLISHED    - Connection active, data transfer
FIN_WAIT_1     - Active close initiated, waiting for FIN
FIN_WAIT_2     - Got ACK for FIN, waiting for remote FIN
CLOSE_WAIT     - Passive close, received FIN
CLOSING        - Both sides sent FIN (rare)
LAST_ACK       - Waiting for final ACK of FIN
TIME_WAIT      - Waiting before final close
```

**State Transitions:**

**Passive Open (Server):**
```
CLOSED → LISTEN → SYN_RECEIVED → ESTABLISHED
         ↑                        ↓
         └── timeout/RST ←───────┘
```

**Active Open (Client):**
```
CLOSED → SYN_SENT → ESTABLISHED
         ↓    ↑         ↓
         └────┘    FIN_WAIT_1 → FIN_WAIT_2 → TIME_WAIT → CLOSED
```

**Active Close:**
```
ESTABLISHED → FIN_WAIT_1 → FIN_WAIT_2 → TIME_WAIT → CLOSED
```

**Passive Close:**
```
ESTABLISHED → CLOSE_WAIT → LAST_ACK → CLOSED
```

**Simultaneous Open:**
```
SYN_SENT → SYN_RECEIVED → ESTABLISHED
```

**Simultaneous Close:**
```
FIN_WAIT_1 → CLOSING → TIME_WAIT → CLOSED
```

### Q36. What is the difference between blocking and non-blocking sockets?
**Answer:**
Blocking and non-blocking sockets differ in how they handle I/O operations.

**Blocking Socket (Default):**
```c
// Call blocks until operation completes or error occurs
char buffer[1024];
int bytes = recv(sock, buffer, 1024, 0);  // Blocks until data available
```

**Characteristics:**
- Thread/process waits for operation to complete
- Issues when multiple connections needed
- Simpler programming model
- Inefficient for servers handling many connections

**Non-blocking Socket:**
```c
// Set non-blocking mode
int flags = fcntl(sock, F_GETFL, 0);
fcntl(sock, F_SETFL, flags | O_NONBLOCK);

// Or Windows:
unsigned long mode = 1;
ioctlsocket(sock, FIONBIO, &mode);

// Operation returns immediately
int bytes = recv(sock, buffer, 1024, 0);  // Returns EAGAIN if no data
```

**Characteristics:**
- Returns immediately with EAGAIN/EWOULDBLOCK if not ready
- Requires polling or event notification
- Enables handling multiple connections
- More complex programming

**Comparison:**

| Aspect | Blocking | Non-blocking |
|--------|----------|--------------|
| Wait | Blocks until ready | Returns immediately |
| Single connection | Efficient | Wasteful |
| Multiple connections | Must use threads | Efficient |
| Programming | Simple | Complex |
| Error handling | Straightforward | Requires checking EAGAIN |

**Usage Patterns:**

**Blocking:**
- Simple client applications
- One connection per thread/process
- Not for high-concurrency servers

**Non-blocking:**
- High-performance servers
- Event-driven models
- Multiplexing with select/poll/epoll

### Q37. Explain IPv6 and differences from IPv4.
**Answer:**
IPv6 is the next-generation Internet Protocol addressing the limitations of IPv4.

**Key Differences:**

| Feature | IPv4 | IPv6 |
|---------|------|------|
| **Address Size** | 32-bit | 128-bit |
| **Address Space** | 4.3 billion | 340 undecillion |
| **Notation** | Dotted decimal | Hexadecimal colons |
| **Header** | 20-60 bytes | 40 bytes (Fixed) |
| **Fragmentation** | Router/Host | Host only |
| **Header Checksum** | Required | Not required |
| **TTL** | Time to Live | Hop Limit |
| **QoS** | Not included | Traffic class & flow label |

**IPv6 Address Structure:**
```
2001:0db8:85a3:0000:0000:8a2e:0370:7334

Prefixes:
2001:db8::/32 - Documentation
fe80::/10 - Link-local
ff00::/8 - Multicast
fc00::/7 - Unique local

Abbreviations:
Leading zeros: 2001:db8::8a2e:370:7334
Consecutive zero groups: 2001:db8::8a2e:370:7334
```

**IPv6 Features:**

**1. Larger Address Space:**
- 2^128 addresses (vs 2^32 in IPv4)
- No need for NAT in most cases

**2. Simplified Header:**
- Fixed 40-byte header (no options in main header)
- Options in extension headers
- Faster processing by routers

**3. No Fragmentation by Router:**
- Only host fragments
- Router sends ICMPv6 "Packet Too Big"
- Host performs PMTUD

**4. Built-in IPSec:**
- Security considerations from ground up
- End-to-end encryption

**5. Auto-configuration:**
- Stateless address autoconfiguration (SLAAC)
- No DHCP required (though available)

**6. Multicast Instead of Broadcast:**
- More efficiency
- Replaces broadcast for most use cases

**Transition Mechanisms:**
- Dual stack (IPv4 and IPv6)
- Tunneling (IPv4 over IPv6)
- NAT64 (IPv6 to IPv4)

### Q38. What is SACK-HOLES and DSACK?
**Answer:**
Advanced variants of SACK for improving TCP reliability and detecting network issues.

**SACK (Selective ACK Review):**
- Covered in Q34
- Allows acknowledging non-contiguous segments

**DSACK (Duplicate SACK - RFC 2883):**
- Extension of SACK
- Acknowledges duplicate/retransmitted segments
- Signals that segment was received before

**How DSACK Works:**
```
Sender transmits: 1-1000, 1001-2000, 2001-3000
Receiver gets: All three segments

Receiver sends: ACK=3001, SACK=[1-1000]
(SACK of already-ACKed segment indicates duplicate)
```

**Uses of DSACK:**
1. **Spurious Retransmission Detection:**
   - Indicates segment was received, retransmission unnecessary
   - Detects false timeouts
   - Adjusts RTO more accurately

2. **Network Duplicate Detection:**
   - Identifies when network duplicates packets

3. **Congestion Control Improvement:**
   - Informs congestion control of spurious retransmit
   - More conservative reaction to packet loss

**SACK-HOLES (Handling):**
- Gaps in SACK ranges where segments missing
- Guides retransmission strategy
- Sender fills "holes" first

**Benefits:**
- Better network diagnosis
- Improved retransmission strategy
- More accurate loss detection
- Better congestion control decisions

### Q39. Explain TCP Urgent Pointer and Out-of-Band Data.
**Answer:**
TCP Urgent Pointer enables sending high-priority data out-of-order, bypassing regular buffering.

**Urgent Data Mechanism:**

**1. URG Flag:**
```
Set in TCP header to indicate urgent data present
```

**2. Urgent Pointer (16-bit):**
```
Points to last byte of urgent data (absolute sequence number)
Urgent data = from current position to urgent pointer

Example:
Sequence = 1000, Urgent Pointer = 1050
Urgent data spans bytes 1000-1050
```

**Process:**
```
1. Sender sets URG flag and Urgent Pointer
2. Sends data with flag
3. Receiver gets SIGURG signal (Unix) or exception
4. Application reads urgent data out-of-band
5. Continues with normal data stream
```

**Characteristics:**
- Only one byte can be marked urgent (in practice)
- Doesn't skip normal buffering but provides notification
- Application must read urgent data explicitly

**Out-of-Band Data vs In-Band:**
```
Out-of-Band (Urgent):
- Separate notification mechanism
- Higher priority delivery
- Processed immediately

In-Band:
- Mixed with regular data
- Standard buffering
- FIFO processing
```

**Real-World Use:**
```
Telnet: Ctrl+C sends urgent data to interrupt command
SSH: Can send urgent signals
FTP: Control commands vs data separation
```

**Limitations:**
- Only indicates "urgent area" not individual bytes
- Modern applications prefer explicit signaling
- Limited use in modern protocols

### Q40. What is TCP Connection Reset (RST) and scenarios?
**Answer:**
TCP RST (Reset) abruptly terminates a connection, immediate connection closure without graceful shutdown.

**When RST Sent:**

| Scenario | Reason |
|----------|--------|
| **No Listening** | SYN received on closed port |
| **Invalid State** | Segment received in inappropriate state |
| **Abort Connection** | Application requests immediate termination |
| **Half-Open Detect** | Data received on apparently closed connection |
| **Timeout Recovery** | Waiting for response that never comes |
| **Protocol Violation** | Invalid TCP header or parameters |

**RST Packet Characteristics:**
```
- RST flag set
- No data payload
- Sequence number valid
- For reply to invalid segment: ACK set, ack number = received seq + length
```

**Example Scenarios:**

**Scenario 1: Port Not Listening**
```
Client sends SYN to port not listening
Server responds: RST, Seq=server_isn, Ack=client_isn+1
Connection refused
```

**Scenario 2: Connection Already Closed**
```
Server closes connection gracefully (FIN)
Client doesn't know, sends data
Server responds: RST with appropriate ACK
```

**Scenario 3: Abort Connection**
```
Application calls socket shutdown(fd, SHUT_RDWR)
Or SO_LINGER with linger=0
TCP sends RST instead of FIN
```

**Effects of RST:**
- Immediate connection termination
- Buffers cleared
- No retransmission of FIN
- No TIME_WAIT state (optional)

**vs. Graceful FIN Close:**
```
FIN: Allows draining buffers, clean shutdown
RST: Immediate, abrupt termination
```

---

## Summary Table

| Topic | Key Points |
|-------|-----------|
| **Layers** | Application, Transport, Internet, Link |
| **Protocols** | TCP (reliable), UDP (fast), IP (routing) |
| **Addressing** | MAC (local), IP (global), Port (service) |
| **Connection** | 3-way handshake, state machine, FIN sequence |
| **Reliability** | Sequence numbers, ACKs, retransmission |
| **Flow Control** | Window size, sliding window |
| **Congestion** | Slow start, congestion avoidance, fast retransmit |
| **Security** | SYN cookies, firewalls, encryption |

---

## Additional Resources

- **RFCs Referenced:**
  -RFC 793 - TCP Specification
  - RFC 791 - IPv4 Specification
  - RFC 1323 - TCP Extensions
  - RFC 2018 - SACK
  - RFC 3168 - ECN
  - RFC 3542 - IPv6 Socket API

- **Tools for Testing:**
  - `netstat` - Show connections and statistics
  - `ping` - Test ICMP connectivity
  - `traceroute/tracert` - Trace path to destination
  - `tcpdump` - Capture network packets
  - `wireshark` - GUI packet analyzer
  - `iperf` - Network performance testing
  - `netcat` - Network utility for connections
  - `curl/wget` - HTTP clients for testing
