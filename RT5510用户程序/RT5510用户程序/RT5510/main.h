//mian.h

#ifndef __MAIN_H__
#define __MAIN_H__

#include "stm8s.h"

#define BIT0  0X01
#define BIT1  0X02
#define BIT2  0X04
#define BIT3  0X08
#define BIT4  0X10
#define BIT5  0X20
#define BIT6  0X40
#define BIT7  0X80

#define C_TIMER_TEMP        0
#define C_TIMER_RUN         1
#define C_TIMER_SLOW        2
#define C_TIMER_500MS       3
#define C_TIMER_WAVE_START  4
#define C_TIMER_5           5
#define C_TIME_RUBBING      6       //用于搓背程序
#define C_TIMER_INDICATE    7

#define MAX_SEND_COUNT			17
#define MAX_RECEIVE_COUNT		7

#define EOI_AA  0XF0
#define EOI_AB  0XF1

#define H10_KEY_WORK_EN  		        0x72 //工作允许
#define H10_KEY_WORK_DIS         	        0x73 //工作禁止。
#define H10_KEY_TIME_ADD                        0X74 //加时间
#define H10_KEY_NONE			        0x7f

//add by zhang on 2018-08-17
#define H10_KEY_CHARGING_OPEN                     0x75    //打开充电
#define H10_KEY_CHARGING_STOP                     0x76    //关闭充电
#define H10_KEY_CONTINUOUS_CHARGING_ENABLE          0x77    //打开一直充电
#define H10_KEY_CONTINUOUS_CHARGING_DISABLE         0x78    //关闭一直充电

//end add 

/********************* ADD ON 2018-08-18 ****************************************/
#define UPDATE_VER_SEND_01      (((0xA5) << 8)|(0x01<<4)|0x01)
#define UPDATE_CMD_GO_01        (((0xA5) << 8)|(0x02<<4)|0x01)

/********************************************************************************/

//#define CHAIR_STATE_IDLE			0  //待机状态
#define CHAIR_STATE_SETTLE			1  //回位状态（提示收藏中）
#define CHAIR_STATE_WAIT_COMMAND	        2  //等待按命令,相关的指示灯闪烁
#define CHAIR_STATE_RUN				3  //运行状态
#define CHAIR_STATE_WAIT_MEMORY		        4  
#define CHAIR_STATE_PROBLEM			5  //故障模式
//#define CHAIR_STATE_ENGINEERING               6  //工程模式
#define CHAIR_STATE_SLEEP			7  //椅子工作在睡眠模式，此时要求待机功耗小于0.5W

#define BACK_SUB_MODE_AUTO_0			1 //疲劳恢复
#define BACK_SUB_MODE_AUTO_1			2 //舒适按摩
#define BACK_SUB_MODE_AUTO_2			3//轻松按摩
#define BACK_SUB_MODE_AUTO_3			4//酸痛改善
#define BACK_SUB_MODE_AUTO_4			5//颈肩自动
#define BACK_SUB_MODE_AUTO_5			6 //背腰自动

#define BACK_SUB_MODE_KNEAD			7
#define BACK_SUB_MODE_NO_ACTION			8


//背背运行主模式
#define BACK_MAIN_MODE_IDLE			        0 //与CHAIR_STATE_IDLE对应
#define BACK_MAIN_MODE_SETTLE		                1 //与CHAIR_STATE_SETTLE对应 
#define BACK_MAIN_MODE_AUTO			        2	
#define BACK_MAIN_MODE_MANUAL		                3
#define BACK_MAIN_MODE_3D                               4
#define BACK_MAIN_MODE_DEMO                             5


//ZONE1:自动程序键值
#define H10_KEY_CHAIR_AUTO_0	      0x10 //疲劳恢复 recovery
#define H10_KEY_CHAIR_AUTO_1	      0x11 //舒展按摩 extend
#define H10_KEY_CHAIR_AUTO_2	      0x12 //轻松按摩 relax
#define H10_KEY_CHAIR_AUTO_3	      0x13 //酸痛改善 refresh
#define H10_KEY_CHAIR_AUTO_4          0x14
#define H10_KEY_CHAIR_AUTO_5          0x15

#define POWER_KEY_RESET   0
#define RUN_OVER_RESET    1


#define SPEED_0		0
#define SPEED_1		1
#define SPEED_2		2
#define SPEED_3		3
#define SPEED_4		4
#define SPEED_5		5
#define SPEED_6		6


#define WALK_LOCATE_Up   1 //机芯上行
#define WALK_LOCATE_Down 2 //机芯下行
#define WALK_LOCATE_Stop 0 //机芯停止

#define Walk_Signal_Time 1  //机芯按时间信号行走
#define Walk_Signal_Sw   0  //机芯按开关信号行走

#define Knead_Stop      0
#define Knead_Positive   1 //正向揉捏
#define Knead_Reverse   2  //反向揉捏
#define Knead_Rubbing   3 //搓背

typedef union
{
	struct
	{
		unsigned bD0:1 ;
		unsigned bD1:1 ;
		unsigned bD2:1 ;
		unsigned bD3:1 ;
		unsigned bD4:1 ;
		unsigned bD5:1 ;
		unsigned bD6:1 ;
		unsigned bD7:1 ;
	} ;
	unsigned char nByte ;
}BITS ;

typedef struct
{
  bool charging_open;   // 表示充电是否在进行，TRUE 表示在进行,该变量置位时默认进入
  unsigned char charging_method;   //充电方式：0x01 表示计时充电 ，0x02 一直充电
  unsigned int charging_time;    //充电剩余时间
} ChargingType;
  
void ADC_Init(void);
char CalculatCheckSum(void);     
void CommProcess(void);
static void InitClock(void);
void Init_Iwdg(void);
void Timer_Flag_50ms_Int(void);
void main_100ms_int(void);
void main_200ms_int(void);
void TX_BUF(void);
void NetBrd_PowerOn(void);
void NetBrd_PowerOff(void);


#endif
