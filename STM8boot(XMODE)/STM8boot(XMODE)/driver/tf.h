#ifndef _tf_h_
#define _tf_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/
//定义TF卡接口
#define TF_CS_PORT              GPIOE
#define TF_CS_PIN               GPIO_PIN_5

#define Set_TF_CS  GPIO_WriteHigh(TF_CS_PORT,TF_CS_PIN)		/* 禁止SD卡 */
#define Clr_TF_CS  GPIO_WriteLow(TF_CS_PORT,TF_CS_PIN)		/* 使能SD卡 */ 

/* 自定义新类型 */
typedef enum
{
	TF_OK      = (u8)0x00,
	TF_ERROR   = (u8)0x01
} TF_Status_TypeDef;		/* SD卡操作成功失败返回值 */

typedef struct
{
	u8  ManufacturerID;       // 生产标识ID
	u16 OEM_AppliID;          // OEM/应用 ID
	u32 ProdName1;            // 产品名称1
	u8  ProdName2;            // 产品名称2
	u8  ProdRev;              // 产品版本
	u32 ProdSN;               // 产品序号
	u8  Reserved1;            // 保留
	u16 ManufactDate;         // 生产日期
	u8  CID_CRC;              // CID CRC
	u8  Reserved2;            // always 1
} TF_CID;	/* CID寄存器 */

/* 自定义宏 */
//SD传输数据结束后是否释放总线宏定义  
#define NO_RELEASE      0
#define RELEASE         1	

/* 
 * SD卡指令表 
 */
#define CMD0    0       //卡复位
#define CMD1    1
#define CMD9    9       //命令9 ，读CSD数据
#define CMD10   10      //命令10，读CID数据
#define CMD12   12      //命令12，停止数据传输
#define CMD16   16      //命令16，设置SectorSize 应返回0x00
#define CMD17   17      //命令17，读sector
#define CMD18   18      //命令18，读Multi sector
#define ACMD23  23      //命令23，设置多sector写入前预先擦除N个block
#define CMD24   24      //命令24，写sector
#define CMD25   25      //命令25，写Multi sector
#define ACMD41  41      //命令41，应返回0x00
#define CMD55   55      //命令55，应返回0x01
#define CMD58   58      //命令58，读OCR信息
#define CMD59   59      //命令59，使能/禁止CRC，应返回0x00
//数据写入回应字意义
#define MSD_DATA_OK                0x05
#define MSD_DATA_CRC_ERROR         0x0B
#define MSD_DATA_WRITE_ERROR       0x0D
#define MSD_DATA_OTHER_ERROR       0xFF
//SD卡回应标记字
#define MSD_RESPONSE_NO_ERROR      0x00
#define MSD_IN_IDLE_STATE          0x01
#define MSD_ERASE_RESET            0x02
#define MSD_ILLEGAL_COMMAND        0x04
#define MSD_COM_CRC_ERROR          0x08
#define MSD_ERASE_SEQUENCE_ERROR   0x10
#define MSD_ADDRESS_ERROR          0x20
#define MSD_PARAMETER_ERROR        0x40
#define MSD_RESPONSE_FAILURE       0xFF




extern void TF_Init(void);
extern unsigned char TF_Idle_Sta(void);
extern unsigned char TF_WaitDataReady(void);
extern unsigned char TF_SendCommand(unsigned char cmd, unsigned int arg, unsigned char crc);
extern unsigned char TF_SendCommand_NoDeassert(unsigned char cmd, unsigned int arg, unsigned char crc);
extern unsigned char TF_ReceiveData(unsigned char *data, unsigned short len, unsigned char release);
extern unsigned char TF_GetCID(TF_CID *cid);
extern unsigned char TF_GetCSD(unsigned char * csd);
extern unsigned int  TF_GetCapacity(void);
extern unsigned char TF_ReadSingleBlock(unsigned int sector, unsigned char *buffer);
extern unsigned char TF_ReadMultiBlock(unsigned int sector, unsigned char *buffer, unsigned char count);
extern unsigned char TF_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes);
extern unsigned char TF_WriteSingleBlock(unsigned int sector, unsigned char * buffer);
extern unsigned char TF_WriteMultiBlock(unsigned int sector, const unsigned char *data, unsigned char count);
#endif
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/