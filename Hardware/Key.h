#ifndef _KEY_H_
#define _KEY_H_
#include "stm32f1xx_hal.h"
#include "main.h"
#include "tim.h"
#include <stdio.h>

#define IS_ENCODER_DT_SET (HAL_GPIO_ReadPin(Encoder_DT_GPIO_Port, Encoder_DT_Pin))
#define IS_ENCODER_CLK_SET (HAL_GPIO_ReadPin(Encoder_CLK_GPIO_Port, Encoder_CLK_Pin))
#define IS_ENCODER_KEY_SET HAL_GPIO_ReadPin(Encoder_Key_GPIO_Port, Encoder_Key_Pin)
#define IS_KEY_SET_SET HAL_GPIO_ReadPin(Key_set_GPIO_Port, Key_set_Pin)
#define IS_Model_SW_SET HAL_GPIO_ReadPin(Model_SW_GPIO_Port, Model_SW_Pin)

#define MODEL_INTERRUPT 0x01
#define ENCODER_INTERRUPT 0x02
#define ENCODER_KEY_INTERRUPT 0x03
#define KEY_SET_INTERRUPT 0x04

#define Automatic 0
#define Manual 1

int8_t Key_Enter_Get(void);
int8_t Key_Back_Get(void);
int8_t Key_Left_Get(void);
int8_t Key_Right_Get(void);
int8_t Key_MF_AF_Get(void);
void Key_Keep(void);


#endif

