#include "comm.h"
#include "Input.h"
#include "WalkMotor.h"

static _Bool b5msFlag;
uint8_t read_io_up ;
uint8_t read_io_down ;
 
void Input_Initial_IO(void)
{
  GPIO_Init(INPUT_WALK_UP_SWTICH_PORT, INPUT_WALK_UP_SWTICH_BIT, GPIO_MODE_IN_FL_NO_IT);//浮空输入   //上拉输入可能会冲行程
  GPIO_Init(INPUT_WALK_DOWN_SWTICH_PORT, INPUT_WALK_DOWN_SWTICH_BIT, GPIO_MODE_IN_FL_NO_IT);//浮空输入//上拉输入可能会冲行程
  GPIO_Init(GPIOC,GPIO_PIN_2,GPIO_MODE_OUT_PP_LOW_FAST);//USB初始化
}

void Input_5ms_Int(void)
{
    b5msFlag = 1;
}

void Input_Proce(void)
{
  read_io_down = DOWNSWITCH
  read_io_up = UPSWITCH
   if(!b5msFlag) return;		
   b5msFlag = 0;      
}




