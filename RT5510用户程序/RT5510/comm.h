
#ifndef  _comm_H
#define  _comm_H
//---------------------------------------------
//#define   addr_1  //�п���խ  ����궨��Ϊѡ��1�Ż�о
#define  STATION   01// 01// //01: 1�Ż�о���п���խ  ��02:2�Ż�оֻ�п�λ��
//------------------------------------------------------
//2�Ż�о����綯��

#define  CMD_AA     0XAA
#define  CMD_AB     0XAB

#define  CMD_AA_LENTH   11
#define  CMD_AB_LENTH   12

#define  CMD_AA_TOTAL_LEN   (CMD_AA_LENTH+6)
#define  CMD_AB_TOTAL_LEN   (CMD_AB_LENTH+6)

//#define  CHECK_TX //  ��о�Բ�ѯ��ʽ�������ݸ����壬Ĭ�����жϷ�ʽ����
//#define test_uart  1


//#define check_pulse   //���жϲ�ѯ��ʽ����
#define uart_lib

#define  KNEAD_MAX_PULSE_STATIOIN_2  110//220/2

#define  KNOCK_MAX_PULSE_STATIOIN_2  30//220/2



#endif
