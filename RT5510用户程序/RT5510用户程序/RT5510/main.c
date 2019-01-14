#include"stm8s.h"
#include"main.h"
#include "comm.h"
#include "timer.h"
#include "time4.h"
#include "uart485.h"
#include "Data_Cul.h"
#include "WalkMotor.h"
#include "KneadMotor.h"
#include "KnockMotor.h"
#include "Input.h"
#include "timer.h"

__no_init BITS GlobalFlags0 ;
#define bTime10ms 	              GlobalFlags0.bD0
#define bTime1msFlag                  GlobalFlags0.bD1
#define sending_485                   GlobalFlags0.bD2
#define bTime100ms                    GlobalFlags0.bD3
#define n_weixin_start_flag           GlobalFlags0.bD4
#define bAutoProgramOver 	      GlobalFlags0.bD5
#define bBackAutoModeInit 	      GlobalFlags0.bD6

#define	KEY_ENABLE	0xCC	// ����IWDG��
#define	KEY_REFRESH	0xAA	// ˢ��IWDG��
#define	KEY_ACCESS	0x55	// ������ܱ�����IWDG_PR��IWDG_RLR����

/***********************************************************************************************************
  Ϊ֧��bootload ���
  add on 2018-08-17 by zhang
************************************************************************************************************/

#define USB_CHARGING_METHOD_STORE_ADDR (0x4010) //USB ���һֱ��緽ʽ�洢��ַ,ע���ַ0x4001~0x4003д�������ⲻ����
#define USB_CHARGING_METHOD_STORE_FLAG (0x4011) //USB��緽ʽ�ѱ�����ñ�־��1 �����ã�0 û�����ù���ע���ַ0x4001~0x4003д�������ⲻ����

bool bContinueChargingIsRun = FALSE;
uint8_t flgRunInUserApp;
uint8_t resp_buff[87];
//uint8_t  *pSWVER = "RT5510_Main_RT000_V1.11_11111111.bin";
//uint8_t  *pSWVER = "RT5510_Main_RT000_V2.22_22222222.bin";
uint8_t  *pSWVER = "RT5510_Main_RT000_V2.04_20190114.bin";
uint8_t  *pPCBAVER =  "4.2.0103314-3";
void Update_Cmd_Resp(void);
void fill_ver_to_buff()
{
  uint8_t *p = pSWVER;
  uint8_t *q = pPCBAVER;
  int i;
  for(i=0;i<48;i++)
  {
    if(i<36)
    {
      //resp_buff[4+i] = p[i]; 
      resp_buff[5+i] = p[i];   //zhang 20181204
    }
    else
    {
      
      //resp_buff[4+i] = ' ';
      resp_buff[5+i] = '\0'; //zhang 20181208
    }
  }
  

   for(i=0;i<32;i++)
  {
    if(i<13)
    {
      //resp_buff[i+52] = q[i];
      
      resp_buff[i+53] = q[i];   //zhang 20181204
    }
    else
    {
      //resp_buff[i+52] = '\0';
      resp_buff[i+53] = '\0'; //zhang 20181204
    }
  }
  
}


void E2PROM_Write_Bytes(uint32_t mem_addr,uint8_t *pbuff,uint16_t len)
{
  uint8_t i;
  sim();
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  //while(FLASH_GetFlagStatus(FLASH_FLAG_PUL) == RESET);
  asm(" nop"); //added in 20181212
  asm(" nop");
  asm(" nop");
  asm(" nop");
  asm(" nop");
  asm(" nop");
  for(i=0;i<len;i++)
  {
    FLASH_ProgramByte(mem_addr++,pbuff[i]);
    FLASH_WaitForLastOperation(FLASH_MEMTYPE_DATA);
  }
  
  FLASH_Lock(FLASH_MEMTYPE_DATA);
  rim();
}


void E2PROM_Write_Byte(uint8_t data,uint32_t mem_addr)
{
  asm(" sim");
  FLASH_SetProgrammingTime(FLASH_PROGRAMTIME_STANDARD);
  FLASH_Unlock(FLASH_MEMTYPE_DATA);
  while((FLASH->IAPSR & 0x08) == 0);
  FLASH_ProgramByte(mem_addr,data);
  while((FLASH->IAPSR & 0x40) == 0);
  FLASH_Lock(FLASH_MEMTYPE_DATA);
  asm(" rim");
}

uint8_t E2PROM_Read_Byte(uint32_t mem_addr)
{
  
  return FLASH_ReadByte(mem_addr);
}
/********************************************************************************************************/

ChargingType charge_type;    //������ͷ�ʽ�ṹ�����
void USB_Charging_Control(); //�����ƺ���
/*******************************************************************************************************/
//add by yqb 20170908
 unsigned int  nGetTotalTime;
__no_init unsigned int runTime;  // 1200;
__no_init unsigned int w_PresetTime;
__no_init unsigned char nCurSubFunction ; 
__no_init unsigned char nTxBufCount;;
__no_init unsigned long massage_step_tm_flag;
__no_init unsigned long massage_step_tm;  //by_Time30s;
__no_init unsigned char nTxBuf[MAX_SEND_COUNT],nRxBuf[MAX_RECEIVE_COUNT];
unsigned char nSendCount,nSendTotalCount;
bool bReceivePacketOk, bMasterSendPacket,bReceivePacket;

unsigned char  Settle_State;

unsigned char nBackMainRunMode,nBackSubRunMode;
unsigned char nCurKneadKnockSpeed,nCurKnockRunStopCounter,nCurActionStepCounter;
unsigned char bKnockMotorInProcess,bKneadMotorInProcess,bWalkMotorInProcess;
unsigned char nACKcurActionStep;
unsigned char nChairRunState;
extern unsigned char Button ;
unsigned int RX_Index = 0;
unsigned char counter_10ms;
unsigned char nCounter_2ms=0;
unsigned char nReceiveClass =0xAB;
extern unsigned int WalkControlTime;//200ms++
unsigned char  autoStep=0;

//static unsigned char  count1=0;

#define CHECKSUM_LENGTH           13
//ʵ����Ч�ֽ�Ϊ14λ������������������ֻУ��ǰ13λ�����һλ���Ҳ�λû����CHEKCSUMЧ�飬
//CHECKSUM_LENGTH ����Ϊ13
struct Motor_Struct
{
  unsigned char  WalkMotorDir:2;        // 0:ֹͣ 1: ���� 2:����
  unsigned char  WalkMotorSingal:2;     //0:���г��ź� 1:��ʱ���ź���
  unsigned char  KneadMotorAction:2;   //������ 1:��������2:�������� 3 �걳
  unsigned int   KneadMotorSpeed ;     //�����ٶȿ���
  unsigned int   WalkMotorRunTime;     //���ߵ��ʱ�����
};
struct Motor_Struct MotorControl;

const struct Motor_Struct AutoMotor[36] = 
{
        {WALK_LOCATE_Up,Walk_Signal_Time,Knead_Positive,32,50},
        {WALK_LOCATE_Up,Walk_Signal_Sw,Knead_Stop,32,0},
	{WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Positive,32,100},
	{WALK_LOCATE_Down,Walk_Signal_Time,Knead_Positive,32,20},
	{WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Rubbing,32,60},
	{WALK_LOCATE_Down,Walk_Signal_Sw,Knead_Positive,32,50},
	{WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Positive,32,10},
	{WALK_LOCATE_Up,Walk_Signal_Time,Knead_Positive,32,80},
	{WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Reverse,32,80},
	{WALK_LOCATE_Up,Walk_Signal_Time,Knead_Positive,32,20},
        //10
	{WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Reverse,32,60},
	{WALK_LOCATE_Down,Walk_Signal_Sw,Knead_Rubbing,32,50},
	{WALK_LOCATE_Up,Walk_Signal_Time,Knead_Reverse,32,70},
        {WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Reverse,32,70},
	{WALK_LOCATE_Up,Walk_Signal_Sw,Knead_Stop,0,40},
        {WALK_LOCATE_Down,Walk_Signal_Time,Knead_Stop,0,40},
        {WALK_LOCATE_Up,Walk_Signal_Sw,Knead_Stop,0,40},
	{WALK_LOCATE_Down,Walk_Signal_Sw,Knead_Positive,32,50},
        {WALK_LOCATE_Up,Walk_Signal_Time,Knead_Positive,32,70},
        {WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Reverse,32,80},
        //20
        {WALK_LOCATE_Up,Walk_Signal_Sw,Knead_Stop,30,0},
        {WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Positive,30,50},
	{WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Rubbing,30,40},
	{WALK_LOCATE_Down,Walk_Signal_Time,Knead_Positive,30,20},
	{WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Rubbing,30,60},
	{WALK_LOCATE_Down,Walk_Signal_Sw,Knead_Positive,30,50},
	{WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Positive,30,20},
	{WALK_LOCATE_Up,Walk_Signal_Time,Knead_Positive,30,80},
	{WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Reverse,30,60},
	{WALK_LOCATE_Up,Walk_Signal_Time,Knead_Positive,30,20},
        //30
	{WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Reverse,30,60},
	{WALK_LOCATE_Down,Walk_Signal_Sw,Knead_Rubbing,30,40},
	{WALK_LOCATE_Up,Walk_Signal_Time,Knead_Reverse,30,70},
        {WALK_LOCATE_Stop,Walk_Signal_Time,Knead_Reverse,30,70},
	{WALK_LOCATE_Up,Walk_Signal_Sw,Knead_Stop,0,50},
	{WALK_LOCATE_Down,Walk_Signal_Sw,Knead_Positive,30,40},    
};

//hardware initial

void NetBrd_Initial_IO(void)
{
    GPIO_Init(GPIOD, GPIO_PIN_7, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_WriteHigh(GPIOD, GPIO_PIN_7);
    GPIO_Init(GPIOD, GPIO_PIN_4, GPIO_MODE_OUT_PP_LOW_SLOW);
    GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
}

void NetBrd_PowerOn(void)
{
    GPIO_WriteHigh(GPIOD, GPIO_PIN_7);
    GPIO_WriteHigh(GPIOD, GPIO_PIN_4);
}

void NetBrd_PowerOff(void)
{
    GPIO_WriteLow(GPIOD, GPIO_PIN_7);
    GPIO_WriteLow(GPIOD, GPIO_PIN_4);
}


void  Main_Initial_IO(void)
{    
  disableInterrupts();//_asm("sim");
  InitClock(); 
  Input_Initial_IO();
  KneadMotor_Init_IO();
  WalkMotor_Init_IO();
  NetBrd_Initial_IO();
  Time4_init();
  uart2_init();
  
  //ADC_Init();
  enableInterrupts(); //_asm("rim");   
}

//software initial
void Main_Initial_Data(void)
{
   GlobalFlags0.nByte=0;
   Timer_Counter_Clear(0);
   nBackMainRunMode = BACK_MAIN_MODE_IDLE ;
   nBackSubRunMode= BACK_SUB_MODE_NO_ACTION;
   nChairRunState = CHAIR_STATE_SLEEP;//CHAIR_STATE_SLEEP;//hzx
   nACKcurActionStep=0;
   nCounter_2ms=0;
   bMasterSendPacket = FALSE ; 
   Data_Init();
   WalkControlTime=0;
   KneadMotor_Control(0,0); 
   WalkMotor_Control(1,0,0);
   Timer_Initial();
   Settle_State=0;
}

void Timer_Flag_50ms_Int(void)
{
  //bMasterSendPacket = TRUE;
}
void main_100ms_int(void)
{
  bReceivePacket =  TRUE;
}
void main_200ms_int(void)
{
  bMasterSendPacket = TRUE;
}

void Main_BackProce(void)
{ 
  uint8_t read_io_down = DOWNSWITCH;
  uint8_t  read_io_up = UPSWITCH;
  //bGetNextActionStep =TRUE;
    if(bGetNextActionStep == TRUE)
    	{
		bGetNextActionStep = FALSE ;
		WalkControlTime=0;
		MotorControl.KneadMotorAction=AutoMotor[autoStep].KneadMotorAction;
		MotorControl.KneadMotorSpeed=AutoMotor[autoStep].KneadMotorSpeed;
		MotorControl.WalkMotorDir=AutoMotor[autoStep].WalkMotorDir;
		MotorControl.WalkMotorRunTime=AutoMotor[autoStep].WalkMotorRunTime;
	        MotorControl.WalkMotorSingal=AutoMotor[autoStep].WalkMotorSingal;
	autoStep++;
	if(autoStep>=36)
		{
		  autoStep=0;
	        }	
      } 
    WalkMotor_Control(MotorControl.WalkMotorSingal,MotorControl.WalkMotorDir,MotorControl.WalkMotorRunTime );//Signal=1 ��ʱ����
    KneadMotor_Control(MotorControl.KneadMotorAction,MotorControl.KneadMotorSpeed );   
}

static void InitClock(void)
{
   //CLK_HSIPrescalerConfig(CLK_PRESCALER_HSIDIV1);    
    CLK_SYSCLKConfig(CLK_PRESCALER_HSIDIV1);
    CLK_HSICmd(ENABLE);   
}

void ADC_Init(void)
{   
    ADC1_DeInit();
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_6, ADC1_PRESSEL_FCPU_D8,
    ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL0,DISABLE);
    ADC1_Init(ADC1_CONVERSIONMODE_SINGLE, ADC1_CHANNEL_7, ADC1_PRESSEL_FCPU_D8,
    ADC1_EXTTRIG_TIM, DISABLE, ADC1_ALIGN_RIGHT, ADC1_SCHMITTTRIG_CHANNEL0,DISABLE);
   //ADC1_ScanModeCmd(ENABLE);//scan channel from AIN0~AINn,so AIN0~AINn channel can't enable output
   //ADC1_DataBufferCmd(ENABLE);//used in scan mode
    ADC1_ITConfig( ADC1_IT_EOCIE,ENABLE);
    ADC1_Cmd(ENABLE); 
    ADC1_StartConversion();
 }

void TX_BUF(void)
{
    unsigned int time;
    if(bMasterSendPacket)
    {    
        bMasterSendPacket = FALSE;  
        nTxBuf[0] = EOI_AA ;//EOI_AA
        nTxBuf[1] = 0;
        //��ʶ 1	��Ħ������״̬ 1	��Ħ�ַ� 3	��Ħ���� 3
        if(nChairRunState == CHAIR_STATE_SLEEP|nChairRunState == CHAIR_STATE_SETTLE)
        {
            nTxBuf[1] = 0 << 6;
        }
        else//0B01000011
        {
            nTxBuf[1] = 1 << 6;
        }
       nTxBuf[1] |= (nChairRunState&0x07);
         //state
       nTxBuf[2] = nBackMainRunMode&0x7f;
       nTxBuf[3] = nBackSubRunMode&0x7f;
       nTxBuf[4] = 0;	 	
       //unsigned int time = Data_Get_TimeSecond();
        time = Data_Get_TimeSecond();//��ȡʣ��ʱ��
        nTxBuf[5] = (time >> 14) & 0x7f;
        nTxBuf[6] = (time >> 7) & 0x7f;
        nTxBuf[7] = time & 0x7f;
       //unsigned int pos = GetWalkMotorPosition();
       nTxBuf[8] = 0;
       nTxBuf[9] = 0;
       nTxBuf[10] = 0;
        //stretch, roller, buzzer ,air_bag.
       nTxBuf[11] =    0;//(st_Stretch.active&0x7f);
       nTxBuf[12] =    0;//(bRollerEnable & 0x7f);
       nTxBuf[13] = charge_type.charging_method;//zhang 20181207
       nTxBuf[14] =    0;//nKeyAirBagLocate&0x7f;
	      
       nTxBuf[15] = CalculatCheckSum();
       nTxBuf[16] = EOI_AB;
       nTxBufCount = 17;
      Uart_SendData(nTxBuf,nTxBufCount);  
    }
}
char CalculatCheckSum(void)
{
  char cCheckSum;
  uint32_t sum;
  sum = 0;
  for(uint8_t Index = 1;Index <= CHECKSUM_LENGTH;Index++)//�ӵ�һ���ֽڿ�ʼ�����һ����������SOI,EOI�������ֵ
  {
    sum +=  nTxBuf[Index];
  }
  cCheckSum = ~sum;
  cCheckSum &= 0x7f; //ASCII_LIMIT = 0x7f.
  return cCheckSum;
} 
  void Main_Stop_All(void)
{
    WalkMotor_Control(0,0,0);
    KneadMotor_Control(0,0);   
}

void Init_Iwdg(void)
{  
    IWDG->KR = 0xCC; //����IWDG    
    IWDG->KR = 0x55; //��� PR �� RLR ��д����    
    IWDG->RLR = 0xff; //���Ź���������װ����ֵ          
    IWDG->PR = 0x06; //��Ƶϵ��Ϊ256 1.02s    
    IWDG->KR = 0xAA; //ˢ��IDDG������������Ź���λ��ͬʱ�ָ� PR �� RLR ��д����״̬  
}

void Main_Settle(void)
{
   unsigned char key;
   uint8_t read_io_up = UPSWITCH;
   uint8_t read_io_down = DOWNSWITCH; 
   nChairRunState = CHAIR_STATE_SETTLE ;//��Ħ�δ����ղ�״̬
   Timer_Initial();
   autoStep=0;
   while(nChairRunState == CHAIR_STATE_SETTLE)
    {   
        //USB_Charging_Control(); //added by zhang in 20181103
        Update_Cmd_Resp(); //add by zhang in 2018-09-18
        
        key = BlueToothUart_GetKey();  
        if(key == H10_KEY_WORK_EN)
        {
          nChairRunState = CHAIR_STATE_WAIT_COMMAND;   
        }  
        
  //��Ħ�ΰ�Ħ������λ
    switch(Settle_State)
    {
         case 0:
	     if(!(KneadMotor_Control(0,0)))
		Settle_State=1;    
            break;
        case 1:
            if(WalkMotor_Control(0,2,0))//Signal=0�ߵ����г�
            	{
                       // count1++;
                        Settle_State=2;
			WalkControlTime=0;
                        
            	}
            break;
        case 2:
             
            
           // if(WalkMotor_Control(1,1,59))//Signal=1 ��ʱ����
            if(WalkMotor_Control(1,1,53)) // modify by zhang in 2018-10-11,Ե�ɣ���������������
            Settle_State = 3;
          break;
        case 3:  
		nChairRunState = CHAIR_STATE_SLEEP;
            break;
        default:
            break;
      }    
       Input_Proce();
       TX_BUF();  
       //Update_Cmd_Handler();
      }   
   bMasterSendPacket = FALSE;
}

void Sleep_Process(void)
{
   unsigned char key;
   nBackMainRunMode = BACK_MAIN_MODE_IDLE ;
   nCurSubFunction = BACK_SUB_MODE_NO_ACTION;
   nChairRunState = CHAIR_STATE_SLEEP;
   nBackSubRunMode = BACK_SUB_MODE_NO_ACTION ;
   Timer_Initial();
   Timer_Counter_Clear(C_TIMER_TEMP);
   uint8_t read_io_up = UPSWITCH;
   uint8_t read_io_down = DOWNSWITCH; 
   //USB_OFF;
   autoStep=0;
   WalkControlTime=0;
   WalkMotor_Control(0,0,0);
   KneadMotor_Control(0,0);
   
  while(CHAIR_STATE_SLEEP == nChairRunState)
    {
      USB_Charging_Control(); //USB�����ƣ�added by zhang in 20181103
      Update_Cmd_Resp();
      //����������
        key = BlueToothUart_GetKey();
        key &= 0x7f;
        if(key == H10_KEY_WORK_EN) //Ӧ���� 0x72
        {
          nChairRunState = CHAIR_STATE_WAIT_COMMAND;   
        }
       Input_Proce();    
       TX_BUF();  
       //Update_Cmd_Handler();
    }  
   /***************�����˳���**************************/ 
}

void Main_WaitCommand(void)
{ 
    unsigned char key;
    nChairRunState = CHAIR_STATE_WAIT_COMMAND ;//��Ħ�εȴ��������� 
    nBackSubRunMode = BACK_SUB_MODE_NO_ACTION ;
    Timer_Counter_Clear(C_TIMER_TEMP);
    Timer_Counter_Clear(C_TIMER_500MS);
    Data_Set_Start(0,0);
    Main_Stop_All();
    nBackMainRunMode = BACK_MAIN_MODE_IDLE ;
    while(nChairRunState == CHAIR_STATE_WAIT_COMMAND)
    { 
      USB_Charging_Control(); //added by zhang in 20181103
      Update_Cmd_Resp();
       //����������  
       key = BlueToothUart_GetKey(); //add by zhang in 20181022
        key &= 0x7f;  
         
          switch(key)
          {
            case H10_KEY_CHAIR_AUTO_0:
              nChairRunState = CHAIR_STATE_RUN ;
              nBackMainRunMode = BACK_MAIN_MODE_AUTO;
              nBackSubRunMode = BACK_SUB_MODE_AUTO_0;
              break ;
              
            case H10_KEY_CHAIR_AUTO_1:
              nChairRunState = CHAIR_STATE_RUN ;
              nBackMainRunMode = BACK_MAIN_MODE_AUTO;
              nBackSubRunMode = BACK_SUB_MODE_AUTO_1;	 
              break ;
              
            case H10_KEY_CHAIR_AUTO_2:
              nChairRunState = CHAIR_STATE_RUN ;
              nBackMainRunMode = BACK_MAIN_MODE_AUTO;
              nBackSubRunMode = BACK_SUB_MODE_AUTO_2;	  
	   
              break ;
              
            case H10_KEY_CHAIR_AUTO_3:
              nChairRunState = CHAIR_STATE_RUN ;
              nBackMainRunMode = BACK_MAIN_MODE_AUTO;
              nBackSubRunMode = BACK_SUB_MODE_AUTO_3;
              break ;
              
            case H10_KEY_CHAIR_AUTO_4:
              nChairRunState = CHAIR_STATE_RUN ;
              nBackMainRunMode = BACK_MAIN_MODE_AUTO;
              nBackSubRunMode = BACK_SUB_MODE_AUTO_4; 
              break ;
              
            case H10_KEY_CHAIR_AUTO_5:
              nChairRunState = CHAIR_STATE_RUN ;
              nBackMainRunMode = BACK_MAIN_MODE_AUTO;
              nBackSubRunMode = BACK_SUB_MODE_AUTO_5;
              break ;         
          default:
            break;
          }
         Input_Proce();
         TX_BUF();  
         //Update_Cmd_Handler();
    }
}


bool bCloseUsbCharging = FALSE;
int tim_close_usbcharging = 60;
void Main_Work_Run(void)
{
    unsigned char key;
    bAutoProgramOver = 0;
    Timer_Counter_Clear(C_TIMER_TEMP);
    Timer_Counter_Clear(C_TIMER_500MS);
    
    //charge_type.charging_method = 0x03; //add by zhang in 20181207
    bCloseUsbCharging = FALSE; //zhang 20181207
    tim_close_usbcharging = 3600; //zhang 20181207 
   //add by yqb 20170907 usb��� ,ɾ����2018-08-18
    USB_ON;
    
    
     
    nGetTotalTime = GetTotalRuntime();		
	             if(nGetTotalTime<5*60)
	                {
	                     runTime = 5*60;
	                }		
	                else
	                {
	                    runTime = nGetTotalTime;
	                }  
                   w_PresetTime=runTime; 
                   nChairRunState = CHAIR_STATE_RUN ;
                   nBackMainRunMode = BACK_MAIN_MODE_AUTO;
                   nBackSubRunMode = BACK_SUB_MODE_AUTO_0;                                                             
    		   Data_Set_Start(1, w_PresetTime);
		   Data_Set_Time(w_PresetTime);	
      
   
//     //add by zhang on 2018-08-17
//      if(E2PROM_Read_Byte(USB_CHARGING_METHOD_STORE_ADDR) == 0x02) //Ĭ��Ϊһֱ��緽ʽʱ�Ĵ���
//       {
//         USB_ON;
//       }
//       else //��һֱ���ģʽ
//       {
//         
//          USB_OFF;
//       }

    
    //��ѭ��
    while(CHAIR_STATE_RUN == nChairRunState)
    { 
       Update_Cmd_Resp();
       USB_Charging_Control(); //ADD BY ZHANG ON 2018-08-17
       
      //����������
       key = BlueToothUart_GetKey();
        key &= 0x7f;   
        switch(key)
        {    
       case   H10_KEY_WORK_DIS :
                nChairRunState = CHAIR_STATE_SETTLE ;           
                n_weixin_start_flag = FALSE;
                break ;
         
        case  H10_KEY_TIME_ADD :
                nGetTotalTime = GetTotalRuntime();		
                runTime = Data_Get_TimeSecond(); //�޸��趨ֵ�������б�Ҫ�޸���
		runTime+=nGetTotalTime;
		w_PresetTime=runTime;	
		Data_Set_Start(1, w_PresetTime);
		Data_Set_Time(w_PresetTime);		
               break;
          
         case H10_KEY_CHAIR_AUTO_0:
          if(nBackSubRunMode == BACK_SUB_MODE_AUTO_0)  break;
          nBackMainRunMode = BACK_MAIN_MODE_AUTO ;
          nBackSubRunMode = BACK_SUB_MODE_AUTO_0 ;
          break ;
          
        case H10_KEY_CHAIR_AUTO_1:
          if(nBackSubRunMode == BACK_SUB_MODE_AUTO_1)  break;
          nBackMainRunMode = BACK_MAIN_MODE_AUTO ;
          nBackSubRunMode = BACK_SUB_MODE_AUTO_1 ;
          Data_Set_Start(1, w_PresetTime);
          break ;
          
        case H10_KEY_CHAIR_AUTO_2:
          if(nBackSubRunMode == BACK_SUB_MODE_AUTO_2)  break;
          nBackMainRunMode = BACK_MAIN_MODE_AUTO ;
          nBackSubRunMode = BACK_SUB_MODE_AUTO_2 ;	
          Data_Set_Start(1, w_PresetTime);
          break ;
          
        case H10_KEY_CHAIR_AUTO_3:
          if(nBackSubRunMode == BACK_SUB_MODE_AUTO_3)  break;
          nBackMainRunMode = BACK_MAIN_MODE_AUTO ;
          nBackSubRunMode = BACK_SUB_MODE_AUTO_3 ;
          Data_Set_Start(1, w_PresetTime);
          break;
          
        case H10_KEY_CHAIR_AUTO_4:
          if(nBackSubRunMode == BACK_SUB_MODE_AUTO_4)  break;
          nBackMainRunMode = BACK_MAIN_MODE_AUTO ;
          nBackSubRunMode = BACK_SUB_MODE_AUTO_4 ;
          Data_Set_Start(1, w_PresetTime);
          break;   
          
        case H10_KEY_CHAIR_AUTO_5:
          if(nBackSubRunMode == BACK_SUB_MODE_AUTO_5)  break;
          nBackMainRunMode = BACK_MAIN_MODE_AUTO;
          nBackSubRunMode = BACK_SUB_MODE_AUTO_5;
          Data_Set_Start(1, w_PresetTime);  
          break;
       default:       
          break;
        }
    
   if(Data_Get_TimeSecond()== 0)  
       {         
         nChairRunState = CHAIR_STATE_SETTLE;  //��Ħʱ�䵽
          Settle_State=0;
//          if(bContinueChargingIsRun == FALSE) //ADD BY ZHANG IN 20181109
//          {
//          USB_OFF;
//          }
       } 
    if(Timer_Counter(C_TIMER_RUN + T_LOOP,1))
       {
         nCurActionStepCounter++ ;
       }
       Input_Proce();
       Data_Time_Counter_Proce();
       TX_BUF(); 
        
      switch(nBackMainRunMode)
      {
      case  BACK_MAIN_MODE_AUTO:  
          //bGetNextActionStep =TRUE;
            Main_BackProce();
            break;
      default:
            break;
     }   
    //Update_Cmd_Handler();
  }
    if(bContinueChargingIsRun == FALSE) //ADD BY ZHANG IN 20181109
    {
         bCloseUsbCharging = TRUE; //��Ҫ��ʱ��رճ���־
         tim_close_usbcharging = 60; //150����ʱ���رճ��
    }
    
}

extern unsigned int ucTotalRunTime;
extern bool tick_1s_flg;
extern void BlueToothUart_SetKey(unsigned char by_Data);


/*******************************************************************************
�������ƣ�USB_Charging_Control()
�����������
����ֵ����
���ܣ�����APP������ָ�����USB���Ŀ������رռ����ķ�ʽ. �ؼ������ݽṹ�����������£�
    typedef struct
    {
      bool charging_open;   // ��ʾ����Ƿ��ڽ��У�TRUE ��ʾ�ڽ���
      unsigned char charging_method;   //��緽ʽ��0x01 ��ʾ��ʱ��� ��0x02 һֱ���, 0x00 �رճ�磬 ����ֵΪ ��Ч������
      unsigned int charging_time;    //���ʣ��ʱ�䡣
    } ChargingType;
    һֱ����������������ȼ����ڹر�һֱ��繦��ǰ��ʱ���ָ���ֹ��
    �����б���bContinueChargingIsRun������ʾһֱ��繦���Ƿ�����.
********************************************************************************/

void USB_Charging_Control()
{
  
    uint8_t key = H10_KEY_NONE;

    key = BlueToothUart_GetKey();
    key &= 0x7f; 
    charge_type.charging_method &= 0x03;
    
    //-->���ݼ�ֵ�������ýṹ����charge_type
    if(key >= 0x75 && key <= 0x78) //key�����й�ʱ������0x75��0x78֮��
    {
       BlueToothUart_SetKey(H10_KEY_NONE); //��button = H10_KEY_NONE
    }
    else //���粻��ص�KEY����ֱ�ӷ���
    {
      return;  
    }
    
    if(key == H10_KEY_CHARGING_OPEN)    //�յ���USB����ʱ�����ָ��
    { 
      {//Uart_SendData("aaa",3);
        charge_type.charging_method |=  0x01;  //��ʱ���
        charge_type.charging_time = ucTotalRunTime; //���ʣ��ʱ��
      }
      
    }
    if(key == H10_KEY_CHARGING_STOP)
        charge_type.charging_method &=  0xFE;
    
    if(key == H10_KEY_CONTINUOUS_CHARGING_ENABLE) 
    { //Uart_SendData("ccc",3);
        charge_type.charging_method |= 0x02;
        bContinueChargingIsRun = TRUE;
    }
    if(key == H10_KEY_CONTINUOUS_CHARGING_DISABLE)
    {
        charge_type.charging_method &= 0xFD;
        charge_type.charging_open = FALSE;
        bContinueChargingIsRun = FALSE;
    }
    //-->��������һֱ��緽ʽ���´ΰ�Ħ��ʼʱ�����Ƿ�ֱ�Ӵ�USB���
    if((key == H10_KEY_CONTINUOUS_CHARGING_ENABLE) || (key == H10_KEY_CONTINUOUS_CHARGING_DISABLE))
    {   //Uart_SendData("eee",3);
        E2PROM_Write_Bytes(USB_CHARGING_METHOD_STORE_ADDR,&(charge_type.charging_method),1);
        BlueToothUart_SetKey(H10_KEY_NONE); //��button = H10_KEY_NONE,��ֹ��ͣ�ض�E2PROM������¼������E2PROM����)
    }
    //<--����������緽ʽ����
    
    //--> ���ݽṹ����charge_type�����õĳ�緽ʽ����򿪻�ر�USB����
    
    
    if(charge_type.charging_method == 0x01  || charge_type.charging_method == 0x02 
        || charge_type.charging_method == 0x03)
    { 
      if(charge_type.charging_open == FALSE)
      {//Uart_SendData("fff",3);
        charge_type.charging_open = TRUE;
        USB_ON;
      }
    }    
    
    if(charge_type.charging_method == 0x00)
    {
      
      if(bContinueChargingIsRun == FALSE) //��ֹ�������ȷ��ֻ��һֱ��������ֹʱ���ܹرճ��
      { //Uart_SendData("hhh",3);
        charge_type.charging_open = FALSE;
        USB_OFF;
        //charge_type.charging_method = 0x03; //��Ч�����Ʒ�ʽ��added by zhang in 20181207
      }
    }
    //<-- �򿪻�ر�USB�������
}

/* �ӳ����ֽڵ�У����ֽ�ǰ���������ȡ������ȡ0x7f */
uint8_t Cal_CheckSum(uint8_t *pdata,uint8_t sIndex, uint8_t len)
{
  //sIndex Ϊ��ʼ����
  uint8_t i;
  uint16_t sum = 0;
  for( i=sIndex; i < sIndex+len; i++)
    sum += pdata[i];
  sum ^= 0xff;
  return ((uint8_t)(sum & 0x007f));
  
}


////֡У��
//bool Check_Frame() 
//{
//  uint8_t len;
//  uint8_t tmp;
//  
// 
//    tmp = recv_buff[0] ;
//    if (tmp != SOI)
//      return FALSE;
//    
//    len = recv_buff[1]+1;
//    tmp = Cal_CheckSum(&recv_buff[0],1,len);
//    
//    if(recv_buff[len+1] == tmp)
//      return TRUE;
//  
//  
//  
//  return FALSE;
//  
//}


//void Delay_ms(short nTicks)
//{
//  ms_count = 0;
//  while(ms_count < nTicks);
//  
//}


         
//extern void FLASH_Read_SW_PCBA_Ver();
void Response_Frame_For_Version(void)
{
 
  fill_ver_to_buff( ); //���汾��
  resp_buff[0] = 0xf0;
  resp_buff[1] = 0x53;
  resp_buff[2] = 0xa5;
  resp_buff[3] = 0x01;
  resp_buff[4] = 0x01; //added by zhang 20181204
  
  /* deleted by zhang in 20181204 */
  //resp_buff[84] =  Cal_CheckSum(&resp_buff[0],1, 83);
  //resp_buff[85] = 0xf1;
  //Uart_SendData(&resp_buff[0],86);
  
  /* edited by zhang in 20181204 */
  resp_buff[85] =  Cal_CheckSum(&resp_buff[0],1, 84);
  resp_buff[86] = 0xf1;
  Uart_SendData(&resp_buff[0],87);

}


void Response_Frame_For_BootLoader(void)
{
  /*deleted by zhang 20181206 */
  //uint8_t recv_buff[6];
//  resp_buff[0] = 0xf0;
//  resp_buff[1] = 0x02;
//  resp_buff[2] = 0xa5;
//  resp_buff[3] = 0x02;
//  resp_buff[4] =  Cal_CheckSum(&resp_buff[0],1,3);
//  resp_buff[5] = 0xf1;
//  Uart_SendData(&resp_buff[0],6);
  
  /* added by zhang 20181206 */
  resp_buff[0] = 0xf0;
  resp_buff[1] = 0x03;
  resp_buff[2] = 0xa5;
  resp_buff[3] = 0x02;
  resp_buff[4] = 0x01;
  resp_buff[5] =  Cal_CheckSum(&resp_buff[0],1,4);
  resp_buff[6] = 0xf1;
  Uart_SendData(&resp_buff[0],7);
}

 void JmpToBootLoader(void)
 {
   //flgRunInUserApp = 0xA5; //֪ͨbootload ʹ���Լ����ж�����
//   asm("LDW X,SP");
//   asm("LD A,$FF");
//   asm("LD XL,A");
//   asm("LDW SP,X");
   asm("JPF $8000"); //��λ����bootload
   
 }
unsigned int k , n ;
extern unsigned short update_cmd;

void delay(int cnt)
{
  short i = 1000;
  while(cnt--)
  {
     while(i--);
  }
  
}

/*******************************************************************************/
//�������ƣ�Update_Cmd_Resp()
//�����������
//����ֵ����
//���ܣ�����APP������ָ��ظ�����汾�Ż��߽�������ģʽ����ת��BootLoader����
/********************************************************************************/
void Update_Cmd_Resp(void)
{
   uint8_t bootload_mode_flag = 0;
  //asm(" sim");
    /*********************��������*****************************************/
    if(update_cmd == UPDATE_VER_SEND_01) //��λ����ȡ�汾��
    {
      Response_Frame_For_Version(); //�ظ������PCB��汾��
      
    }
    else if(update_cmd == UPDATE_CMD_GO_01) //��λ��Ҫ���������ģʽ
    {
      
     Response_Frame_For_BootLoader();  //�ظ�BootLoader�û������������ģʽ
     //delay(100);
     bootload_mode_flag = 0x01;
     E2PROM_Write_Byte(bootload_mode_flag ,0x4000); //д�ñ�־�󣬱�֤BootLoader�����������״̬
     delay(20);
     JmpToBootLoader(); //��ת��BootLoader
    }
    update_cmd = 0;
    
    /*******************************************************************/
   // asm(" rim");
  
  
}  

  uint8_t ch;
int main(void)
{ 
   
  update_cmd = 0;
  
  Main_Initial_IO();   //hardware initial
  
  Main_Initial_Data(); //software initial
  
  Init_Iwdg();
  asm(" rim"); 
  
  nChairRunState = CHAIR_STATE_SETTLE;
  
  //STM8S��Ƭ����һ�����س��������غ��һ������ʱ������3�д���������
  if(E2PROM_Read_Byte(USB_CHARGING_METHOD_STORE_FLAG) == 0)
  {
    E2PROM_Write_Byte(1,USB_CHARGING_METHOD_STORE_FLAG);
    E2PROM_Write_Byte(0x02,USB_CHARGING_METHOD_STORE_ADDR);
    USB_ON;
    bContinueChargingIsRun = TRUE;
    charge_type.charging_method =  0x02; //һֱ���
    charge_type.charging_open = TRUE;
    charge_type.charging_time = 0;
  }
  else
  {
       if(E2PROM_Read_Byte(USB_CHARGING_METHOD_STORE_ADDR) == 0x02) //Ĭ��Ϊһֱ��緽ʽʱ�Ĵ���
       {
         USB_ON;
         bContinueChargingIsRun = TRUE;
         charge_type.charging_method =  0x02; //һֱ���
         charge_type.charging_open = TRUE;
         charge_type.charging_time = 0;
         // Uart_SendData("KKKKKK",6);
       }
       else //��һֱ���ģʽ
       {
          bContinueChargingIsRun = FALSE;
          USB_OFF;
          charge_type.charging_method =  0x00; //��Ħ��ɨ����Ĭ�ϳ�緽ʽ
          charge_type.charging_open = FALSE;
          charge_type.charging_time = 0;
          // Uart_SendData("NNNNNN",6);
       }

  }
  

   //E2PROM_Write_Byte(0,USB_CHARGING_METHOD_STORE_FLAG);
   // E2PROM_Write_Byte(0x00,USB_CHARGING_METHOD_STORE_ADDR);
  while(1)
  {
   
    IWDG->KR = 0xAA; 
    switch(nChairRunState)
          {		
          case CHAIR_STATE_SETTLE:
                Main_Settle();
                break; 
          case CHAIR_STATE_SLEEP:
               Sleep_Process();
               break;
          case CHAIR_STATE_WAIT_COMMAND:
               Main_WaitCommand();
               break;  
          case CHAIR_STATE_RUN:
               Main_Work_Run();     
               break; 
          default:
               break;
          } 
    }
}


