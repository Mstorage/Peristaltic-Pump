#include "sysdelay.h"

void SYS_Delay_us(uint32_t us)
{       
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000000);	//设置HAL_Delay函数的中断间隔为us
    HAL_Delay(us-1);
    HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);	//设置HAL_Delay函数的中断间隔回到ms
}
