/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dma.h"
#include "i2c.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "sysdelay.h"
#include "OLED_I2C.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
unsigned int Speed = 2000;
unsigned int Flag_SW = 0;
unsigned int Model_MP = 0;				//切换手动档和自动�?
uint16_t AdcBuf = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_DMA_Init();
  MX_TIM3_Init();
  MX_I2C1_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */
	OLED_I2C_Init();
	OLED_I2C_Clear();
  	
	__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);	//清除定时器中断标志位，防止一开机就进定时器中断
	HAL_ADCEx_Calibration_Start(&hadc1);		//启动adc1的自动校准
	HAL_ADC_Start_DMA(&hadc1,(uint32_t *)AdcBuf, 1);	//启用adc1的DMA传输
	
	OLED_ShowNum(1,1,Speed,4,16,10);
	OLED_ShowNum(3,3,Model_MP,1,16,10);
	OLED_ShowNum(5,5,AdcBuf,1,16,10);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	if(Model_MP == Automatic)
	{
		SYS_Delay_us(Speed);
	  	Motor_Step_R();
	}  
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == Model_SW_Pin)
	{
		Flag_SW = 1;
		__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);//清除定时器中断标志位，防止一开机就进定时器中断
		HAL_TIM_Base_Start_IT(&htim3);		//打开定时器中断
	}else if(GPIO_Pin == Encoder_CLK_Pin)
	{
		Flag_SW = 2;
		if(HAL_GPIO_ReadPin(Encoder_DT_GPIO_Port,  Encoder_DT_Pin) && Model_MP == Automatic){
			Speed += 100;
			OLED_ShowNum(1,1,Speed,4,16,10);
			OLED_ShowNum(5,5,(uint32_t)AdcBuf,1,16,10);
		}else if(! HAL_GPIO_ReadPin(Encoder_DT_GPIO_Port,  Encoder_DT_Pin) && Model_MP == Automatic){
			Speed -= 100;
			OLED_ShowNum(1,1,Speed,4,16,10);
			OLED_ShowNum(5,5,(uint32_t)AdcBuf,1,16,10);
		}else if(! HAL_GPIO_ReadPin(Encoder_DT_GPIO_Port,  Encoder_DT_Pin) && Model_MP == Manual){
			Motor_MultiStep(1, 0);
		}else if(HAL_GPIO_ReadPin(Encoder_DT_GPIO_Port,  Encoder_DT_Pin) && Model_MP == Manual){
			Motor_MultiStep(1, 1);	
		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	//Speed += 1;
	//if(htim == &htim3)
		__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);//清除定时器中断标志位，防止一�?机就进定时器中断�?
		HAL_TIM_Base_Stop_IT(&htim3);
		__HAL_TIM_SET_COUNTER(&htim3, 0);
		if(! HAL_GPIO_ReadPin(Model_SW_GPIO_Port, Model_SW_Pin) && Flag_SW == 1)
		{
			if(Model_MP == Automatic){Model_MP = Manual;}
			else if(Model_MP == Manual){Model_MP = Automatic;}
			OLED_ShowNum(3,3,Model_MP,1,16,10);
		}
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
