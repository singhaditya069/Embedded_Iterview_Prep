# Microcontroller Programming - Conceptual Questions

## Register-level Programming
1. How do you read and write microcontroller registers?
2. What are bit fields and how are they used?
3. Explain volatile keyword usage with registers.
4. How do you handle register permissions and protection?

## GPIO Programming
1. How do you configure GPIO pins (input, output, alternate function)?
2. Explain GPIO modes: push-pull, open-drain, alternate function.
3. How do you implement GPIO interrupts?
4. Button debouncing techniques.

## Timer/Counter Programming
1. How do you initialize and use timers?
2. Explain different timer modes (normal, CTC, PWM).
3. How do you generate PWM signals?
4. Timer interrupt handling.

## ADC Programming
1. How do you configure ADC (resolution, clock, pins)?
2. Single-ended vs. differential conversion.
3. ADC interrupt handling vs. polling.
4. DMA transfer for ADC data.

## Serial Communication Programming
1. UART initialization and configuration.
2. Handling UART interrupts.
3. Implementing circular buffers for UART.
4. Flow control (CTS/RTS) implementation.

## Interrupt Programming
1. How do you configure interrupt priorities?
2. Interrupt enable/disable masks.
3. Critical sections and interrupt safety.
4. Nested interrupt handling.

## Real-world Scenario Questions

### Scenario 1: Multi-UART System
**Context**: Communicating with multiple devices via different UARTs.

**Questions**:
1. How would you handle separate UART interrupts?
2. What is the best code organization?
3. How would you manage multiple circular buffers?

### Scenario 2: Precise PWM Generation
**Context**: Generating PWM to control motor speed with precise frequency.

**Questions**:
1. What are the frequency and phase accuracy requirements?
2. How would you implement PWM using timer interrupts?
3. How would you change PWM parameters on-the-fly?

