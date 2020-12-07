/**
  ******************************************************************************
  * @file    RTC/Calendar/rtc.h 
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   This file contains the headers of the calendar API.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ARTERYTEK SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2018 ArteryTek</center></h2>
  ******************************************************************************
  */ 
 
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H
#define __RTC_H	    

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"

/* Exported types ------------------------------------------------------------*/
typedef struct 
{
	__IO uint8_t hour;
	__IO uint8_t min;
	__IO uint8_t sec;			
	__IO uint16_t w_year;
	__IO uint8_t  w_month;
	__IO uint8_t  w_date;
	__IO uint8_t  week;		 
}_calendar_obj;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
extern _calendar_obj calendar;	
extern uint8_t const mon_table[12];
void RTC_Init(void);   
void RTC_Get(void);
uint8_t Is_Leap_Year(uint16_t year);
uint8_t RTC_Alarm_Set(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);
uint8_t RTC_Get_Week(uint16_t year, uint8_t month, uint8_t day);
uint8_t RTC_Set(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);

#endif

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
