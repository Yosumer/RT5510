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
****函数功能:定时器2PWM初始化
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void TIMER2_PWMInit(void)
{
    TIM2_DeInit();  
    /*
     * TIM2 Frequency = TIM2 counter clock/(ARR + 1) 
     * 这里设置TIM2的计数频率为 16M/8/(1999+1)=1K
     */
    TIM2_TimeBaseInit(TIM2_PRESCALER_8, 1999);	
    
    /* 
     * PWM1 Mode configuration: Channel1
     * TIM2 Channel1 duty cycle = [TIM2_CCR1/(TIM2_ARR + 1)] * 100 = 50%
     * TIM2 Channel2 duty cycle = [TIM2_CCR2/(TIM2_ARR + 1)] * 100 = 50%
     */ 
    
    /* 测试通道1 */
    TIM2_OC1Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 1000, TIM2_OCPOLARITY_HIGH);
    TIM2_OC1PreloadConfig(ENABLE);
    TIM2_Cmd(ENABLE);
}
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/