/**
  ******************************************************************************
  * @file    IWDG/IWDG_Reset/at32f4xx_it.c 
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Interrupt program body
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
#include "at32f4xx_it.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */
  
/** @addtogroup IWDG_Reset
  * @{
  */

__IO uint16_t IC1ReadValue1 = 0, IC1ReadValue2 = 0;
__IO uint16_t CaptureNumber = 0;
__IO uint32_t Capture = 0;
extern uint32_t LsiFreq;
extern uint8_t EXTI_flag;

void NMI_Handler(void)
{
}

void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}


void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

void SVC_Handler(void)
{
}

void DebugMon_Handler(void)
{
}

void PendSV_Handler(void)
{
}

void SysTick_Handler(void)
{
}

/******************************************************************************/
/*            AT32F403A Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{
  if(EXTI_GetIntStatus(EXTI_Line0) != RESET)
  {
    EXTI_flag = 1;
    
    /* Clear the  EXTI line 0 pending bit */
    EXTI_ClearIntPendingBit(EXTI_Line0);
  }
}

/******************************************************************************/
/*                 at32f4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_at32f403_xx.s).                                            */
/******************************************************************************/

/**
  * @}
  */

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/ 
