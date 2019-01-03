#include"stm8s.h"
#include "comm.h"
#include"main.h"
#include "uart485.h"

extern unsigned char nTxBuf[MAX_SEND_COUNT],nRxBuf[MAX_RECEIVE_COUNT],nRxBuf2[MAX_RECEIVE_COUNT];
extern  bool bReceivePacketOk, bMasterSendPacket,bReceivePacket;
extern unsigned int RX_Index ;
unsigned char RXOK = 0;
unsigned int nctrlType = NORMAL_CTRL;
unsigned char Button;
unsigned int ucTotalRunTime=0;
extern __no_init BITS GlobalFlags0 ;
#define bTime10ms 	GlobalFlags0.bD0
#define bTime1msFlag 	GlobalFlags0.bD1
#define BUFFER_LENGTH          32

void uart2_init(void)
{
          //GPIO_Init(GPIOD,GPIO_PIN_5,GPIO_MODE_OUT_PP_LOW_FAST);//TX2
          //GPIO_Init(GPIOD,GPIO_PIN_6,GPIO_MODE_OUT_PP_LOW_FAST);//Rx2
           GPIO_Init(UART2_PORT, UART2_RX_PIN, GPIO_MODE_IN_FL_NO_IT);
           GPIO_Init(UART2_PORT, UART2_TX_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
           CLK_PeripheralClockConfig(CLK_PERIPHERAL_UART2, ENABLE);
           UART2_DeInit();
           UART2_Init(9600, UART2_WORDLENGTH_8D, UART2_STOPBITS_1, UART2_PARITY_NO, UART2_SYNCMODE_CLOCK_DISABLE,UART2_MODE_TXRX_ENABLE);
           UART2_ITConfig(UART2_IT_RXNE_OR, ENABLE);//中断使能接收
          //UART2_ITConfig(UART2_IT_TXE, ENABLE);//中断使能发送
           UART2_Cmd(ENABLE);
            /* 
              UART2->CR2 = 0x00; //disable Tx & Rx
              UART2->CR1  = 0x00;
              UART2->CR3  = 0x00;	
              UART2->BRR2 = 0x0A;//	//115200 bit/s
              UART2->BRR1 = 0x08;//;
              UART2->CR2  = 0X2C;//0x26 | 0x08;	//0x0C;
              */
}
unsigned short update_cmd;
extern void Update_Cmd_Resp(void);



void  uart2_received(void)
{   

  
   // unsigned int i;
    unsigned char nInChar;
    unsigned char ucCheckSum; 
    unsigned long time_L,time_H;        
  // UART2_SR &= 0xDF;		//RXNE可由软件清0
    if( UART2_GetITStatus( UART2_IT_RXNE ) == SET)
    {
                nInChar = UART2_ReceiveData8();
            
		if((RX_Index == 0) && (nInChar != EOI_AA)) return ; 
		if((RX_Index == 0) && (nInChar == EOI_AA))
		{
		    RX_Index = 0 ;
		    nRxBuf[RX_Index] = nInChar ;
		    RX_Index++ ;
		}
		else
		{
                    nRxBuf[RX_Index] = nInChar ;
		    RX_Index++ ;	
		    //RX_Index %= BUFFER_LENGTH ;//MAX_INBUFFER_COUNT;         
                  if(nInChar==EOI_AB )
                      {
                                ucCheckSum=0;
                                ucCheckSum += nRxBuf[1] ;//+ ucBlueToothRXBuffer[1];
                                ucCheckSum += nRxBuf[2] ;//+ ucBlueToothRXBuffer[1];
                                ucCheckSum += nRxBuf[3] ;//+ ucBlueToothRXBuffer[1];
                                ucCheckSum += nRxBuf[4] ;//+ ucBlueToothRXBuffer[1];
                                ucCheckSum = ~ucCheckSum;
                                ucCheckSum &= 0x7f;
                          if(ucCheckSum == nRxBuf[5])
                            { 
                               //memcpy( &Key, &Uart_RX_Buf[1], 6);
                               // Data = nRxBuf[1];
                                //nctrlType = Data;
                                Button = nRxBuf[2];
                                
                               //Button = nRxBuf[2];
                                time_L = nRxBuf[3]; //注意是5位。
	                        time_H = nRxBuf[4]; //注意是5位。
                                time_L &= 0x1f;
	                        time_H &= 0x1f;
                                if(H10_KEY_WORK_EN ==  Button)
                                  {
                                     ucTotalRunTime = (time_H * 32)+time_L;
                                     ucTotalRunTime *= 60; //秒
                                  }
                                 if(nRxBuf[2] == 0xA5 && nRxBuf[3] == 0x01 && nRxBuf[4] == 0x01) 
                                  {
                                    update_cmd = UPDATE_VER_SEND_01;
                                    //Update_Cmd_Resp();
                                  }
                                  else  if(nRxBuf[2] == 0xA5 && nRxBuf[3] == 0x02 && nRxBuf[4] == 0x01) 
                                  {
                                    update_cmd = UPDATE_CMD_GO_01;
                                    //Update_Cmd_Resp();
                                  }
                              }   
                           RX_Index=0;   
                          
                           //RXOK=1;
                       /* for(i=0 ;i<7;i++)
                           {
                            nRxBuf[i] = 0;
                           }  */
                    } 

                 }  
    }
    
      
}

/*bool Uart_Send_Enable;
uint16_t Uart_Send_Count;
void Uart_Send_TIMHanle(void)
{
    if( Uart_Send_Count >= UART_SEND_TIME )
    {
        Uart_Send_Enable = TRUE;
        Uart_Send_Count = 0;
    }
    else
        Uart_Send_Count++;
}
*/

void Uart2_SendChar(uint8_t TxData)
{
    while(UART2_GetFlagStatus(UART2_FLAG_TXE) == 0); 
        UART2_SendData8(TxData);   
}
/**********************************************************************
* 函数名称： InitTimer2
* 功能描述： pwm hot control
* 输入参数： none
* 输出参数： none
* 返 回 值： none
* 其它说明： 
* 修改日期        版本号     修改人	      修改内容
* -----------------------------------------------
* 2016/08/01	     V1.0	  	      ******
***********************************************************************/

void Uart_SendData(unsigned char *p,unsigned char len)//nTxBuf 17
{
     unsigned char i;
   //unsigned int  sum=0;
     for(i=0;i<len;i++)
     {
        Uart2_SendChar(*p);
       // sum+= *p;
        *p++;
     }     
}

unsigned int GetTotalRuntime(void)
{
  return ucTotalRunTime;
}
void BlueToothUart_SetKey(unsigned char by_Data)
{
  Button = by_Data;
}
unsigned char BlueToothUart_GetKey(void)
{
  return Button;
}

