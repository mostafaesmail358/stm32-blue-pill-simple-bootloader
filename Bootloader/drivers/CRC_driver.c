#include "crc_driver.h"


void CRC_Reset(void) {
    /* Reset internal logic circuits and set initial state value to 0xFFFFFFFF */
    *CRC_REG_CR |= CRC_CR_RESET;
}

void CRC_Accumulate(unsigned long data) {
    /* Stream 32 bits into the data register to execute the hardware calculation */
    *CRC_REG_DR = data;
}

unsigned long CRC_GetResult(void) {
    /* Extract the updated CRC from the hardware accumulator */
    return *CRC_REG_DR;
}
