#ifndef _uart_h_
#define _uart_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
extern void UART_Init(void);
extern void Send_Dat(u8 dat);
extern void Send_Hex(unsigned char dat);
extern void Send_Str(unsigned char *dat);
u8  Rx_Dat(void);
unsigned char get_rx_dat(void);
#endif
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/