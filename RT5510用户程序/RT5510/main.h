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
#define C_TIME_RUBBING      6       //���ڴ걳����
#define C_TIMER_INDICATE    7

#define MAX_SEND_COUNT			17
#define MAX_RECEIVE_COUNT		7

#define EOI_AA  0XF0
#define EOI_AB  0XF1

#define H10_KEY_WORK_EN  		        0x72 //��������
#define H10_KEY_WORK_DIS         	        0x73 //������ֹ��
#define H10_KEY_TIME_ADD                        0X74 //��ʱ��
#define H10_KEY_NONE			        0x7f

//add by zhang on 2018-08-17
#define H10_KEY_CHARGING_OPEN                     0x75    //�򿪳��
#define H10_KEY_CHARGING_STOP                     0x76    //�رճ��
#define H10_KEY_CONTINUOUS_CHARGING_ENABLE          0x77    //��һֱ���
#define H10_KEY_CONTINUOUS_CHARGING_DISABLE         0x78    //�ر�һֱ���

//end add 

/********************* ADD ON 2018-08-18 ****************************************/
#define UPDATE_VER_SEND_01      (((0xA5) << 8)|(0x01<<4)|0x01)
#define UPDATE_CMD_GO_01        (((0xA5) << 8)|(0x02<<4)|0x01)

/********************************************************************************/

//#define CHAIR_STATE_IDLE			0  //����״̬
#define CHAIR_STATE_SETTLE			1  //��λ״̬����ʾ�ղ��У�
#define CHAIR_STATE_WAIT_COMMAND	        2  //�ȴ�������,��ص�ָʾ����˸
#define CHAIR_STATE_RUN				3  //����״̬
#define CHAIR_STATE_WAIT_MEMORY		        4  
#define CHAIR_STATE_PROBLEM			5  //����ģʽ
//#define CHAIR_STATE_ENGINEERING               6  //����ģʽ
#define CHAIR_STATE_SLEEP			7  //���ӹ�����˯��ģʽ����ʱҪ���������С��0.5W

#define BACK_SUB_MODE_AUTO_0			1 //ƣ�ͻָ�
#define BACK_SUB_MODE_AUTO_1			2 //���ʰ�Ħ
#define BACK_SUB_MODE_AUTO_2			3//���ɰ�Ħ
#define BACK_SUB_MODE_AUTO_3			4//��ʹ����
#define BACK_SUB_MODE_AUTO_4			5//�����Զ�
#define BACK_SUB_MODE_AUTO_5			6 //�����Զ�

#define BACK_SUB_MODE_KNEAD			7
#define BACK_SUB_MODE_NO_ACTION			8


//����������ģʽ
#define BACK_MAIN_MODE_IDLE			        0 //��CHAIR_STATE_IDLE��Ӧ
#define BACK_MAIN_MODE_SETTLE		                1 //��CHAIR_STATE_SETTLE��Ӧ 
#define BACK_MAIN_MODE_AUTO			        2	
#define BACK_MAIN_MODE_MANUAL		                3
#define BACK_MAIN_MODE_3D                               4
#define BACK_MAIN_MODE_DEMO                             5


//ZONE1:�Զ������ֵ
#define H10_KEY_CHAIR_AUTO_0	      0x10 //ƣ�ͻָ� recovery
#define H10_KEY_CHAIR_AUTO_1	      0x11 //��չ��Ħ extend
#define H10_KEY_CHAIR_AUTO_2	      0x12 //���ɰ�Ħ relax
#define H10_KEY_CHAIR_AUTO_3	      0x13 //��ʹ���� refresh
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


#define WALK_LOCATE_Up   1 //��о����
#define WALK_LOCATE_Down 2 //��о����
#define WALK_LOCATE_Stop 0 //��оֹͣ

#define Walk_Signal_Time 1  //��о��ʱ���ź�����
#define Walk_Signal_Sw   0  //��о�������ź�����

#define Knead_Stop      0
#define Knead_Positive   1 //��������
#define Knead_Reverse   2  //��������
#define Knead_Rubbing   3 //�걳

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
  bool charging_open;   // ��ʾ����Ƿ��ڽ��У�TRUE ��ʾ�ڽ���,�ñ�����λʱĬ�Ͻ���
  unsigned char charging_method;   //��緽ʽ��0x01 ��ʾ��ʱ��� ��0x02 һֱ���
  unsigned int charging_time;    //���ʣ��ʱ��
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
#endif
