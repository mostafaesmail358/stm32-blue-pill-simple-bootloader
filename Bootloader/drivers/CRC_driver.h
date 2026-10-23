#ifndef CRC_DRIVER_H
#define CRC_DRIVER_H

/* --- Hardware Register Memory Map Addresses --- */
#define CRC_REG_BASE            0x40023000UL
#define CRC_REG_DR              ((volatile unsigned long*)(CRC_REG_BASE + 0x00)) /* Data Register */
#define CRC_REG_CR              ((volatile unsigned long*)(CRC_REG_BASE + 0x08)) /* Control Register */

#define RCC_AHBENR              ((volatile unsigned long*)0x40021014UL)

/* --- Bit Manipulation Masks --- */
#define CRC_CR_RESET            (1UL << 0)   /* Resets CRC calculations to 0xFFFFFFFF */

/* --- Core Function Drivers --- */
void CRC_Reset(void);
void CRC_Accumulate(unsigned long data);
unsigned long CRC_GetResult(void);

#endif /* CRC_DRIVER_H */
