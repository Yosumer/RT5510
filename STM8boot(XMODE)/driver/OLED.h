#ifndef _oled_h_
#define _oled_h_
/*******************************************************************************
****蓝旗嵌入式系统 STM8S105 EasyKit
****LENCHIMCU.TAOBAO.COM
****版本:V1.0
****日期:14-2-2014
****说明:主芯片STM8S105C4T6
********************************************************************************/

//定义OLED借口
#define LCD_CS_PORT (GPIOD)
#define LCD_CS_PIN  (GPIO_PIN_3)

#define LCD_RST_PORT (GPIOE)
#define LCD_RST_PIN  (GPIO_PIN_0)

#define LCD_DC_PORT (GPIOD)
#define LCD_DC_PIN  (GPIO_PIN_1)

#define LCD_LINE_MAX_CHAR ((u8)21)
//定义指令
#define COMMAND_TYPE       ((u8)0xF8)
#define DATA_TYPE          ((u8)0xFA)

//声明功能函数
void LCD_ReadStatus(void);
void LCD_ChipSelect(FunctionalState NewState);
void LCD_SendByte(u8 DataType, u8 DataToSend);
void LCD_SendBuffer(u8 *pTxBuffer, u8 *pRxBuffer, u8 NumByte);
void OLCD_Init(void);
void LCD_Clear(void);
void LCD_ClearLine(u8 Line);
void LCD_SetCursorPos(u8 Line, u8 Offset);
void OLED_GOTO(unsigned char x,unsigned char y);
void LCD_PrintChar(u8 Ascii);
void LCD_PrintString(u8 Line, FunctionalState AutoComplete, FunctionalState Append, u8 *ptr);
void LCD_PrintMsg(u8 *ptr);
void LCD_Print(u8 *ptr);
void LCD_PrintDec1(u8 Number);
void LCD_PrintDec2(u8 Number);
void LCD_PrintDec3(u16 Number);
void LCD_PrintDec4(u16 Number);
void LCD_PrintHex1(u8 Number);
void LCD_PrintHex2(u8 Number);
void LCD_PrintHex3(u16 Number);
void LCD_PrintBin2(u8 Number);
void LCD_PrintBin4(u8 Number);
void LCD_RollString(u8 Line, u8 *ptr, u16 speed);
void display_Of_num(unsigned char x,unsigned char y,unsigned char num1,unsigned char num2);
void display_num(unsigned char x,unsigned char y,unsigned int num);
void display_HEX(unsigned char x,unsigned char y,unsigned char num);
void display_list_char(unsigned char x,unsigned char y,unsigned char *data_s);
void display_one_char(unsigned char x,unsigned char y,unsigned char char_data);
void Display_Bar(unsigned char x,unsigned char y,unsigned char num);
void Display_TimeFont(unsigned char x,unsigned char y,unsigned char num);
void Display_vol(unsigned char x,unsigned char y,unsigned char vol);
void Display_Freq(unsigned char x,unsigned char y,unsigned int Freq);

#endif 
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/