#ifndef RCC_H
#define RCC_H

typedef unsigned int uint32_t;

typedef struct
{
  volatile uint32_t Reserved[5]; /* Offset 0x00 to 0x10: Skips first 5 RCC registers */
  volatile uint32_t AHBENR;      /* Offset 0x14: Enables clocks for AHB peripherals (CRC, DMA, etc.) */
  volatile uint32_t APB2ENR;     /* Offset 0x18: Enables clocks for APB2 peripherals (GPIO, UART) */
} RCC_TypeDef;

/* Base memory addresses */
#define PERIPH_BASE                 0x40000000UL
#define AHBPERIPH_BASE              (PERIPH_BASE + 0x20000UL) /* 0x40020000 */
#define RCC_BASE                    (AHBPERIPH_BASE + 0x1000UL)  /* 0x40021000 */

/* Cast base address to pointer structure */
#define RCC                         ((RCC_TypeDef *) RCC_BASE)

#define RCC_APB2ENR_IOPAEN          (1UL << 2U)  /* GPIOA clock enable */
#define RCC_APB2ENR_IOPCEN          (1UL << 4U)  /* GPIOC clock enable */
#define RCC_APB2ENR_USART1EN        (1UL << 14U) /* USART1 clock enable */

#define RCC_AHBENR_CRCEN            (1UL << 6U)  /* CRC clock enable */

#endif /* RCC_H */
