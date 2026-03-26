# Debugging and Tools - Conceptual Questions

## Debugging Interfaces
1. JTAG (Joint Test Action Group) protocol basics.
2. SWD (Serial Wire Debug) protocol.
3. SEGGER J-Link and similar debuggers.
4. OpenOCD for open-source debugging.

## GDB Usage
1. GDB basics: stepping, breakpoints, watchpoints.
2. Remote GDB debugging over JTAG/SWD.
3. Post-mortem debugging with crash dumps.
4. Symbol table and debug information.

## Logging and Tracing
1. Serial logging for diagnostics.
2. Log levels and filtering.
3. Circular buffer logging to avoid blocking.
4. ETM (Embedded Trace Macrocell) tracing.

## Hardware Debugging Tools
1. Logic analyzers for protocol analysis.
2. Oscilloscopes for signal verification.
3. CAN/SPI bus analyzers.
4. Power consumption measurement.

## Embedded Testing Frameworks
1. Unit testing frameworks for embedded.
2. Mocking hardware peripherals.
3. Test coverage analysis.
4. Continuous integration for firmware.

## Common Issues and Diagnosis
1. Hard faults and fault diagnosis.
2. Watchdog resets and causes.
3. Memory corruption detection.
4. Performance profiling.

## Real-world Scenario Questions

### Scenario 1: Mystery Hard Fault
**Context**: Device crashes with hard fault, difficult to reproduce.

**Questions**:
1. How would you set up crash logging?
2. What information would you capture?
3. How would you generate crash reports?
4. How would you analyze fault data?

### Scenario 2: Real-time Constraint Violation
**Context**: System occasionally misses real-time deadline.

**Questions**:
1. How would you profile to find the culprit?
2. How would you measure task execution time?
3. How would you use tracing to verify fix?

