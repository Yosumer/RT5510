/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
#include "include.h"

extern unsigned short LowTime;//低电平时间
extern unsigned short HighTime;//高电平时间
unsigned char a[4];    //储存用户码、用户反码与键数据码、键数据反码
/*******************************************************************************
****函数名称:
****函数功能:初始化红外传感器接口
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:一体化红外接收头接PA4，设置这个端口位外部中断，下降沿触发
********************************************************************************/
void  IR_Init(void)
{
   EXTI_DeInit();//初始化相关寄存器位默认值
   GPIO_Init(GPIOA, GPIO_PIN_4, GPIO_MODE_IN_PU_IT);//上拉输入带中断
   EXTI_SetExtIntSensitivity(EXTI_PORT_GPIOA, EXTI_SENSITIVITY_FALL_ONLY);//下降沿触发
   enableInterrupts();//使能所有中断
}

/*******************************************************************************
****函数名称:
****函数功能:红外解码函数
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:1-解码正确 0-解码错误
****说明:无
********************************************************************************/
unsigned char DeCode(void)
{
    unsigned char  i,j;
    unsigned char temp;    //储存解码出的数据
    for(i=0;i<4;i++)
    {
      for(j=0;j<8;j++)
      {
         temp=temp>>1;  //temp中的各数据位右移一位，因为先读出的是高位数据	
         TIM2_SetCounter(0x0000);
         TIM2_Cmd(ENABLE);
         while(!GPIO_ReadInputPin(GPIOA, GPIO_PIN_4));
         TIM2_Cmd(DISABLE);
         LowTime=TIM2_GetCounter();
  
         TIM2_SetCounter(0x0000);
         TIM2_Cmd(ENABLE);
         while(GPIO_ReadInputPin(GPIOA, GPIO_PIN_4));
         TIM2_Cmd(DISABLE);
         HighTime=TIM2_GetCounter();
         
         if((LowTime<900)||(LowTime>1300))
         {
            return 0;
         }
         if((HighTime>1000)&&(HighTime<1200))
         {
           temp=temp&0x7f;
         }
         if((HighTime>3000)&&(HighTime<3500))
         {
           temp=temp|0x80;
         }
      }
      a[i]=temp;
      
    }
    if(a[2]=~a[3])//校验
      return 1;
    else
      return 0;
}

/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/