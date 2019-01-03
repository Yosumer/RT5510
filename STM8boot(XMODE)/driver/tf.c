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
****函数功能:TF卡端口初始化函数
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
void TF_Init(void)
{
      SPI_Init(SPI_FIRSTBIT_MSB, SPI_BAUDRATEPRESCALER_4, SPI_MODE_MASTER,\
      SPI_CLOCKPOLARITY_LOW, SPI_CLOCKPHASE_1EDGE, \
      SPI_DATADIRECTION_2LINES_FULLDUPLEX, SPI_NSS_SOFT, 0x07);
	
      SPI_Cmd(ENABLE);	/* 使能SPI */
	
	/* 配置PE7引脚，用于设置SD的CS引脚 */
      GPIO_Init(TF_CS_PORT, TF_CS_PIN, GPIO_MODE_OUT_PP_HIGH_FAST);
      Clr_TF_CS;
}
/*******************************************************************************
****函数名称:
****函数功能:将TF卡设置为SPI模式并初始化TF卡函数
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
unsigned char TF_Idle_Sta(void)
{
  	unsigned char r1;
	unsigned short i;
	unsigned int retry = 0;
 	
	do
	{
		for(i=0;i<10;i++) 
			SPI_FLASH_SendByte(0xff);
		
		r1 = TF_SendCommand(CMD0, 0,0x95);      //发CMD0对卡进行复位	
		if(++retry > 100) 
                  return 1;          //超时退出
	}while(r1 != 0x01);   /* 返回值为0x01时复位命令成功 */
	
	retry = 0;
	do
	{
		r1 =TF_SendCommand(CMD1, 0,0x95);      //发CMD1对卡进行初始化  		
		if(++retry > 100) 
                  return 1;         //超时退出
	} while(r1);	/* 返回值为0时初始化命令成功 */
	
	r1 = TF_SendCommand(CMD59, 0,0x95);        //关crc
	
	r1 = TF_SendCommand(CMD16, 512,0x95);      //设扇区大小512
	
	return 0;                           	//正常返回，可以对卡进行读写
}	
/*******************************************************************************
****函数名称:
****函数功能:等待TF卡回应函数
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
unsigned char TF_GetResponse(unsigned char Response)
{
	unsigned short Count=0xFFF;//等待次数	
	
	while ((SPI_FLASH_SendByte(0XFF)!=Response)&&Count)
		Count--;//等待得到准确的回应  	  
	
	if (Count==0)
	  	return MSD_RESPONSE_FAILURE;//得到回应失败   
	else return 
	  	MSD_RESPONSE_NO_ERROR;//正确回应
}
/*******************************************************************************
****函数名称:
****函数功能:等待TF卡写入完成函数
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:无
****说明:
********************************************************************************/
unsigned char TF_WaitDataReady(void)
{
	unsigned char r1=MSD_DATA_OTHER_ERROR;
	unsigned int retry;
	retry=0;
	do
	{
		r1=SPI_FLASH_SendByte(0xFF)&0X1F;//读到回应
		if(retry==0xfffe)
                  return 1; 
			retry++;
		switch (r1)
		{					   
			case MSD_DATA_OK://数据接收正确了	 
			r1=MSD_DATA_OK;
			break;  
			case MSD_DATA_CRC_ERROR:  //CRC校验错误
			return MSD_DATA_CRC_ERROR;  
			case MSD_DATA_WRITE_ERROR://数据写入错误
			return MSD_DATA_WRITE_ERROR;  
			default://未知错误    
			r1=MSD_DATA_OTHER_ERROR;
			break;	 
		}   
	}
	while(r1==MSD_DATA_OTHER_ERROR); //数据错误时一直等待
	
	retry=0;
	while(SPI_FLASH_SendByte(0XFF)==0)//读到数据为0,则数据还未写完成
	{
		retry++;
		//delay_us(10);//TF卡写等待需要较长的时间
		if(retry>=0XFFFFFFFE)
                  return 0XFF;//等待失败了
	};	    
	
	return 0;//成功了
}	
/*******************************************************************************
****函数名称:
****函数功能:向TF卡写入命令函数
****版本:V1.0
****日期:14-2-2014
****入口参数:cmd命令，arg命令参数，CRC校验
****出口参数:
****说明:
********************************************************************************/
unsigned char TF_SendCommand(unsigned char cmd, unsigned int arg, unsigned char crc)
{
	unsigned char r1;	
	unsigned char Retry=0;	
	
	Set_TF_CS;	/* 拉高片选线 */
	SPI_FLASH_SendByte(0xff);
	SPI_FLASH_SendByte(0xff);     
	SPI_FLASH_SendByte(0xff);  	 
	SPI_FLASH_SendByte(0xff);		/* 至少74个时钟延迟 */
	
	
	Clr_TF_CS; //片选端置低，选中TF卡
	
	/* 按一定命令格式发送 */
	SPI_FLASH_SendByte(cmd | 0x40);//分别写入命令
	SPI_FLASH_SendByte(arg >> 24);
	SPI_FLASH_SendByte(arg >> 16);
	SPI_FLASH_SendByte(arg >> 8);
	SPI_FLASH_SendByte(arg);
	SPI_FLASH_SendByte(crc); 
	
	//等待响应，或超时退出
	r1=SPI_FLASH_SendByte(0xFF);
	while(r1==0xFF)
	{
		r1=SPI_FLASH_SendByte(0xFF);
		Retry++;	    
		if(Retry>200)break; 
	}   
	
	Set_TF_CS;	//关闭片选
	
	SPI_FLASH_SendByte(0xFF);	//在总线上额外增加8个时钟，让SD卡完成剩下的工作
	
	return r1;	//返回状态值
}	
/*******************************************************************************
****函数名称:
****函数功能:向TF卡写入命令函数，最后CS继续有效，有后续数据传来
****版本:V1.0
****日期:14-2-2014
****入口参数:cmd命令，arg命令参数，CRC校验
****出口参数:
****说明:
********************************************************************************/
unsigned char TF_SendCommand_NoDeassert(unsigned char cmd, unsigned int arg, unsigned char crc)
{
	unsigned char Retry=0;	         
	unsigned char r1;			   
	
	SPI_FLASH_SendByte(0xff);
	SPI_FLASH_SendByte(0xff);  	 	 
	Clr_TF_CS;//片选端置低，选中TF卡	
	
	//发送
	SPI_FLASH_SendByte(cmd | 0x40); //分别写入命令
	SPI_FLASH_SendByte(arg >> 24);
	SPI_FLASH_SendByte(arg >> 16);
	SPI_FLASH_SendByte(arg >> 8);
	SPI_FLASH_SendByte(arg);
	SPI_FLASH_SendByte(crc);   
	
	//等待响应，或超时退出
	while((r1=SPI_FLASH_SendByte(0xFF))==0xFF)
	{
		Retry++;	    
		if(Retry>200)break; 
	}  	  
	//返回响应值
	return r1;
}
/*******************************************************************************
****函数名称:
****函数功能:从TF卡读取指定长度的数据
****版本:V1.0
****日期:14-2-2014
****入口参数:u8 *data(存放读回数据的内存>len)
             u16 len(数据长度）
             u8 release(传输完成后是否释放总线CS置高 0：不释放 1：释放）	
****出口参数:
****说明:
********************************************************************************/
unsigned char TF_ReceiveData(unsigned char *data, unsigned short len, unsigned char release)
{
	// 启动一次传输
	Clr_TF_CS;				  	  
	if(TF_GetResponse(0xFE))//等待TF卡发回数据起始令牌0xFE
	{	  
		Set_TF_CS;
		return 1;
	}
	
	while(len--)//开始接收数据
	{
		*data=SPI_FLASH_SendByte(0xFF);
		data++;
	}
	//下面是2个伪CRC（dummy CRC）
	SPI_FLASH_SendByte(0xFF);
	SPI_FLASH_SendByte(0xFF);
	
	if(release==RELEASE)//按需释放总线，将CS置高
	{
		Set_TF_CS;//传输结束
		SPI_FLASH_SendByte(0xFF);
	}											  					    
	return 0;
}	
/*******************************************************************************
****函数名称:
****函数功能:从TF卡读取CID
****版本:V1.0
****日期:14-2-2014
****入口参数:*cid存放CID的内存
****出口参数:
****说明:
********************************************************************************/
unsigned char TF_GetCID(TF_CID *cid)
{
	unsigned char r1;	
	unsigned char cidTable[16];
	
	//发CMD10命令，读CID
	r1 = TF_SendCommand(CMD10,0,0xFF);
	
	if(r1 != 0x00)
	  	return r1;  //没返回正确应答，则退出，报错
	
	TF_ReceiveData(cidTable,16,RELEASE);	//接收16个字节的数据	
	
	// 把获取值放入CID结构体中
	cid->ManufacturerID = cidTable[0];           // Byte 0
	cid->OEM_AppliID = cidTable[1] << 8;      	 // Byte 1
	cid->OEM_AppliID |= cidTable[2];           	 // Byte 2
	cid->ProdName1 = cidTable[3] << 24;     	 // Byte 3
	cid->ProdName1 |= cidTable[4] << 16;     	 // Byte 4
	cid->ProdName1 |= cidTable[5] << 8;      	 // Byte 5
	cid->ProdName1 |= cidTable[6];           	 // Byte 6
	cid->ProdName2 = cidTable[7];           	 // Byte 7
	cid->ProdRev = cidTable[8];           	 	 // Byte 8
	cid->ProdSN = cidTable[9] << 24;     		 // Byte 9
	cid->ProdSN |= cidTable[10] << 16;    		 // Byte 10
	cid->ProdSN |= cidTable[11] << 8;     		 // Byte 11
	cid->ProdSN |= cidTable[12];          		 // Byte 12
	cid->Reserved1 |= (cidTable[13] & 0xF0) >> 4;       	// Byte 13
	cid->ManufactDate = (cidTable[13] & 0x0F) << 8;         // Byte 14
	cid->ManufactDate |= cidTable[14];           			// Byte 15
	cid->CID_CRC = (cidTable[15] & 0xFE) >> 1;
	cid->Reserved2 = 1;
	
	return 0;
}
/*******************************************************************************
****函数名称:
****函数功能:从TF卡读取CSD
****版本:V1.0
****日期:14-2-2014
****入口参数:*csd存放CSD的内存，至少16个字节
****出口参数:
****说明:
********************************************************************************/
unsigned char TF_GetCSD(unsigned char * csd)
{
	unsigned char r1;	 
	
	r1=TF_SendCommand(CMD9, 0, 0xFF);//发CMD9命令，读CSD
	
	if(r1)
	  	return r1;  //没返回正确应答，则退出，报错  
	
	TF_ReceiveData(csd, 16, RELEASE);//接收16个字节的数据 

	return 0;
}  
/*******************************************************************************
****函数名称:
****函数功能:获取TF卡容量
****版本:V1.0
****日期:14-2-2014
****入口参数:无
****出口参数:
****说明:
********************************************************************************/
unsigned int TF_GetCapacity(void)
{
	unsigned char csd[16];
	unsigned int Capacity;
	unsigned char r1;
	unsigned short i;
	unsigned short temp;  				
	
	//取CSD信息，如果期间出错，返回0
	if(TF_GetCSD(csd)!=0) return 0;	   
	
	//如果为SDHC卡，按照下面方式计算
	if((csd[0]&0xC0)==0x40)
	{									  
		Capacity=((u32)csd[8])<<8;
		Capacity+=(u32)csd[9]+1;	 
		Capacity = (Capacity)*1024;//得到扇区数
		Capacity*=512;//得到字节数			   
	}
	else
	{		    
		i = csd[6]&0x03;
		i<<=8;
		i += csd[7];
		i<<=2;
		i += ((csd[8]&0xc0)>>6);
		//C_SIZE_MULT
		r1 = csd[9]&0x03;
		r1<<=1;
		r1 += ((csd[10]&0x80)>>7);	 
		r1+=2;//BLOCKNR
		temp = 1;
		while(r1)
		{
			temp*=2;
			r1--;
		}
		Capacity = ((u32)(i+1))*((u32)temp);	 
		// READ_BL_LEN
		i = csd[5]&0x0f;
		//BLOCK_LEN
		temp = 1;
		while(i)
		{
			temp*=2;
			i--;
		}
		//The final result
		Capacity *= (u32)temp;//字节为单位 	  
	}
	return (unsigned int)Capacity;
}
/*******************************************************************************
****函数名称:
****函数功能:读取TF卡一个block
****版本:V1.0
****日期:14-2-2014
****入口参数:sector扇区地址 *buffer数据存放地址
****出口参数:
****说明:
********************************************************************************/
unsigned char TF_ReadSingleBlock(unsigned int sector, unsigned char *buffer)
{
	unsigned char  r1;          
	unsigned short i;
	//byte retry=0;
	
	r1 =TF_SendCommand(CMD17, sector<<9,0x95);//读命令 	
	if(r1 != 0x00)return r1;
	
	Clr_TF_CS;
	
	//等数据的开始
	while(SPI_FLASH_SendByte(0xff) != 0xfe);
	
	for(i=0; i<512; i++)//读512个数据
	{
		*buffer++ = SPI_FLASH_SendByte(0xff);
	}
	
	(void)SPI_FLASH_SendByte(0xff);//伪crc
	(void)SPI_FLASH_SendByte(0xff);  	
	Set_TF_CS;
	
	return 0;
}
/*******************************************************************************
****函数名称:
****函数功能:读取TF卡多个block
****版本:V1.0
****日期:14-2-2014
****入口参数:sector扇区地址 *buffer数据存放地址 count扇区数目
****出口参数:
****说明:
********************************************************************************/
unsigned char TF_ReadMultiBlock(unsigned int sector, unsigned char *buffer, unsigned char count)
{
	unsigned char  r1;          
	unsigned short i;
	r1 =TF_SendCommand(CMD18, sector<<9,0x95);//读多个Block命令 	
	
	if(r1 != 0x00)return r1;
		Clr_TF_CS;
		
	while(SPI_FLASH_SendByte(0xff) != 0xfe)
	  	;
	
	do
	{
		for(i=0; i<512; i++)//读512个数据
		{
			*buffer++ = SPI_FLASH_SendByte(0xff);
		}
		(void)SPI_FLASH_SendByte(0xff);//伪crc
		(void)SPI_FLASH_SendByte(0xff); 
	}while(--count);
	
	//全部传输完毕，发送停止命令
	TF_SendCommand(CMD12, 0, 0);
	//释放总线
	Set_TF_CS;
	(void)SPI_FLASH_SendByte(0xff); 
	
	if(count != 0)
	  	return count;   //如果没有传完，返回剩余个数	 
	else 
	  	return 0;
}
/*******************************************************************************
****函数名称:
****函数功能:从指定位置读出指定字节的数据
****版本:V1.0
****日期:14-2-2014
****入口参数:address扇区地址 *buf数据存放缓冲 offset扇区内偏移量 bytes需要读的字节数目
****出口参数:
****说明:
********************************************************************************/
unsigned char TF_Read_Bytes(unsigned long address,unsigned char *buf,unsigned int offset,unsigned int bytes)
{
	unsigned char r1;u16 i=0;  
	r1=TF_SendCommand(CMD17, address<<9,0x95);//发送读扇区命令  
    
	if(r1)
	  	return r1;  //应答不正确，直接返回
	
	Clr_TF_CS;//选中TF卡
	
	if(TF_GetResponse(0xFE))//等待TF卡发回数据起始令牌0xFE
	{
		Set_TF_CS; //关闭TF卡
		return 1;//读取失败
	}	 
	
	for(i=0;i<offset;i++)
	  	SPI_FLASH_SendByte(0xff);//跳过offset位 
	
	for(;i<offset+bytes;i++)
	  	*buf++=SPI_FLASH_SendByte(0xff);//读取有用数据	
	
	for(;i<512;i++) 
	  	SPI_FLASH_SendByte(0xff); 	 //读出剩余字节
	
	SPI_FLASH_SendByte(0xff);//发送伪CRC码
	SPI_FLASH_SendByte(0xff);  
	
	Set_TF_CS;//关闭SD卡
	
	return 0;
}
/*******************************************************************************
****函数名称:
****函数功能:写1个block的数据
****版本:V1.0
****日期:14-2-2014
****入口参数:sector扇区地址*data 数据缓冲
****出口参数:
****说明:
********************************************************************************/
unsigned char TF_WriteSingleBlock(unsigned int sector, unsigned char * buffer)
{
	unsigned char r1;
	unsigned short i;
	//byte retry=0;
	if(sector<1) 
	  	return 0xff;         //为了保护TF卡引导区，跳过该区
	
	r1 =TF_SendCommand(CMD24, sector<<9,0x95);  //写命令
	
	if(r1 != 0x00) 
	  	return r1;
	
	Clr_TF_CS;
	
	(void)SPI_FLASH_SendByte(0xff);
	(void)SPI_FLASH_SendByte(0xff);
	(void)SPI_FLASH_SendByte(0xff); 
	(void)SPI_FLASH_SendByte(0xfe);        //发开始符
	
	for(i=0; i<512; i++)              //送512字节数据
	{
		(void)SPI_FLASH_SendByte(*buffer++);
	}
	
	(void)SPI_FLASH_SendByte(0xff);
	(void)SPI_FLASH_SendByte(0xff);
	
	r1 = SPI_FLASH_SendByte(0xff);
	
	if((r1&0x1f) != 0x05)             //等待是否成功
	{
		Set_TF_CS;
		return r1;
	}
	//等待操作完
	while(!SPI_FLASH_SendByte(0xff));      
	
	Set_TF_CS;
	
	return 0;     
}
/*******************************************************************************
****函数名称:
****函数功能:写多个block的数据
****版本:V1.0
****日期:14-2-2014
****入口参数:sector扇区地址*data 数据缓冲 count 扇区数目
****出口参数:
****说明:
********************************************************************************/
unsigned char TF_WriteMultiBlock(unsigned int sector, const unsigned char *data, unsigned char count)
{
	unsigned char r1;
	unsigned short i;	 	
	if(sector<1) 
	  	return 0xff;         /*为了保护TF卡引导区，跳过该区*/
	
	r1 =TF_SendCommand(CMD25, sector<<9,0x95);  /*发多块写入指令命令*/
	
	if(r1 != 0x00)
	  	return r1;  /*应答不正确，直接返回	*/ 
	
	Clr_TF_CS;	/*开始准备数据传输 */
	
	(void)SPI_FLASH_SendByte(0xff);/*先放3个空数据，等待TF卡准备好*/
	(void)SPI_FLASH_SendByte(0xff);   
	
	/*下面是N个sector写入的循环部分*/
	do
	{
		/*放起始令牌0xFC 表明是多块写入*/
		(void)SPI_FLASH_SendByte(0xFC);	  
		
		/*放一个sector的数据*/
		for(i=0;i<512;i++)
		{
			(void)SPI_FLASH_SendByte(*data++);
		}
		
		//发2个Byte的dummy CRC
		(void)SPI_FLASH_SendByte(0xff);
		(void)SPI_FLASH_SendByte(0xff);
	
		//等待SD卡应答
		r1 = SPI_FLASH_SendByte(0xff);
		if((r1&0x1F)!=0x05)
		{
			Set_TF_CS;    /*如果应答为报错，则带错误代码直接退出*/
			return r1;
		}		   
	}while(--count);//本sector数据传输完成  
	
	/*发结束传输令牌0xFD*/
	r1 = SPI_FLASH_SendByte(0xFD);
	if(r1==0x00)
	{
		count =  0xfe;
	}		   
	
	Set_TF_CS; 
	return count;   //返回count值，如果写完则count=0，否则count=1
}
/******************* (C) COPYRIGHT 2014 LENCHIMCU.TAOBAO.COM ******************/