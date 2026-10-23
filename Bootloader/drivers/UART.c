#include "uart.h"

#define PCLK_FREQ   8000000UL //blue pill is working on the internal RC clock of 8MHz
#define baudrate    115200U

void UART1_Init(void)
{
    USART1->BRR = PCLK_FREQ / baudrate;

    /* Enable transmitter (TE), receiver (RE), and USART peripheral (UE) */
    USART1->CR1 = USART_CR1_TE | USART_CR1_RE | USART_CR1_UE;
}

void UART1_WriteChar(char ch)
{
    /* Wait until the transmit data register is empty (TXE bit is set) */
    while (!(USART1->SR & USART_SR_TXE));

    /* Write character to the single Data Register (DR) */
    USART1->DR = ch;
}

void UART1_WriteString(const char *str)
{
    /* Loop until the end of the string (null terminator) */
    while (*str != '\0')
    {
        UART1_WriteChar(*str);
        str++; /* Move to next character */
    }
}

char UART1_ReadChar(void)
{
    /* Wait until a character is received (RXNE bit is set) */
    while (!(USART1->SR & USART_SR_RXNE));

    /* Return the received character from the Data Register (DR) */
    return (char)USART1->DR;
}

