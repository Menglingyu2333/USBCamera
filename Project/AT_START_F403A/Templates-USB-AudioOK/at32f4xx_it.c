/**
 **************************************************************************
 * @file      : EXTI/EXTI_Config/at32f4xx_it.c
 * @author    : Artery Technology
 * @version   : V1.0.4
 * @date      : 2018-12-28
 * @brief     : Main Interrupt Service Routines.
 *              This file provides template for all exceptions handler and peripherals
 *              interrupt service routine.
 **************************************************************************
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
#include "at32f4xx_it.h"
#include "at32_board.h"

#include "ov2640.h"
#include "lcd.h"

#include "usb_istr.h"
#include "usb_int.h"


/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup EXTI_Config
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
}

/**
  * @brief  This function handles External line 0 interrupt request.
  * @param  None
  * @retval None
  * trigger when frame start
  */
void EXTI1_IRQHandler(void)
{
	if(EXTI_GetIntStatus(EXTI_Line1) != RESET)
	{
    YSYNC_FLAG = 1;
    HREF_FLAG = 0;
    ImagePtr_H = 0;
    ImagePtr_W = 0;
  	LCD_SetCursor(0, 0);
		EXTI_ClearIntPendingBit(EXTI_Line1);  ///<Clear the  EXTI line 0 pending bit
	}
}
// trigger when line start
void EXTI2_IRQHandler(void)
{
	if(EXTI_GetIntStatus(EXTI_Line2) != RESET)
	{
    HREF_FLAG = 1;
    ImagePtr_W = 0;
		EXTI_ClearIntPendingBit(EXTI_Line2);  ///<Clear the  EXTI line 0 pending bit
	}
}

// trigger byte line start
void EXTI3_IRQHandler(void)
{
	if(EXTI_GetIntStatus(EXTI_Line3) != RESET)
	{
    AT32_LEDn_Toggle(LED3);
//    AT32_LEDn_OFF(LED3);
    if(YSYNC_FLAG == 1 && HREF_FLAG == 1)
    {
			ImageData(ImagePtr_H, ImagePtr_W) = OV2640_DATA;

			LCD_RS_SET;
			LCD_CS_CLR;
			DATAOUT(yuv_8_to_RGBGray(ImageData(ImagePtr_H, ImagePtr_W)));
//			DATAOUT(ImageData(ImagePtr_H, ImagePtr_W));
			LCD_WR_CLR;
			LCD_WR_SET;
			LCD_CS_SET;
      ImagePtr_W++;

      if(ImagePtr_W >= ImageWidth)
      {
        ImagePtr_W = 0;
        HREF_FLAG = 0;
        ImagePtr_H++;
        if(ImagePtr_H >= ImageHeight)
        {
          ImagePtr_H = 0;
          YSYNC_FLAG = 0;
        }
      }
    }
//    AT32_LEDn_ON(LED3);
		EXTI_ClearIntPendingBit(EXTI_Line3);  ///<Clear the  EXTI line 0 pending bit
	}
}

/**
  * @brief  This function handles External lines 9 to 5 interrupt request.
  * @param  None
  * @retval None
  */
//void EXTI9_5_IRQHandler(void)
//{
//	if(EXTI_GetIntStatus(EXTI_Line9) != RESET)
//	{
//		AT32_LEDn_Toggle(LED4);             ///<Toggle LED4
//		EXTI_ClearIntPendingBit(EXTI_Line9);///<Clear the  EXTI line 9 pending bit
//	}
//}

/**
  * @}
  */

/**
  * @}
  */

/**
  * @brief  This function handles USB High Priority or CAN TX interrupts
  *         requests.
  * @param  None
  * @retval None
  */
void USB_HP_CAN1_TX_IRQHandler(void)
{
  CTR_HP();
}

/**
  * @brief  This function handles USB Low Priority or CAN RX0 interrupts
  *         requests.
  * @param  None
  * @retval None
  */
void USB_LP_CAN1_RX0_IRQHandler (void)
{
  USB_Istr();
}

/**
  * @brief  This function handles USB WakeUp interrupt request.
  * @param  None
  * @retval None
  */
void USBWakeUp_IRQHandler(void)
{
    EXTI_ClearIntPendingBit(EXTI_Line18);
}
/**
  * @brief  This function handles ACC interrupt request.
  * @param  None
  * @retval None
  */
void ACC_IRQHandler(void)
{
  if(ACC_GetFlagStatus(ACC_FLAG_CALRDY) == SET)
  {
    /*Claer ACC Calibration ready flag*/
    ACC_ClearFlag(ACC_FLAG_CALRDY);
  }
  if(ACC_GetFlagStatus(ACC_FLAG_RSLOST) == SET)
  {
    /*Claer ACC Reference Signal Lost flag*/
    ACC_ClearFlag(ACC_FLAG_RSLOST);
  }
}

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
