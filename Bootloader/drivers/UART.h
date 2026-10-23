#ifndef UART_H
#define UART_H

typedef unsigned int uint32_t;

typedef struct
{
  volatile uint32_t SR;   /* Status register,            Offset: 0x00 */
  volatile uint32_t DR;   /* Data register,              Offset: 0x04 */ //the tx data register has the same address as rx address but one has only read signal and the other write signal
  volatile uint32_t BRR;  /* Baud rate register,         Offset: 0x08 */
  volatile uint32_t CR1;  /* Control register 1,         Offset: 0x0C */
  volatile uint32_t CR2;  /* Control register 2,         Offset: 0x10 */
  volatile uint32_t CR3;  /* Control register 3,         Offset: 0x14 */
  volatile uint32_t GTPR; /* Guard time/prescaler reg,   Offset: 0x18 */
} USART_TypeDef;

/* Base memory addresses */
#define PERIPH_BASE                 0x40000000UL
#define APB2PERIPH_BASE             (PERIPH_BASE + 0x10000UL)   /* 0x40010000 */
#define USART1_BASE                 (APB2PERIPH_BASE + 0x3800UL) /* 0x40013800 */

/* Cast base address to pointer structure */
#define USART1                      ((USART_TypeDef *) USART1_BASE)

/* Control Register 1 (USART_CR1) Bit Definitions */
#define USART_CR1_UE                (1UL << 13U) /* USART Enable */
#define USART_CR1_TE                (1UL << 3U)  /* Transmitter Enable */
#define USART_CR1_RE                (1UL << 2U)  /* Receiver Enable */

/* Status Register (USART_SR) Bit Definitions */
#define USART_SR_TXE                (1UL << 7U)  /* Transmit data register empty */
#define USART_SR_RXNE               (1UL << 5U)  /* Read data register not empty */

/* Driver function prototypes */
void UART1_Init(void);
void UART1_WriteChar(char ch);
void UART1_WriteString(const char *str);
char UART1_ReadChar(void);


#endif /* UART_H */
