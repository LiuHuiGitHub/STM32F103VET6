#include "system.h"

/*LED��ʼ��*/
void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	LED1 = 0;LED2 = 0;
}
///*��������ʼ��*/
//void BEEP_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOB, &GPIO_InitStructure);
//	BEEP = 0;	
//}
///*�������ʼ��*/
//void DirectionButton_Init(void)
//{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);
//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4;
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_10MHz;
//	GPIO_Init(GPIOD, &GPIO_InitStructure);
//}

/*������ʼ��*/
void BUTTON_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOC, ENABLE);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);//@�����ⲿ�жϱ��뿪��AFIOʱ��
/**
@code
  =========================================================================================
    NVIC_PriorityGroup   | NVIC_IRQChannelPreemptionPriority | NVIC_IRQChannelSubPriority  
  =========================================================================================
   NVIC_PriorityGroup_0  |                0                  |            0-15             
  -----------------------------------------------------------------------------------------
	 NVIC_PriorityGroup_1  |                0-1                |            0-7              
  -----------------------------------------------------------------------------------------  
   NVIC_PriorityGroup_2  |                0-3                |            0-3              
  -----------------------------------------------------------------------------------------   
   NVIC_PriorityGroup_3  |                0-7                |            0-1              
  -----------------------------------------------------------------------------------------   
   NVIC_PriorityGroup_4  |                0-15               |            0                                
  =========================================================================================
@endcode
*/
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;//�ж�Դ �ⲿ�ж���0
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;//�ж���ռ����Ȩ
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;//�жϴ����ȼ�
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//ʹ���ж�
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;//�ж�Դ �ⲿ�ж���1
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;//�ж�Դ �ⲿ�ж���2
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;//�ж�Դ �ⲿ�ж���9-5
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
//	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;//�ж�Դ �ⲿ�ж���15-10
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource0);//ѡ��PA0���ⲿ�ж���·
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource1);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC, GPIO_PinSource2);

	EXTI_ClearFlag(EXTI_Line0 | EXTI_Line1 | EXTI_Line2);//����ⲿ�жϱ�־
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line1 | EXTI_Line2;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//����EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//����EXTI��·Ϊ�ж�����
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//�����ش���
	EXTI_InitStructure.EXTI_LineCmd	= ENABLE;
	EXTI_Init(&EXTI_InitStructure);
}
/*��ʱnus����*/
void Delay_us( uint16_t xus )
{
	volatile uint8_t i;
	while(xus--)for(i=0; i<9; i++);
}
/*��ʱnms����*/
void Delay_ms(uint16_t xms)
{
	while(xms--)Delay_us(1000);
}

///*��ȡоƬID*/
//uint32_t STM32ID[3];
//void GetDeviceSerialID(void)
//{
//    STM32ID[0] = *(__IO uint32_t*)(0x1FFFF7E8);
//    STM32ID[1] = *(__IO uint32_t*)(0x1FFFF7EC);
//    STM32ID[2] = *(__IO uint32_t*)(0x1FFFF7F0);
//}
/*��ʼ��*/
void SYSTEM_Init(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);//�ж�������
//	GetDeviceSerialID();
	LED_Init();
//	BEEP_Init();
	BUTTON_Init();
//	DirectionButton_Init();
	SysTick_Config(SystemCoreClock/1000);
}
