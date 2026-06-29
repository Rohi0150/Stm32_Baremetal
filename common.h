#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#define PERIPH_BASE        0x40000000UL
#define RCC_OFFSET  (0x00023800UL)
#define RCC_BASE           (PERIPH_BASE + RCC_OFFSET)

typedef struct
{
    volatile uint32_t CR;          //0x00
    volatile uint32_t PLLCFGR;     //0x04
    volatile uint32_t CFGR;        //0x08
	volatile uint32_t CIR;         //0x0C
	volatile uint32_t AHB1RSTR;    //0x10
	uint32_t RESERVED0[3];
	volatile uint32_t APB1RSTR;    //0x20
	volatile uint32_t APB2RSTR;    //0x24
	uint32_t RESERVED1[2];
	volatile uint32_t AHB1ENR;     //0x30
	uint32_t RESERVED2[2];
	volatile uint32_t AHB2ENR;     //0x3C
	volatile uint32_t APB1ENR;     //0x40
	volatile uint32_t APB2ENR;     //0x44
} RCC_RegDef_t;

#define RCC ((RCC_RegDef_t*)RCC_BASE)

#endif
