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
****函数功能:初始化KEY接口
****版本:V1.0
****日期:14-2-2014
****入口参数:需要初始化的LED
取值下列值之一或者它们之间的“或运算”组合形式
            KEY1
            KEY2
            KEY3
****出口参数:无
****说明:
********************************************************************************/
void KEY_Init(u8 KeyNum)
{
   if(KeyNum&KEY1)
   {
      GPIO_Init(KEY1_GPIO_PORT, KEY1_GPIO_PINS, GPIO_MODE_IN_FL_NO_IT);
   }
   if(KeyNum&KEY2)
   {
      GPIO_Init(KEY2_GPIO_PORT, KEY2_GPIO_PINS, GPIO_MODE_IN_FL_NO_IT);
   }
   if(KeyNum&KEY3)
   {
      GPIO_Init(KEY3_GPIO_PORT, KEY3_GPIO_PINS, GPIO_MODE_IN_FL_NO_IT);
   }
}
/*******************************************************************************
****函数名称:
****函数功能:返回键值
****版本:V1.0
****日期:14-2-2014
****入口参数:需要初始化的LED
取值下列值之一或者它们之间的“或运算”组合形式
            KEY1
            KEY2
            KEY3
****出口参数:键值
KEY1/KEY2/KEY2的位组合形式，有下列几种情况
            0x00    没有按键按下
            0x01    KEY1被按下
            0x02    KEY2被按下
            0x03    KEY1和KEY2都被按下
            0x04    KEY3被按下
            0x05    KEY1和KEY3被按下
            0x06    KEY2和KEY3被按下
            0x07    KEY1/KEY2/KEY3都被按下
****说明:
********************************************************************************/
u8 KEY_Get(u8 KeyNum)
{
  u8 KeyValue=0x00;
  if(KeyNum&KEY1)
  {
    if( GPIO_ReadInputPin(KEY1_GPIO_PORT, KEY1_GPIO_PINS)==RESET)
      KeyValue|=0x01;
  }
  if(KeyNum&KEY2)
  {
    if( GPIO_ReadInputPin(KEY2_GPIO_PORT, KEY2_GPIO_PINS)==RESET)
      KeyValue|=0x02;
  }
  if(KeyNum&KEY3)
  {
    if( GPIO_ReadInputPin(KEY3_GPIO_PORT, KEY3_GPIO_PINS)==RESET)
      KeyValue|=0x04;
  }
  return KeyValue;
}
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/