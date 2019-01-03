/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
#include "include.h"

unsigned char  TxAddr[]={0x34,0x43,0x10,0x10,0x01};//发送地址
extern void Delay(uint16_t nCount);
/*******************************************************************************
****函数名称:
****函数功能:初始化NRF24L01接口
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void NRF24L01_Init(void)
{
  // 初始化SPI--高位在前，, 主模式，SCK空闲为低电平，第一个边沿采集数据 
	SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_4, SPI_MODE_MASTER,\
		 SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, \
		 SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
	
	SPI_Cmd(ENABLE);	/* 使能SPI */
	
	GPIO_Init(NRF24L01_CSN_PORT, NRF24L01_CSN_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);	//CSN引脚初始化为输出 
	GPIO_Init(NRF24L01_CE_PORT, NRF24L01_CE_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);	// CE引脚初始化为输出 
}
/*******************************************************************************
****函数名称:
****函数功能:读写一个字节数据
****版本:V1.0
****日期:14-2-2014
****入口参数:data要写的数据
****出口参数:读取的数据
****说明:
********************************************************************************/
unsigned char NRF24L01_RW(unsigned char data)
{
	//等待DR寄存器非空 
	while (SPI_GetFlagStatus( SPI_FLAG_TXE) == RESET);
	
	//通过SPI发送一个字节 
	SPI_SendData(data);
	
	//等待接收一个字节 
	while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);
	
	//返回SPI总线上的字节 
	return SPI_ReceiveData();  
}
/*******************************************************************************
****函数名称:
****函数功能:读寄存器
****版本:V1.0
****日期:14-2-2014
****入口参数:RegAddr寄存器地址
****出口参数:读取的数据
****说明:
********************************************************************************/
unsigned char NRF24L01_Read_Reg(unsigned char RegAddr)
{
   u8 BackData;
   
   ClrCSN;				// 启动时序 
   NRF24L01_RW(RegAddr);		// 写寄存器地址 
   BackData = NRF24L01_RW(0x00);	// 写入读寄存器指令 
   SetCSN;
   
   return(BackData); 			// 返回状态值 
}
/*******************************************************************************
****函数名称:
****函数功能:写寄存器
****版本:V1.0
****日期:14-2-2014
****入口参数:RegAddr寄存器地址 data要写入的数据
****出口参数:返回的数据
****说明:
********************************************************************************/
unsigned char NRF24L01_Write_Reg(unsigned char RegAddr,unsigned char data)
{
   u8 BackData;
   
   ClrCSN;				//启动时序 
   BackData = NRF24L01_RW(RegAddr);	// 写入地址 
   NRF24L01_RW(data);			// 写入值 
   SetCSN;
   
   return(BackData);
}
/*******************************************************************************
****函数名称:
****函数功能:读取RX FIFO里面的数据
****版本:V1.0
****日期:14-2-2014
****入口参数:RegAddr寄存器地址 *RxData数据缓冲区 DataLen数据长度
****出口参数:返回的数据
****说明:接收模式下使用
********************************************************************************/
unsigned char NRF24L01_Read_RxData(unsigned char RegAddr, unsigned char *RxData, unsigned char DataLen)
{ 
	u8 BackData,i;
	
	ClrCSN;					//启动时序 
	BackData = NRF24L01_RW(RegAddr);	//写入要读取的寄存器地址 
	for(i = 0; i < DataLen; i++) 		// 读取数据 
	{
		RxData[i] = NRF24L01_RW(0);
	} 
	
	SetCSN;
	
	return(BackData); 
}
/*******************************************************************************
****函数名称:
****函数功能:写数据到TX FIFO里面
****版本:V1.0
****日期:14-2-2014
****入口参数:RegAddr寄存器地址 *RxData数据缓冲区 DataLen数据长度
****出口参数:返回的数据
****说明:
********************************************************************************/
unsigned char NRF24L01_Write_TxData(unsigned char RegAddr,unsigned char *TxData,unsigned char DataLen)
{ 
	u8 BackData,i;
	
	ClrCSN;
	BackData = NRF24L01_RW(RegAddr);	// 写入要写入寄存器的地址 
	for(i = 0; i < DataLen; i++)		// 写入数据 
	{
	    NRF24L01_RW(*TxData++);
	}   
	
	SetCSN;
	return(BackData);
}
/*******************************************************************************
****函数名称:
****函数功能:设置为发送模式
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void NRF24L01_Set_TxMode()
{
        ClrCE; 
	//设置发送地址和地址长度 
   	NRF24L01_Write_TxData(W_REGISTER+TX_ADDR,TxAddr,TX_ADDR_WITDH);	
	// 为了应答接收设备，接收通道0地址和发送地址相同 
	NRF24L01_Write_TxData(W_REGISTER+RX_ADDR_P0,TxAddr,TX_ADDR_WITDH);	

	/******下面有关寄存器配置**************/
  	NRF24L01_Write_Reg(W_REGISTER+EN_AA,0x01);       /* 使能接收通道0自动应答 */
  	NRF24L01_Write_Reg(W_REGISTER+EN_RXADDR,0x01);   /* 使能接收通道0 */
   	NRF24L01_Write_Reg(W_REGISTER+SETUP_RETR,0x0a);  /* 自动重发延时等待250us+86us，自动重发10次 */
  	NRF24L01_Write_Reg(W_REGISTER+RF_CH,0x40);       /* 选择射频通道0x40 */
  	NRF24L01_Write_Reg(W_REGISTER+RF_SETUP,0x07);    /* 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益 */
	NRF24L01_Write_Reg(W_REGISTER+CONFIG,0x0e);      /* CRC使能，16位CRC校验，上电 */ 
	SetCE;
	Delay(5);	// 按照芯片手册要求 保持10us秒以上 
}
/*******************************************************************************
****函数名称:
****函数功能:设置为接收模式
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void NRF24L01_Set_RxMode()
{
	ClrCE; 
	// 接收设备接收通道0使用和发送设备相同的发送地址 
  	NRF24L01_Write_TxData(W_REGISTER+RX_ADDR_P0, TxAddr, TX_ADDR_WITDH);  
	
	/******下面有关寄存器配置**************/
  	NRF24L01_Write_Reg(W_REGISTER+EN_AA,0x01);       /* 使能接收通道0自动应答 */
  	NRF24L01_Write_Reg(W_REGISTER+EN_RXADDR,0x01);   /* 使能接收通道0 */
  	NRF24L01_Write_Reg(W_REGISTER+RF_CH,0x40);       /* 选择射频通道0x40 */
	NRF24L01_Write_Reg(W_REGISTER+RX_PW_P0,TX_DATA_WITDH);  /* 接收通道0选择和发送通道相同有效数据宽度 */
  	NRF24L01_Write_Reg(W_REGISTER+RF_SETUP,0x07);    /* 数据传输率1Mbps，发射功率0dBm，低噪声放大器增益 */
	NRF24L01_Write_Reg(W_REGISTER+CONFIG,0x0f);      /* CRC使能，16位CRC校验，上电 */ 
	SetCE;
	Delay(5);	// 按照芯片手册要求 保持10us秒以上 
  
}
/*******************************************************************************
****函数名称:
****函数功能:发送数据
****版本:V1.0
****日期:14-2-2014
****入口参数:*data数据缓冲区
****出口参数:无
****说明:
********************************************************************************/
void NRF24L01_SendData(unsigned char *data)
{
	ClrCE;
	NRF24L01_Write_TxData(W_TX_PAYLOAD, data, TX_DATA_WITDH);	//写入要发送的数据 
	SetCE;
	
	Delay(5);
}
/*******************************************************************************
****函数名称:
****函数功能:发送数据
****版本:V1.0
****日期:14-2-2014
****入口参数:*RevData数据缓冲区
****出口参数:0-接收成功 1-接收失败
****说明:
********************************************************************************/
unsigned char NRF24L01_RevData(unsigned char *RevData)
{
	u8 RevFlags = 1;
	u8 sta;
	
	sta = NRF24L01_Read_Reg(R_REGISTER+STATUS);	/* 读取状态寄存器的值 */
	if(sta & 0x40)				/* 判断是否接收到数据 */
	{
		ClrCE; 			
		NRF24L01_Read_RxData(R_RX_PAYLOAD,RevData,RX_DATA_WITDH);	/* 从RXFIFO读取4位数据 */
		
		/* 接收到数据后RX_DR,TX_DS,MAX_PT都置高为1，通过写1来清楚中断标志 */
		NRF24L01_Write_Reg(W_REGISTER+STATUS,0xff); 
		
		ClrCSN;
	   	NRF24L01_RW(FLUSH_RX);	/* 用于清空FIFO */
	   	SetCSN;	
		
		RevFlags = 0;	   
	}
	
	return(RevFlags);
}
/*******************************************************************************
****函数名称:
****函数功能:是否发送成功
****版本:V1.0
****日期:14-2-2014
****入口参数:*data数据缓冲区
****出口参数:0-发送成功 1-发送失败
****说明:
********************************************************************************/
unsigned char NRRF24L01_CheckACK()
{  
  	u8 sta;
	
	sta=NRF24L01_Read_Reg(R_REGISTER+STATUS);  	/* 发送后读取状态寄存器的值 */
	//UART1_SendByte(sta);
	if((sta&0x20)||(sta&0x10)) /* 是否产生发送完毕中断和重复发送中断 */
	{
	   NRF24L01_Write_Reg(W_REGISTER+STATUS,0xff);  /* 清除TX_DS或MAX_RT中断标志 */
	   ClrCSN;
	   NRF24L01_RW(FLUSH_TX);	/* 用于清空FIFO */   
           SetCSN; 
	   return 0;
	}
	else
	   return 1;
}
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/