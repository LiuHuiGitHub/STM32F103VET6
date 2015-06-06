#include "stm32f10x_it.h"
#include "system.h"

//#define USE_UCOS_II
#ifdef USE_UCOS_II
  extern void OSIntEnter(void);
  extern void OSIntExit(void);
#else
  #define OSIntEnter()
  #define OSIntExit()
#endif

void EXTI0_IRQHandler(void)
{
	OSIntEnter();
	/*KEY_WKUP*/
	if(EXTI_GetITStatus(EXTI_Line0)==SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line0);
	}
	OSIntExit();
}
void EXTI1_IRQHandler(void)
{
	OSIntEnter();
	/*KEY_S3*/
	if(EXTI_GetITStatus(EXTI_Line1)==SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line1);
	}
	OSIntExit();
}
void EXTI2_IRQHandler(void)
{
	OSIntEnter();
	/*KEY_S2*/
	if(EXTI_GetITStatus(EXTI_Line2)==SET)
	{
		EXTI_ClearITPendingBit(EXTI_Line2);
	}
	OSIntExit();
}

void drv_irHandler(void);//红外接收中断

void EXTI9_5_IRQHandler(void)
{
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line5)==SET)
	{
		drv_irHandler();
		EXTI_ClearITPendingBit(EXTI_Line5);
	}
//	if(EXTI_GetITStatus(EXTI_Line8)==SET)
//	{
//		EXTI_ClearITPendingBit(EXTI_Line8);
//	}
	OSIntExit();
}

extern void drv_touchHandler(void);

void EXTI15_10_IRQHandler(void)
{
	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line12)==SET)
	{
		drv_touchHandler();
		EXTI_ClearITPendingBit(EXTI_Line12);
	}
//	if(EXTI_GetITStatus(EXTI_Line13)==SET)
//	{
//		
//		EXTI_ClearITPendingBit(EXTI_Line13);
//	}
	OSIntExit();
}

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
	while (1)
	{
	}
}

void MemManage_Handler(void)
{
	while (1)
	{
	}
}

void BusFault_Handler(void)
{
	while (1)
	{
	}
}

void UsageFault_Handler(void)
{
	while (1)
	{
	}
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}
void SysTick_Handler1ms(void);		//define in main.c
void SysTick_Handler(void)
{
	SysTick_Handler1ms();
}

