# Embedded Linux Kernel - Conceptual Questions

## Embedded Linux Characteristics
1. Linux kernel in resource-constrained devices.
2. Minimal kernel build for embedded systems.
3. Kernel size optimization.
4. Memory footprint reduction.

## Device Tree
1. Device tree structure and syntax.
2. Device tree compilation (dtc).
3. Device tree bindings.
4. Runtime device tree modifications.

## U-Boot Bootloader
1. U-Boot architecture and features.
2. U-Boot configuration and scripting.
3. Kernel loading and parameters.
4. Booting from different media (SD, NAND).

## Embedded Kernel Configuration
1. Menuconfig for kernel compilation.
2. Minimizing kernel features for size.
3. Static vs. dynamic module loading.
4. Critical drivers for embedded systems.

## Real-time Linux
1. PREEMPT_RT patch for real-time.
2. Deterministic scheduling with RT kernels.
3. Real-time capabilities and limitations.
4. Tracing real-time systems.

## Root Filesystem
1. busybox for minimal utilities.
2. Buildroot for cross-compilation.
3. Yocto Project for embedded imaging.
4. Filesystem initialization scripts.

## Real-world Scenario Questions

### Scenario 1: Embedded Linux Board Bring-up
**Context**: New embedded board with custom hardware.

**Questions**:
1. How would you create device tree?
2. How would you adapt bootloader?
3. How would you optimize kernel config?
4. What would you test first?

### Scenario 2: Real-time Application Development
**Context**: Hard real-time requirements on embedded Linux.

**Questions**:
1. Would you use PREEMPT_RT?
2. What are the trade-offs?
3. How would you verify real-time guarantees?
4. How would you profile latency?

