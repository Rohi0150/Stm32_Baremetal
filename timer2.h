#ifndef TIMER2_H
#define TIMER2_H

#include "common.h"

#define TIM2_BASE 0x40000000UL

typedef struct
{

volatile uint32_t CR1;       //0x00
volatile uint32_t CR2;       //0x04
volatile uint32_t SMCR;      //0x08
volatile uint32_t DIER;      //0x0C
volatile uint32_t SR;        //0x10
volatile uint32_t EGR;       //0x14
volatile uint32_t CCMR1;     //0x18
volatile uint32_t CCMR2;     //0x1C
volatile uint32_t CCER;      //0x20

volatile uint32_t CNT;       //0x24
volatile uint32_t PSC;       //0x28
volatile uint32_t ARR;       //0x2C

}TIM_RegDef_t;


#define TIM2 ((TIM_RegDef_t*)TIM2_BASE)

void TIM2_EnableClock(void)
{
    RCC->APB1ENR |= (1<<0);
}

void TIM2_Init(void)
{

    TIM2_EnableClock();
    TIM2->PSC = 15;        //16MHz / 16 = 1MHz
    TIM2->ARR = 1000-1;    //1ms
    TIM2->EGR |= 1;
    TIM2->CR1 |= 1;
}

void TIM2_Delay_us(uint32_t us)
{

    TIM2->CNT = 0;
    TIM2->ARR = us-1;
    TIM2->SR &= ~(1<<0);
    while(!(TIM2->SR & (1<<0)));

}

void TIM2_Delay_ms(uint32_t ms)
{
    while(ms--)
    {
        TIM2_Delay_us(1000);
    }
}

void TIM2_Delay_s(uint32_t sec)
{
    while(sec--)
    {
        TIM2_Delay_ms(1000);
    }
}

#endif
