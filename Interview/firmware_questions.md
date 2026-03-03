# Firmware Interview Questions (Hardware Product Companies)

---

**Q: What is firmware? How is it different from software?**

**A:** Firmware is low-level code stored in non-volatile memory (Flash/ROM) that directly controls hardware. Software is a broader term for programs running on a system.

---

**Q: How do you update firmware in the field?**

**A:** Use bootloaders to receive new firmware over UART, USB, or network, write to flash, verify integrity (CRC/checksum), and switch execution to the new image.

---

**Q: What is a bootloader?**

**A:** A bootloader is a small program that loads and verifies the main firmware image at startup, often supporting firmware updates.

---

**Q: How do you handle firmware rollback?**

**A:** Store multiple firmware images and a version table in flash. If the new image fails verification, revert to the previous version.

---

**Q: How do you ensure firmware security?**

**A:** Use cryptographic signatures, secure boot, encrypted storage, and secure update mechanisms to prevent unauthorized code execution.

---

**Q: What is memory-mapped I/O?**

**A:** Memory-mapped I/O maps hardware registers into the processor's address space, allowing access via standard memory instructions.

---

**Q: How do you debug firmware on a target device?**

**A:** Use JTAG/SWD debuggers, serial output, hardware breakpoints, and trace tools.

---

<!-- Add more firmware questions as needed -->
