#ifndef __SYSTEM_H
#define __SYSTEM_H 

#include "stm32f10x.h"
#include "Typedef.h"
#include "include.h"
#include "stdlib.h"
#include "string.h"
#include "stdio.h"
#include "math.h"

/*LED*/
#define LED1					PCout(4)
#define LED2					PCout(5)

/*独立按键*/
#define KEY_WKUP			PAin(0)						//高电平有效
#define KEY_S2				PCin(2)
#define KEY_S3				PCin(1)

//#define KEY_Up		PDin(4)
//#define KEY_Down	PDin(1)
//#define KEY_Left	PDin(2)
//#define KEY_Right	PDin(3)
//#define KEY_Sel		PDin(0)

/*蜂鸣器*/
//#define BEEP      PBout(5)
//#define _BEEP(x)	BEEP=1;Delay_ms(x);BEEP=0;

/*函数声明*/
/*初始化函数*/
	void SYSTEM_Init(void);
	
/*简单延时函数*/
	void Delay_us(uint16_t xus);
	void Delay_ms(uint16_t xms);
	
#endif
