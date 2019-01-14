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
****函数功能:初始化ADC
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
/* Private define ------------------------------------------------------------*/
#define STATUS_TYPE 0xFC
//定义字库
const unsigned char Font6x8[][6] =
{
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },   // sp
	{ 0x00, 0x00, 0x00, 0x2f, 0x00, 0x00 },   // !
	{ 0x00, 0x00, 0x07, 0x00, 0x07, 0x00 },   // "
	{ 0x00, 0x14, 0x7f, 0x14, 0x7f, 0x14 },   // #
	{ 0x00, 0x24, 0x2a, 0x7f, 0x2a, 0x12 },   // $
	{ 0x00, 0x62, 0x64, 0x08, 0x13, 0x23 },   // %
	{ 0x00, 0x36, 0x49, 0x55, 0x22, 0x50 },   // &
	{ 0x00, 0x00, 0x05, 0x03, 0x00, 0x00 },   // '
	{ 0x00, 0x00, 0x1c, 0x22, 0x41, 0x00 },   // (
	{ 0x00, 0x00, 0x41, 0x22, 0x1c, 0x00 },   // )
	{ 0x00, 0x14, 0x08, 0x3E, 0x08, 0x14 },   // *
	{ 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08 },   // +
	{ 0x00, 0x00, 0x00, 0xA0, 0x60, 0x00 },   // ,
	{ 0x00, 0x08, 0x08, 0x08, 0x08, 0x08 },   // -
	{ 0x00, 0x00, 0x60, 0x60, 0x00, 0x00 },   // .
	{ 0x00, 0x20, 0x10, 0x08, 0x04, 0x02 },   // /
	{ 0x00, 0x3E, 0x51, 0x49, 0x45, 0x3E },   // 0
	{ 0x00, 0x00, 0x42, 0x7F, 0x40, 0x00 },   // 1
	{ 0x00, 0x42, 0x61, 0x51, 0x49, 0x46 },   // 2
	{ 0x00, 0x21, 0x41, 0x45, 0x4B, 0x31 },   // 3
	{ 0x00, 0x18, 0x14, 0x12, 0x7F, 0x10 },   // 4
	{ 0x00, 0x27, 0x45, 0x45, 0x45, 0x39 },   // 5
	{ 0x00, 0x3C, 0x4A, 0x49, 0x49, 0x30 },   // 6
	{ 0x00, 0x01, 0x71, 0x09, 0x05, 0x03 },   // 7
	{ 0x00, 0x36, 0x49, 0x49, 0x49, 0x36 },   // 8
	{ 0x00, 0x06, 0x49, 0x49, 0x29, 0x1E },   // 9
	{ 0x00, 0x00, 0x36, 0x36, 0x00, 0x00 },   // :
	{ 0x00, 0x00, 0x56, 0x36, 0x00, 0x00 },   // ;
	{ 0x00, 0x08, 0x14, 0x22, 0x41, 0x00 },   // <
	{ 0x00, 0x14, 0x14, 0x14, 0x14, 0x14 },   // =
	{ 0x00, 0x00, 0x41, 0x22, 0x14, 0x08 },   // >
	{ 0x00, 0x02, 0x01, 0x51, 0x09, 0x06 },   // ?
	{ 0x00, 0x32, 0x49, 0x59, 0x51, 0x3E },   // @
	{ 0x00, 0x7C, 0x12, 0x11, 0x12, 0x7C },   // A
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x36 },   // B
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x22 },   // C
	{ 0x00, 0x7F, 0x41, 0x41, 0x22, 0x1C },   // D
	{ 0x00, 0x7F, 0x49, 0x49, 0x49, 0x41 },   // E
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x01 },   // F
	{ 0x00, 0x3E, 0x41, 0x49, 0x49, 0x7A },   // G
	{ 0x00, 0x7F, 0x08, 0x08, 0x08, 0x7F },   // H
	{ 0x00, 0x00, 0x41, 0x7F, 0x41, 0x00 },   // I
	{ 0x00, 0x20, 0x40, 0x41, 0x3F, 0x01 },   // J
	{ 0x00, 0x7F, 0x08, 0x14, 0x22, 0x41 },   // K
	{ 0x00, 0x7F, 0x40, 0x40, 0x40, 0x40 },   // L
	{ 0x00, 0x7F, 0x02, 0x0C, 0x02, 0x7F },   // M
	{ 0x00, 0x7F, 0x04, 0x08, 0x10, 0x7F },   // N
	{ 0x00, 0x3E, 0x41, 0x41, 0x41, 0x3E },   // O
	{ 0x00, 0x7F, 0x09, 0x09, 0x09, 0x06 },   // P
	{ 0x00, 0x3E, 0x41, 0x51, 0x21, 0x5E },   // Q
	{ 0x00, 0x7F, 0x09, 0x19, 0x29, 0x46 },   // R
	{ 0x00, 0x46, 0x49, 0x49, 0x49, 0x31 },   // S
	{ 0x00, 0x01, 0x01, 0x7F, 0x01, 0x01 },   // T
	{ 0x00, 0x3F, 0x40, 0x40, 0x40, 0x3F },   // U
	{ 0x00, 0x1F, 0x20, 0x40, 0x20, 0x1F },   // V
	{ 0x00, 0x3F, 0x40, 0x38, 0x40, 0x3F },   // W
	{ 0x00, 0x63, 0x14, 0x08, 0x14, 0x63 },   // X
	{ 0x00, 0x07, 0x08, 0x70, 0x08, 0x07 },   // Y
	{ 0x00, 0x61, 0x51, 0x49, 0x45, 0x43 },   // Z
	{ 0x00, 0x00, 0x7F, 0x41, 0x41, 0x00 },   // [
	{ 0x00, 0x55, 0x2A, 0x55, 0x2A, 0x55 },   // 55
	{ 0x00, 0x00, 0x41, 0x41, 0x7F, 0x00 },   // ]
	{ 0x00, 0x04, 0x02, 0x01, 0x02, 0x04 },   // ^
	{ 0x00, 0x40, 0x40, 0x40, 0x40, 0x40 },   // _
	{ 0x00, 0x00, 0x01, 0x02, 0x04, 0x00 },   // '
	{ 0x00, 0x20, 0x54, 0x54, 0x54, 0x78 },   // a
	{ 0x00, 0x7F, 0x48, 0x44, 0x44, 0x38 },   // b
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x20 },   // c
	{ 0x00, 0x38, 0x44, 0x44, 0x48, 0x7F },   // d
	{ 0x00, 0x38, 0x54, 0x54, 0x54, 0x18 },   // e
	{ 0x00, 0x08, 0x7E, 0x09, 0x01, 0x02 },   // f
	{ 0x00, 0x18, 0xA4, 0xA4, 0xA4, 0x7C },   // g
	{ 0x00, 0x7F, 0x08, 0x04, 0x04, 0x78 },   // h
	{ 0x00, 0x00, 0x44, 0x7D, 0x40, 0x00 },   // i
	{ 0x00, 0x40, 0x80, 0x84, 0x7D, 0x00 },   // j
	{ 0x00, 0x7F, 0x10, 0x28, 0x44, 0x00 },   // k
	{ 0x00, 0x00, 0x41, 0x7F, 0x40, 0x00 },   // l
	{ 0x00, 0x7C, 0x04, 0x18, 0x04, 0x78 },   // m
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x78 },   // n
	{ 0x00, 0x38, 0x44, 0x44, 0x44, 0x38 },   // o
	{ 0x00, 0xFC, 0x24, 0x24, 0x24, 0x18 },   // p
	{ 0x00, 0x18, 0x24, 0x24, 0x18, 0xFC },   // q
	{ 0x00, 0x7C, 0x08, 0x04, 0x04, 0x08 },   // r
	{ 0x00, 0x48, 0x54, 0x54, 0x54, 0x20 },   // s
	{ 0x00, 0x04, 0x3F, 0x44, 0x40, 0x20 },   // t
	{ 0x00, 0x3C, 0x40, 0x40, 0x20, 0x7C },   // u
	{ 0x00, 0x1C, 0x20, 0x40, 0x20, 0x1C },   // v
	{ 0x00, 0x3C, 0x40, 0x30, 0x40, 0x3C },   // w
	{ 0x00, 0x44, 0x28, 0x10, 0x28, 0x44 },   // x
	{ 0x00, 0x1C, 0xA0, 0xA0, 0xA0, 0x7C },   // y
	{ 0x00, 0x44, 0x64, 0x54, 0x4C, 0x44 },   // z
	{0x00,  0x3C,  0x3C, 0x42,0xFF,  0x00},/*"未命名文件",0*/
};

const unsigned char Font_Time[][32]={
		{0x00,0xF8,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0xFC,0xF8,0x00,0x00,0x3E,0x7E,0x40,
		0x40,0x40,0x40,0x40,0x40,0x7E,0x3E,0x00},/*"0",0*/

		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x3E,0x3E,0x00},/*"1",1*/

		{0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0xFC,0xF8,0x00,0x00,0x3E,0x7F,0x41,
		0x41,0x41,0x41,0x41,0x41,0x41,0x00,0x00},/*"2",2*/

		{0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0xFC,0xF8,0x00,0x00,0x00,0x41,0x41,
		0x41,0x41,0x41,0x41,0x41,0x7F,0x3E,0x00},/*"3",3*/

		{0x00,0xFC,0xFC,0x00,0x00,0x00,0x00,0x00,0x00,0xFC,0xFC,0x00,0x00,0x00,0x01,0x01,
		0x01,0x01,0x01,0x01,0x01,0x3F,0x3E,0x00},/*"4",4*/

		{0x00,0xF8,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x00,0x41,0x41,
		0x41,0x41,0x41,0x41,0x41,0x7F,0x3E,0x00},/*"5",5*/

		{0x00,0xF8,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x00,0x00,0x00,0x3E,0x7F,0x41,
		0x41,0x41,0x41,0x41,0x41,0x7F,0x3E,0x00},/*"6",6*/

		{0x00,0x00,0x04,0x04,0x04,0x04,0x04,0x04,0x04,0xFC,0xF8,0x00,0x00,0x00,0x00,0x00,
		0x00,0x00,0x00,0x00,0x00,0x3E,0x3E,0x00},/*"7",7*/

		{0x00,0xF8,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0xFC,0xF8,0x00,0x00,0x3E,0x7F,0x41,
		0x41,0x41,0x41,0x41,0x41,0x7F,0x3E,0x00},/*"8",8*/

		{0x00,0xF8,0xFC,0x04,0x04,0x04,0x04,0x04,0x04,0xFC,0xF8,0x00,0x00,0x00,0x41,0x41,
		0x41,0x41,0x41,0x41,0x41,0x7F,0x3E,0x00},/*"9",9*/

		{0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x60,0x60,0x00,},/*":",0*/
};
const unsigned char vol_bar[]={0x80,0xc0,0xe0,0xf0,0xf8,0xfc,0xfe,0xff};


/*******************************************************************************
****函数名称:
****函数功能:延时函数
****版本:V1.0
****日期:14-2-2014
****入口参数:延时值
****出口参数:无
****说明:
********************************************************************************/

void LCD_Delay(u16 nCount)
{
  while (nCount != 0)
  {
    nCount--;
  }
}

/*******************************************************************************
****函数名称:
****函数功能:读取LCD状态
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void LCD_ReadStatus(void)
{
  return;
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
static void LCD_SPISendByte(u8 DataToSend)
{

  SPI_SendData(DataToSend);
 
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_ChipSelect(FunctionalState NewState)
{
  if (NewState == DISABLE)
  {
   // GPIO_WriteHigh(LCD_CS_PORT, LCD_CS_PIN); /* CS pin low: LCD disabled */
  }
  else
  {
   // GPIO_WriteLow(LCD_CS_PORT, LCD_CS_PIN); /* CS pin high: LCD enabled */
  }
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_SendByte(u8 DataType, u8 DataToSend)
{
	if(DataType==COMMAND_TYPE)
	GPIO_WriteLow(LCD_DC_PORT, LCD_DC_PIN); /* DC pin Low: command */
	else if(DataType==DATA_TYPE)
	GPIO_WriteHigh(LCD_DC_PORT, LCD_DC_PIN); /* DC pin high: Data */
	else return;

  LCD_ChipSelect(ENABLE); /* Enable access to LCD */

  LCD_SPISendByte(DataToSend); /* Send  byte */

  LCD_ChipSelect(DISABLE); /* Disable access to LCD */

}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
 void LCD_WDATA(unsigned char data)
 {
	 LCD_SendByte(DATA_TYPE,data);
 }
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_WCOMM(unsigned char commend)
{
	LCD_SendByte(COMMAND_TYPE,commend);
}

/*******************************************************************************
****函数名称:
****函数功能:OLED初始化函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void OLCD_Init(void)
{
   GPIO_Init(LCD_DC_PORT, LCD_DC_PIN | LCD_RST_PIN, GPIO_MODE_OUT_PP_LOW_FAST);
   GPIO_WriteLow(LCD_RST_PORT, LCD_RST_PIN);
   LCD_Delay(200);
   GPIO_WriteHigh(LCD_RST_PORT, LCD_RST_PIN);
//Charge Pump Setting
  LCD_SendByte(COMMAND_TYPE, 0x8d); 
  LCD_SendByte(COMMAND_TYPE, 0x14);
  LCD_SendByte(COMMAND_TYPE, 0xaf); 
//Set Display Clock 
  LCD_SendByte(COMMAND_TYPE, 0xd5);
  LCD_SendByte(COMMAND_TYPE, 0xf0); 
//Set Pre-charge Period
  LCD_SendByte(COMMAND_TYPE, 0xd9);  
	LCD_SendByte(COMMAND_TYPE, 0x11); 
//Set VCOMH Deselect Level 
  LCD_SendByte(COMMAND_TYPE, 0xdb);
  LCD_SendByte(COMMAND_TYPE, 0x0); 
//Set Norma Display 
  LCD_SendByte(COMMAND_TYPE, 0xa6);
//Set Contrast Control
  LCD_SendByte(COMMAND_TYPE, 0x81);  
	LCD_SendByte(COMMAND_TYPE, 0xff); //1-256 
//Set Segment Re-map
  LCD_SendByte(COMMAND_TYPE, 0xa1);
//Set COM Output Scan Direction 
  LCD_SendByte(COMMAND_TYPE, 0xc8); 
//Set Memory Addressing Mode
  LCD_SendByte(COMMAND_TYPE, 0x20);
  LCD_SendByte(COMMAND_TYPE, 0x00); 

  LCD_Clear(); /* Clear the LCD */
}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void OLED_GOTO(unsigned char x,unsigned char y)
{
//Set Page Address 
LCD_SendByte(COMMAND_TYPE, 0x22);
LCD_SendByte(COMMAND_TYPE, y);  // start 
LCD_SendByte(COMMAND_TYPE, 7);  // end
//Set Column Address
LCD_SendByte(COMMAND_TYPE, 0x21);
LCD_SendByte(COMMAND_TYPE, x);  // start 
LCD_SendByte(COMMAND_TYPE, 127);  // end
}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_Clear(void)
{
  u16 i;
	OLED_GOTO(0,0);
  for(i=0;i<1024;i++)LCD_SendByte(DATA_TYPE,0x0);
}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void display_one_char(unsigned char x,unsigned char y,unsigned char char_data)
{
	unsigned char i;
	OLED_GOTO(x*6+3,y);
	char_data-=32;
	for (i=0;i<6;i++)
	{
		LCD_WDATA(Font6x8[char_data][i]);
	}
	LCD_WDATA(0);
}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void display_list_char(unsigned char x,unsigned char y,unsigned char *data_s)
{
	while(*data_s) 
	{
		display_one_char(x, y, *data_s); //显示单个字符
		x++;
		data_s++;
	}
}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void display_HEX(unsigned char x,unsigned char y,unsigned char num)
{
   unsigned char temp;
   temp=(num>>4)&0x0f;
   if(temp<10)display_one_char(x,y,temp+0x30);
	else display_one_char(x,y,temp+'A'-10);
   temp=num&0x0F;
   if(temp<10)display_one_char(x+1,y,temp+0x30);
	else display_one_char(x+1,y,temp+'A'-10);
}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void display_num(unsigned char x,unsigned char y,unsigned int num)
{
  //unsigned char i;
   display_one_char(x++,y,'[');
   if(num<10)
   {
	display_one_char(x++,y,num+0x30);
	display_one_char(x++,y,']');
	display_one_char(x++,y,' ');
	display_one_char(x++,y,' ');
	display_one_char(x++,y,' ');
    }
   else if(num<100)
   {
	display_one_char(x++,y,num/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	display_one_char(x++,y,']');
	display_one_char(x++,y,' ');
	display_one_char(x++,y,' ');
    }
   else if(num<1000)
   {
	display_one_char(x++,y,num/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	display_one_char(x++,y,']');
	display_one_char(x++,y,' ');
   }
   else
   {
	display_one_char(x++,y,num/1000+0x30);
	display_one_char(x++,y,num%1000/100+0x30);
	display_one_char(x++,y,num%100/10+0x30);
	display_one_char(x++,y,num%10+0x30);
	display_one_char(x++,y,']');
   }
	
}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void display_Of_num(unsigned char x,unsigned char y,unsigned char num1,unsigned char num2)
{
   display_one_char(x++,y,'[');
   if(num2<10)
     display_one_char(x++,y,num2+0x30);
   else 
   {
	display_one_char(x++,y,num2/10+0x30);
	display_one_char(x++,y,num2%10+0x30);
   }
   display_one_char(x++,y,'-');
   if(num1<10)
     display_one_char(x++,y,num1+0x30);
   else 
   {
	display_one_char(x++,y,num1/10+0x30);
	display_one_char(x++,y,num1%10+0x30);
   }
   display_one_char(x++,y,']');

}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void Display_Bar(unsigned char x,unsigned char y,unsigned char num)
{
  unsigned char i,j;
  i=num/13;  //0-19
  OLED_GOTO(x,y);
  LCD_WDATA(0x7e);
  LCD_WDATA(0x81);
  for(j=0;j<i;j++)
  {
    LCD_WDATA(0xbd);
    LCD_WDATA(0xbd);
    LCD_WDATA(0xbd);
    LCD_WDATA(0x81);
  }
  for(j=0;j<19-i;j++)
  {
    LCD_WDATA(0x81);
    LCD_WDATA(0x81);
    LCD_WDATA(0x81);
    LCD_WDATA(0x81);
  }
  LCD_WDATA(0x7e);

}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_ClearLine(u8 Line)
{

  u8 CharPos;

  /* Select the line to be cleared */
  OLED_GOTO(0,Line);

  /* Clear the selected line */
  for (CharPos = 0; CharPos < 128; CharPos++)
  {
    LCD_SendByte(DATA_TYPE, 0x00);
  }

}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_SetCursorPos(u8 Line, u8 Offset)
{
//  LCD_SendByte(COMMAND_TYPE, (u8)(Line + Offset));
  OLED_GOTO(Offset,Line);
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintChar(u8 Ascii)
{
  unsigned char i;
 // LCD_SendByte(DATA_TYPE, Ascii);
  Ascii-=32;
  for (i=0;i<6;i++)
  {

    LCD_SendByte(DATA_TYPE,Font6x8[Ascii][i]); 

  }
  LCD_SendByte(DATA_TYPE,0);

}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintString(u8 Line, FunctionalState AutoComplete, FunctionalState Append, u8 *ptr)
{

  u8 CharPos = 0;
  OLED_GOTO(0,Line);

  /* Set cursor position at beginning of Line if Append option is enabled */
  if (Append == DISABLE)
  {
   
  }

  /* Display each character of the string */
  while ((*ptr != 0) && (CharPos < LCD_LINE_MAX_CHAR))
  {
    LCD_PrintChar(*ptr);
    CharPos++;
    ptr++;
  }

  /* Complete the line with spaces if AutoFill option is enabled */
  if (AutoComplete == ENABLE)
  {
    while (CharPos < LCD_LINE_MAX_CHAR)
    {
      LCD_PrintChar(' ');
      CharPos++;
    }
  }

}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintMsg(u8 *ptr)
{

  u8 Char = 0;
  u8 CharPos = 0;
	u8 linenum = 0;
  LCD_Clear(); /* Clear the LCD display */

  /* Set cursor to home position on line 1 */
  OLED_GOTO(0, linenum);

  /* Send String */
  while ((*ptr != 0) && (CharPos < (LCD_LINE_MAX_CHAR * 2)))
  {

    /* Check if string length is bigger than LINE1 */
    if (CharPos == LCD_LINE_MAX_CHAR)
    {
      OLED_GOTO(0, ++linenum); /* Select second line */
    }

    Char = *ptr;

    switch (Char)
    {
      case ('\r'):
        /* Carriage return */
        CharPos++;
        ptr++;
      break;
      case ('\n'):
        CharPos = 0;
        ptr++;
        /* Set cursor to line 2 */
        OLED_GOTO(0, ++linenum);
      break;
      default:
        /* Display characters different from (\r, \n) */
        LCD_PrintChar(Char);
        CharPos++;
        ptr++;
      break;
    }
  }

}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintDec1(u8 Number)
{

  u8 NbreTmp;

  if (Number < (u8)10)
  {
   
    /* Display second digit of the number : 10 */
    NbreTmp = (u8)(Number / (u8)10);
   
    /* Display last digit of the number : Units */
    NbreTmp = (u8)(Number - (u8)((u8)10 * NbreTmp));
    LCD_PrintChar((u8)(NbreTmp + (u8)0x30));
   
  }

}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintDec2(u8 Number)
{

  u8 NbreTmp;

  if (Number < (u8)100)
  {
   
    /* Display second digit of the number : 10 */
    NbreTmp = (u8)(Number / (u8)10);
    LCD_PrintChar((u8)(NbreTmp + (u8)0x30));
   
    /* Display last digit of the number : Units */
    NbreTmp = (u8)(Number - (u8)((u8)10 * NbreTmp));
    LCD_PrintChar((u8)(NbreTmp + (u8)0x30));
   
  }

}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintDec3(u16 Number)
{

  u8 Nbre1Tmp;
  u8 Nbre2Tmp;

  if (Number < (u16)1000)
  {
   
    /* Display first digit of the number : 100 */
    Nbre1Tmp = (u8)(Number / (u8)100);
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
   
    /* Display second digit of the number : 10 */
    Nbre1Tmp = (u8)(Number - ((u8)100 * Nbre1Tmp));
    Nbre2Tmp = (u8)(Nbre1Tmp / (u8)10);
    LCD_PrintChar((u8)(Nbre2Tmp + (u8)0x30));
   
    /* Display last digit of the number : Units */
    Nbre1Tmp = ((u8)(Nbre1Tmp - (u8)((u8)10 * Nbre2Tmp)));
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
  }
  
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintDec4(u16 Number)
{

  u16 Nbre1Tmp;
  u16 Nbre2Tmp;

  if (Number < (u16)10000)
  {
   
    /* Display first digit of the number : 1000 */
    Nbre1Tmp = (u16)(Number / (u16)1000);
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
   
    /* Display second digit of the number : 100 */
    Nbre1Tmp = (u16)(Number - ((u16)1000 * Nbre1Tmp));
    Nbre2Tmp = (u16)(Nbre1Tmp / (u8)100);
    LCD_PrintChar((u8)(Nbre2Tmp + (u8)0x30));
   
    /* Display second digit of the number : 10 */
    Nbre1Tmp = (u16)(Nbre1Tmp - ((u16)100 * Nbre2Tmp));
    Nbre2Tmp = (u16)(Nbre1Tmp / (u16)10);
    LCD_PrintChar((u8)(Nbre2Tmp + (u8)0x30));
   
    /* Display last digit of the number : Units */
    Nbre1Tmp = ((u16)(Nbre1Tmp - (u16)((u16)10 * Nbre2Tmp)));
    LCD_PrintChar((u8)(Nbre1Tmp + (u8)0x30));
  }

}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintHex1(u8 Number)
{
  if (Number < (u8)0x0A)
  {
    LCD_PrintChar((u8)(Number + (u8)0x30));
  }
  else
    if (Number < (u8)0x10)
    {
      LCD_PrintChar((u8)(Number + (u8)0x37));
    }
    else
    {
      LCD_PrintChar('-');
    }
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintHex2(u8 Number)
{
  LCD_PrintHex1((u8)(Number >> (u8)4));
  LCD_PrintHex1((u8)(Number & (u8)0x0F));
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintHex3(u16 Number)
{
  LCD_PrintHex1((u8)(Number >> (u8)8)); 
  LCD_PrintHex1((u8)((u8)(Number) >> (u8)4));
  LCD_PrintHex1((u8)((u8)(Number) & (u8)0x0F));
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintBin2(u8 Number)
{
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x02) >> (u8)1));
  LCD_PrintHex1((u8)(Number & (u8)0x01));
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_PrintBin4(u8 Number)
{
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x08) >> (u8)3));
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x04) >> (u8)2));
  LCD_PrintHex1((u8)((u8)(Number & (u8)0x02) >> (u8)1));
  LCD_PrintHex1((u8)(Number & (u8)0x01));
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_RollString(u8 Line, u8 *ptr, u16 speed)
{

  u8 CharPos = 0;
  u8 *ptr2;
  
  /* Set cursor position at beginning of line */
  LCD_SetCursorPos(Line,0);
  
  ptr2 = ptr;
  
  /* Display each character of the string */
  while (*ptr2 != 0)
  {
   
    if (*ptr != 0)
    {
      LCD_PrintChar(*ptr);
      ptr++;
    }
    else
    {
      LCD_PrintChar(' ');
    }
    
    CharPos++;
   
    if (CharPos == LCD_LINE_MAX_CHAR)
    {
      LCD_Delay(speed);
      LCD_ClearLine(Line);
      LCD_SetCursorPos(Line,0);
      CharPos = 0;
      ptr2++;
      ptr = ptr2;
    }
    
  }

}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void LCD_Print(u8 *ptr)
{
  while (*ptr) 					// Display the string */
  LCD_PrintChar(*ptr++);
}

/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void Display_TimeFont(unsigned char x,unsigned char y,unsigned char num)
{
	unsigned char i;
	if(y>=7)
          return;
	if(num<10)
        {
	  OLED_GOTO(x,y++);
	  for(i=0;i<12;i++)
            LCD_SendByte(DATA_TYPE,Font_Time[num][i]);
	  OLED_GOTO(x,y);
	  for(i=12;i<24;i++)
            LCD_SendByte(DATA_TYPE,Font_Time[num][i]);
	}
	else
        {
	  OLED_GOTO(x,y++);
	  for(i=0;i<6;i++)
            LCD_SendByte(DATA_TYPE,Font_Time[num][i]);
	  OLED_GOTO(x,y);
	  for(i=6;i<12;i++)
            LCD_SendByte(DATA_TYPE,Font_Time[num][i]);
	}

}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void Display_vol(unsigned char x,unsigned char y,unsigned char vol)
{
	unsigned char i;

	OLED_GOTO(x,y);
	for(i=0;i<38;i++)
          LCD_SendByte(DATA_TYPE,0);
	OLED_GOTO(x,y);
	vol+=1;
	vol>>=1;
	for (i=0;i<6;i++)
	{

	  LCD_SendByte(DATA_TYPE,Font6x8['z'+1-32][i]);

	}
	LCD_SendByte(DATA_TYPE,0);

	for(i=0;i<vol;i++)
        {
		LCD_SendByte(DATA_TYPE,vol_bar[i]);
		LCD_SendByte(DATA_TYPE,vol_bar[i]);
		LCD_SendByte(DATA_TYPE,vol_bar[i]);
		LCD_SendByte(DATA_TYPE,0);
	}
}
/*******************************************************************************
****函数名称:
****函数功能:SPI发送数据函数
****版本:V1.0
****日期:14-2-2014
****入口参数:需要发送的数据
****出口参数:无
****说明:
********************************************************************************/
void Display_Freq(unsigned char x,unsigned char y,unsigned int Freq)
{
	//if(Freq/1000!=0)
	Display_TimeFont(x,y,Freq/1000);
	Display_TimeFont(x+12,y,Freq%1000/100);
	Display_TimeFont(x+24,y,Freq%100/10);
	Display_TimeFont(x+36,y,10);
	Display_TimeFont(x+42,y,Freq%10);
	display_one_char((x+42+12)/6+0,y+1,'M');
	display_one_char((x+42+12)/6+1,y+1,'h');
	display_one_char((x+42+12)/6+2,y+1,'z');
}
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/