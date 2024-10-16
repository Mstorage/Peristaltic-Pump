/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define Encoder_DT_Pin GPIO_PIN_0
#define Encoder_DT_GPIO_Port GPIOA
#define Encoder_CLK_Pin GPIO_PIN_1
#define Encoder_CLK_GPIO_Port GPIOA
#define Encoder_CLK_EXTI_IRQn EXTI1_IRQn
#define Model_SW_Pin GPIO_PIN_2
#define Model_SW_GPIO_Port GPIOA
#define Model_SW_EXTI_IRQn EXTI2_IRQn
#define Motor_Step_Pin GPIO_PIN_3
#define Motor_Step_GPIO_Port GPIOA
#define Motor_Dir_Pin GPIO_PIN_4
#define Motor_Dir_GPIO_Port GPIOA
#define FLASH_CS_Pin GPIO_PIN_12
#define FLASH_CS_GPIO_Port GPIOB
#define FLASH_SCK_Pin GPIO_PIN_13
#define FLASH_SCK_GPIO_Port GPIOB
#define FLASH_MISO_Pin GPIO_PIN_14
#define FLASH_MISO_GPIO_Port GPIOB
#define FLASH_MOSI_Pin GPIO_PIN_15
#define FLASH_MOSI_GPIO_Port GPIOB
#define Motor_PWMstep_Pin GPIO_PIN_8
#define Motor_PWMstep_GPIO_Port GPIOA
#define OLED_SCK_Pin GPIO_PIN_3
#define OLED_SCK_GPIO_Port GPIOB
#define OLED_CS_Pin GPIO_PIN_4
#define OLED_CS_GPIO_Port GPIOB
#define OLED_MOSI_Pin GPIO_PIN_5
#define OLED_MOSI_GPIO_Port GPIOB
#define OLED_DC_Pin GPIO_PIN_6
#define OLED_DC_GPIO_Port GPIOB
#define OLED_RST_Pin GPIO_PIN_7
#define OLED_RST_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
#define Automatic 0
#define Manual 1
#define ADC_BUF_LENGTH 100
#define MODEL_INTERRUPT 0x01
#define ENCODER_INTERRUPT 0x02

#define SPEED (((10000 / (PSC_Speed + 1)) * 1.8) / 360) * 15 // 通过TIM的PSC值来计算出当前泵的流速
#define IS_ENCODER_DT_SET (HAL_GPIO_ReadPin(Encoder_DT_GPIO_Port, Encoder_DT_Pin))
#define IS_Model_SW_SET HAL_GPIO_ReadPin(Model_SW_GPIO_Port, Model_SW_Pin)

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
