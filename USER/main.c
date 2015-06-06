#include "include.h"
#include "hwa_key.h"

void SysTick_Handler1ms(void);
void SysTick_Handler10ms(void);

void sys_Init(void)
{
	SYSTEM_Init();
	drv_usart1Init(115200);
//	sys_usartInit(USART2, 115200);
//	sys_usartInit(USART3, 115200);
//	sys_adcInit();
//	sys_canInit();
	sys_pwmInit();
	sys_pwmSetRatio(1, 50);
	
//	dr_flashInit();
//	dr_e2Init();
	drv_irInit();
	
//	BACK_COLOR = BLUE;
//	POINT_COLOR = RED;
	drv_lcdInit();
	drv_touchInit();
	
	hwa_keyInit();
}

int main(void)
{
	UINT8 s[20];
	UINT8 i = 0;
	sys_Init();
	
	while(1)
	{
		if(b_irRevFlag == TRUE)
		{
			b_irRevFlag = FALSE;
			sprintf((char*)s, "%2x%2x%2x\0", u8_irRecAdd1, u8_irRecAdd2, u8_irRecData1);
			drv_lcdShowString(0, 0, s);
			LED1 = 1;
			Delay_ms(10);
			LED1 = 0;
		}
		if(Pen_Point.Key_Sta == Pen_Down)
		{
			Pen_Point.Key_Sta = Pen_Up;
			drv_touchReadCoord();
			drv_lcdDrawBigPoint(Pen_Point.X0, Pen_Point.Y0);
		}
		if(KEY_K[0].Flags.state == KEY_DOWN)
		{
			KEY_K[0].Flags.state = KEY_UP;
			drv_lcdClear(BLUE);
			LED1 = !LED1;
		}
		if(KEY_K[0].Flags.trig == TRUE)
		{
			KEY_K[0].Flags.trig = FALSE;
			LED1 = 0;
			sprintf((char*)s, "%d\0", (int)i++);
			drv_lcdShowString(0, 0, s);
		}
		if(KEY_K[1].Flags.trig == TRUE)
		{
			KEY_K[1].Flags.trig = FALSE;
			LED1 = 0;
			sprintf((char*)s, "%d\0", (int)i++);
			drv_lcdShowString(0, 50, s);
		}
		if(KEY_K[2].Flags.trig == TRUE)
		{
			KEY_K[2].Flags.trig = FALSE;
			LED1 = 0;
			sprintf((char*)s, "%d\0", (int)i++);
			drv_lcdShowString(0, 100, s);
		}
	}
}

void SysTick_Handler1ms(void)						//1ms interrupt ,and turn time max 1ms 
{
	static UINT8 u8_TimeCount10ms = 0;
	u8_TimeCount10ms++;
	if(u8_TimeCount10ms >= 10)
	{
		u8_TimeCount10ms = 0;
		SysTick_Handler10ms();
	}
}

void SysTick_Handler10ms(void)						//10ms interrupt ,and turn time max 1ms 
{
	static UINT8 u8_TimeCount1s = 0;
	u8_TimeCount1s++;
	if(u8_TimeCount1s >= 50)
	{
		u8_TimeCount1s = 0;
		LED2 = !LED2;
	}
	hwa_keyHandler10ms();
}
