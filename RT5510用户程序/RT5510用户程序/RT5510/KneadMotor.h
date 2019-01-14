#ifndef __KNEAD_MOTOR_H__
#define __KNEAD_MOTOR_H__

#define KNEAD_SET_VOLTAGE       250000

#define KNEAD_SPEED0_PWM        0 //0% 100%:560
#define KNEAD_SPEED1_PWM 	50//85//105//85//70//300
#define KNEAD_SPEED2_PWM 	60//95//111//95//82//60//350
#define KNEAD_SPEED3_PWM 	70//105//117//105//94//45//400
#define KNEAD_SPEED4_PWM 	80//115//122//115//106//30//450
#define KNEAD_SPEED5_PWM 	90//125//127//125//118//15//500
#define KNEAD_SPEED6_PWM 	100//131//132//0//560

#define STATE_KNEAD_CLOCK_RUN     1
#define STATE_KNEAD_UNCLOCK_RUN   2
#define STATE_KNEAD_IDLE          0

#define STATE_KNEAD_STOP_AT_MIN   3
#define STATE_KNEAD_STOP_AT_MED   4  
#define STATE_KNEAD_STOP_AT_MAX   5  

#define MOTOR_STOP_BREAK    0   //马达停止并刹车
#define MOTOR_STOP_HZ       1   //马达停止，马达输出端口为高阻
#define MOTOR_RUN           2   //马达运行
#define MOTOR_STOP_HZ_TIME  10  //单位10ms 
extern unsigned int KnesdReverseTime;//揉捏反向时间
enum
{
  KNEAD_MOTOR_CURRENT_HIGH,
  KNEAD_MOTOR_CURRENT_LOW
};

enum
{
  KNEAD_MOTOR_NORMAL,
  KNEAD_MOTOR_FAIL
};

enum
{
    KNEAD_MOTOR_GO_UP, 
    KNEAD_MOTOR_GO_DOWN 
};

void KneadMotor_Init_IO(void);
void KneadMotor_Reset(void);
void KneadMotor_Reset_Cancel(void);
void KneadMotorBrake_On(void);
void KneadMotorBrake_Off(void);
//void KneadMotor_10ms_Int(void);
_Bool KneadMotor_IsRun(void);
void KneadMotor_ClockRun(uint16_t pwm);
void KneadMotor_UnClockRun(uint16_t pwm);
void KneadMotor_Break(void);
_Bool KneadMotor_Get_Fault(void);
void KneadMotorUpdateSpeed(unsigned int speed);
void KneadMotorTest(void);
void KneadRubbing(unsigned int Positivetime,unsigned int ReverseTime,unsigned char speed );
unsigned char KneadMotor_Control(unsigned char Direction,unsigned char speed);
#endif