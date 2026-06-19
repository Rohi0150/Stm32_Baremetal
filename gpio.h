#ifndef GPIO_H
#define GPIO_H

#include "common.h"

#define GPIOA_BASE 0x40020000UL
#define GPIOB_BASE 0x40020400UL
#define GPIOC_BASE 0x40020800UL

typedef struct
{
    volatile uint32_t MODER;
    volatile uint32_t OTYPER;
    volatile uint32_t OSPEEDR;
    volatile uint32_t PUPDR;
    volatile uint32_t IDR;
    volatile uint32_t ODR;
    volatile uint32_t BSRR;
    volatile uint32_t LCKR;
    volatile uint32_t AFRL;
    volatile uint32_t AFRH;
} GPIO_RegDef_t;

#define GPIOA ((GPIO_RegDef_t*)GPIOA_BASE)
#define GPIOB ((GPIO_RegDef_t*)GPIOB_BASE)
#define GPIOC ((GPIO_RegDef_t*)GPIOC_BASE)

typedef enum
{
    INPUT,
    OUTPUT
} GPIO_Mode_t;
typedef enum
{
    LOW,
    HIGH
} GPIO_STATUS;


void GPIO_EnableClock(GPIO_RegDef_t *port)
{
    uint32_t bitpos = ((uint32_t)port - GPIOA_BASE) / 0x400UL;
    RCC->AHB1ENR |= (1U << bitpos);
}

void GPIO_Init(GPIO_RegDef_t *port, uint8_t pin, GPIO_Mode_t mode)
{
    GPIO_EnableClock(port);

    port->MODER &= ~(3U << (pin * 2));          // always clear first -> this alone IS input mode (00)

    if (mode == OUTPUT)
    {
        port->MODER  |= (1U << (pin * 2));       // 01 = output
        port->OTYPER &= ~(1U << pin);            // push-pull
    }
    // GPIO_MODE_INPUT needs nothing further here — 00 was already set above

    port->PUPDR &= ~(3U << (pin * 2));           // floating either way (see note below)
}

void GPIO_SetPin(GPIO_RegDef_t *port, uint8_t pin)
{
    port->BSRR = (1U << pin);
}

void GPIO_ClearPin(GPIO_RegDef_t *port, uint8_t pin)
{
    port->BSRR = (1U << (pin + 16));
}

void GPIO_TogglePin(GPIO_RegDef_t *port, uint8_t pin)
{
    port->ODR ^= (1U << pin);
}

uint8_t GPIO_ReadPin(GPIO_RegDef_t *port, uint8_t pin)
{
    return (uint8_t)((port->IDR >> pin) & 0x1U);
}

#endif
