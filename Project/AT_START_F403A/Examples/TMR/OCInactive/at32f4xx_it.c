/**
  ******************************************************************************
  * @file    TMR/OCInactive/at32f4xx_it.c
  * @author  ArteryTek Technology
  * @version V1.0.4
  * @date    2019-01-04
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and peripherals
  *          interrupt service routine.
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

/** @addtogroup TMR_OCInactive
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{}

/******************************************************************************/
/*            AT32F4xx Peripherals Interrupt Handlers                        */
/******************************************************************************/

/**
  * @brief  This function handles TMR2 global interrupt request.
  * @param  None
  * @retval None
  */
void TMR2_GLOBAL_IRQHandler(void)
{
  if (TMR_GetINTStatus(TMR2, TMR_INT_CC1) != RESET)
  {
    /* Clear TMR2 Capture Compare1 interrupt pending bit*/
    TMR_ClearITPendingBit(TMR2, TMR_INT_CC1);

    /* PC.06 turnoff after 1000 ms */
    GPIO_ResetBits(GPIOC, GPIO_Pins_6);
  }
  else if (TMR_GetINTStatus(TMR2, TMR_INT_CC2) != RESET)
  {
    /* Clear TMR2 Capture Compare2 interrupt pending bit*/
    TMR_ClearITPendingBit(TMR2, TMR_INT_CC2);

    /* PC.07 turnoff after 500 ms */
    GPIO_ResetBits(GPIOC, GPIO_Pins_7);
  }
  else if (TMR_GetINTStatus(TMR2, TMR_INT_CC3) != RESET)
  {
    /* Clear TMR2 Capture Compare3 interrupt pending bit*/
    TMR_ClearITPendingBit(TMR2, TMR_INT_CC3);

    /* PC.08 turnoff after 250 ms */
    GPIO_ResetBits(GPIOC, GPIO_Pins_8);
  }
  else
  {
    /* Clear TMR2 Capture Compare4 interrupt pending bit*/
    TMR_ClearITPendingBit(TMR2, TMR_INT_CC4);

    /* PC.09 turnoff after 125 ms */
    GPIO_ResetBits(GPIOC, GPIO_Pins_9);
  }
}


/**
  * @}
  */ 

/**
  * @}
  */
  
/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
  
