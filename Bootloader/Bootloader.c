#include "rcc.h"
#include "gpio.h"
#include "uart.h"
#include "crc_driver.h"
#include "flash_driver.h"

#define SOF_BYTE        0x02
#define EOF_BYTE        0x03
#define FLASH_APP_START 0x08002000UL // Target address where user application starts (Page 8)
#define MAX_BUFFER_SIZE 64

unsigned char frame_buffer[MAX_BUFFER_SIZE];

void System_Init(void) {
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_IOPCEN | RCC_APB2ENR_USART1EN;
    RCC->AHBENR  |= RCC_AHBENR_CRCEN;
    GPIO_Init();
    UART1_Init();
    Flash_Unlock();
}

int main(void) {
    /* 1. Initialize all dynamic clocks and GPIO modes first */
    System_Init();

/* 2. Read the state of Pin PA0 via the structured Input Data Register (IDR) */
    unsigned long emergency_mode = GPIOA->IDR & GPIO_IDR_IDR0;

    /* 
	 * (( This part decides whether we should jump to the app or continue to the bootloader based on PA0))
     * If PA0 is disconnected (floating), the internal Pull-Down pulls it firmly to 0 (LOW).
     * Therefore, if emergency_mode == 0, we immediately jump to the User Application.
     */
    if (emergency_mode == 0) {
        unsigned long check_stack = *(volatile unsigned long*)FLASH_APP_START;
        
		//We check if the app first address is not empty so that there is app
        if (check_stack != 0xFFFFFFFF) {
            unsigned long app_stack_pointer = check_stack;
            void (*app_reset_handler)(void) = (void (*)(void))(*(volatile unsigned long*)(FLASH_APP_START + 4));
            
			// Change the main stack pointer.
            __asm volatile ("msr msp, %0" : : "r" (app_stack_pointer));
			
			// This is a crucial step to shift the vector table to be at the start of our app by writing in the following memory address.
            (*(volatile unsigned int*)0xE000ED08UL) = FLASH_APP_START;
            
            
            app_reset_handler();
            while(1); 
        }
    }

    /* 
     * --- BOOTLOADER UART MODE ACTIVE --- 
     * Execution drops here ONLY if PA0 is manually connected to 3.3V (HIGH),
     * or if the application flash area is completely empty/blank.
     */
    unsigned long current_flash_address = FLASH_APP_START;

    Flash_ErasePage(current_flash_address);

    while (1) {
        if (UART1_ReadChar() != SOF_BYTE) {
            UART1_WriteChar('M'); 
            continue;
        }

        unsigned char len = UART1_ReadChar();

        for (int i = 0; i < len; i++) {
            frame_buffer[i] = UART1_ReadChar();
        }

        unsigned long received_crc = 0;
        received_crc |= ((unsigned long)UART1_ReadChar() << 24);
        received_crc |= ((unsigned long)UART1_ReadChar() << 16);
        received_crc |= ((unsigned long)UART1_ReadChar() << 8);
        received_crc |= ((unsigned long)UART1_ReadChar());

        if (UART1_ReadChar() != EOF_BYTE) {
            UART1_WriteChar('N'); 
            continue;
        }

        if (len == 0) {
            UART1_WriteChar('F'); 
            Flash_Lock(); 
            
            unsigned long app_stack_pointer = *(volatile unsigned long*)FLASH_APP_START;
            void (*app_reset_handler)(void) = (void (*)(void))(*(volatile unsigned long*)(FLASH_APP_START + 4));
            
            __asm volatile ("msr msp, %0" : : "r" (app_stack_pointer));
            (*(volatile unsigned int*)0xE000ED08UL) = FLASH_APP_START;
            
            
            app_reset_handler();
            while(1); 
        }

        CRC_Reset();
        for (int i = 0; i < len; i += 4) {
            unsigned long word = ((unsigned long)frame_buffer[i]   << 24) |
                                 ((unsigned long)frame_buffer[i+1] << 16) |
                                 ((unsigned long)frame_buffer[i+2] << 8)  |
                                 ((unsigned long)frame_buffer[i+3]);
            CRC_Accumulate(word);
        }
        unsigned long calculated_crc = CRC_GetResult();

        if (calculated_crc != received_crc) {
            UART1_WriteChar('O'); 
            continue;
        }

        for (int i = 0; i < len; i += 2) {
            unsigned short half_word = (frame_buffer[i+1] << 8) | frame_buffer[i];
            
            if ((current_flash_address % 1024) == 0) {
                Flash_ErasePage(current_flash_address);
            }

            while (Flash_WriteHalfWord(current_flash_address, half_word) != 0) {}
            current_flash_address += 2;
        }

        UART1_WriteChar('A'); 
    }
}
