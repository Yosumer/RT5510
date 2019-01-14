#ifndef _gpio_h_
#define _gpio_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/

//  定义LED名称
#define  LED1       0x01
#define  LED2       0x02
#define  LED3       0x04
//定义LED接口
#define LED1_GPIO_PORT  (GPIOC)
#define LED1_GPIO_PINS  (GPIO_PIN_3)

#define LED2_GPIO_PORT  (GPIOC)
#define LED2_GPIO_PINS  (GPIO_PIN_4)

#define LED3_GPIO_PORT  (GPIOD)
#define LED3_GPIO_PINS  (GPIO_PIN_2)
//声明功能函数
extern void LED_Init(u8 LedNum);
extern void LED_On(u8 LedNum);
extern void LED_Off(u8 LedNum);
extern void LED_Toggle(u8 LedNum);
#endif
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/