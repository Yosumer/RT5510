#ifndef _24l01_h_
#define _24l01_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
//nRF24L01控制引脚宏定义
#define NRF24L01_CE_PORT  GPIOC
#define NRF24L01_IRQ_PORT GPIOC
#define NRF24L01_CSN_PORT GPIOC

#define NRF24L01_CE_PIN  GPIO_PIN_3
#define NRF24L01_IRQ_PIN GPIO_PIN_2
#define NRF24L01_CSN_PIN GPIO_PIN_1
 
#define SetCE	GPIO_WriteHigh(NRF24L01_CE_PORT, NRF24L01_CE_PIN)
#define ClrCE	GPIO_WriteLow(NRF24L01_CE_PORT, NRF24L01_CE_PIN)

#define SetCSN	GPIO_WriteHigh(NRF24L01_CSN_PORT, NRF24L01_CSN_PIN)
#define ClrCSN	GPIO_WriteLow(NRF24L01_CSN_PORT, NRF24L01_CSN_PIN)

#define TX_ADDR_WITDH 	5		/* 发送地址宽度设置为5个字节 */
#define RX_ADDR_WITDH 	5		/* 接收地址宽度设置为5个字节 */
#define TX_DATA_WITDH 	32		/* 发送数据宽度4个字节 */
#define RX_DATA_WITDH 	32		/* 接收数据宽度4个字节 */



//命令寄存器
#define  R_REGISTER      0x00//读取配置寄存器
#define  W_REGISTER      0x20//写配置寄存器
#define  R_RX_PAYLOAD 	 0x61//读取RX有效数据
#define  W_TX_PAYLOAD	 0xa0//写TX有效数据
#define  FLUSH_TX		 0xe1//清除TXFIFO寄存器
#define  FLUSH_RX		 0xe2//清除RXFIFO寄存器
#define  REUSE_TX_PL     0xe3//重新使用上一包有效数据
#define  NOP             0xff//空操作

//寄存器地址
#define  CONFIG          0x00//配置寄存器
#define  EN_AA			 0x01//使能自动应答
#define  EN_RXADDR       0x02//接收通道使能0-5个通道
#define  SETUP_AW        0x03//设置数据通道地址宽度3-5
#define  SETUP_RETR      0x04//建立自动重发
#define  RF_CH           0x05//射频通道设置
#define  RF_SETUP        0x06//射频寄存器
#define  STATUS          0x07//状态寄存器
#define  OBSERVE_TX      0x08//发送检测寄存器
#define  CD              0x09//载波
#define  RX_ADDR_P0      0x0a//数据通道0接收地址
#define  RX_ADDR_P1      0x0b//数据通道1接收地址
#define  RX_ADDR_P2      0x0c//数据通道2接收地址
#define  RX_ADDR_P3      0x0d//数据通道3接收地址
#define  RX_ADDR_P4      0x0e//数据通道4接收地址
#define  RX_ADDR_P5      0x0f//数据通道5接收地址
#define  TX_ADDR         0x10//发送地址
#define  RX_PW_P0        0x11//P0通道数据宽度设置
#define  RX_PW_P1        0x12//P1通道数据宽度设置
#define  RX_PW_P2        0x13//P2通道数据宽度设置
#define  RX_PW_P3        0x14//P3通道数据宽度设置
#define  RX_PW_P4        0x15//P4通道数据宽度设置
#define  RX_PW_P5        0x16//P5通道数据宽度设置
#define  FIFO_STATUS     0x17//FIFO状态寄存器

extern void NRF24L01_Init(void);
extern unsigned char NRF24L01_RW(unsigned char data);
extern unsigned char NRF24L01_Read_Reg(unsigned char RegAddr);
extern unsigned char NRF24L01_Write_Reg(unsigned char RegAddr,unsigned char data);
extern unsigned char NRF24L01_Read_RxData(unsigned char RegAddr, unsigned char *RxData, unsigned char DataLen);
extern unsigned char NRF24L01_Write_TxData(unsigned char RegAddr,unsigned char *TxData,unsigned char DataLen);
extern void NRF24L01_Set_TxMode();
extern void NRF24L01_Set_RxMode();
extern void NRF24L01_SendData(unsigned char *data);
extern unsigned char NRF24L01_RevData(unsigned char *RevData);
extern unsigned char NRRF24L01_CheckACK();
#endif
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/