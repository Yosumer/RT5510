#ifndef _ir_h_
#define _ir_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
//定义红外接收头接口
#define IR_PORT GPIOA
#define IR_PINS GPIO_PIN_4
//声明相关功能函数
extern unsigned char a[4];
extern void  IR_Init(void);
extern unsigned char DeCode(void);
#endif

/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/