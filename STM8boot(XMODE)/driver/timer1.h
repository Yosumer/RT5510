#ifndef _timer1_h_
#define _timer1_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
extern void Time1_InputCap();
extern u16 Timer1_InputCapCFre(void);
extern void Get_FrequencyArray(u16 FrequencyArray[], u8 times);
extern u16 Get_AverageFrequency(u16 GetFrequency[],u8 n);
extern void Timer1_OutCompare(void);

#endif
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/