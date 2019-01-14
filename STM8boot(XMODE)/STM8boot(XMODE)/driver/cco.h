#ifndef _cco_h_
#define _cco_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
//定义CCO相关时钟源与分频值
#define CCO_HSI (CLK_OUTPUT_HSI)
#define CCO_LSI (CLK_OUTPUT_LSI)
#define CCO_HSE (CLK_OUTPUT_HSE)
#define CCO_CPU (CLK_OUTPUT_CPU)
#define CCO_CPUDIV2 (CLK_OUTPUT_CPUDIV2)
#define CCO_CPUDIV4 (CLK_OUTPUT_CPUDIV4)
#define CCO_CPUDIV8 (CLK_OUTPUT_CPUDIV8)
#define CCO_CPUDIV16 (CLK_OUTPUT_CPUDIV16)
#define CCO_CPUDIV32 (CLK_OUTPUT_CPUDIV32)
#define CCO_CPUDIV64 (CLK_OUTPUT_CPUDIV64)
#define CCO_HSIRC (CLK_OUTPUT_HSIRC)
#define CCO_MASTERC (CLK_OUTPUT_MASTER)
#define CCO_OTHERS (CLK_OUTPUT_OTHERS)
extern void CCO_Out(CLK_Output_TypeDef CCOSOURCE);

#endif 
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/