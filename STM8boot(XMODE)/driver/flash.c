/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
#include "include.h"

/*******************************************************************************
****函数名称:
****函数功能:初始化SPI
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void SPI_Flash_Init()
{
  /* 初始化SPI */
        SPI_DeInit();
	SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER,\
		SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, \
		SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
	
	SPI_Cmd(ENABLE);	/* 使能SPI */
	
	/* 配置CS管脚 */
	GPIO_Init(SPI_CS_PORT, SPI_CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	SPI_FLASH_CS_HIGH();		/* 不使能外部SPI设备 */
}
/*******************************************************************************
****函数名称:
****函数功能:发送字节
****版本:V1.0
****日期:14-2-2014
****入口参数:byte要发送的数据
****出口参数:SPI接收到的数据
****说明:
********************************************************************************/
unsigned char SPI_FLASH_SendByte(unsigned char byte)
{
  //等待发货缓冲区空
  while (SPI_GetFlagStatus( SPI_FLAG_TXE) == RESET);

  //发送数据
  SPI_SendData(byte);

  //等待接收到数据
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);

  //返回接收到的数据
  return SPI_ReceiveData();	 
 }
/*******************************************************************************
****函数名称:
****函数功能:读取flash芯片ID
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:Device_ID芯片ID
****说明:
********************************************************************************/
unsigned short SPI_FLASH_ReadID(void)
{
  unsigned short Device_ID = 0;
  //使能片选信号
  SPI_FLASH_CS_LOW();

  //发送读ID指令
  SPI_FLASH_SendByte(0x90);
  
  SPI_FLASH_SendByte(0X00);
  SPI_FLASH_SendByte(0X00);
  SPI_FLASH_SendByte(0X00);//90 000000 返回标准制造号和特殊器件ID，90 000001 返回顺序倒过来
  //接收ID数据
  Device_ID = (SPI_FLASH_SendByte(Dummy_Byte)<<8);
  Device_ID|=SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_CS_HIGH();
  return Device_ID;
}
/*******************************************************************************
****函数名称:
****函数功能:写使能
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  //使能片选信号
  SPI_FLASH_CS_LOW();

 //发送写使能指令
  SPI_FLASH_SendByte(WREN);

  //禁能片选信号
  SPI_FLASH_CS_HIGH();
}
/*******************************************************************************
****函数名称:
****函数功能:芯片擦除
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void SPI_Flash_EraseChip(void)
{
  
  SPI_FLASH_WriteEnable();
  //使能片选信号
  SPI_FLASH_CS_LOW();

  //发送芯片擦出指令
  SPI_FLASH_SendByte(CE);

  //禁能片选信号
  SPI_FLASH_CS_HIGH();
  //等待写结束
  SPI_FLASH_WaitForWriteEnd();
}
/*******************************************************************************
****函数名称:
****函数功能:等待写结束
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  unsigned char FLASH_Status = 0;

  //使能片选信号
  SPI_FLASH_CS_LOW();

  //发送相关指令
  SPI_FLASH_SendByte(RDSR);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  //禁能片选信号
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
****函数名称:
****函数功能:向flash芯片按字节写入数据
****版本:V1.0
****日期:14-2-2014
****入口参数:* pBuffer-要写入的数据nSector-地址nBytes-字节数目
****出口参数:无
****说明:
********************************************************************************/
void SPI_FLASH_WriteOneByte(unsigned char * pBuffer,unsigned int nSector ,unsigned char nBytes)
{
  unsigned char i=0;
  //写使能
  SPI_FLASH_WriteEnable();
  //使能片选信号
  SPI_FLASH_CS_LOW();

  //发送写指令
  SPI_FLASH_SendByte(WRITE);

  /****************************************************************/
  /* Send WriteAddr high nibble address byte to write to */
  SPI_FLASH_SendByte((nSector & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI_FLASH_SendByte((nSector & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI_FLASH_SendByte(nSector & 0xFF);
 /*****************************************************************/
  for(i=0;i<nBytes;i++)
    SPI_FLASH_SendByte(pBuffer[i]);
  //禁能片选
  SPI_FLASH_CS_HIGH();

  //等待写结束
  SPI_FLASH_WaitForWriteEnd();
}
/*******************************************************************************
****函数名称:
****函数功能:向flash芯片按页写数据
****版本:V1.0
****日期:14-2-2014
****入口参数:* pBuffer-要写入的数据WriteAddr-地址NumByteToWrite-字节数目
****出口参数:无
****说明:
********************************************************************************/
void SPI_FLASH_PageWrite(unsigned char * pBuffer, unsigned int WriteAddr, unsigned short NumByteToWrite)
{
  
  SPI_FLASH_WriteEnable();  //写使能

  
  SPI_FLASH_CS_LOW();       //拉低CS
  
  SPI_FLASH_SendByte(WRITE);//发送页编程指令
  //发送24位地址
  /****************************************************************/
  /* Send WriteAddr high nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI_FLASH_SendByte(WriteAddr & 0xFF);
 /*****************************************************************/

  
  while (NumByteToWrite--)                              //写数据
  {
    /* Send the current byte */
    SPI_FLASH_SendByte(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  SPI_FLASH_CS_HIGH();//拉高CS

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
****函数名称:
****函数功能:向flash芯片写入块数据
****版本:V1.0
****日期:14-2-2014
****入口参数:* pBuffer-要写入的数据WriteAddr-地址NumByteToWrite-字节数目
****出口参数:无
****说明:
********************************************************************************/
void SPI_FLASH_BufferWrite(unsigned char * pBuffer, unsigned int WriteAddr, unsigned short NumByteToWrite)
{
  unsigned char NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

  Addr = WriteAddr % W25X16_PAGE_SIZE;
  count = W25X16_PAGE_SIZE - Addr;
  NumOfPage =  NumByteToWrite / W25X16_PAGE_SIZE;
  NumOfSingle = NumByteToWrite % W25X16_PAGE_SIZE;

  if (Addr == 0) /* WriteAddr is W25X16_PAGE_SIZE aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < W25X16_PAGE_SIZE */
    {
      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
    }
    else /* NumByteToWrite > W25X16_PAGE_SIZE */
    {
      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, W25X16_PAGE_SIZE);
        WriteAddr +=  W25X16_PAGE_SIZE;
        pBuffer += W25X16_PAGE_SIZE;
      }

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
    }
  }
  else /* WriteAddr is not W25X16_PAGE_SIZE aligned  */
  {
    if (NumOfPage == 0) /* NumByteToWrite < W25X16_PAGE_SIZE */
    {
      if (NumOfSingle > count)
      { /* (NumByteToWrite + WriteAddr) > W25X16_PAGE_SIZE */
        temp = NumOfSingle - count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
        WriteAddr +=  count;
        pBuffer += count;

        SPI_FLASH_PageWrite(pBuffer, WriteAddr, temp);
      }
      else
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumByteToWrite);
      }
    }
    else /* NumByteToWrite > W25X16_PAGE_SIZE */
    {
      NumByteToWrite -= count;
      NumOfPage =  NumByteToWrite / W25X16_PAGE_SIZE;
      NumOfSingle = NumByteToWrite % W25X16_PAGE_SIZE;

      SPI_FLASH_PageWrite(pBuffer, WriteAddr, count);
      WriteAddr +=  count;
      pBuffer += count;

      while (NumOfPage--)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, W25X16_PAGE_SIZE);
        WriteAddr +=  W25X16_PAGE_SIZE;
        pBuffer += W25X16_PAGE_SIZE;
      }

      if (NumOfSingle != 0)
      {
        SPI_FLASH_PageWrite(pBuffer, WriteAddr, NumOfSingle);
      }
    }
  }
}
/*******************************************************************************
****函数名称:
****函数功能:从flash芯片读出块数据
****版本:V1.0
****日期:14-2-2014
****入口参数:* pBuffer-要读取的数据暂存地址 ReadAddr-地址NumByteToRead-字节数目
****出口参数:无
****说明:
********************************************************************************/
void SPI_FLASH_BufferRead(unsigned char * pBuffer, unsigned int  ReadAddr, unsigned short NumByteToRead)
{
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();

  /* Send "Read from Memory " instruction */
  SPI_FLASH_SendByte(READ);

  /* Send ReadAddr high nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr & 0xFF0000) >> 16);
  /* Send ReadAddr medium nibble address byte to read from */
  SPI_FLASH_SendByte((ReadAddr& 0xFF00) >> 8);
  /* Send ReadAddr low nibble address byte to read from */
  SPI_FLASH_SendByte(ReadAddr & 0xFF);

  while (NumByteToRead--) /* while there is data to be read */
  {
    /* Read a byte from the FLASH */
    *pBuffer = SPI_FLASH_SendByte(Dummy_Byte);
    /* Point to the next location where the byte read will be saved */
    pBuffer++;
  }

  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
****函数名称:
****函数功能:从flash芯片读出块数据
****版本:V1.0
****日期:14-2-2014
****入口参数:BlockAddr-地址
****出口参数:无
****说明:
********************************************************************************/
void SPI_FLASH_BlockErase(unsigned int BlockAddr)
{
  BlockAddr*=W25X16_BLOCK_SIZE;
  /* Send write enable instruction */
  SPI_FLASH_WriteEnable();
  /* Sector Erase */
  /* Select the FLASH: Chip Select low */
  SPI_FLASH_CS_LOW();
  /* Send Block Erase instruction */
  SPI_FLASH_SendByte(BE);
  /* Send BlockAddr high nibble address byte */
  SPI_FLASH_SendByte((BlockAddr & 0xFF0000) >> 16);
  /* Send BlockAddr medium nibble address byte */
  SPI_FLASH_SendByte((BlockAddr & 0xFF00) >> 8);
  /* Send BlockAddr low nibble address byte */
  SPI_FLASH_SendByte(BlockAddr & 0xFF);
  /* Deselect the FLASH: Chip Select high */
  SPI_FLASH_CS_HIGH();

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/