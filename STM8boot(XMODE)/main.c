/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
//主函数

#include "include.h"
#define BOOTLOADER_ENDADDRESS      (0X9800) //rt8600 8K 0x800
#define CHECKSUM_ADDRESS     (0xbeff)//+800
#define PRNTINFO 1
__root const long reintvec[]@".intvec"=
{   
    0x82008080,0x82009804,0x82009808,0x8200980c,
    0x82009880,0x82009814,0x82009818,0x8200981c,
    0x82009820,0x82009824,0x82009828,0x8200982c,
    0x82009830,0x82009834,0x82009838,0x8200983c,
    0x82009840,0x82009844,0x82009848,0x8200984c,
    0x82009850,0x82009854,0x82009858,0x8200985c,
    0x82009860,0x82009864,0x82009868,0x8200986c,
    0x82009870,0x82009874,0x82009878,0x8200987c,
}; 

/*******************************************************************************
****函数名称:
****函数功能:主函数
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
#define  FLASH_START_ADDR      0x9800
void E2PROM_Write_Byte(uint8_t data,uint32_t mem_addr);
void E2PROM_Write_Byte(uint8_t data,uint32_t mem_addr)
{
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  FLASH_ProgramByte(mem_addr,data);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
  
}


u8  buffer[512];
unsigned int i;
volatile u8 adds=0;

u8 r_byte;
u8 data;
unsigned char* ucAppCodeAddr=(unsigned char*)FLASH_START_ADDR;
  
 void commandlineLoop(void)  
 {
   uint32_t flashSize;
   flashSize=0xBF00;
  while (1)
 {

   data= Rx_Dat();
   switch(data)
   {
   case 'u':
    Send_Str("READY");
    if( XMODEM_download(BOOTLOADER_ENDADDRESS,flashSize )==0)
    {
      E2PROM_Write_Byte(0x00,0x4000);
      return ;
    }
     break;
//     case'c': //deleted by zhang 20181215
//      asm("JP $9800");
//    break;
   }

  }  
 }
 
 uint8_t operate_flg = 0;
 
void main(void)
{
    unsigned char* p;
    unsigned char programFlag;

    CLK_DeInit();
    CLK_HSICmd(ENABLE);
    UART_Init();
    FLASH_DeInit();

    FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);

    //FLASH_Unlock(FLASH_MEMTYPE_PROG); //deleted by zhang in 20181215

    disableInterrupts();
    operate_flg = FLASH_ReadByte(0x4000);


    unsigned char* pSum;
    pSum = (unsigned char*)(CHECKSUM_ADDRESS);
    
    unsigned short checksum = *pSum;  //获取代码中的checksum
    unsigned char appCheckSum = 0;
    for(unsigned int i=FLASH_START_ADDR;i<CHECKSUM_ADDRESS;i++)
    {
      appCheckSum += *(unsigned char*)i;
    }
    
    #ifdef PRNTINFO
   if(checksum != appCheckSum)
   {

      Send_Str("Checksum err!\r\n");
     
   }
   #endif 
   if(operate_flg==1  || checksum != appCheckSum)
    {
       //---------------------------------------------------------------------->
      //added by zhang 20181215
      
      FLASH_Unlock(FLASH_MEMTYPE_PROG);
      for(int i=0; i<100; i++)
      {
      asm("nop");
      asm("nop");
      asm("nop");
      asm("nop");
      }
      
      //<---------------------------------------------------------------------- 
      
      commandlineLoop();
      //---------------------------------------------------------------------->
      //added by zhang 20181215
      #ifdef PRNTINFO //20181215 zhang
        Send_Str("Sucessful!\r\n");
      #endif 
      FLASH_Lock(FLASH_MEMTYPE_PROG);
      for(int i=0; i<100; i++)
      {
      asm("nop");
      asm("nop");
      asm("nop");
      asm("nop");
      }
      
      //<-------------------------------------------------------------------- 
      
      asm("nop");
      asm("nop");
      asm("nop");
	  asm("LDW X,SP");
      asm("LD A,$FF");
      asm("LD XL,A");
      asm("LDW SP,X");
      asm("JP $9800");
    }
    else
    {
      //---------------------------------------------------------------------->
      //added by zhang 20181215
      
      FLASH_Lock(FLASH_MEMTYPE_PROG);
      for(int i=0; i<100; i++)
      {
      asm("nop");
      asm("nop");
      asm("nop");
      asm("nop");
      }
      
      //<-------------------------------------------------------------------- 
      asm("LDW X,SP");
	  asm("LD A,$FF");
      asm("LD XL,A");
      asm("LDW SP,X");
      asm("JP $9800");
    }
       
  
           
}

/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/
