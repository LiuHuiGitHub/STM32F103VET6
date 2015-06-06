#include "include.h"
#include "sys.h"
#include "hwa_key.h"
#include "app_can.h"
#include "app_taskCan.h"

void sys_Init(void)
{
	SYSTEM_Init();
	drv_usart1Init(115200);
//	sys_usartInit(USART2, 115200);
//	sys_usartInit(USART3, 115200);
	sys_pwmInit(10000);
	sys_pwmSetRatio(1, 40);
}

void app_Init(void)
{
//	app_canInit();
}

void taskLED1(void)
{
	LED1 = !LED1;
}

void taskLight(void)
{
    static UINT8 i = 0;
    if(i++ >= 200)
        i = 0;
    if(i<100)
        sys_pwmSetRatio(1, i);
    else
        sys_pwmSetRatio(1, 200-i);
}

int main(void)
{
	sys_Init();
	app_Init();
	sys_taskInit();
	sys_taskAdd(app_taskCanHandler10ms, 10, 0);
	sys_taskAdd(hwa_keyHandler10ms, 10, 1);
    sys_taskAdd(taskLED1, 500, 2);
    sys_taskAdd(taskLight, 50, 4);
	sys_taskStart();
	return 0;
}

void SysTick_Handler1ms(void)						//1ms interrupt ,and turn time max 1ms
{
//	app_canHandler1ms();
}

#ifdef  USE_FULL_ASSERT
volatile UINT8 *pfile;
volatile UINT32 pline;
void assert_failed(uint8_t *file, uint32_t line)
{
	pfile = file;
	pline = line;
	while(1);
}
#endif

