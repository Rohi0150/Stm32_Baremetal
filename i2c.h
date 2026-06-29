/*
 * i2c.h - Software I2C Implementation
 * 
 * PA4 = SCL
 * PA5 = SDA
 */
#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>
#include "gpio.h"
#include "timer2.h"

#define SCL_PIN   4
#define SDA_PIN   5

#define I2C_ACK   0
#define I2C_NACK  1

/* SCL and SDA pin control macros */
static inline void SCL_HIGH(void) { GPIO_SetPin(GPIOB, SCL_PIN);   }
static inline void SCL_LOW(void)  { GPIO_ClearPin(GPIOB, SCL_PIN); }
static inline void SDA_HIGH(void) { GPIO_SetPin(GPIOB, SDA_PIN);   }
static inline void SDA_LOW(void)  { GPIO_ClearPin(GPIOB, SDA_PIN); }

/* I2C timing delay */
void I2C_Delay(void)
{
    TIM2_Delay_us(50);
}

/* Generate I2C START condition
 * SDA transitions from HIGH to LOW while SCL is HIGH */
void i2c_start(void)
{
    SDA_HIGH();
    SCL_HIGH();
    I2C_Delay();

    SDA_LOW();       /* sda high to low while scl is high */
    I2C_Delay();

    SCL_LOW();
    I2C_Delay();
}

/* Transmit one byte and read ACK/NACK from slave
 * Returns: 0 = ACK, 1 = NACK */
uint8_t i2c_transf_byte(uint8_t byte)
{
    for (uint8_t i = 0; i < 8; i++)
    {
        if (byte & 0x80)
            SDA_HIGH();
        else
            SDA_LOW();
        byte <<= 1;
        I2C_Delay();
        SCL_HIGH();
        I2C_Delay();
        SCL_LOW();
        I2C_Delay();
    }

    /* Read ACK/NACK from slave */
    SDA_HIGH();          /* release sda */
    I2C_Delay();
    SCL_HIGH();
    I2C_Delay();

    uint8_t nack = GPIO_ReadPin(GPIOB, SDA_PIN);
    /* SDA = 0 means Slave pulled LOW  = ACK
     * SDA = 1 means SDA stayed HIGH   = NACK */
    SCL_LOW();
    I2C_Delay();

    return nack;
}

/* Generate I2C STOP condition
 * SDA transitions from LOW to HIGH while SCL is HIGH */
void i2c_stop(void)
{
    SDA_LOW();
    I2C_Delay();

    SCL_HIGH();
    I2C_Delay();

    SDA_HIGH();    /* SDA rises while SCL is HIGH */
    I2C_Delay();
}

/* Initialize I2C pins and peripherals */
void i2c_init(void)
{
    TIM2_Init();
    GPIO_EnableClock(GPIOB);
    GPIO_Init(GPIOB, SCL_PIN, OUTPUT);
    GPIO_Init(GPIOB, SDA_PIN, OUTPUT);
}

#endif /* __I2C_H__ */
