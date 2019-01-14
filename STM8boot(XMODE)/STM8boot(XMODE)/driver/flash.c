/*******************************************************************************
****����Ƕ��ʽϵͳ STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****�汾:V1.0
****����:14-2-2014
****˵��:��оƬSTM8S105C4T6
********************************************************************************/
#include "include.h"

/*******************************************************************************
****��������:
****��������:��ʼ��SPI
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:��
****˵��:
********************************************************************************/
void SPI_Flash_Init()
{
  /* ��ʼ��SPI */
        SPI_DeInit();
	SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_16, SPI_MODE_MASTER,\
		SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, \
		SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
	
	SPI_Cmd(ENABLE);	/* ʹ��SPI */
	
	/* ����CS�ܽ� */
	GPIO_Init(SPI_CS_PORT, SPI_CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
	SPI_FLASH_CS_HIGH();		/* ��ʹ���ⲿSPI�豸 */
}
/*******************************************************************************
****��������:
****��������:�����ֽ�
****�汾:V1.0
****����:14-2-2014
****��ڲ���:byteҪ���͵�����
****���ڲ���:SPI���յ�������
****˵��:
********************************************************************************/
unsigned char SPI_FLASH_SendByte(unsigned char byte)
{
  //�ȴ�������������
  while (SPI_GetFlagStatus( SPI_FLAG_TXE) == RESET);

  //��������
  SPI_SendData(byte);

  //�ȴ����յ�����
  while (SPI_GetFlagStatus(SPI_FLAG_RXNE) == RESET);

  //���ؽ��յ�������
  return SPI_ReceiveData();	 
 }
/*******************************************************************************
****��������:
****��������:��ȡflashоƬID
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:Device_IDоƬID
****˵��:
********************************************************************************/
unsigned short SPI_FLASH_ReadID(void)
{
  unsigned short Device_ID = 0;
  //ʹ��Ƭѡ�ź�
  SPI_FLASH_CS_LOW();

  //���Ͷ�IDָ��
  SPI_FLASH_SendByte(0x90);
  
  SPI_FLASH_SendByte(0X00);
  SPI_FLASH_SendByte(0X00);
  SPI_FLASH_SendByte(0X00);//90 000000 ���ر�׼����ź���������ID��90 000001 ����˳�򵹹���
  //����ID����
  Device_ID = (SPI_FLASH_SendByte(Dummy_Byte)<<8);
  Device_ID|=SPI_FLASH_SendByte(Dummy_Byte);
  SPI_FLASH_CS_HIGH();
  return Device_ID;
}
/*******************************************************************************
****��������:
****��������:дʹ��
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:��
****˵��:
********************************************************************************/
void SPI_FLASH_WriteEnable(void)
{
  //ʹ��Ƭѡ�ź�
  SPI_FLASH_CS_LOW();

 //����дʹ��ָ��
  SPI_FLASH_SendByte(WREN);

  //����Ƭѡ�ź�
  SPI_FLASH_CS_HIGH();
}
/*******************************************************************************
****��������:
****��������:оƬ����
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:��
****˵��:
********************************************************************************/
void SPI_Flash_EraseChip(void)
{
  
  SPI_FLASH_WriteEnable();
  //ʹ��Ƭѡ�ź�
  SPI_FLASH_CS_LOW();

  //����оƬ����ָ��
  SPI_FLASH_SendByte(CE);

  //����Ƭѡ�ź�
  SPI_FLASH_CS_HIGH();
  //�ȴ�д����
  SPI_FLASH_WaitForWriteEnd();
}
/*******************************************************************************
****��������:
****��������:�ȴ�д����
****�汾:V1.0
****����:14-2-2014
****��ڲ���:��
****���ڲ���:��
****˵��:
********************************************************************************/
void SPI_FLASH_WaitForWriteEnd(void)
{
  unsigned char FLASH_Status = 0;

  //ʹ��Ƭѡ�ź�
  SPI_FLASH_CS_LOW();

  //�������ָ��
  SPI_FLASH_SendByte(RDSR);

  /* Loop as long as the memory is busy with a write cycle */
  do
  {
    /* Send a dummy byte to generate the clock needed by the FLASH
    and put the value of the status register in FLASH_Status variable */
    FLASH_Status = SPI_FLASH_SendByte(Dummy_Byte);

  }
  while ((FLASH_Status & WIP_Flag) == SET); /* Write in progress */

  //����Ƭѡ�ź�
  SPI_FLASH_CS_HIGH();
}

/*******************************************************************************
****��������:
****��������:��flashоƬ���ֽ�д������
****�汾:V1.0
****����:14-2-2014
****��ڲ���:* pBuffer-Ҫд�������nSector-��ַnBytes-�ֽ���Ŀ
****���ڲ���:��
****˵��:
********************************************************************************/
void SPI_FLASH_WriteOneByte(unsigned char * pBuffer,unsigned int nSector ,unsigned char nBytes)
{
  unsigned char i=0;
  //дʹ��
  SPI_FLASH_WriteEnable();
  //ʹ��Ƭѡ�ź�
  SPI_FLASH_CS_LOW();

  //����дָ��
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
  //����Ƭѡ
  SPI_FLASH_CS_HIGH();

  //�ȴ�д����
  SPI_FLASH_WaitForWriteEnd();
}
/*******************************************************************************
****��������:
****��������:��flashоƬ��ҳд����
****�汾:V1.0
****����:14-2-2014
****��ڲ���:* pBuffer-Ҫд�������WriteAddr-��ַNumByteToWrite-�ֽ���Ŀ
****���ڲ���:��
****˵��:
********************************************************************************/
void SPI_FLASH_PageWrite(unsigned char * pBuffer, unsigned int WriteAddr, unsigned short NumByteToWrite)
{
  
  SPI_FLASH_WriteEnable();  //дʹ��

  
  SPI_FLASH_CS_LOW();       //����CS
  
  SPI_FLASH_SendByte(WRITE);//����ҳ���ָ��
  //����24λ��ַ
  /****************************************************************/
  /* Send WriteAddr high nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF0000) >> 16);
  /* Send WriteAddr medium nibble address byte to write to */
  SPI_FLASH_SendByte((WriteAddr & 0xFF00) >> 8);
  /* Send WriteAddr low nibble address byte to write to */
  SPI_FLASH_SendByte(WriteAddr & 0xFF);
 /*****************************************************************/

  
  while (NumByteToWrite--)                              //д����
  {
    /* Send the current byte */
    SPI_FLASH_SendByte(*pBuffer);
    /* Point on the next byte to be written */
    pBuffer++;
  }

  SPI_FLASH_CS_HIGH();//����CS

  /* Wait the end of Flash writing */
  SPI_FLASH_WaitForWriteEnd();
}

/*******************************************************************************
****��������:
****��������:��flashоƬд�������
****�汾:V1.0
****����:14-2-2014
****��ڲ���:* pBuffer-Ҫд�������WriteAddr-��ַNumByteToWrite-�ֽ���Ŀ
****���ڲ���:��
****˵��:
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
****��������:
****��������:��flashоƬ����������
****�汾:V1.0
****����:14-2-2014
****��ڲ���:* pBuffer-Ҫ��ȡ�������ݴ��ַ ReadAddr-��ַNumByteToRead-�ֽ���Ŀ
****���ڲ���:��
****˵��:
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
****��������:
****��������:��flashоƬ����������
****�汾:V1.0
****����:14-2-2014
****��ڲ���:BlockAddr-��ַ
****���ڲ���:��
****˵��:
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