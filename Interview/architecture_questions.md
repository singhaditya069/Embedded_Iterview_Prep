# Architecture Interview Questions (Hardware Product Companies)

---

**Q: Explain the difference between Harvard and Von Neumann architectures.**

**A:** Harvard has separate buses for instructions and data; Von Neumann uses a single bus for both, which can cause bottlenecks.

---

**Q: What is pipelining in CPU architecture?**

**A:** Pipelining allows overlapping execution of instructions by dividing the CPU into stages. Each stage processes a different instruction simultaneously, increasing throughput.

---

**Q: What is cache coherence? How is it maintained in multi-core systems?**

**A:** Cache coherence ensures all processors see the same memory value. Protocols like MESI (Modified, Exclusive, Shared, Invalid) are used to maintain coherence.

---

**Q: Explain MMU (Memory Management Unit) and its role in embedded systems.**

**A:** MMU translates virtual addresses to physical addresses, provides memory protection, and enables features like paging and segmentation.

---

**Q: What is the difference between RISC and CISC architectures?**

**A:** RISC (Reduced Instruction Set Computer) uses simple instructions and is optimized for speed; CISC (Complex Instruction Set Computer) uses complex instructions, often requiring fewer lines of assembly code.

---

**Q: What is DMA and how does it improve system performance?**

**A:** DMA (Direct Memory Access) allows peripherals to transfer data to/from memory without CPU intervention, freeing the CPU for other tasks and improving throughput.

---

**Q: What is a bus arbiter?**

**A:** A bus arbiter manages access to a shared bus among multiple masters, ensuring orderly and conflict-free communication.

---

**Q: Explain the concept of interrupt latency.**

**A:** Interrupt latency is the time between the generation of an interrupt and the start of the corresponding ISR. It is critical in real-time systems.

---

<!-- Add more architecture questions as needed -->
