#include "stm8s.h"
#include "comm.h"
#include "stm8s_gpio.h"
#include "Data_Cul.h"
#include "time4.h"
#include "timer.h"
__no_init static unsigned int w_Time;
__no_init static unsigned int programExecTime; //程序执行的时间，自动程序切换不影响这个时间
__no_init static unsigned int bStart ;
__no_init static unsigned int bSecond;

void Data_Init(void)
{
  w_Time = RUN_TIME_20;
  bStart = 0;
  bSecond = 0;
  programExecTime = 0;
}

void Data_Set_Start(char by_Dat,unsigned int time)
{
  if(by_Dat) 
  {
    bStart = 1;
    w_Time = time;
  //w_Time = DEFAULT_RUN_TEST_TIME;// only test
  }
   else 
   {
     bStart = 0;
     w_Time = 0;
   }
  if(bStart == 0)
  {
   programExecTime = 0; 
  }
}

void Data_Flag_Int(void)
{
   bSecond = 1;
}

unsigned int Data_Get_Time(void)
{
  unsigned int out = 0; 
  out = w_Time/60;
  if((w_Time % 60) != 0) out++;  
  return(out);
}

unsigned int Data_Get_TimeSecond(void)
{
  return(w_Time);
}
unsigned int Data_Get_ProgramExecTime(void)
{
  return(programExecTime);
}
void Data_Clear_ProgramExecTime(void)
{
  programExecTime = 0;
}
void Data_Set_Time(unsigned int time)
{
  w_Time = time;
}
void Data_Update_Time(unsigned int time)
{
  if(!bStart) return;
  w_Time = time;
}

extern unsigned long massage_step_tm_flag;
extern unsigned long massage_step_tm;  
void  Clr_Massage_Step_Timer(void)
{
	massage_step_tm = 0; 	
	return;
}

unsigned int Data_Time_Counter_Proce(void)
{
  if(!bStart) return(0);
  if(!bSecond) return(0);
  bSecond = 0;
 if(w_Time > 0) 
   w_Time--;
 if(programExecTime >= 6*59*59)
   programExecTime = 0;
 else
   programExecTime++;
  //---------借用这里的定时,这里比较准。---------
	 if(massage_step_tm>=(60)) //60S
	   {
		    massage_step_tm = 0;
		    massage_step_tm_flag = 1;		//一分钟时间标志位		
	   }
	   else
	   {
		   massage_step_tm++;
	   }     
 //----------------------------------------- 
 return(1);
}


