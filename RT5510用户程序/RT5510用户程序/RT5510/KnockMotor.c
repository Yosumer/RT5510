

#include "Input.h"
#include "KnockMotor.h"
//TIM2_TypeDef  TIME2_BASE;
extern unsigned short nVoltage;


void KnockMotor_Init_IO(void)
{
GPIO_Init(KNOCK_MOTOR_RESET_PORT, KNOCK_MOTOR_RESET_BIT, GPIO_MODE_OUT_PP_HIGH_FAST);
GPIO_Init(KNOCK_MOTOR_FAULT_PORT, KNOCK_MOTOR_FAULT_BIT, GPIO_MODE_IN_PU_NO_IT);//�����������ж�
GPIO_Init(KNOCK_MOTOR_ENABLE_PORT, KNOCK_MOTOR_ENABLE_BIT, GPIO_MODE_OUT_PP_LOW_FAST);
#ifdef addr_1  //�п���խ
  
    TIM1_DeInit();

    TIM1_ARRPreloadConfig(ENABLE);//�Զ���װ������λ
    TIM1_SetAutoreload(100);//set 20k���ڼ���ֵ=TIM1_PSCRL = 0x07;
      // Ԥ��ƵΪ799+1��fCK_CNT = 20k
    TIM1_SetCounter(0x0000);
    //TIM1_PrescalerConfig(7, TIM1_PSCRELOADMODE_IMMEDIATE);
    TIM1_PrescalerConfig(7, TIM1_PSCRELOADMODE_UPDATE);
   TIM1_OC2PreloadConfig(ENABLE);//TIM1->CCMR1=1������TIM1_CCR1�Ĵ�����Ԥװ�ع��ܣ�00��CC1ͨ��������Ϊ�����


TIM1_OC2Init(TIM1_OCMODE_PWM1, TIM1_OUTPUTSTATE_ENABLE, TIM1_OUTPUTNSTATE_DISABLE, 0x0000, TIM1_OCPOLARITY_HIGH, TIM1_OCNPOLARITY_HIGH, TIM1_OCIDLESTATE_RESET, TIM1_OCNIDLESTATE_RESET);   
       
   //   TIM1->CCMR2 |= 0x68;//TIM1_CCMR1 |= 0x68;//TIM1_CH1ͨ��������Ϊ����Ƚ�ģʽ
     // TIM1->CCER1 |=0x10;

 
    
    TIM1_CtrlPWMOutputs(ENABLE);//TIM1_BKR |= 0x80;	// Output enable
    TIM1_Cmd(ENABLE);//���ʹ��TIM1_CR1 |= 0x81;	// Counter enable
  
  
#else  //�޿���խ�»�о

TIM2_DeInit();
TIM2_ARRPreloadConfig(ENABLE);//�Զ���װ������λ
TIM2_SetAutoreload(100);//(100);//set 20k���ڼ���ֵ=TIM1_PSCRL = 0x07;
  // Ԥ��ƵΪ799+1��fCK_CNT = 20k
TIM2_SetCounter(0x0000);
TIM2_PrescalerConfig(3, TIM2_PSCRELOADMODE_UPDATE);
TIM2_OC3PreloadConfig(ENABLE);//TIM1->CCMR1=1������TIM1_CCR1�Ĵ�����Ԥװ�ع��ܣ�00��CC1ͨ��������Ϊ�����

TIM2_OC3Init(TIM2_OCMODE_PWM1, TIM2_OUTPUTSTATE_ENABLE, 0x0000, TIM2_OCPOLARITY_HIGH);
TIM2_Cmd(ENABLE);//���ʹ��TIM1_CR1 |= 0x81;	// Counter enable

#endif


}
//RSET=1 ,��λ8803
//nEnable=0 ,ʹ��8803
//in=duty
//FAULT =0,������
void KnockMotor_Test(void)
{
  #ifdef addr_1  //�п���խ
  #else
  
  
   GPIO_WriteLow(KNOCK_MOTOR_RESET_PORT, KNOCK_MOTOR_RESET_BIT);
   #endif
/*
  #ifdef addr_1  //�п���խ
    TIM1_SetCompare2(100);   
#else
    TIM2_SetCompare3(100);  
    
#endif*/
    KnockMotor_Set_Pwm_Data(100);
    
    
 //  GPIO_Init(KNOCK_MOTOR_ENABLE_PORT, KNOCK_MOTOR_ENABLE_BIT, GPIO_MODE_OUT_PP_HIGH_FAST/*GPIO_MODE_OUT_PP_LOW_FAST*/);
}

void KnockMotor_Enbl(void)
{
    GPIO_WriteLow(KNOCK_MOTOR_RESET_PORT, KNOCK_MOTOR_RESET_BIT);
		//bKnockReset = 0;
}
void KnockMotor_DisEnbl(void)
{
    GPIO_WriteHigh(KNOCK_MOTOR_RESET_PORT, KNOCK_MOTOR_RESET_BIT);
		//bKnockReset = 1;
}

unsigned int KnockMotor_VoltageAdj(unsigned int setDuty)
{
    unsigned short adc24;      //�˴��ĵ�ѹֵ�Ѿ�������100��
		unsigned int scale,yushu;
    //ADC_Get_Voltage(ADC_V24,&adc24);
  //   nVoltage=2500;
    adc24 = nVoltage;		
    if(adc24 <= KNOCK_SET_VOLTAGE/100) 
    {
        return setDuty;        //��ѹֵƫ�ͣ�����Ԥ��ֵ
    }
    scale = KNOCK_SET_VOLTAGE / adc24; //�������趨��ѹ�ı���ֵ
    setDuty *= scale;
    
    yushu = setDuty  % 100;
    setDuty /= 100;
    if(yushu > 50) setDuty++;
    
    return setDuty; 
}


unsigned char KnockMotor_Control(unsigned int ulDuty)
{
   unsigned char nRetVal;   
   nRetVal = FALSE;
   KnockMotor_Set_Pwm_Data( ulDuty);
   nRetVal = TRUE;
   return nRetVal;
  
}
void KnockMotor_Set_Pwm_Data(unsigned int ulDuty)
{
  GPIO_WriteLow(KNOCK_MOTOR_RESET_PORT, KNOCK_MOTOR_RESET_BIT);
#ifdef addr_1  //�п���խ
    if(ulDuty == 0)
    {
        
            
           TIM1_SetCompare2(0);    
             
        return;
    }
    
  //  ulDuty = KnockMotor_VoltageAdj(ulDuty);
  //  
    if(KnockMotor_Get_Fault() == KNOCK_MOTOR_FAIL) 
    {
        KnockMotor_DisEnbl();

	asm("nop");
	asm("nop");
        KnockMotor_Enbl();
    }
        
    TIM1_SetCompare2(ulDuty);   
  
  
#else
    if(ulDuty == 0)
    {
        
            
           TIM2_SetCompare3(0);    
             
        return;
    }
    
  //  ulDuty = KnockMotor_VoltageAdj(ulDuty);
    
    if(KnockMotor_Get_Fault() == KNOCK_MOTOR_FAIL) 
    {
        KnockMotor_DisEnbl();

	asm("nop");
	asm("nop");
        KnockMotor_Enbl();
    }
     
    TIM2_SetCompare3(ulDuty);        
#endif
    
    
}

void KnockMotor_ClockRun(void)
{
    //GPIO_PinOutSet(KNOCK_MOTOR_ENBL_PORT, KNOCK_MOTOR_RESET_BIT);
    //GPIO_PinOutClear(KNOCK_MOTOR_PHASE_PORT, KNOCK_MOTOR_PHASE_BIT);
    //GPIO_PinOutClear(KNOCK_MOTOR_DECAY_PORT, KNOCK_MOTOR_DECAY_BIT);
}
void KnockMotor_UnClockRun(void)
{
    //GPIO_PinOutSet(KNOCK_MOTOR_ENBL_PORT, KNOCK_MOTOR_RESET_BIT);
    //GPIO_PinOutClear(KNOCK_MOTOR_PHASE_PORT, KNOCK_MOTOR_PHASE_BIT);
    //GPIO_PinOutClear(KNOCK_MOTOR_DECAY_PORT, KNOCK_MOTOR_DECAY_BIT);
}
void KnockMotor_Break(void)
{GPIO_WriteLow(KNOCK_MOTOR_RESET_PORT, KNOCK_MOTOR_RESET_BIT);
    KnockMotor_Set_Pwm_Data(0);
    //GPIO_PinOutClear(KNOCK_MOTOR_DECAY_PORT, KNOCK_MOTOR_DECAY_BIT);
}

_Bool KnockMotor_Get_Fault(void)
{
   // if(bKnockFault == 1)
      if(GPIO_ReadInputPin(KNOCK_MOTOR_FAULT_PORT,KNOCK_MOTOR_FAULT_BIT))    
      {
          return KNOCK_MOTOR_NORMAL;
      }
      else
      {
          return KNOCK_MOTOR_FAIL;
      }
      
}