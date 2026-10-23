#include "rcc.h"
#include "gpio.h"



// This is a dummy const array used to make sure that writing multiple pages is done with no issues.
__attribute__((used)) const int Dummy_Flash_Padding[1536] = {
    0xAA, 0xBB, 0xCC, 0xDD, 0x11, 0x22, 0x33, 0x44, [1535] = 0xFF
};



/* Simple software delay loop function */
void Delay(volatile uint32_t count) {
    while (count--) {
        __asm volatile ("nop");
    }
}

int main(void) {
   
    /* 2. Turn on the clock for GPIO Port C and Port A using your RCC definitions */
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN | RCC_APB2ENR_IOPAEN;

    /* 3. Initialize Pin 13 as a 2MHz Output Push-Pull pin using your GPIO driver */
    GPIO_Init();

    /* 4. Infinite loop toggling the LED pin state */
    while (1) {
        /* Clear bit 13 to pull the pin LOW (Turns Blue Pill Onboard LED ON) */
        GPIOC->ODR &= ~GPIO_ODR_ODR13;
        Delay(300000);

        /* Set bit 13 to pull the pin HIGH (Turns Blue Pill Onboard LED OFF) */
        GPIOC->ODR |= GPIO_ODR_ODR13;
        Delay(300000);
    }

    return 0; 
}