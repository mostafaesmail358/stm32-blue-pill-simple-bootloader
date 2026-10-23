/*
    @file   startup_stm32f103xx.c
    @author Ahmed Abd El-Ghafar Mohammed
    @brief  Startup code
*/


#define NULL 0

typedef unsigned int uint32_t;

/* Symbols exported from the Linker Script */
extern uint32_t _estack, _etext, _sdata, _edata, _sbss, _ebss, _sidata;

extern int main(void);

void Reset_Handler                     (void);
void NMI_Handler                       (void) __attribute__ ((alias ("Default_Handler")));
void HardFault_Handler                 (void) __attribute__ ((alias ("Default_Handler")));
void MemManage_Handler                 (void) __attribute__ ((alias ("Default_Handler")));
void BusFault_Handler                  (void) __attribute__ ((alias ("Default_Handler")));
void UsageFault_Handler                (void) __attribute__ ((alias ("Default_Handler")));
void SVC_Handler                       (void) __attribute__ ((alias ("Default_Handler")));
void DebugMon_Handler                  (void) __attribute__ ((alias ("Default_Handler")));
void PendSV_Handler                    (void) __attribute__ ((alias ("Default_Handler")));
void SysTick_Handler                   (void) __attribute__ ((alias ("Default_Handler")));
void WWDG_IRQHandler                   (void) __attribute__ ((alias ("Default_Handler")));
void PVD_IRQHandler                    (void) __attribute__ ((alias ("Default_Handler")));
void TAMPER_IRQHandler                 (void) __attribute__ ((alias ("Default_Handler")));
void RTC_IRQHandler                    (void) __attribute__ ((alias ("Default_Handler")));
void FLASH_IRQHandler                  (void) __attribute__ ((alias ("Default_Handler")));
void RCC_IRQHandler                    (void) __attribute__ ((alias ("Default_Handler")));
void EXTI0_IRQHandler                  (void) __attribute__ ((alias ("Default_Handler")));
void EXTI1_IRQHandler                  (void) __attribute__ ((alias ("Default_Handler")));
void EXTI2_IRQHandler                  (void) __attribute__ ((alias ("Default_Handler")));
void EXTI3_IRQHandler                  (void) __attribute__ ((alias ("Default_Handler")));
void EXTI4_IRQHandler                  (void) __attribute__ ((alias ("Default_Handler")));
void DMA1_Channel1_IRQHandler          (void) __attribute__ ((alias ("Default_Handler")));
void DMA1_Channel2_IRQHandler          (void) __attribute__ ((alias ("Default_Handler")));
void DMA1_Channel3_IRQHandler          (void) __attribute__ ((alias ("Default_Handler")));
void DMA1_Channel4_IRQHandler          (void) __attribute__ ((alias ("Default_Handler")));
void DMA1_Channel5_IRQHandler          (void) __attribute__ ((alias ("Default_Handler")));
void DMA1_Channel6_IRQHandler          (void) __attribute__ ((alias ("Default_Handler")));
void DMA1_Channel7_IRQHandler          (void) __attribute__ ((alias ("Default_Handler")));
void ADC1_2_IRQHandler                 (void) __attribute__ ((alias ("Default_Handler")));
void USB_HP_CAN1_TX_IRQHandler         (void) __attribute__ ((alias ("Default_Handler")));
void USB_LP_CAN1_RX0_IRQHandler        (void) __attribute__ ((alias ("Default_Handler")));
void CAN1_RX1_IRQHandler               (void) __attribute__ ((alias ("Default_Handler")));
void CAN1_SCE_IRQHandler               (void) __attribute__ ((alias ("Default_Handler")));
void EXTI9_5_IRQHandler                (void) __attribute__ ((alias ("Default_Handler")));
void TIM1_BRK_IRQHandler               (void) __attribute__ ((alias ("Default_Handler")));
void TIM1_UP_IRQHandler                (void) __attribute__ ((alias ("Default_Handler")));
void TIM1_TRG_COM_IRQHandler           (void) __attribute__ ((alias ("Default_Handler")));
void TIM1_CC_IRQHandler                (void) __attribute__ ((alias ("Default_Handler")));
void TIM2_IRQHandler                   (void) __attribute__ ((alias ("Default_Handler")));
void TIM3_IRQHandler                   (void) __attribute__ ((alias ("Default_Handler")));
void TIM4_IRQHandler                   (void) __attribute__ ((alias ("Default_Handler")));
void I2C1_EV_IRQHandler                (void) __attribute__ ((alias ("Default_Handler")));
void I2C1_ER_IRQHandler                (void) __attribute__ ((alias ("Default_Handler")));
void I2C2_EV_IRQHandler                (void) __attribute__ ((alias ("Default_Handler")));
void I2C2_ER_IRQHandler                (void) __attribute__ ((alias ("Default_Handler")));
void SPI1_IRQHandler                   (void) __attribute__ ((alias ("Default_Handler")));
void SPI2_IRQHandler                   (void) __attribute__ ((alias ("Default_Handler")));
void USART1_IRQHandler                 (void) __attribute__ ((alias ("Default_Handler")));
void USART2_IRQHandler                 (void) __attribute__ ((alias ("Default_Handler")));
void USART3_IRQHandler                 (void) __attribute__ ((alias ("Default_Handler")));
void EXTI15_10_IRQHandler              (void) __attribute__ ((alias ("Default_Handler")));
void RTC_Alarm_IRQHandler              (void) __attribute__ ((alias ("Default_Handler")));
void USBWakeUp_IRQHandler              (void) __attribute__ ((alias ("Default_Handler")));



//we want the _estack to be stored in the first address of the vector table 
//so we make the address in which the value is stored is in the begining of the vector table
uint32_t * const MSP_Value = (uint32_t *)&_estack;

/* Vector table for STM32F103C8T6 (Cortex-M3) */
uint32_t *Vector_Table[] __attribute__ ((section (".isr_vector"))) = {
    (uint32_t *)MSP_Value,              /* Main Stack Pointer (MSP) */
    (uint32_t *)Reset_Handler,          /* Reset Handler */
    (uint32_t *)NMI_Handler,            /* Non-maskable Interrupt */
    (uint32_t *)HardFault_Handler,      /* All classes of fault */
    (uint32_t *)MemManage_Handler,      /* Memory Management Fault */
    (uint32_t *)BusFault_Handler,       /* Bus Fault (Pre-fetch / Memory Access) */
    (uint32_t *)UsageFault_Handler,     /* Usage Fault (Undefined instruction / Illegal state) */
    0, 0, 0, 0,                         /* Reserved slots */
    (uint32_t *)SVC_Handler,            /* System Service Call via SWI instruction */
    (uint32_t *)DebugMon_Handler,       /* Debug Monitor */
    0,                                  /* Reserved slot */
    (uint32_t *)PendSV_Handler,         /* Pendable request for system service */
    (uint32_t *)SysTick_Handler,        /* System Tick Timer */

    /* Peripheral-specific External Interrupts (STM32F103 Specific Layout) */
    (uint32_t *)WWDG_IRQHandler,
    (uint32_t *)PVD_IRQHandler,
    (uint32_t *)TAMPER_IRQHandler,
    (uint32_t *)RTC_IRQHandler,
    (uint32_t *)FLASH_IRQHandler,
    (uint32_t *)RCC_IRQHandler,
    (uint32_t *)EXTI0_IRQHandler,
    (uint32_t *)EXTI1_IRQHandler,
    (uint32_t *)EXTI2_IRQHandler,
    (uint32_t *)EXTI3_IRQHandler,
    (uint32_t *)EXTI4_IRQHandler,
    (uint32_t *)DMA1_Channel1_IRQHandler,
    (uint32_t *)DMA1_Channel2_IRQHandler,
    (uint32_t *)DMA1_Channel3_IRQHandler,
    (uint32_t *)DMA1_Channel4_IRQHandler,
    (uint32_t *)DMA1_Channel5_IRQHandler,
    (uint32_t *)DMA1_Channel6_IRQHandler,
    (uint32_t *)DMA1_Channel7_IRQHandler,
    (uint32_t *)ADC1_2_IRQHandler,
    (uint32_t *)USB_HP_CAN1_TX_IRQHandler,
    (uint32_t *)USB_LP_CAN1_RX0_IRQHandler,
    (uint32_t *)CAN1_RX1_IRQHandler,
    (uint32_t *)CAN1_SCE_IRQHandler,
    (uint32_t *)EXTI9_5_IRQHandler,
    (uint32_t *)TIM1_BRK_IRQHandler,
    (uint32_t *)TIM1_UP_IRQHandler,
    (uint32_t *)TIM1_TRG_COM_IRQHandler,
    (uint32_t *)TIM1_CC_IRQHandler,
    (uint32_t *)TIM2_IRQHandler,
    (uint32_t *)TIM3_IRQHandler,
    (uint32_t *)TIM4_IRQHandler,
    (uint32_t *)I2C1_EV_IRQHandler,
    (uint32_t *)I2C1_ER_IRQHandler,
    (uint32_t *)I2C2_EV_IRQHandler,
    (uint32_t *)I2C2_ER_IRQHandler,
    (uint32_t *)SPI1_IRQHandler,
    (uint32_t *)SPI2_IRQHandler,
    (uint32_t *)USART1_IRQHandler,
    (uint32_t *)USART2_IRQHandler,
    (uint32_t *)USART3_IRQHandler,
    (uint32_t *)EXTI15_10_IRQHandler,
    (uint32_t *)RTC_Alarm_IRQHandler,
    (uint32_t *)USBWakeUp_IRQHandler,
};

static void System_Initialization(void) {
    /* Low-level system clock setup can be implemented here */
}

void Reset_Handler(void) {
    uint32_t Section_Size = 0;
    uint32_t *MemSourceAddr = NULL;
    uint32_t *MemDestAddr = NULL;

    /* 1) Copy the data segment initializers from Flash to SRAM (.data) */
    Section_Size = &_edata - &_sdata;
    MemSourceAddr = (uint32_t *)&_sidata;
    MemDestAddr = (uint32_t *)&_sdata;
    
    for(uint32_t MemCounter = 0; MemCounter < Section_Size; MemCounter++){
        *MemDestAddr++ = *MemSourceAddr++;
    }

    /* 2) Initialize the .bss segment with zero in SRAM */
    Section_Size = &_ebss - &_sbss;
    MemDestAddr = (uint32_t *)&_sbss;

    for(uint32_t MemCounter = 0; MemCounter < Section_Size; MemCounter++){
        *MemDestAddr++ = 0;
    }

    /* 3) Call the system initialization sequence */
    System_Initialization();

    /* 4) Jump to the main application function */
    main();
}

void Default_Handler(void) {
    
}
