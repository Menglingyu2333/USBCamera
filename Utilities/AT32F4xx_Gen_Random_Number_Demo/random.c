/*
 **************************************************************************
 * File Name    : random.c
 * Description  : 1. Set of firmware functions to manage Leds, push-button and COM ports.
 * Date         : 2018-11-28
 * Version      : V1.0.2
 **************************************************************************
 */
#include "at32_board.h"
#include "random.h"
#include "stdio.h"
#include <stdlib.h> 

/* ��ȡAT32 MCU��PID/UID�Ļ���ַ */
#define DEVICE_ID_ADDR1 0x1FFFF7E8

/**
  * @brief  Get UID Value As Seed
  * @param  None
  * @retval UID_One_Word
  */
int Get_UID_For_Seed (void)
{
	/* ���ڴ�Ŷ�ȡ��ID */
	uint32_t   ID[3] = {0};
	uint32_t   UID_One_Word;
  /* ��ȡPID/UID */
  ID[0] = *(int*)DEVICE_ID_ADDR1;
  ID[2] = *(int*)(DEVICE_ID_ADDR1+8);
	UID_One_Word=ID[0]&0x7FFFFFFF+((ID[2]&(~0x7FFFFFFF))<<16);
	return UID_One_Word;
}

/**
  * @brief  Enable RTC As Seed
  * @param  None
  * @retval None
  */
#if EnableRTCAsSeed
#ifdef AT32F415xx
void ERTC_Init_For_Seed(void)
{
  ERTC_TimeType  ERTC_TimeStructure;
  ERTC_InitType  ERTC_InitStructure;
  ERTC_DateType ERTC_DateStructure;

  __IO uint32_t uwAsynchPrediv = 0;
  __IO uint32_t uwSynchPrediv = 0;

  if (ERTC_ReadBackupRegister(ERTC_BKP_DT0) != 0x32F1)
  {
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR, ENABLE);

    /* Allow access to ERTC */
    PWR_BackupAccessCtrl(ENABLE);

    /* LSI used as ERTC source clock*/
    /* The ERTC Clock may varies due to LSI frequency dispersion */   
    /* Enable the LSI OSC */ 
    RCC_LSICmd(ENABLE);

    /* Wait till LSI is ready */  
    while(RCC_GetFlagStatus(RCC_FLAG_LSISTBL) == RESET)
    {
    }

    /* Select the ERTC Clock Source */
    RCC_ERTCCLKConfig(RCC_ERTCCLKSelection_LSI);

    /* ck_spre(1Hz) = ERTCCLK(LSI) /(uwAsynchPrediv + 1)*(uwSynchPrediv + 1)*/
    uwSynchPrediv = 0xFF;
    uwAsynchPrediv = 0x7F;


    /* Enable the ERTC Clock */
    RCC_ERTCCLKCmd(ENABLE);

    /* Wait for ERTC APB registers synchronisation */
    ERTC_WaitForSynchro();

    /* Configure the ERTC data register and ERTC prescaler */
    ERTC_InitStructure.ERTC_AsynchPrediv = uwAsynchPrediv;
    ERTC_InitStructure.ERTC_SynchPrediv = uwSynchPrediv;
    ERTC_InitStructure.ERTC_HourFormat = ERTC_HourFormat_24;
    ERTC_Init(&ERTC_InitStructure);
    

    /* Set the date: Friday January 11th 2013 */
    ERTC_DateStructure.ERTC_Year = 0x13;
    ERTC_DateStructure.ERTC_Month = ERTC_Month_JAN;
    ERTC_DateStructure.ERTC_Date = 0x11;
    ERTC_DateStructure.ERTC_WeekDay = ERTC_Week_SAT;
    ERTC_SetDateValue(ERTC_Format_BCD, &ERTC_DateStructure);

    /* Set the time to 05h 20mn 00s AM */
    ERTC_TimeStructure.ERTC_AMPM    = ERTC_H12_AM;
    ERTC_TimeStructure.ERTC_Hours   = 0x05;
    ERTC_TimeStructure.ERTC_Minutes = 0x20;
    ERTC_TimeStructure.ERTC_Seconds = 0x00; 

    ERTC_SetTimeValue(ERTC_Format_BCD, &ERTC_TimeStructure);   

    /* Indicator for the ERTC configuration */
    ERTC_WriteBackupRegister(ERTC_BKP_DT0, 0x32F1);  
    
  }
 
}
#else
void RTC_Init_For_Seed (void)
{
  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR, ENABLE);	
  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_BKP, ENABLE);
  PWR_BackupAccessCtrl(ENABLE);	//ʹ�ܺ󱸼Ĵ�������  
    
	if (BKP_ReadBackupReg(BKP_DT1) != 0x5051)	//����ǲ��ǵ�һ������ʱ��
  {	 			
		BKP_Reset();	//��λ�������� 	
		RCC_LSEConfig(RCC_LSE_ENABLE);	//�����ⲿ���پ���(LSE),ʹ��������پ���
    while (RCC_GetFlagStatus(RCC_FLAG_LSESTBL) == RESET)
    {}    
		RCC_RTCCLKConfig(RCC_RTCCLKSelection_LSE);		//����RTCʱ��(RTCCLK),ѡ��LSE��ΪRTCʱ��    
		RCC_RTCCLKCmd(ENABLE);	//ʹ��RTCʱ��  
            
    RTC_WaitForSynchro();		//�ȴ�RTC�Ĵ���ͬ��  
	  RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_INTConfig(RTC_INT_PACE, ENABLE);		//ʹ��RTC���ж�
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RTC_SetDIV(32767); //����RTCԤ��Ƶ��ֵ
		RTC_WaitForLastTask();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
		RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR | RCC_APB1PERIPH_BKP, ENABLE);	//ʹ��PWR��BKP����ʱ��  
		PWR_BackupAccessCtrl(ENABLE);	//ʹ��RTC�ͺ󱸼Ĵ������� 
		RTC_SetCounter(0x44);	//����RTC��������ֵ

		BKP_WriteBackupReg(BKP_DT1, 0x5051);	//��ָ���ĺ󱸼Ĵ�����д���û���������
  }
	else//ϵͳ������ʱ
  {
    RTC_WaitForSynchro();	//�ȴ����һ�ζ�RTC�Ĵ�����д�������
    RTC_WaitForLastTask();
  }

}

#endif 
#endif 
/**
  * @brief  Random Number Test
  * @param  None
  * @retval None
  */
 void RandNum_Test ( void)
{
  

	
#if EnableRTCAsSeed		
#ifdef AT32F415xx
  u32 Get_ERTC_Value=0;
  ERTC_TimeType  ERTC_TimeStructure;
  ERTC_DateType ERTC_DateStructure;
	ERTC_Init_For_Seed();
  
  ERTC_GetTimeValue(ERTC_Format_BIN, &ERTC_TimeStructure);  
  ERTC_GetDateValue(ERTC_Format_BIN,&ERTC_DateStructure);
  
  Get_ERTC_Value=(ERTC_TimeStructure.ERTC_Seconds<<24)+(ERTC_TimeStructure.ERTC_Minutes<<16)+ERTC_GetSubSecondValue();
  
	srand(Get_ERTC_Value+Get_UID_For_Seed());// set UID and RTC as seed for random
#else    
  RTC_Init_For_Seed();
  srand(RTC_GetCounter()+Get_UID_For_Seed());// set UID and RTC as seed for random  
#endif  
#else
	srand(Get_UID_For_Seed());// set only  UID as seed for random
#endif
	
	Get_UID_For_Seed();
	while(1)
	{
		Delay_ms(500);
		printf("%d\r\n",rand());
	}
	 
}


