#include "comm.h"
#include "Input.h"
#include "KneadMotor.h"
#include "main.h"

unsigned int  KnesdReverseTime;//揉捏反向时间
volatile unsigned char nKneadLoss;

void KneadMotor_Init_IO(void)
{
  GPIO_Init(KNEAD_MOTOR_CLOCK_PORT, KNEAD_MOTOR_CLOCK_BIT, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(KNEAD_MOTOR_ANTI_CLOCK_PORT, KNEAD_MOTOR_ANTI_CLOCK_BIT, GPIO_MODE_OUT_PP_LOW_FAST);
 /* 
     fclk= 0.0625us; time2 clk= fclk * 1024 *16*32 = 32.768ms;  
 */
  TIM3_TimeBaseInit(TIM3_PRESCALER_16, 32);  //30khz
  //TIM3_UpdateRequestConfig(TIM3_UPDATESOURCE_REGULAR);
 
  TIM3_OC1Init(TIM3_OCMODE_PWM1,TIM3_OUTPUTSTATE_ENABLE,32/2,TIM3_OCPOLARITY_HIGH);
  TIM3_OC1PreloadConfig(ENABLE);
  
  TIM3_OC2Init(TIM3_OCMODE_PWM1,TIM3_OUTPUTSTATE_ENABLE,32/2,TIM3_OCPOLARITY_HIGH);
  TIM3_OC2PreloadConfig(ENABLE);

  TIM3_ARRPreloadConfig(ENABLE);
  TIM3_Cmd(ENABLE);
}

void KneadMotor_ClockRun(uint16_t pwm)
{  
  TIM3->CCR2H = (uint8_t)(pwm >> 8);
  TIM3->CCR2L = (uint8_t)(pwm & 0xff);
  TIM3->CCR1H = (uint8_t)(0);
  TIM3->CCR1L = (uint8_t)(0);	
}

void KneadMotor_UnClockRun(uint16_t pwm)
{   
  TIM3->CCR1H = (uint8_t)(pwm >> 8);
  TIM3->CCR1L = (uint8_t)(pwm & 0xff);
  TIM3->CCR2H = (uint8_t)(0);
  TIM3->CCR2L = (uint8_t)(0);
}

void KneadMotor_Break(void)
{
  TIM3->CCR1H = (uint8_t)(0);
  TIM3->CCR1L = (uint8_t)(0);
  TIM3->CCR2H = (uint8_t)(0);
  TIM3->CCR2L = (uint8_t)(0);
}

unsigned char KneadMotor_Control(unsigned char Direction,unsigned char speed)
{
  static unsigned char Knead_Speed, KneadMotor_State;
  static unsigned char nRetVal ;
       KneadMotor_State= Direction;
       Knead_Speed=speed;
  switch( KneadMotor_State )
    {
        case 0: 
            KneadMotor_Break();
			nRetVal=0;
            break;        
        case 1:
           KneadMotor_ClockRun(Knead_Speed);
			nRetVal=1;
            break;
        case 2:
             KneadMotor_UnClockRun(Knead_Speed);
			nRetVal=2;
            break;
	  case 3:
            KneadRubbing(10,15,Knead_Speed );//5,10
			nRetVal=3;
            break;
        default:
            break;
    }
   return nRetVal;
}

void KneadRubbing(unsigned int Positivetime,unsigned int ReverseTime,unsigned char speed )
{
if(KnesdReverseTime<Positivetime)
	KneadMotor_ClockRun(speed);
else if(KnesdReverseTime<ReverseTime)
	KneadMotor_UnClockRun(speed);
else KnesdReverseTime=0;
	
}