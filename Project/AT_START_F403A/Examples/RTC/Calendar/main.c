/**
  ******************************************************************************
  * @file    RTC/Calendar//main.c 
  * @author  Artery Technology 
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Main program body
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
	
/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"
#include "at32_board.h"
#include "rtc.h"
#include <stdio.h>

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup RTC_Calendar
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
char const weekday_table[7][10]={ "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

/* Private function prototypes -----------------------------------------------*/

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	/* USART configuration */
	UART_Print_Init(115200);

	/* RTC configuration */
	RTC_Init();

	/* print initial message */
	printf("\r\n\nInitial ok");

	while(1)
	{
		if(RTC_GetFlagStatus(RTC_FLAG_PACE) != RESET)	
		{
			/* Update current time */
			RTC_Get();
			
			/* print the current time */
			printf("\r\n");
			printf("%d/%d/%d ", calendar.w_year, calendar.w_month, calendar.w_date);
			printf("%02d:%02d:%02d %s", calendar.hour, calendar.min, calendar.sec, weekday_table[calendar.week]);
		
			/* Clear the RTC Second flag */
			RTC_ClearFlag(RTC_FLAG_PACE);
			
			/* Wait until last write operation on RTC registers has finished */
			RTC_WaitForLastTask();
		}
	}
}

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/ 
