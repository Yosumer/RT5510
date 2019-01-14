#include"main.h"
#include "comm.h"
#include"stm8s.h"
#include "time4.h"
#include "timer.h"
#include "Input.h"
#include "WalkMotor.h"
#include "KneadMotor.h"
#include "Data_Cul.h"

extern unsigned char counter_10ms;
extern unsigned char nCounter_2ms;
extern unsigned int WalkControlTime;
extern unsigned char bNetBrdNormalFlag;

extern __no_init BITS  GlobalFlags0 ;
#define bTime10ms 	GlobalFlags0.bD0
#define bTime1msFlag 	GlobalFlags0.bD1
#define sending_485     GlobalFlags0.bD2
#define bTime100ms      GlobalFlags0.bD3

unsigned char bNetBrdOffFlag;
/*********************************
void Time1_init(void)
tim1��ʼ������1MS��ʱ
**********************************/
void Time4_init(void)
{
/*  --��⺯����ʽ
TIM4_DeInit();
TIM4_TimeBaseInit(TIM4_PRESCALER_64, 250);
//TIM4_TimeBaseInit(800, TIM4_COUNTERMODE_UP, 10, 0);
TIM4_ITConfig(TIM4_IT_UPDATE, ENABLE);//�����ж�
TIM4_SetCounter(0x00);
TIM4_UpdateRequestConfig(TIM4_UPDATESOURCE_REGULAR);
TIM4_Cmd(ENABLE);
  �ô�CPUʱ��16MHZ��64��Ƶ����������250+1���Σ�1MS�ж�һ�Ρ�
  */
	TIM4->PSCR = 0x06;		// Configure TIM4_PRESCALER_64
	TIM4->ARR  = 250;		// Configure TIM4 period 250 (0-255)
	//TIM4 counter enable
	TIM4->CR1 |= 0x05;		// Enable TIM4  ���UDIS������������¼�����ֻ�е������¼�����ʱ�Ų��������жϣ���UIF��1��ʹ�ܼ�����
	TIM4->IER |= 0x01;		// Enable TIM4 OVR interrupt ��������жϡ�
}
/*********************************
__interrupt void TIM1_OVF_IRQHandler(void)
tim1�ж���ں���
**********************************/
//#pragma vector=ITC_IRQ_TIM4_OVF
  //__interrupt void TIM4_OVF_IRQHandler(void)
extern bool bContinueChargingIsRun;
extern bool bCloseUsbCharging;
extern int tim_close_usbcharging;
extern void Uart_SendData(unsigned char *p,unsigned char len);
extern ChargingType charge_type; // add on 2018-08-17
//extern __no_init volatile uint8_t flgRunInUserApp @ 0x062e;
    void time4_interrupt(void)
    {  
    static unsigned char by_Time5ms = 0;
    static unsigned char by_Time10ms = 0;
    static unsigned char by_Time50ms = 0;
    static unsigned char by_Time100ms = 0;
    static  unsigned char by_Time200ms = 0;
    static volatile unsigned int by_Time1s = 0;
    static unsigned int by_Time60s = 0;
    static unsigned int by_Time30s = 0;
	
  //Uart_SendData("In UserApp Timer4\r\n",19);
   
    TIM4->SR1&= ~(0x01);//Ӳ����1, �����0 ״̬�Ĵ��� 

     //if(TIM4_GetFlagStatus(TIM4_FLAG_UPDATE))  //�⺯����ʽ
        {
     //TIM4_ClearFlag(TIM4_FLAG_UPDATE);  //�⺯����ʽ
		//���յ���ȷ���ݰ���ʱ1ms��ʼ��������
		if(counter_10ms>=2)//2)//5)//5)
		{
			counter_10ms=0; 
			bTime10ms=1;          
				
		}
		else
		{
			counter_10ms=counter_10ms+1;
		}
		
		if(nCounter_2ms>=2)
		{       nCounter_2ms=0;
			bTime1msFlag=1;
		}
		else
		{
			nCounter_2ms=nCounter_2ms+1;
		}
		
		if(by_Time5ms >= 4)
		{
		    by_Time5ms = 0; 
		    Input_5ms_Int();
                }
		else
		{
		    ++by_Time5ms;
                }
		if(by_Time10ms >= 9)
		{
		    by_Time10ms = 0; 
                    
	        }
		else
	        {
		    ++by_Time10ms;
		}
                
                if(by_Time50ms >= 50)
		{ 
		    by_Time50ms = 0;
		    Timer_Flag_50ms_Int();
                    IWDG_ReloadCounter();
	        }
		else
	        {
		    ++by_Time50ms;
		}
                
                
		if(by_Time100ms >= 100)
		{  
                 
		    by_Time100ms = 0;
		    Timer_Flag_100ms_Int();
                    main_100ms_int();
	        }
		else
	        {
		    ++by_Time100ms;
		}

        if(by_Time60s >= 60000)
        {
            by_Time60s = 0;
            if(bNetBrdNormalFlag)
                bNetBrdNormalFlag = 0;
            else{
                NetBrd_PowerOff();
                bNetBrdOffFlag = 1;
                }
        }
        else by_Time60s++;

        if(by_Time30s >= 30000)
        {
            by_Time30s = 0;
            NetBrd_PowerOn();
            bNetBrdOffFlag = 0;
        }else if(bNetBrdOffFlag)
            by_Time30s++;
        
                  if(by_Time1s >= 1000)
		{  
                    //ADD BY ZHANG IN 20181110
                     if( bCloseUsbCharging == TRUE) //��Ҫ��ʱ��رճ���־
                     {
                        tim_close_usbcharging--;
                        if(tim_close_usbcharging <1)
                        {
                          USB_OFF;
                          bCloseUsbCharging = FALSE;
                           
                        }
                        
                     }
                     //END TO ADD
                     
                    if((bContinueChargingIsRun == FALSE) && (charge_type.charging_method == 0x01))//��ʱ���
                    {
                      if(--charge_type.charging_time == 0) //���ʣ��ʱ��Ϊ0��
                      {
                        
                        charge_type.charging_open = FALSE; //�ر�USB���
                        charge_type.charging_method = 0x00; //��緽ʽ��λ������磩
                      }
                    }
                    by_Time1s = 0;
                    Data_Flag_Int(); 
	        }
		else
	        {
		    ++by_Time1s;
		}       
              if(by_Time200ms >= 200)
		{  
                    by_Time200ms = 0;
                    main_200ms_int();
                    WalkControlTime++;
                    KnesdReverseTime++;
	        }
		else
	        {
		    ++by_Time200ms;
		}      
        }
    }
