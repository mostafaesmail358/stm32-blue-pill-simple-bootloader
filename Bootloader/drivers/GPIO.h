#ifndef GPIO_H
#define GPIO_H

typedef unsigned int uint32_t;

typedef struct
{
  volatile uint32_t CRL;       /* Offset 0x00: Controls pins 0 to 7 configuration */
  volatile uint32_t CRH;       /* Offset 0x04: Controls pins 8 to 15 configuration */
  volatile uint32_t IDR;       /* Offset 0x08: Reads input state of the pins */
  volatile uint32_t ODR;       /* Offset 0x0C: Controls output states of the pins */
} GPIO_TypeDef;

/* Base memory addresses */
#define PERIPH_BASE                 0x40000000UL
#define APB2PERIPH_BASE             (PERIPH_BASE + 0x10000UL)   /* 0x40010000 */
#define GPIOA_BASE                  0x40010800UL
#define GPIOC_BASE                  (APB2PERIPH_BASE + 0x1000UL) /* 0x40011000 */

/* Cast base address to pointer structure */
#define GPIOA                       ((GPIO_TypeDef *) GPIOA_BASE)
#define GPIOC                       ((GPIO_TypeDef *) GPIOC_BASE)

/* --- CRH Pin Control Definitions (Pins 8 to 15) --- */
#define GPIO_CRH_PA9_AF_PP          (0xBUL << 4U)  /* Alternate Function Push-Pull, 50MHz speed */
#define GPIO_CRH_PA10_INPUT_FLOAT   (0x4UL << 8U)  /* Floating Input */
#define GPIO_CRH_PC13_OUT_2MHZ      (0x2UL << 20U) /* Output Mode 2MHz, Push-Pull */

#define GPIO_CRH_Msk9               (0xFUL << 4U)
#define GPIO_CRH_Msk10              (0xFUL << 8U)
#define GPIO_CRH_Msk13              (0xFUL << 20U)

/* --- CRL Pin Control Definitions (Pins 0 to 7) --- */
#define GPIO_CRL_PA0_INPUT_PULL     (0x8UL << 0U)  /* Input with Pull-Up / Pull-Down configuration */
#define GPIO_CRL_Msk0               (0xFUL << 0U)  /* Mask for Pin 0 configuration bits */

/* --- Output/Input Bit Positions --- */
#define GPIO_ODR_ODR0               (1UL << 0U)    /* Pin 0 Output data configuration bit */
#define GPIO_ODR_ODR13              (1UL << 13U)   /* Pin 13 Output data configuration bit */
#define GPIO_IDR_IDR0               (1UL << 0U)    /* Pin 0 Input data check bit */

/* Driver function prototypes */
void GPIO_Init(void);

#endif /* GPIO_H */
