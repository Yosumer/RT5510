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
****函数功能:初始化外部中断
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:三个按键接PA4/5/6，设置这三个端口位外部中断，下降沿触发
********************************************************************************/
void  EXTI_Init(void)
{
   EXTI_DeInit();//恢复外部中断相关寄存器为默认值
   GPIO_Init(GPIOB, GPIO_PIN_4|GPIO_PIN_3, GPIO_MODE_IN_PU_IT);//上拉输入有中断
   EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOB, EXTI_SENSITIVITY_FALL_ONLY);//下降沿触发
   
   GPIO_Init(GPIOD, GPIO_PIN_3|GPIO_PIN_7, GPIO_MODE_IN_PU_IT);//上拉输入有中断
   EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOD, EXTI_SENSITIVITY_FALL_ONLY);//下降沿触发
   
   enableInterrupts();//使能所有中断
}
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/