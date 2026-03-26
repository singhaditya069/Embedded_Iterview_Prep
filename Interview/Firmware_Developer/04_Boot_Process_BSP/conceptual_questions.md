# Boot Process and BSP - Conceptual Questions

## Bootloader Concept
1. What is a bootloader and its role?
2. Explain boot from ROM, Flash, external memory.
3. Two-stage bootloading process.
4. Bootloader vs. application code organization.

## ARM Boot Process
1. Explain ARM reset vector and initialization sequence.
2. What happens at power-on reset?
3. Explain CPU modes: User, Supervisor, Secure, etc.
4. Stack pointer initialization in bootloader.

## Vector Table
1. What is an interrupt vector table?
2. How do you set up the vector table address?
3. Relocation of vector table.
4. Default and custom interrupt handlers.

## BSP (Board Support Package)
1. What is a BSP and its components?
2. Bootloader in BSP.
3. Device tree in BSP.
4. BSP initialization sequence.

## Memory Initialization
1. Stack and heap initialization.
2. Global variable initialization (data copy from flash).
3. BSS (uninitialized data) section clearing.
4. Constructor/destructor calls for C++.

## Firmware Image Organization
1. Application sections: text (code), rodata, data, bss.
2. Linker script organization.
3. Flash and RAM layout.
4. Firmware size optimization.

## Real-world Scenario Questions

### Scenario 1: Bootloader with Dual Images
**Context**: Firmware with A/B boot images for safe updates.

**Questions**:
1. How would you structure dual images?
2. How would you implement failsafe boot?
3. How would you detect corrupted images?
4. How would you perform safe image transitions?

### Scenario 2: Custom Linker Script
**Context**: Complex memory layout with different sections for optimization.

**Questions**:
1. How would you organize sections?
2. How would you handle scatter-loading?
3. How would you optimize for cache?

