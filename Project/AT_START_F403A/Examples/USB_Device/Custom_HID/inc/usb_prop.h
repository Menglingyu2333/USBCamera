/**
  ******************************************************************************
  * @file    USB_Device/Custom_HID/inc/usb_prop.h
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
void CustomHID_init(void);
void CustomHID_Reset(void);
void CustomHID_SetConfiguration(void);
void CustomHID_SetDeviceAddress (void);
void CustomHID_Status_In (void);
void CustomHID_Status_Out (void);
RESULT CustomHID_Data_Setup(uint8_t);
RESULT CustomHID_NoData_Setup(uint8_t);
RESULT CustomHID_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *CustomHID_GetDeviceDescriptor(uint16_t );
uint8_t *CustomHID_GetConfigDescriptor(uint16_t);
uint8_t *CustomHID_GetStringDescriptor(uint16_t);
RESULT CustomHID_SetProtocol(void);
uint8_t *CustomHID_GetProtocolValue(uint16_t Length);
RESULT CustomHID_SetProtocol(void);
uint8_t *CustomHID_GetReportDescriptor(uint16_t Length);
uint8_t *CustomHID_GetHIDDescriptor(uint16_t Length);
RESULT CustomHID_SetIDLE_Feature(void);

/* Exported define -----------------------------------------------------------*/
#define CustomHID_GetConfiguration          NOP_Process
//#define CustomHID_SetConfiguration          NOP_Process
#define CustomHID_GetInterface              NOP_Process
#define CustomHID_SetInterface              NOP_Process
#define CustomHID_GetStatus                 NOP_Process
#define CustomHID_ClearFeature              NOP_Process
#define CustomHID_SetEndPointFeature        NOP_Process
#define CustomHID_SetDeviceFeature          NOP_Process
//#define CustomHID_SetDeviceAddress          NOP_Process

#define REPORT_DESCRIPTOR                  0x22


#endif /* __USB_PROP_H */


