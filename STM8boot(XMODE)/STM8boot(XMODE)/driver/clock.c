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
****函数功能:系统时钟切换至HSI
****版本:V1.0
****日期:14-2-2014
****入口参数:HsiDiv-HSI分频数;CpuDiv-系统时钟分频
****出口参数:无
****说明:
********************************************************************************/
void CLOCK_Hsi(CLK_Prescaler_TypeDef HsiDiv,CLK_Prescaler_TypeDef CpuDiv)
{
   CLK_HSICmd(ENABLE);  //使能HSI
  
   while(CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE)==ERROR);//切换配置
   while(CLK_GetFlagStatus(CLK_FLAG_HSIRDY)==RESET); //HSI是否就绪
   CLK_ClockSwitchCmd(ENABLE);//使能切换

   CLK_HSIPrescalerConfig(HsiDiv);//分频设置
   CLK_SYSCLKConfig(CpuDiv);      //使能时钟安全
}
/*******************************************************************************
****函数名称:
****函数功能:系统时钟切换至HSE
****版本:V1.0
****日期:14-2-2014
****入口参数:CpuDiv-系统时钟分频
****出口参数:无
****说明:
********************************************************************************/
void CLOCK_Hse(CLK_Prescaler_TypeDef CpuDiv)
{
    CLK_HSECmd(ENABLE);//使能HSE
    while(CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSE, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE)==ERROR);//切换配置
    while(CLK_GetFlagStatus(CLK_FLAG_HSERDY)==RESET);//等待HSE就绪
    
    CLK_ClockSwitchCmd(ENABLE);//切换 
   
    CLK_ClockSecuritySystemEnable();//使能时钟安全
    CLK_SYSCLKConfig(CpuDiv);//分频设置

}
/*******************************************************************************
****函数名称:
****函数功能:系统时钟切换至LSI
****版本:V1.0
****日期:14-2-2014
****入口参数:CpuDiv-系统时钟分频
****出口参数:无
****说明:LSI需要配置OPTIONBYTE对应位
********************************************************************************/
void CLOCK_Lsi(CLK_Prescaler_TypeDef CpuDiv)
{
    CLK_LSICmd(ENABLE);//使能LSI
    
    while(CLK_ClockSwitchConfig(CLK_SWITCHMODE_AUTO, CLK_SOURCE_LSI, DISABLE, CLK_CURRENTCLOCKSTATE_DISABLE)==ERROR);//切换配置
    while(CLK_GetFlagStatus(CLK_FLAG_LSIRDY)==RESET);//等待LSI就绪
   
    CLK_ClockSwitchCmd(ENABLE);//使能切换
    CLK_SYSCLKConfig(CpuDiv);  //分频

}

/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/