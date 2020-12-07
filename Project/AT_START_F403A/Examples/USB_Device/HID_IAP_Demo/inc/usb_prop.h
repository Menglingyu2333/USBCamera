/**
  ******************************************************************************
  * @file    USB_Device/HID_IAP_Demo/inc/usb_prop.h
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   All processing related HID demo (Endpoint 0)
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_PROP_H
#define __USB_PROP_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
typedef enum _HID_REQUESTS
{
  GET_REPORT = 1,
  GET_IDLE,
  GET_PROTOCOL,

  SET_REPORT = 9,
  SET_IDLE,
  SET_PROTOCOL
} HID_REQUESTS;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void IapHID_init(void);
void IapHID_Reset(void);
void IapHID_SetConfiguration(void);
void IapHID_SetDeviceAddress (void);
void IapHID_Status_In (void);
void IapHID_Status_Out (void);
RESULT IapHID_Data_Setup(uint8_t);
RESULT IapHID_NoData_Setup(uint8_t);
RESULT IapHID_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *IapHID_GetDeviceDescriptor(uint16_t );
uint8_t *IapHID_GetConfigDescriptor(uint16_t);
uint8_t *IapHID_GetStringDescriptor(uint16_t);
RESULT IapHID_SetProtocol(void);
uint8_t *IapHID_GetProtocolValue(uint16_t Length);
RESULT IapHID_SetProtocol(void);
uint8_t *IapHID_GetReportDescriptor(uint16_t Length);
uint8_t *IapHID_GetHIDDescriptor(uint16_t Length);
RESULT IapHID_SetIDLE_Feature(void);


/* Exported define -----------------------------------------------------------*/
#define IapHID_GetConfiguration          NOP_Process
//#define IapHID_SetConfiguration          NOP_Process
#define IapHID_GetInterface              NOP_Process
#define IapHID_SetInterface              NOP_Process
#define IapHID_GetStatus                 NOP_Process
#define IapHID_ClearFeature              NOP_Process
#define IapHID_SetEndPointFeature        NOP_Process
#define IapHID_SetDeviceFeature          NOP_Process
//#define IapHID_SetDeviceAddress          NOP_Process

#define REPORT_DESCRIPTOR                  0x22


#endif /* __USB_PROP_H */


