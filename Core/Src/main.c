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
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "motor.h"
#include "OLED_SPI.h"
#include "Flash_W25Q.h"
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
uint8_t PSC_Speed = 9;						//定义PSC
unsigned int Speed = 0;
uint8_t Flag_SW = 0;     //用于判断使那个引脚的中断触发了定时器按键防抖中断
uint8_t Model_MP = 0;				//切换手动和自动
uint16_t AdcBuf[ADC_BUF_LENGTH] = {0};
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */
uint32_t AdcBuf_Average(uint16_t* AdcBuf);
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
  MX_ADC1_Init();
  MX_TIM2_Init();
  MX_TIM1_Init();
  MX_USART1_UART_Init();
  MX_SPI1_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  OLED_Init();
  OLED_Clear();

	__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);	
	__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);	
	
	HAL_ADCEx_Calibration_Start(&hadc1);		//启动adc1的auto-correction
	HAL_TIM_Base_Start_IT(&htim2);				//启动定时器TIM2，开始每秒采集一次ADC信号
	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);			//启用TIM1的CH1通道来输出控制步进电机的50%占空比的PWM信号
	HAL_ADC_Start_DMA(&hadc1,(uint32_t*)AdcBuf, ADC_BUF_LENGTH);	//启用adc1的DMA传输

  OLED_GRAM_ShowNum(10, 10, 1234, 4, FONT_SIZE_16X8, 8);
	OLED_GRAM_Printf(30, 25, FONT_SIZE_16X8, "ABC%d", 123);
	//OLED_GRAM_ShowString(30, 25, "ABC", FONT_SIZE_16X8);
	
  for (uint8_t i = 0; i < 2; i++)
    {
      for(uint8_t j = 0; j < 8;j++)
      {
        printf("%x",GRAM[i][j]);
      }
    }
    printf("\n");
	
 
	//OLED_ShowNum(1,4,AdcBuf_Average(AdcBuf),4,16);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */
    /*
	  OLED_DrawBMP(0, 0, 128, 8, BMP1);
	  HAL_Delay(100);
	  OLED_DrawBMP(0, 0, 128, 8, BMP2);
	  HAL_Delay(100);
	  OLED_DrawBMP(0, 0, 128, 8, BMP3);
	  HAL_Delay(100);
	  OLED_DrawBMP(0, 0, 128, 8, BMP4);
	  HAL_Delay(100);
	  OLED_DrawBMP(0, 0, 128, 8, BMP5);
	  HAL_Delay(100);
	  OLED_DrawBMP(0, 0, 128, 8, BMP6);
	  HAL_Delay(100);
	  OLED_DrawBMP(0, 0, 128, 8, BMP7);
	  HAL_Delay(100);
	  OLED_DrawBMP(0, 0, 128, 8, BMP8);
	  HAL_Delay(100);
    */
		OLED_GRAMLODING();	
		   //OLED_GRAMLODING();
		//OLED_ShowString(2,4,"ABC",8);

    /* USER CODE BEGIN 3 */
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
/**
  * @brief  GPIO中断回调函数，用于处理按键和编码器事件;
  * @param  GPIO_Pin：中断函数发送过来的中断引脚;
  * @retval none;
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if(GPIO_Pin == Model_SW_Pin)
	{
		Flag_SW = MODEL_INTERRUPT;
		printf("Flag=1\n");
		__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);
		HAL_TIM_Base_Start_IT(&htim3);		//打开TIM3定时器中断
	}else if(GPIO_Pin == Encoder_CLK_Pin)
	{
		Flag_SW = ENCODER_INTERRUPT;
    /**********自动模式下检测到顺时钟旋转编码器**********/
		if(IS_ENCODER_DT_SET && (Model_MP == Automatic))
    {
			PSC_Speed += 1;
      Motor_PWM_Output_Init(PSC_Speed);
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		OLED_ShowNum(36,0,SPEED,4,8);
		OLED_ShowNum(36,1,PSC_Speed,4,8);
			//OLED_ShowNum(4,1,AdcBuf_Average(AdcBuf),4,16);

		/**********自动模式下检测到逆时钟旋转编码器**********/
    }else if(!IS_ENCODER_DT_SET && (Model_MP == Automatic)){
			PSC_Speed -= 1;
      Motor_PWM_Output_Init(PSC_Speed);
      HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
		OLED_ShowNum(36,0,SPEED,4,8);
		OLED_ShowNum(36,1,PSC_Speed,4,8);
			//OLED_ShowNum(1,4,AdcBuf_Average(AdcBuf),4,16);

    /**********手动模式下检测到顺时钟旋转编码器**********/
		}else if(! IS_ENCODER_DT_SET && (Model_MP == Manual)){
      Motor_GPIO_Output_Init();
			Motor_MultiStep(1, 0);

    /**********手动模式下检测到逆时钟旋转编码器**********/
		}else if(IS_ENCODER_DT_SET && (Model_MP == Manual)){
      Motor_GPIO_Output_Init();
			Motor_MultiStep(1, 1);	
		}
		if(PSC_Speed >= 0){
		htim1.Instance->PSC = PSC_Speed;
		}//修改电机定时器PWM波输出的PSC
 		HAL_TIM_GenerateEvent(&htim1, TIM_EVENTSOURCE_UPDATE);	// 更新定时器的设置
	}
}

/**
  * @brief  TIM定时器中断回调函数;
  * @param  htim：中断函数发送过来的定时器地址;
  * @retval none;
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/**********控制ADC采样的TIM2发生中断时**********/
  if(htim == &htim2){
		__HAL_TIM_CLEAR_IT(&htim2, TIM_IT_UPDATE);
		HAL_ADC_Start_DMA(&hadc1,(uint32_t *)AdcBuf, ADC_BUF_LENGTH);

		//OLED_ShowNum(1,4,AdcBuf_Average(AdcBuf),4,16);

  /**********控制按键防抖的TIM3发生中断时**********/
	}else if(htim == &htim3){

		__HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);
		HAL_TIM_Base_Stop_IT(&htim3);
		__HAL_TIM_SET_COUNTER(&htim3, 0);

		printf("keyunshake_OK\n");
		if(! IS_Model_SW_SET && Flag_SW == MODEL_INTERRUPT)
		{
			if(Model_MP == Automatic){

        Model_MP = Manual;
        Motor_GPIO_Output_Init();

      }else if(Model_MP == Manual){

        Model_MP = Automatic;
        Motor_PWM_Output_Init(PSC_Speed);

      }

			OLED_ShowNum(1,3,Model_MP,1,16);
		}
  }
}

/**
  * @brief  ADC采样计算平均值函数，求出最准确的采样电流值;
  * @param  Adc_Buf：ADC通过DMA传输记录的数组值;
  * @retval sample_current： 计算得出的采样电流值;
  */
uint32_t AdcBuf_Average(uint16_t* AdcBuf)
{
  uint32_t sum = 0;
  double average = 0;
  uint16_t sample_current = 0;
    // 计算ADC数组元素的总和
    for (int i = 0; i < ADC_BUF_LENGTH; ++i) {
        sum += AdcBuf[i];
    }
    // 计算平均值
    average = sum / ADC_BUF_LENGTH;
    sample_current = (average * 165) / 205;
    //average = ((sum / ADC_BUF_LENGTH) / 4095) * 165; //165 = 3.3V / 20mR
    //计算采样电流值
    //sample_current = (uint32_t)average;
    return sample_current;
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
