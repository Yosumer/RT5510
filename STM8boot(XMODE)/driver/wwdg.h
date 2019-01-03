#ifndef _wwdg_h_
#define _wwdg_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
#define COUNTERINIT 0x7f	/* 计数初始值 */
#define WINDOW      0x77	/* 窗口上限值 */
extern void WWDG_Ini(u8 Count,u8 WinCount);
extern void WWDG_RstStatus(void);
extern void WWDG_ReloadCount(void);
#endif
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/