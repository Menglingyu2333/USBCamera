/**
  ******************************************************************************
  * @file    FLASH/run_in_SPIM/main.c 
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
#include "run_in_SPIM.h"
#include "at32f4xx.h"
#include "at32_board.h"
#include <stdio.h>

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup FLASH_Run_In_SPIM
  * @{
  */

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configures the ext.flash
  * @param  None
  * @retval None
  */
void FLASH_InitExtFlash(void)
{
  GPIO_InitType GPIO_InitStructure;
  
  /* Enable ext.flash GPIO clock */
  RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_AFIO|RCC_APB2PERIPH_GPIOA|RCC_APB2PERIPH_GPIOB, ENABLE);
  
  /* Configure ext.flash pin */
  GPIO_StructInit(&GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_8;
  GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_InitStructure.GPIO_Pins = GPIO_Pins_1|GPIO_Pins_6|GPIO_Pins_7|GPIO_Pins_10|GPIO_Pins_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);
  
  /* Select PB10, PB11 as SPIF_TX and SPIF_RX */
  GPIO_PinsRemapConfig(GPIO_Remap_EXT_FLASH, ENABLE);
  GPIO_PinsRemapConfig(AFIO_MAP7_SPIF_1001, ENABLE);
  
  /* In this example, use on-board EN25QH128A as ext.flash */
  FLASH->B3SEL = FLASH_BANK3_TYPE2;
  
  /* Unlock the ext.flash program erase controller */  
  while (BIT_READ(FLASH->STS3, FLASH_STS_BSY));
  FLASH->FCKEY3 = FLASH_KEY1;
  FLASH->FCKEY3 = FLASH_KEY2;
  while (BIT_READ(FLASH->CTRL3, FLASH_CTRL_LCK));
  
  /* If the data written to ext.flash need to be scrambled, please specify the scrambled range */
  FLASH->DA = 0;  
  
  return;
}

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{ 
  /* USART1 configured as follow:
        - BaudRate = 115200 baud  
        - Word Length = 8 Bits
        - One Stop Bit
        - No parity
        - Hardware flow control disabled (RTS and CTS signals)
        - Receive and transmit enabled
  */
  UART_Print_Init(115200);
  printf("this message is running in bank1.\r\n");
  
  /* Configures the ext.flash */
  FLASH_InitExtFlash();
  
  /* Check the LED toggle in SPIM */
  printf("check the LED toggle in SPIM.\r\n");
  SPIM_run();    
}

/**
  * @}
  */ 

/**
  * @}
  */ 

