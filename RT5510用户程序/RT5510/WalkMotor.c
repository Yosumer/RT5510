#include "Input.h"
#include "WalkMotor.h"

extern uint8_t read_io_up ;
extern uint8_t read_io_down; 
uint8_t  bGetNextActionStep;
unsigned int WalkControlTime;//200ms++
volatile unsigned char nWalkLoss;
void WalkMotor_Init_IO(void)
{
  GPIO_Init(WALK_MOTOR_CLOCK_PORT, WALK_MOTOR_CLOCK_BIT, GPIO_MODE_OUT_PP_LOW_FAST);
  GPIO_Init(WALK_MOTOR_ANTI_CLOCK_PORT, WALK_MOTOR_ANTI_CLOCK_BIT, GPIO_MODE_OUT_PP_LOW_FAST);
   /* 
      fclk= 0.0625us; time2 clk= fclk * 1024 * 312 = 20ms;  50khz
   */
  TIM1_DeInit();
  //CLK_ClockSwitchConfig (CLK_SWITCHMODE_AUTO, CLK_SOURCE_HSI,DISABLE, CLK_CURRENTCLOCKSTATE_ENABLE );
  //TIM1_PrescalerConfig((uint16_t)16, TIM1_PSCRELOADMODE_IMMEDIATE);
  TIM1_TimeBaseInit(1, TIM1_COUNTERMODE_UP,312,0);//TIM_PERIOD
  //TIM1_UpdateRequestConfig(TIM1_UPDATESOURCE_REGULAR); 
  TIM1_OC3Init(TIM1_OCMODE_PWM1,  TIM1_OUTPUTSTATE_ENABLE,TIM1_OUTPUTNSTATE_DISABLE, 312/2,  TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_LOW, TIM1_OCIDLESTATE_SET,TIM1_OCNIDLESTATE_RESET);
  TIM1_CCxCmd(TIM1_CHANNEL_3, ENABLE);  
  TIM1_OC3PreloadConfig(ENABLE);
  
  TIM1_OC4Init(TIM1_OCMODE_PWM1,  TIM1_OUTPUTSTATE_ENABLE, 312/2,  TIM1_OCPOLARITY_HIGH, TIM1_OCIDLESTATE_SET);
  TIM1_CCxCmd(TIM1_CHANNEL_4, ENABLE);  
  TIM1_OC4PreloadConfig(ENABLE); 
  
  //TIM1_ARRPreloadConfig(ENABLE);  
  TIM1_CtrlPWMOutputs(ENABLE); 
  TIM1_Cmd(ENABLE);
}

void WalkMotor_Up(uint16_t pwm)
{
            TIM1->CCR3H = (uint8_t)(pwm >> 8);  /*!< capture/compare register 3 high */
            TIM1->CCR3L = (uint8_t)(pwm & 0xff);/*!< capture/compare register 3 low */
            TIM1->CCR4H = (uint8_t)(0);
            TIM1->CCR4L = (uint8_t)(0);         
}

void WalkMotor_Down(uint16_t pwm)
{
            TIM1->CCR4H = (uint8_t)(pwm >> 8);  /*!< capture/compare register 4 high */
            TIM1->CCR4L = (uint8_t)(pwm & 0xff);/*!< capture/compare register 4 low */
            TIM1->CCR3H = (uint8_t)(0);
            TIM1->CCR3L = (uint8_t)(0);    
}

void WalkMotor_Break(void)
{
    TIM1->CCR3H = (uint8_t)(0);
    TIM1->CCR3L = (uint8_t)(0);
    TIM1->CCR4H = (uint8_t)(0);
    TIM1->CCR4L = (uint8_t)(0);  
}

unsigned char WalkMotor_Control(unsigned char Signal,unsigned Direction,unsigned int time)
{
 unsigned char bRetVal ;
 read_io_up = UPSWITCH;
 read_io_down=DOWNSWITCH;
 switch( Direction )
    {
        case 0: 
           bRetVal=0;
           WalkMotor_Break();
		if(WalkControlTime>=time&&(Signal==1))//200ms WalkControlTime++
		       {
			 bGetNextActionStep=TRUE;
			 bRetVal=1;                         
			}
            break;        
        case 1:
          if((WalkControlTime>time&&(Signal==1))||(!read_io_up))//200ms WalkControlTime++
          	{
                          bGetNextActionStep=TRUE;
	                  WalkMotor_Break();
			  bRetVal=1;
			  break;
	         }
	   bRetVal=0;   
           WalkMotor_Up(WALK_MOTOR_RUN_VALUE);
            break;        
        case 2:
	  if((WalkControlTime>=time&&(Signal==1))||(!read_io_down))//200ms WalkControlTime++
          	{
                        bGetNextActionStep=TRUE;
			bRetVal=1;
	                WalkMotor_Break();
			break;
	         }
			 bRetVal=0;            
            WalkMotor_Down(WALK_MOTOR_RUN_VALUE) ;
            break;        
        default:   
            break;   
    }
 return bRetVal;
}

