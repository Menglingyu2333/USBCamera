/**
  ******************************************************************************
  * @file    SRAM/extend_SRAM/EXTEND_SRAM/extend_SRAM.c
  * @author  Artery Technology 
  * @version V1.1.3
  * @date    2019-12-25
  * @brief   This file contains the function extend_SRAM_test used to extend SRAM size
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
#include "stdio.h"   

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

 /** @addtogroup SRAM_Extended_SRAM
  * @{
  */

/* Private typedef -----------------------------------------------------------*/

#define TEST_RAM_SIZE  0x800

/* Private variables ---------------------------------------------------------*/



/* Private functions ---------------------------------------------------------*/
/**
  * @brief  To extend SRAM size
  * @param  None
  * @retval None
  */
void extend_SRAM(void)
{
/* Target set_SRAM_96K is selected */
#ifdef EXTEND_SRAM_96K
/* check if RAM has been set to 96K, if not, change EOPB0 */
  if(((UOPTB->EOPB0)&0xFF)!=0xFF)
  {
    /* Unlock Option Bytes Program Erase controller */
    FLASH_Unlock();
    /* Erase Option Bytes */
    FLASH_EraseUserOptionBytes();
    /* Change SRAM size to 96KB */
    FLASH_ProgramUserOptionByteData((uint32_t)&UOPTB->EOPB0,0xFF);
    NVIC_SystemReset();
  }
#endif

/* Target set_SRAM_224K is selected */
#ifdef EXTEND_SRAM_224K
/* check if RAM has been set to 224K, if not, change EOPB0 */
  if(((UOPTB->EOPB0)&0xFF)!=0xFE)
  {
    /* Unlock Option Bytes Program Erase controller */
    FLASH_Unlock();
    /* Erase Option Bytes */
    FLASH_EraseUserOptionBytes();
    /* Change SRAM size to 224KB */
    FLASH_ProgramUserOptionByteData((uint32_t)&UOPTB->EOPB0,0xFE);
    NVIC_SystemReset();
  }
#endif
}

/**
  * @}
  */ 

/**
  * @}
  */ 
  
