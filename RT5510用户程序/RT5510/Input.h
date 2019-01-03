#ifndef __INPUT_H__
#define __INPUT_H__

#include"stm8s.h"
#include "comm.h"

      #define   WALK_MOTOR_CLOCK_PORT               GPIOC
      #define   WALK_MOTOR_CLOCK_BIT                GPIO_PIN_4
      #define   WALK_MOTOR_ANTI_CLOCK_PORT          GPIOC
      #define   WALK_MOTOR_ANTI_CLOCK_BIT           GPIO_PIN_3

      #define   KNEAD_MOTOR_CLOCK_PORT               GPIOD
      #define   KNEAD_MOTOR_CLOCK_BIT                GPIO_PIN_2
      #define   KNEAD_MOTOR_ANTI_CLOCK_PORT          GPIOD
      #define   KNEAD_MOTOR_ANTI_CLOCK_BIT           GPIO_PIN_0

      #define   INPUT_WALK_UP_SWTICH_PORT    GPIOB
      #define   INPUT_WALK_UP_SWTICH_BIT     GPIO_PIN_6

      #define   INPUT_WALK_DOWN_SWTICH_PORT    GPIOB
      #define   INPUT_WALK_DOWN_SWTICH_BIT     GPIO_PIN_7

  #define USB_ON              GPIO_WriteHigh(GPIOC,GPIO_PIN_2)
  #define USB_OFF             GPIO_WriteLow(GPIOC,GPIO_PIN_2)
  #define  UPSWITCH    ((GPIO_ReadInputPin(INPUT_WALK_UP_SWTICH_PORT,INPUT_WALK_UP_SWTICH_BIT)&0x40)>>6);//Read the status of the modified pin 0/1
  #define  DOWNSWITCH  ((GPIO_ReadInputPin(INPUT_WALK_UP_SWTICH_PORT, INPUT_WALK_DOWN_SWTICH_BIT)&0x80)>>7);//Read the status of the modified pin 0/1

//#define   DOWNSWITCH  (( GPIO_ReadInputPin(INPUT_WALK_DOWN_SWTICH_PORT,INPUT_WALK_DOWN_SWTICH_BIT)&0x40)>>6);//读取第七位的状态0/1
//#define   UPSWITCH    ((GPIO_ReadInputPin(INPUT_WALK_UP_SWTICH_PORT, INPUT_WALK_UP_SWTICH_BIT)&0x20)>>5);//读取第六位的状态0/1

void Input_5ms_Int(void);
void Input_Proce(void);
void Input_Initial_IO(void);
#endif