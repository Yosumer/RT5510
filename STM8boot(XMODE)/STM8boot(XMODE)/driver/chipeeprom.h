#ifndef _chipeeprom_h_
#define _chipeeprom_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/

/* 自定义宏 */
extern void WriteMultiBlockByte(u8 BlockStartAddress,FLASH_MemType_TypeDef FLASH_MemType, 
                FLASH_ProgramMode_TypeDef FLASH_ProgMode, uint8_t *Buffer,uint8_t BlockNum);
extern void ReadMultiBlockByte(u8 BlockStartAddress,uint8_t BlockNum,
                        uint8_t ReadBlockByte[]);
#endif
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/