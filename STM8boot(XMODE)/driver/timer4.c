/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
#include "include.h"
/*******************************************************************************
****函数名称:
****函数功能:初始化定时器4
****版本:V1.0
****日期:14-2-2014
****入口参数:需要初始化的LED
****出口参数:无
****说明:
********************************************************************************/
void Timer4_Init(u8 Count)
{
        TIM4_DeInit();  	
       	TIM4_TimeBaseInit(TIM4_PRESCALER_16,Count);
        /* Clear TIM4 update flag */
        TIM4_ClearFlag(TIM4_FLAG_UPDATE);
	/* Enable update interrupt */
	TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);
	TIM4_Cmd(ENABLE);
}
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/