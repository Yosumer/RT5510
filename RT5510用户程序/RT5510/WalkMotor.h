#ifndef __WALK_MOTOR_H__
#define __WALK_MOTOR_H__

#define STATE_RUN_WALK_DOWN     0
#define STATE_RUN_WALK_UP       1
#define STATE_WALK_IDLE         2
#define STATE_RUN_WALK_POSITION 3

#define MOTOR_ON	                1
#define MOTOR_OFF	                0

#define WALK_MOTOR_RUN_VALUE       280
extern uint8_t  bGetNextActionStep;

enum
{
    WALK_MOTOR_POWER_ON, 
    WALK_MOTOR_POWER_OFF 
};

void WalkMotor_Down(uint16_t pwm);
void WalkMotor_Down(uint16_t pwm);
void WalkMotor_Break(void);
unsigned char WalkMotor_Control(unsigned char Signal,unsigned Direction,unsigned int time);
unsigned int WalkMotor_GetDirection(void);
void WalkMotorTest(void);
void WalkMotor_Init_IO(void);
void WalkMotor_Set_Pwm_Data(unsigned long ulDuty);
_Bool WalkMotor_Get_Fault(void);
void Walk_UP_DOWN(void);
void Walk_UP_DOWN_2(void);
void WalkMotor_Reset(void);
void WalkMotor_Reset_Cancel(void);
#endif

