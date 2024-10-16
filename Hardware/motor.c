#include "motor.h"

/**
  * @brief  控制电机顺时钟方向旋转（泵抽水方向）;
  * @param  none
  * @retval none
  */
void Motor_Step_R(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}

/**
  * @brief  控制电机逆时钟方向旋转（泵吸水方向）;
  * @param  none
  * @retval none
  */
void Motor_Step_L(void)
{
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
}

/**
  * @brief  控制步进电机向指定方向前进若干步;
  * @param  step：需要步进的长度;
  * @param  step：需要步进的长度;
  * @retval 返回步进电机状态 MOTOR_OK or MOTOR_BUSY;
  */
uint8_t Motor_MultiStep(unsigned int step, unsigned char direction)
{
	if(direction == Right){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_RESET);}
	else if(direction == Left){HAL_GPIO_WritePin(GPIOA, GPIO_PIN_4, GPIO_PIN_SET);}
	for(int i = 0; i < step; i++)
	{
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_8, GPIO_PIN_RESET);
	}
	return MOTOR_OK;
}



