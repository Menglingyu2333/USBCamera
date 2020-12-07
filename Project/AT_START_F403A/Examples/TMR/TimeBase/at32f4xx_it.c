/**
  ******************************************************************************
  * @file    TMR/TimeBase/at32f4xx_it.c
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

/** @addtogroup TMR_TimeBase
  * @{
  */
  
uint16_t capture = 0;
extern __IO uint16_t CCR1_Val;
extern __IO uint16_t CCR2_Val;
extern __IO uint16_t CCR3_Val;
extern __IO uint16_t CCR4_Val;

void TMRingDelay_Decrement(void);
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
    {
    }
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
    {
    }
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
    {
    }
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
    {
    }
}
/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}
/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}
/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
//  TMRingDelay_Decrement();
}

void TMR2_GLOBAL_IRQHandler(void)
{
  if (TMR_GetINTStatus(TMR2, TMR_INT_CC1) != RESET)
  {
    TMR_ClearITPendingBit(TMR2, TMR_INT_CC1);

    /* Pin PC.06 toggling with frequency = 73.24 Hz */
    GPIO_WriteBit(GPIOC, GPIO_Pins_6, (BitState)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pins_6)));
    capture = TMR_GetCapture1(TMR2);
    TMR_SetCompare1(TMR2, capture + CCR1_Val);
  }
  else if (TMR_GetINTStatus(TMR2, TMR_INT_CC2) != RESET)
  {
    TMR_ClearITPendingBit(TMR2, TMR_INT_CC2);

    /* Pin PC.07 toggling with frequency = 109.8 Hz */
    GPIO_WriteBit(GPIOC, GPIO_Pins_7, (BitState)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pins_7)));
    capture = TMR_GetCapture2(TMR2);
    TMR_SetCompare2(TMR2, capture + CCR2_Val);
  }
  else if (TMR_GetINTStatus(TMR2, TMR_INT_CC3) != RESET)
  {
    TMR_ClearITPendingBit(TMR2, TMR_INT_CC3);

    /* Pin PC.08 toggling with frequency = 219.7 Hz */
    GPIO_WriteBit(GPIOC, GPIO_Pins_8, (BitState)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pins_8)));
    capture = TMR_GetCapture3(TMR2);
    TMR_SetCompare3(TMR2, capture + CCR3_Val);
  }
  else
  {
    TMR_ClearITPendingBit(TMR2, TMR_INT_CC4);

    /* Pin PC.09 toggling with frequency = 439.4 Hz */
    GPIO_WriteBit(GPIOC, GPIO_Pins_9, (BitState)(1 - GPIO_ReadOutputDataBit(GPIOC, GPIO_Pins_9)));
    capture = TMR_GetCapture4(TMR2);
    TMR_SetCompare4(TMR2, capture + CCR4_Val);
  }
}

/******************************************************************************/
/*                 AT32F4xx Peripherals Interrupt Handlers                   */
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

