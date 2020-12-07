 /**
 **************************************************************************
 * @file      : PWR/STOP/main.c
 * @author    : Artery Technology
 * @version   : V1.0.4
 * @date      : 2018-12-28
 * @brief     : Main program body
 **************************************************************************
 * @brief     : Initializes and turn on LED.Configure RTC and external interrupt line 17.
 *              Turn off LED2,then enter Stop mode.The  RTC Alarm will wake-up the system within 3 second,then turn on LED2.As the infinite loop.
 *              If generate a RTC alarm event,an interrupt on external line 17 will happen,and it will cause a Toggle on LED4
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
 ***************************************************************************
 */
 
 /* Includes ------------------------------------------------------------------*/
#include <stdio.h>
#include "at32f4xx.h"
#include "at32_board.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup PWR_SLEEP
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
GPIO_InitType GPIO_InitStructure;
TMR_TimerBaseInitType  TMR_TMReBaseStructure;
NVIC_InitType NVIC_InitStructure = {0};

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
 *	@brief  main function
 *	@param  None
 *	@retval None
 */
int main(void)
{
	uint32_t i;
	GPIO_StructInit(&GPIO_InitStructure);     
	AT32_Board_Init();      ///<Initialize LEDs
	AT32_LEDn_ON(LED2);
	AT32_LEDn_ON(LED3);
	AT32_LEDn_ON(LED4);
	
	RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_PWR | RCC_APB1PERIPH_TMR2, ENABLE); ///<Enable PWR and TMR2 clock
    
    TMR_TimeBaseStructInit(&TMR_TMReBaseStructure);
    TMR_TMReBaseStructure.TMR_Period = 65535;
    TMR_TMReBaseStructure.TMR_DIV = 5860;
    TMR_TMReBaseStructure.TMR_ClockDivision = 0;
    TMR_TMReBaseStructure.TMR_CounterMode = TMR_CounterDIR_Up;
    TMR_TimeBaseInit(TMR2, &TMR_TMReBaseStructure);
    
    TMR_INTConfig(TMR2, TMR_INT_Overflow, ENABLE);
    
    NVIC_InitStructure.NVIC_IRQChannel = TMR2_GLOBAL_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;

    NVIC_Init(&NVIC_InitStructure);
    TMR_Cmd(TMR2, ENABLE);
    while (1)
	{
		AT32_LEDn_OFF(LED2);                             ///<Turn off LED2
		PWR_EnterSleepMode(PWR_SLEEPEntry_WFI);///Request to enter SLEEP mode
		AT32_LEDn_ON(LED2);                              ///<Turn on LED2
		for(i=0;i<500000;i++);
	}
}


/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/ 
