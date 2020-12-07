/**
  ******************************************************************************
  * @file    USB_Device/HID_IAP_Demo/src/usb_endp.c
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
#include "usb_istr.h"
#include "iap_user.h"
#include "at32_board.h"
/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup USB_HID_IAP_Demo
  * @{
  */ 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
uint8_t Receive_Buffer[2];
extern __IO uint8_t PrevXferComplete;
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/**
  * @brief  EP2 OUT Callback Routine, rx data from host use endpoint 2
  * @param  None
  * @retval None.
  */
void EP2_OUT_Callback(void)
{
  uint32_t data_len = 0;
  /* Read received data (n bytes) */  
  data_len = USB_SIL_Read(EP2_OUT, IAP_Info.IAP_Rx_Buffer);
  if (data_len > 2 )
      IAP_Command_Parse(IAP_Info.IAP_Rx_Buffer, data_len);
  SetEPRxStatus(ENDP2, EP_RX_VALID);
 
}

/**
  * @brief  EP1 IN Callback, Tx data complete
  * @param  None
  * @retval None.
  */
void EP1_IN_Callback(void)
{
  IAP_Info.IAP_Tx_status = 0;
  if ( IAP_Info.IAP_Step == IAP_JMP_PRE )
  {
      IAP_Info.IAP_Step = IAP_JUMP;
  }
}

/**
  * @brief  EP1 respond data to host
  * @param  res_buf: respond data pointer
  * @param  len: respond data len
  * @retval None.
  */
void EP1_Respond_Data(uint8_t *res_buf, uint32_t len)
{
  USB_SIL_Write(EP1_IN, res_buf, len);
  SetEPTxStatus(ENDP1, EP_TX_VALID);
}

/**
  * @}
  */ 

/**
  * @}
  */
 

