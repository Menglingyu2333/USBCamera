/**
  ******************************************************************************
  * @file    USB_Device/MassStorage/src/usb_endp.c
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Endpoint routines
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
#include "usb_lib.h"
#include "usb_bot.h"
#include "usb_istr.h"
/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup USB_MassStorage
  * @{
  */ 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  EP1 IN Callback Routine.
  * @param  None.
  * @retval None.
  */
void EP1_IN_Callback(void)
{
  Mass_Storage_In();
}

/**
  * @brief  EP2 OUT Callback Routine.
  * @param  None.
  * @retval None.
  */
void EP2_OUT_Callback(void)
{
  Mass_Storage_Out();
}
/**
  * @}
  */

/**
  * @}
  */
  

