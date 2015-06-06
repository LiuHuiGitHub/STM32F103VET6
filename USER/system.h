#ifndef __SYSTEM_H
#define __SYSTEM_H

#include "stm32f10x.h"
#include "Typedef.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

#ifdef STM32F10X_HD
/*LED*/
#define LED1					PCout(4)
#define LED2					PCout(5)
#else
#define LED1					PBout(8)
#endif

/*函数声明*/
/*初始化函数*/
void SYSTEM_Init(void);
void sys_GPIO_Init( GPIO_TypeDef* GPIOx,\
                    UINT16 GPIO_Pin,\
                    GPIOMode_TypeDef GPIO_Mode,\
                    GPIOSpeed_TypeDef GPIO_Speed);
/*简单延时函数*/
void Delay_us(uint16_t xus);
void Delay_ms(uint16_t xms);

#endif
