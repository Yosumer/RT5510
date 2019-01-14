
#ifndef  _comm_H
#define  _comm_H
//---------------------------------------------
//#define   addr_1  //有宽中窄  这个宏定义为选择1号机芯
#define  STATION   01// 01// //01: 1号机芯，有宽中窄  ，02:2号机芯只有宽位置
//------------------------------------------------------
//2号机芯的捏肩动作

#define  CMD_AA     0XAA
#define  CMD_AB     0XAB

#define  CMD_AA_LENTH   11
#define  CMD_AB_LENTH   12

#define  CMD_AA_TOTAL_LEN   (CMD_AA_LENTH+6)
#define  CMD_AB_TOTAL_LEN   (CMD_AB_LENTH+6)

//#define  CHECK_TX //  机芯以查询方式发送数据给主板，默认以中断方式发送
//#define test_uart  1


//#define check_pulse   //非中断查询方式计数
#define uart_lib

#define  KNEAD_MAX_PULSE_STATIOIN_2  110//220/2

#define  KNOCK_MAX_PULSE_STATIOIN_2  30//220/2



#endif
