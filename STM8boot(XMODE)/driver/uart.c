/*******************************************************************************
****����Ƕ��ʽϵͳ STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****�汾:V1.0
****����:14-2-2014
****˵��:��оƬSTM8S105C4T6
********************************************************************************/
#include "include.h"

uint8_t HexTable[]={'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};
/*******************************************************************************
****��������:
****��������:��ʼ��UART
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:��
****˵��:
********************************************************************************/
void UART_Init(void)
{
    UART2_DeInit();
    UART2_Init((u32)9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE, UART2_MODE_TXRX_ENABLE);
   UART2_ITConfig(UART2_IT_RXNE_OR, DISABLE);
    
  //  UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);//�ж�ʹ�ܽ���
    UART2_Cmd(ENABLE);
}
/*******************************************************************************
****��������:
****��������:����8λ����
****�汾:V1.0
****����:14-2-2014
****��ڲ���:dat-��Ҫ���͵�����
****���ڲ���:��
****˵��:
********************************************************************************/
 void Send_Dat(u8 dat)
{
   while(( UART2_GetFlagStatus(UART2_FLAG_TXE)==RESET));
   UART2_SendData8(dat);
}
 u8 u8_data,flag;
/*******************************************************************************/
//deleted by zhang in 20181215
//u8  Rx_Dat(void)
//{
//
//  //if(flag) 
//  nop();
//  nop();
//  nop();
//  while(UART2_GetFlagStatus(UART2_FLAG_RXNE)==RESET);    
//  flag=UART2_GetFlagStatus(UART2_FLAG_RXNE);
//  u8_data = UART2_ReceiveData8();
// //UART2_ClearFlag(UART2_FLAG_RXNE);
////Send_Dat(u8_data);
//
//  return u8_data;
//
//   
// // return  0; 
//}

//modified by zhang in 20181215
u8  Rx_Dat(void)
{
  u32 timeout_tick = 4000000;
  //if(flag) 
  nop();
  nop();
  nop();
  while(UART2_GetFlagStatus(UART2_FLAG_RXNE)==RESET)
  {
    if( --timeout_tick <= 1)
    {
      return 0;
    }
  }
  flag=UART2_GetFlagStatus(UART2_FLAG_RXNE);
  u8_data = UART2_ReceiveData8();
 //UART2_ClearFlag(UART2_FLAG_RXNE);
//Send_Dat(u8_data);

  return u8_data;

   
 // return  0; 
}
/******************************************************************************/
unsigned char get_rx_dat(void)
{
  return u8_data;
}
/*******************************************************************************
****��������:
****��������:����Hex����
****�汾:V1.0
****����:14-2-2014
****��ڲ���:dat-��Ҫ���͵�����
****���ڲ���:��      
****˵��:
********************************************************************************/
void Send_Hex(unsigned char dat)
{
   Send_Dat('0');
   Send_Dat('x');
   Send_Dat(HexTable[dat>>4]);
   Send_Dat(HexTable[dat&0x0f]);
   Send_Dat(' ');
}
/*******************************************************************************
****��������:
****��������:�����ַ���
****�汾:V1.0
****����:14-2-2014
****��ڲ���:dat-��Ҫ���͵�����
****���ڲ���:��
****˵��:
********************************************************************************/
void Send_Str(unsigned char *dat)
{
  while(*dat!='\0')
		
    {			
      Send_Dat(*dat);			
      dat++;	
    }
}
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/