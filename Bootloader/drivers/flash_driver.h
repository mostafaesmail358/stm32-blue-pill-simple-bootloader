/**

 * CORE MEMORY SPECIFICATIONS:
 * - Total Flash Size: 64 KB (Official spec, though some boards have 128 KB).
 * - Layout Structure: Single-bank architecture.
 * - Granularity: Divided into 64 distinct Pages.
 * - Page Size: Exactly 1 KB (1024 bytes / 0x400 bytes) per page.
 * - Memory Range: Starts at 0x0800 0000 and ends at 0x0800 FFFF.
 * DESIGN CONSTRAINTS FOR SINGLE-BANK FLASH:
 * 1. CPU Stall: The CPU cannot read instructions from flash while an erase or 
 *    write operation is actively executing. The hardware pauses the bus automatically.
 * 2. Interrupt Safety: Because of the CPU stall, any interrupt triggered during 
 *    a flash write/erase that tries to execute code from flash will cause a hard fault.
 *    Global interrupts MUST be disabled during write/erase cycles.
 * 3. Write Alignment: The STM32F1 internal flash controller only permits writing 
 *    in 16-bit boundaries (Half-Words). You cannot write single bytes or 32-bit words directly.
 */

#ifndef FLASH_DRIVER_H
#define FLASH_DRIVER_H

/* --- Flash Peripheral Hardware Addresses --- */
/* The flash controller registers are mapped to a specific base location in memory */
#define FLASH_REG_BASE          0x40022000UL

#define FLASH_REG_ACR           ((volatile unsigned long*)(FLASH_REG_BASE + 0x00)) /* Access Control */
#define FLASH_REG_KEYR          ((volatile unsigned long*)(FLASH_REG_BASE + 0x04)) /* Key Register */
#define FLASH_REG_SR            ((volatile unsigned long*)(FLASH_REG_BASE + 0x0C)) /* Status Register */
#define FLASH_REG_CR            ((volatile unsigned long*)(FLASH_REG_BASE + 0x10)) /* Control Register */
#define FLASH_REG_AR            ((volatile unsigned long*)(FLASH_REG_BASE + 0x14)) /* Address Register */

/* --- Flash Register Bits & Masks --- */
#define FLASH_CR_PG             (1UL << 0)   /* Programming Mode Bit */
#define FLASH_CR_PER            (1UL << 1)   /* Page Erase Mode Bit */
#define FLASH_CR_STRT           (1UL << 6)   /* Start Erase Bit */
#define FLASH_CR_LOCK           (1UL << 7)   /* Write/Erase Lock Bit */

#define FLASH_SR_BSY            (1UL << 0)   /* Hardware Busy Flag */
#define FLASH_SR_PGERR          (1UL << 2)   /* Programming Error Flag */
#define FLASH_SR_WRPRTERR       (1UL << 4)   /* Write Protection Error Flag */

/* --- Flash Unlock Keys --- */
#define FLASH_KEY1              0x45670123UL
#define FLASH_KEY2              0xCDEF89ABUL

/* --- Core Driver Function Prototypes --- */

/**
 * @brief Unlocks the flash controller configuration registers.
 */
void Flash_Unlock(void);

/**
 * @brief Locks the flash configuration to secure memory against accidental edits.
 */
void Flash_Lock(void);

/**
 * @brief Erases a single 1 KB chunk of flash memory.
 * @param pageAddress The 32-bit physical starting address of the target page.
 * @return 0 on success, 1 on hardware error.
 */
unsigned char Flash_ErasePage(unsigned long pageAddress);

/**
 * @brief Writes 16 bits of data to a specific flash memory address.
 * @param address The target physical destination address (must be 2-byte aligned).
 * @param data The 16-bit word to write.
 * @return 0 on success, 1 on hardware or validation error.
 */
unsigned char Flash_WriteHalfWord(unsigned long address, unsigned short data);

#endif /* FLASH_DRIVER_H */
