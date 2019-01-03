#ifndef _beep_h_
#define _beep_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
//定义蜂鸣器频率
#define beep_1k (BEEP_FREQUENCY_1KHZ)
#define beep_2k (BEEP_FREQUENCY_2KHZ)
#define beep_4k (BEEP_FREQUENCY_4KHZ)

extern void Beep_Init(BEEP_Frequency_TypeDef beep_fre);
#endif
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/