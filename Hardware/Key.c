#include "Key.h"

uint8_t Flag_SW = 0;     //用于判断使那个引脚的中断触发了定时器按键防抖中断
uint8_t Model_MP = 0;			//切换手动和自动
uint8_t flag_encoder = 0; //编码器会进两次中断标记中断次数 0或1
uint8_t PSC_Speed = 9;						//定义PSC

struct 
{
	uint8_t Enter;	//确认键
	uint8_t Back;	//返回键
	uint8_t Left;	//左
	uint8_t Right;	//右
    uint8_t MF_AF;	//手自动切换
} Key;

/**
  * @brief  获取确认按键函数
  * @param  none
  * @retval 1:确认键按下
  */
int8_t Key_Enter_Get(void)	//确认键
{
	if(Key.Enter)
	{
		Key.Enter = 0;
		return 1;
	}
	return 0;
}
/**
  * @brief  获取返回按键函数
  * @param  none
  * @retval 1:返回键按下
  */
int8_t Key_Back_Get(void)	//返回键
{
	if(Key.Back)
	{
		Key.Back = 0;
		return 1;
	}
	return 0;
}
/**
  * @brief  获取向右按键函数
  * @param  none
  * @retval 1:编码器顺时钟旋转
  */
int8_t Key_Right_Get(void)	//右键
{
	if(Key.Right)
	{
		Key.Right = 0;
		return 1;
	}
	return 0;
}
/**
  * @brief  获取向右按键函数
  * @param  none
  * @retval 1:编码器逆时钟旋转
  */
int8_t Key_Left_Get(void)	//左键
{
	if(Key.Left)
	{
		Key.Left = 0;
		return 1;
	}
	return 0;
}
/**
  * @brief  获取手动自动挡切换按键函数
  * @param  none
  * @retval 1:切换挡位
  */
int8_t Key_MF_AF_Get(void)	//手动货自动键
{
	if(Key.MF_AF)
	{
		Key.MF_AF = 0;
		return 1;
	}
	return 0;
}

void Key_KeepPress(void)
{
    if(! IS_Model_SW_SET && Flag_SW == MODEL_INTERRUPT)
		{
            Key.Enter = 1;
			//OLED_ShowNum(1,3,Model_MP,1,16);
		}else{
            Key.Enter = 0;
        }
}

/**
  * @brief  GPIO中断回调函数，用于处理按键和编码器事件;
  * @param  GPIO_Pin：中断函数发送过来的中断引脚;
  * @retval none;
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    /**********自动模式下检测到顺时钟旋转编码器**********/
	if(GPIO_Pin == Model_SW_Pin)
	{
		Flag_SW = MODEL_INTERRUPT;
        __HAL_TIM_CLEAR_IT(&htim3, TIM_IT_UPDATE);
	    HAL_TIM_Base_Start_IT(&htim3);		//打开TIM3定时器中断
        Key.Enter = 1;
		printf("Flag=1\n");
	}
    
    else if(GPIO_Pin == Encoder_CLK_Pin)
	{
		Flag_SW = ENCODER_INTERRUPT;
    /**********自动模式下检测到顺时钟旋转编码器**********/
	    if(IS_ENCODER_DT_SET && flag_encoder == 0 &&(Model_MP == Automatic))
    {
        if(!IS_ENCODER_CLK_SET)
      {
        flag_encoder = 1;
      } 
    /**********自动模式下检测到逆时钟旋转编码器**********/
    }else if(!IS_ENCODER_DT_SET && flag_encoder == 0 && (Model_MP == Automatic)){
      if(!IS_ENCODER_CLK_SET)
      {
        flag_encoder = 1;
      } 
    }

    if(!IS_ENCODER_DT_SET && flag_encoder == 1 &&(Model_MP == Automatic))
    {
        int Delay = 1000; while(Delay--);
        if(IS_ENCODER_CLK_SET){
            PSC_Speed += 1;
            flag_encoder = 0;
            Key.Right = 1;
            //Motor_PWM_Output_Init(PSC_Speed);
            //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        printf("right\n");
      }
		
    }else if(IS_ENCODER_DT_SET && flag_encoder == 1 && (Model_MP == Automatic)){
        int Delay = 1000; while(Delay--);
		if(IS_ENCODER_CLK_SET){
            PSC_Speed -= 1;
            flag_encoder = 0;
            Key.Left = 1;
            //Motor_PWM_Output_Init(PSC_Speed);
            //HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
        printf("left\n");
      }
    }
    /**********手动模式下检测到顺时钟旋转编码器**********/
		}else if(! IS_ENCODER_DT_SET && (Model_MP == Manual)){
            Motor_GPIO_Output_Init();
			Motor_MultiStep(1, 0);
            printf("right_MF\n");
    /**********手动模式下检测到逆时钟旋转编码器**********/
		}else if(IS_ENCODER_DT_SET && (Model_MP == Manual)){
            Motor_GPIO_Output_Init();
			Motor_MultiStep(1, 1);	
            printf("left_MF\n");
		}
	// 	if(PSC_Speed >= 0){
	// 	htim1.Instance->PSC = PSC_Speed;

    //     HAL_EXTI_ClearPending(EXTI_LINE_1,EXTI_TRIGGER_RISING_FALLING);//新增 清除中断
	// }//修改电机定时器PWM波输出的PSC
 	// 	HAL_TIM_GenerateEvent(&htim1, TIM_EVENTSOURCE_UPDATE);	// 更新定时器的设置
}
