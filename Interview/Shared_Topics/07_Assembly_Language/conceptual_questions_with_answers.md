# Assembly Language (ARM Thumb) - Comprehensive Q&A

## ARM Assembly Basics

### Q1: ARM assembly registers and basic instructions
**Answer:**
```asm
; ARM Thumb-2 assembly (Cortex-M processors)

; Registers:
; R0-R3   = parameter/result
; R4-R11  = local variables (preserved)
; R12     = intermediate
; R13 (SP) = stack pointer
; R14 (LR) = link register (return address)
; R15 (PC) = program counter

; Basic arithmetic instructions
MOV  R0, #100       ; R0 = 100 (8-bit constant, needs shifting)
MOV  R0, #0x100     ; Invalid! Constant out of range
MOVW R0, #0x1234    ; R0 = 0x1234 (16-bit, word)
MOVT R0, #0x5678    ; R0 = 0x56781234 (high 16-bits)

ADD  R0, R1, R2     ; R0 = R1 + R2
SUB  R0, R1, R2     ; R0 = R1 - R2
MUL  R0, R1, R2     ; R0 = R1 * R2 (32-bit result)
UMULL R0, R1, R2, R3  ; R1:R0 = R2 * R3 (64-bit result, unsigned)
SMULL R0, R1, R2, R3  ; R1:R0 = R2 * R3 (64-bit result, signed)

; Bitwise operations
AND  R0, R1, R2     ; R0 = R1 & R2
ORR  R0, R1, R2     ; R0 = R1 | R2
EOR  R0, R1, R2     ; R0 = R1 ^ R2 (XOR)
BIC  R0, R1, R2     ; R0 = R1 & ~R2 (bit clear)
LSL  R0, R1, #8     ; R0 = R1 << 8 (logical shift left)
LSR  R0, R1, #8     ; R0 = R1 >> 8 (logical shift right)
ASR  R0, R1, #8     ; R0 = R1 >> 8 (arithmetic shift right)
ROR  R0, R1, #8     ; R0 = R1 rotated right by 8

; Logical control instructions
CMP  R0, R1         ; Compare R0 to R1 (sets flags)
CMN  R0, R1         ; Compare R0 to -R1
BEQ  loop           ; Branch if equal
BNE  loop           ; Branch if not equal
BLE  loop           ; Branch if less than or equal
BLT  loop           ; Branch if less than
BGE  loop           ; Branch if greater or equal
BGT  loop           ; Branch if greater
BHS  loop           ; Branch if higher or same (unsigned)
BLO  loop           ; Branch if lower (unsigned)
```

**Constants in ARM:**
```c
// ARM can only use 8-bit constants rotated by even number of bits
// Valid constants:
0xFF, 0xFF00, 0xFF0000, 0xFF000000
0x03, 0x0C, 0x30, 0xC0      // etc.

// Invalid (need MOVW/MOVT):
0x1234, 0x5678, 0xABCD

// Compiler trick:
MOV  R0, #0x100     // Compiler expands to:
// MOVW R0, #0x0100
```

---

### Q2: Function calls and stack frames
**Answer:**
```asm
; Function call conventions (AAPCS - ARM Application Binary Interface)

; Calling function (caller-save)
caller_func:
    ; Arguments in R0-R3, rest on stack
    MOV  R0, #10        ; arg1 = 10
    MOV  R1, #20        ; arg2 = 20
    MOV  R2, #30        ; arg3 = 30
    MOV  R3, #40        ; arg4 = 40
    PUSH {R4-R11, LR}   ; Save registers (LR for return)
    BL   callee_func    ; Branch with Link (saves PC+2 to LR)
    POP  {R4-R11, PC}   ; Restore registers and return (PC from LR)

; Called function
callee_func:
    ; R0-R3 = arguments
    ; R0-R1 = return value
    PUSH {R4-R11, LR}   ; Prologue: save non-volatile registers
    SUB  SP, SP, #16    ; Local stack space
    
    ; Function body
    MOV  R4, R0         ; Save arg1 to R4 (non-volatile)
    ADD  R5, R1, R2     ; R5 = arg2 + arg3
    MUL  R0, R4, R5     ; R0 = result
    
    ADD  SP, SP, #16    ; Epilogue: release stack
    POP  {R4-R11, PC}   ; Restore and return

; Stack frame layout when inside callee_func:
; SP+44 │ LR save       │
; SP+40 │ R11 save      │
; ...
; SP+20 │ R4 save       │
; SP+16 │───────────────│  ← SP before SUB
; SP+12 │ Local var 4   │
; SP+8  │ Local var 3   │
; SP+4  │ Local var 2   │
; SP+0  │ Local var 1   │  ← SP after SUB
```

---

## Memory Access

### Q1: Load and store instructions
**Answer:**
```asm
; Load instructions (read from memory)
LDR  R0, [R1]           ; Load 32-bit word: R0 = *(uint32_t*)R1
LDRH R0, [R1]           ; Load 16-bit half-word: R0 = *(uint16_t*)R1
LDRB R0, [R1]           ; Load 8-bit byte: R0 = *(uint8_t*)R1
LDRSH R0, [R1]          ; Load signed byte
LDRSB R0, [R1]          ; Load signed half-word

; Store instructions (write to memory)
STR  R0, [R1]           ; Store 32-bit: *(uint32_t*)R1 = R0
STRH R0, [R1]           ; Store 16-bit
STRB R0, [R1]           ; Store 8-bit

; Addressing modes
LDR  R0, [R1]           ; Direct: R0 = *(R1)
LDR  R0, [R1, #4]       ; Offset: R0 = *(R1 + 4)
LDR  R0, [R1, R2]       ; Register offset: R0 = *(R1 + R2)
LDR  R0, [R1, R2, LSL #2]  ; Scaled: R0 = *(R1 + R2*4)

; Pre-index (update address before load)
LDR  R0, [R1, #4]!      ; R0 = *(R1+4); R1 = R1+4

; Post-index (update address after load)
LDR  R0, [R1], #4       ; R0 = *(R1); R1 = R1+4

; Multiple load/store
LDMIA R0!, {R1-R3}      ; Load 3 words, increment after (load multiple)
STMDB R0!, {R1-R3}      ; Store 3 words, decrement before (push)

; Practical example: memcpy in assembly
memcpy_asm:
    ; R0 = destination, R1 = source, R2 = count
    CMP  R2, #0
    BEQ  memcpy_done
    
memcpy_loop:
    LDR  R3, [R1], #4      ; Load 4 bytes from source, post-increment
    STR  R3, [R0], #4      ; Store to destination, post-increment
    SUBS R2, R2, #4        ; Decrement count (and update flags)
    BNE  memcpy_loop       ; Branch if not zero
    
memcpy_done:
    BX   LR                ; Return
```

---

## Interrupt Handling in Assembly

### Q1: Interrupt handlers and context saving
**Answer:**
```asm
; Exception/Interrupt handling in ARM Cortex-M

; Vector table (at 0x00000000)
    ORG 0x00000000
    DCD 0x20008000         ; Initial SP (top of stack)
    DCD Reset_Handler      ; Reset handler (PC on power-up)
    DCD NMI_Handler        ; Non-maskable interrupt
    DCD HardFault_Handler  ; Hard fault
    DCD MemManage_Handler  ; Memory management
    DCD BusFault_Handler   ; Bus fault
    DCD UsageFault_Handler ; Usage fault
    ; ... more exceptions ...
    DCD SysTick_Handler    ; SysTick interrupt
    DCD UART0_IRQHandler   ; UART0 interrupt
    DCD SPI0_IRQHandler    ; SPI0 interrupt
    ; ... more IRQs ...

; Interrupt handler - Cortex-M automatically saves context!
; Hardware saves: R0, R1, R2, R3, R12, LR, PC, xPSR
; So we only need to save non-volatile registers if used

UART0_IRQHandler:
    PUSH {R4-R7, LR}       ; Save non-volatile registers
    
    ; Read status
    LDR  R0, =UART0_BASE
    LDR  R1, [R0, #4]      ; Read status register
    TST  R1, #0x01         ; Test RX ready bit
    BEQ  uart_tx_check
    
    ; Handle RX
    LDR  R2, [R0, #0]      ; Read receive buffer
    BL   uart_rx_handler
    
uart_tx_check:
    TST  R1, #0x02         ; Test TX ready bit
    BEQ  uart_done
    
    ; Handle TX
    BL   uart_tx_handler
    
uart_done:
    POP  {R4-R7, PC}       ; Restore and return (PC recovery automatic)

; Important: DO NOT use BX LR in interrupt handlers!
; The return address is already on stack, POP {PC} handles it

; Hard fault (default exception handler for debugging)
HardFault_Handler:
    BKPT #0                ; Breakpoint for debugger
    B    HardFault_Handler ; Infinite loop
```

**Context saving mechanism:**
```c
// When interrupt occurs in Cortex-M:
// Hardware automatically:
// 1. Pushes: xPSR, PC, LR, R12, R3-R0 (onto current stack)
// 2. Sets LR to special EXC_RETURN value (0xFFFFFFFx)
// 3. Switches to handler stack if needed
// 4. Starts executing handler

// Handler stack frame:
// SP+28 │ xPSR      │ ← Saved by hardware
// SP+24 │ PC        │
// SP+20 │ LR_return │
// SP+16 │ R12       │
// SP+12 │ R3        │
// SP+8  │ R2        │
// SP+4  │ R1        │
// SP+0  │ R0        │
//
// After PUSH {R4-R7}:
// SP+12 │ R7        │
// SP+8  │ R6        │
// SP+4  │ R5        │
// SP+0  │ R4        │
```

---

## Optimization Techniques

### Q1: Register usage and optimization
**Answer:**
```asm
; Inefficient: Multiple memory accesses
slow_sum:
    MOV  R0, #0         ; sum = 0
    MOV  R1, #0         ; i = 0
    LDR  R2, =array_ptr
    LDR  R2, [R2]       ; R2 = array pointer

loop_slow:
    CMP  R1, #100       ; if (i == 100)
    BEQ  done_slow
    
    LDR  R3, [R2, R1, LSL #2]  ; Load array[i]
    LDR  R0, [sp, #0]          ; Load sum from stack (inefficient!)
    ADD  R0, R0, R3
    STR  R0, [sp, #0]          ; Store sum to stack (inefficient!)
    
    ADD  R1, R1, #1
    B    loop_slow

done_slow:
    LDR  R0, [sp, #0]
    BX   LR

; Efficient: Keep in registers
fast_sum:
    MOV  R0, #0         ; R0 = sum (register variable)
    MOV  R1, #0         ; R1 = i
    LDR  R2, =array_ptr
    LDR  R2, [R2]       ; R2 = array pointer
    
loop_fast:
    CMP  R1, #100
    BEQ  done_fast
    
    LDR  R3, [R2, R1, LSL #2]  ; Load array[i]
    ADD  R0, R0, R3            ; Add to R0 (already in register!)
    
    ADD  R1, R1, #1
    B    loop_fast

done_fast:
    BX   LR

; Even more efficient: Use DSP instructions (M4/M7)
; NEON/SIMD for parallel operations
```

---

## Real-World Example

```asm
; CRC32 calculation in optimized assembly

; Arguments: R0 = data buffer, R1 = count
; Return: R0 = CRC result

crc32_calculate:
    PUSH {R4-R7, LR}
    
    MOV  R2, #0xFFFFFFFF  ; R2 = CRC (initial)
    MOV  R3, #0           ; R3 = index
    LDR  R4, =crc_table   ; R4 = CRC lookup table
    
crc_loop:
    CMP  R3, R1           ; if (index >= count)
    BEQ  crc_done
    
    ; Load byte
    LDRB R5, [R0, R3]     ; R5 = data[index]
    
    ; Calculate table index
    EOR  R6, R2, R5       ; R6 = CRC ^ byte
    AND  R6, R6, #0xFF    ; R6 &= 0xFF
    
    ; Lookup table value
    LDR  R7, [R4, R6, LSL #2]  ; R7 = table[R6]
    
    ; Update CRC
    LSR  R2, R2, #8       ; CRC >>= 8
    EOR  R2, R2, R7       ; CRC ^= table value
    
    ADD  R3, R3, #1       ; index++
    B    crc_loop
    
crc_done:
    MOV  R0, R2           ; Return CRC in R0
    
    POP  {R4-R7, PC}

; Key optimizations:
; 1. Keep CRC in R2 (avoid stack access)
; 2. Use LDRB for byte load (single instruction)
; 3. Pre-calculate shifts (LSL #2 in addressing mode)
; 4. Minimize branches (only loop counter)
; 5. Use non-volatile registers (R4-R7) to preserve across calls
```

Assembly language provides optimal performance for embedded systems!
