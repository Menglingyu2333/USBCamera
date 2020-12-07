/**
  ******************************************************************************
  * @file    SRAM/extend_SRAM/main.c 
  * @author  Artery Technology 
  * @version V1.1.3
  * @date    2019-12-25
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
  * <h2><center>&copy; COPYRIGHT 2019 ArteryTek</center></h2>
  ******************************************************************************
  */ 
  
/* Includes ------------------------------------------------------------------*/
#include "extend_SRAM.h"
#include "at32f4xx.h"
#include "at32_board.h"
#include <stdio.h>

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

 /** @addtogroup SRAM_Extended_SRAM
  * @{
  */

/* Private functions ---------------------------------------------------------*/
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
  
  /* Check EOPB0 */
#ifdef EXTEND_SRAM_96K

  if(((UOPTB->EOPB0)&0xFF)==0xFF)
  {
  	  printf("Extend 96KB SRAM success.\r\n");
  } else {
  	  printf("Extend 96KB SRAM failed.\r\n");
  }		  
#endif

#ifdef EXTEND_SRAM_224K

  if(((UOPTB->EOPB0)&0xFF)==0xFE)
  {
  	  printf("Extend 224KB SRAM success.\r\n");
  } else {
  	  printf("Extend 224KB SRAM failed.\r\n");
  }		  
#endif
  while(1)
  {
  }   
}


/**
  * @}
  */ 

/**
  * @}
  */ 
  
/******************* (C) COPYRIGHT 2019 ArteryTek *****END OF FILE****/
