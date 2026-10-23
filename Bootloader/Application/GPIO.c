#include "gpio.h"

void GPIO_Init(void)
{
    /* 1. Setup PC13 LED */
    GPIOC->CRH &= ~GPIO_CRH_Msk13;
    GPIOC->CRH |= GPIO_CRH_PC13_OUT_2MHZ;

    /* 2. Setup PA9 (TX) as Alternate Function Push-Pull */
    GPIOA->CRH &= ~GPIO_CRH_Msk9;
    GPIOA->CRH |= GPIO_CRH_PA9_AF_PP;

    /* 3. Setup PA10 (RX) as Floating Input */
    GPIOA->CRH &= ~GPIO_CRH_Msk10;
    GPIOA->CRH |= GPIO_CRH_PA10_INPUT_FLOAT;

    /* 
     * 4. Setup PA0 as Input with internal Pull-Down resistor
     * Clear CRL bits and apply 0x8 mode (Input with pull-up/pull-down configuration).
     * To select PULL-DOWN logic, we MUST clear bit 0 in the ODR register (write 0).
     */
    GPIOA->CRL &= ~GPIO_CRL_Msk0;
    GPIOA->CRL |= GPIO_CRL_PA0_INPUT_PULL;
    GPIOA->ODR &= ~GPIO_ODR_ODR0; // Clear bit 0 to firmly enforce PULL-DOWN to GND
}
