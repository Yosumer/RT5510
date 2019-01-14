/**************************************************************************//**
 * @file
 * @brief XMODEM protocol for RT5510/RT5512 with 4G-SIMCOM BOAD ,20181217
 * @author Energy Micro AS
* @version 1.00
 ******************************************************************************
 * @section License
 * <b>(C) Copyright 2009 Energy Micro AS, http://www.energymicro.com</b>
 ******************************************************************************
 *
 * This source code is the property of Energy Micro AS. The source and compiled
 * code may only be used on Energy Micro "EFM32" microcontrollers.
 *
 * This copyright notice may not be removed from the source code nor changed.
 *
 * DISCLAIMER OF WARRANTY/LIMITATION OF REMEDIES: Energy Micro AS has no
 * obligation to support this Software. Energy Micro AS is providing the
 * Software "AS IS", with no express or implied warranties of any kind,
 * including, but not limited to, any implied warranties of merchantability
 * or fitness for any particular purpose or warranties against infringement
 * of any proprietary rights of a third party.
 *
 * Energy Micro AS will not be liable for any consequential, incidental, or
 * special damages, or any other relief, or for any claim by any third party,
 * arising from your use of this Software.
 *
 *****************************************************************************/
#include <stdio.h>
#include "include.h"
#include "xmodem.h"
#include "crc.h"

#define ALIGNMENT(base,align) (((base)+((align)-1))&(~((align)-1)))
#define PRNTINFO 1

uint8_t rawPacket[2][ALIGNMENT(sizeof(XMODEM_packet),4)];


/**************************************************************************//**
 * @brief Verifies checksum, packet numbering and
 * @param pkt The packet to verify
 * @param sequenceNumber The current sequence number.
 * @returns -1 on packet error, 0 otherwise
 *****************************************************************************/
 int XMODEM_verifyPacketChecksum(XMODEM_packet *pkt, int sequenceNumber)
{
  uint16_t packetCRC;
  uint16_t calculatedCRC;

  /* Check the packet number integrity */
  if (pkt->packetNumber + pkt->packetNumberC != 255)
  {
    return -1;
  }

  /* Check that the packet number matches the excpected number */
  if (pkt->packetNumber != (sequenceNumber % 256))
  {
    return -1;
  }

  calculatedCRC = CRC_calc((uint8_t *) pkt->data, (uint8_t *) &(pkt->crcHigh));
  packetCRC     = pkt->crcHigh << 8 | pkt->crcLow;

  /* Check the CRC value */
  if (calculatedCRC != packetCRC)
  {
    return -1;
  }
  return 0;
}

/**************************************************************************//**
 * @brief Starts a XMODEM download.
 *
 * @param baseAddress
 *   The address to start writing from
 *
 * @param endAddress
 *   The last address. This is only used for clearing the flash
 *****************************************************************************/
XMODEM_packet *pkt; 
int XMODEM_download(uint32_t baseAddress, uint32_t endAddress)
{
  
  uint32_t      i;
  uint32_t      addr;
   
  uint32_t      byte;
  uint32_t      sequenceNumber = 1;
  static uint8_t SOH_ERR_CNT = 0; //ZHANG 20181204
//  for (addr = baseAddress; addr < endAddress; addr += 16)
//  {
//    FLASH_EraseByte(addr);
//  }
  /* Send one start transmission packet. Wait for a response. If there is no
   * response, we resend the start transmission packet.
   * Note: This is a fairly long delay between retransmissions(~6 s). */
  while (1)
  {
    Send_Dat(XMODEM_NCG);
    for (i = 0; i < 100000; i++)
    {

        if (UART2_GetFlagStatus(UART2_FLAG_RXNE))//2017  
          
      {
        goto xmodem_transfer;
      }
    }
  }
 xmodem_transfer:
  while (1)
  {
    /* Swap buffer for packet buffer */
    pkt = (XMODEM_packet *) rawPacket[sequenceNumber & 1];

    /* Fetch the first byte of the packet explicitly, as it defines the
     * rest of the packet */
    pkt->header = Rx_Dat();

    /* Check for end of transfer */
    if (pkt->header == XMODEM_EOT)
    {
      /* Acknowledget End of transfer */
      for(int i = 0;i<1000;i++)
      {
        asm(" nop");
        asm(" nop");
        asm(" nop");
        asm(" nop");
        
      }
        
      Send_Dat(XMODEM_ACK);
      break;
    }

    /* If the header is not a start of header (SOH), then cancel *
     * the transfer. */
    if (pkt->header != XMODEM_SOH)
    {
      SOH_ERR_CNT++;
      if(SOH_ERR_CNT < 20)
      {
        
      Send_Dat(XMODEM_NAK);
      continue;
      }
      else
      {
       SOH_ERR_CNT = 0; 
       return -1; //zhang 20181204
      } 
    }

    SOH_ERR_CNT = 0; //added by zhang
    /* Fill the remaining bytes packet */
    /* Byte 0 is padding, byte 1 is header */
    for (byte = 2; byte < sizeof(XMODEM_packet); byte++)
    {
      *(((uint8_t *) pkt) + byte) = Rx_Dat();
    }

    if (XMODEM_verifyPacketChecksum(pkt, sequenceNumber) != 0)
    {
      /* On a malformed packet, we send a NAK, and start over */
#ifdef PRNTINFO //20181215 zhang
      Send_Str("Verify err.\r\n");
#endif      
      Send_Dat(XMODEM_NAK);
      continue;
    }


 for (unsigned char byte1 = 0; byte1 < XMODEM_DATA_SIZE; byte1++)
    {
    //  FLASH_EraseByte(baseAddress+byte1);
      FLASH_ProgramByte( baseAddress+byte1, *(pkt->data+byte1));
    }
    
    
/*****************************************************************************/
    //zhang 20181215 检验FLASH写入有无成功
 bool bWriteFlashPassed = TRUE;
 for (unsigned char byte1 = 0; byte1 < XMODEM_DATA_SIZE; byte1++)
    {
    //  FLASH_EraseByte(baseAddress+byte1);
      if( FLASH_ReadByte( baseAddress+byte1) != *(pkt->data+byte1))
      {
        bWriteFlashPassed = FALSE;
        break;
      }
    }
 if(bWriteFlashPassed == FALSE)
 {
   #ifdef PRNTINFO //20181215 zhang
      Send_Str("Flash err!\r\n");
    #endif    
   
   Send_Dat(XMODEM_NAK);
   continue; 
 }
 
/******************************************************************************/    
 
 
baseAddress+=128;

    sequenceNumber++;
    /* Send ACK */
    Send_Dat(XMODEM_ACK);
  }
  /* Wait for the last DMA transfer to finish. */
//  while (DMA->CHENS & DMA_CHENS_CH0ENS) ;
  
   
  return 0;
}
