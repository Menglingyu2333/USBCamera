/**
  ******************************************************************************
  * @file    USB_Device/USB_Audio_Demo/src/usb_endp.c
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
#include "at32_audio_conf.h"


/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup USB_Audio_Demo
  * @{
  */ 
  
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
uint8_t u8EndPBuffer[288];
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : EP2_OUT_Callback
* Description    : Endpoint 2 out callback routine. This Speaker data from PC to device
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP2_OUT_Callback(void)
{
  uint16_t Data_Len;       /* data length*/
  if (GetENDPOINT(ENDP2) & EP_DTOG_RX)
  {
    /*read from ENDP1_BUF0Addr buffer*/
    Data_Len = GetEPDblBuf0Count(ENDP2);
    PMAToUserBufferCopy(u8EndPBuffer, ENDP2_BUF0Addr, Data_Len);
  }
  else
  {
    /*read from ENDP1_BUF1Addr buffer*/
    Data_Len = GetEPDblBuf1Count(ENDP2);
    PMAToUserBufferCopy(u8EndPBuffer, ENDP2_BUF1Addr, Data_Len);
  }
  Audio_WriteDataToSpeakerBuffer(u8EndPBuffer, Data_Len);
  FreeUserBuffer(ENDP2, EP_DBUF_OUT);
    
}

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    : Endpoint 1 IN callback routine. This is Microphone send data to PC host
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP1_IN_Callback(void)
{
  uint8_t *u8pBuffer = Audio_GetMicrophoneBuffer();
  uint32_t u32SendLen = Audio_GetMicrophoneLen();
  if ( u8pBuffer == 0 )
      return ;
    
  if (GetENDPOINT(ENDP1) & EP_DTOG_RX)
  {
    /*Write to ENDP1_BUF0Addr buffer*/
    UserToPMABufferCopy(u8pBuffer, ENDP1_BUF0Addr, u32SendLen);
    SetEPDblBuf0Count(ENDP1, EP_DBUF_IN, u32SendLen);
  }
  else
  {
    /*Write to  ENDP1_BUF1Addr buffer*/
    UserToPMABufferCopy(u8pBuffer, ENDP1_BUF1Addr, u32SendLen);
    SetEPDblBuf1Count(ENDP1, EP_DBUF_IN, u32SendLen);
  }
  FreeUserBuffer(ENDP1, EP_DBUF_IN);
    
}

/*******************************************************************************
* Function Name  : EP1_IN_Callback
* Description    : Endpoint 1 IN callback routine. This is Microphone send data to PC host
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void EP3_IN_Callback(void)
{
  uint8_t *u8pBuffer = Audio_GetSpeakerFeedback();
  uint32_t u32SendLen = 3;
//  if ( u8pBuffer == 0 )
//      return ;
//    
  if (GetENDPOINT(ENDP3) & EP_DTOG_RX)
  {
    /*read from ENDP1_BUF0Addr buffer*/
    UserToPMABufferCopy(u8pBuffer, ENDP3_BUF0Addr, u32SendLen);
    SetEPDblBuf0Count(ENDP3, EP_DBUF_IN, u32SendLen);
  }
  else
  {
    /*read from ENDP3_BUF1Addr buffer*/
    UserToPMABufferCopy(u8pBuffer, ENDP3_BUF1Addr, u32SendLen);
    SetEPDblBuf1Count(ENDP3, EP_DBUF_IN, u32SendLen);
  }
  FreeUserBuffer(ENDP3, EP_DBUF_IN);
    
}
/**
  * @}
  */

/**
  * @}
  */