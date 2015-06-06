#ifndef __SYS_PWM_H__
#define __SYS_PWM_H__

#include "typedef.h"

#define PWM_FREQUENCY				4796		//Hz		PWM_FREQUENCY >= 1 Hz

#define ENABLE_OC1
//#define ENABLE_OC2
//#define ENABLE_OC3
//#define ENABLE_OC4

void sys_pwmInit(void);
void sys_pwmSetRatio(UINT8 Channal, SINT8 ratio);

#endif
