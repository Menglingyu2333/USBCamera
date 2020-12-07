/**
  ******************************************************************************
  * @file    RCC/MCO/main.c 
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
#include <stdio.h>
#include "at32f4xx.h"
#include "at32_board.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup RCC_MCO
  * @{
  */ 

  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  MCO configuration program
  * @param  None
  * @retval None
  */
void MCO_config(void)
{
  GPIO_InitType GPIO_InitStructure;
  
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOA,ENABLE); 
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8; 
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;	
  GPIO_Init(GPIOA, &GPIO_InitStructure);
 
  RCC_CLKOUTConfig(RCC_CLKOUT_PLL_Div4, RCC_MCOPRE_1);
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
  AT32_Board_Init();

  MCO_config();
  
  for(;;)
  {
    AT32_LEDn_Toggle(LED4);
    Delay_ms(200);
  }
}


/**
  * @}
  */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/ 
