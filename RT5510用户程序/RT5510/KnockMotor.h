#ifndef __KNOCK_MOTOR_H__
#define __KNOCK_MOTOR_H__
#include "comm.h"
#define KNOCK_SPEED0_PWM 	0       //0%
#define KNOCK_SPEED1_PWM 	50//300
#define KNOCK_SPEED2_PWM 	60//350
#define KNOCK_SPEED3_PWM 	70//400
#define KNOCK_SPEED4_PWM 	80//450
#define KNOCK_SPEED5_PWM 	90//500
#define KNOCK_SPEED6_PWM 	101//550

#define KNOCK_SET_VOLTAGE       250000 

enum
{
    KNOCK_MOTOR_CURRENT_HIGH,
    KNOCK_MOTOR_CURRENT_LOW
};

enum{
    KNOCK_MOTOR_NORMAL,
    KNOCK_MOTOR_FAIL
};

enum
{
    KNOCK_MOTOR_GO_UP, 
    KNOCK_MOTOR_GO_DOWN 
};




void KnockMotor_Init_IO(void);
void KnockMotor_Set_Pwm_Data(unsigned int ulDuty);
void KnockMotor_ClockRun(void);
void KnockMotor_UnClockRun(void);
//void KnockMotor_Break(void);
void KnockMotor_Enbl(void);
void KnockMotor_DisEnbl(void);
//_Bool KnockMotor_Get_Fault(void);
void KnockMotor_Test(void);
unsigned char KnockMotor_Control(unsigned int ulDuty);

#endif