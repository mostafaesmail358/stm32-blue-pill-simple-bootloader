
#include "flash_driver.h"

/* Assembly instructions implemented natively using compiler __asm volatile commands.
   This bypasses the need for any standard CMSIS system headers. */
   
// to control something in the mcu usually we have control registers enables us to interface with other peripherals but here to deal with enable
//or disable the interrupts due to its importance we need specific arm instruction in the ISA to do so not just writing in a register   

/*
__asm__ volatile ( 
    "assembly code" 
    : output operands     // Optional 
    : input operands      // Optional 
    : clobbered registers // Optional 
);
*/

//CPS: Change Processor State ID: Interrupt Disable   i: IRQ (General Peripherals).
//"memory": The compiler is strictly forbidden from moving any memory read or write operations from the part before the assembly block to after it, or vice versa.
//It guarantees that your code will absolutely finish preparing variables and fully shut down interrupts before a single line of flash erasing begins.

#define DISABLE_INTERRUPTS()    __asm volatile ("cpsid i" : : : "memory")
#define ENABLE_INTERRUPTS()     __asm volatile ("cpsie i" : : : "memory")

void Flash_Unlock(void) {
    /* Check if the configuration register is currently locked */
    if ((*FLASH_REG_CR & FLASH_CR_LOCK) != 0) {
        /* Pass the mandatory two-part password sequence into the key register this is like the state machine you take inputs to go certain state */
        *FLASH_REG_KEYR = FLASH_KEY1; 
        *FLASH_REG_KEYR = FLASH_KEY2;
    }
}

void Flash_Lock(void) {
    /* Assert the lock bit to immediately shut down write access */
    *FLASH_REG_CR |= FLASH_CR_LOCK;
}

unsigned char Flash_ErasePage(unsigned long pageAddress) {
    /* 1. Spin loop until any previous flash hardware operations finish */
    while ((*FLASH_REG_SR & FLASH_SR_BSY) != 0);

    /* 2. Turn off global interrupts using inline assembly. 
          Stalling single-bank flash without doing this will crash the system. */
    DISABLE_INTERRUPTS();

    /* 3. Turn on the Page Erase flag inside the control register */
    *FLASH_REG_CR |= FLASH_CR_PER;

    /* 4. Assign the target page start address to the address hardware register */
    *FLASH_REG_AR = pageAddress;

    /* 5. Trigger the electric page erase sequence */
    *FLASH_REG_CR |= FLASH_CR_STRT;

    /* 6. Wait for the physical array to clear and reset back to 0xFFFF values */
    while ((*FLASH_REG_SR & FLASH_SR_BSY) != 0);

    /* 7. Turn off the Page Erase configuration flag to restore standard operation */
    *FLASH_REG_CR &= ~FLASH_CR_PER;

    /* 8. Safe to restore chip interrupts */
    ENABLE_INTERRUPTS();

    /* 9. Verify if any write-protection or programming hardware faults occurred */
    if ((*FLASH_REG_SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR)) != 0) {
        return 1; /* Hardware error reported */
    }

    return 0; /* Page successfully cleared */
}

unsigned char Flash_WriteHalfWord(unsigned long address, unsigned short data) {
    /* 1. Ensure flash controller state machine is idle */
    while ((*FLASH_REG_SR & FLASH_SR_BSY) != 0);

    /* 2. Temporarily pause interrupts during the flash bus freeze */
    DISABLE_INTERRUPTS();

    /* 3. Set the programming configuration flag */
    *FLASH_REG_CR |= FLASH_CR_PG;

    /* 4. Perform the 16-bit raw memory access assignment.
          Casting the address to a volatile unsigned short pointer forces the compiler 
          to generate an STRH(store register half word) assembly command, which matches the hardware specification. */
    *(volatile unsigned short*)address = data;

    /* 5. Wait for the hardware to write the bits */
    while ((*FLASH_REG_SR & FLASH_SR_BSY) != 0);

    /* 6. Clear the programming mode flag */
    *FLASH_REG_CR &= ~FLASH_CR_PG;

    /* 7. Restore system interrupts */
    ENABLE_INTERRUPTS();

    /* 8. Check for hardware error flags */
    if ((*FLASH_REG_SR & (FLASH_SR_PGERR | FLASH_SR_WRPRTERR)) != 0) {
        return 1;
    }

    /* 9. Integrity Check: Ensure the data sitting in memory matches what we sent */
    if (*(volatile unsigned short*)address != data) {
        return 1; /* Verification failed */
    }

    return 0; /* Write successful */
}
