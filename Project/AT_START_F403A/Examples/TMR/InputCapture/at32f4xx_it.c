/**
  ******************************************************************************
  * @file    TMR/InputCapture/at32f4xx_it.c
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
extern int Sys_Counter;

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup TMR_Input_Capture
  * @{
  */ 

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
__IO uint32_t Capture = 0;
extern __IO uint16_t CaptureNumber ;
extern __IO uint32_t TMR3Freq ;

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
{
  Sys_Counter++;
}

/******************************************************************************/
/*            AT32F403A Peripherals Interrupt Handlers                        */
/******************************************************************************/
/**
  * @brief  This function handles TMR3 global interrupt request.
  * @param  None
  * @retval None
  */
void TMR3_GLOBAL_IRQHandler(void)
{ 
  if(TMR_GetINTStatus(TMR3, TMR_INT_CC2) == SET) 
  {
    /* Clear TMR3 Capture compare interrupt pending bit */
    TMR_ClearITPendingBit(TMR3, TMR_INT_CC2);
    if(CaptureNumber == 0)
    {
      /* Get the Input Capture value */
      IC3ReadValue1 = TMR_GetCapture2(TMR3);
      CaptureNumber = 1;
    }
    else if(CaptureNumber == 1)
    {
      /* Get the Input Capture value */
      IC3ReadValue2 = TMR_GetCapture2(TMR3); 
      
      /* Capture computation */
      if (IC3ReadValue2 > IC3ReadValue1)
      {
        Capture = (IC3ReadValue2 - IC3ReadValue1); 
      }
      else
      {
        Capture = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2); 
      }
      /* Frequency computation */ 
      TMR3Freq = (uint32_t) SystemCoreClock / Capture;
      CaptureNumber = 0;
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

