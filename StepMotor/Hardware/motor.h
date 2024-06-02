#ifndef _MOTOR_H_
#define _MOTOR_H_
#endif

#include "stm32f1xx_hal.h"

#define Right 0
#define Left 1

void Motor_Step_R(void);
void Motor_Step_L(void);
void Motor_MultiStep(unsigned int step, unsigned char direction);

