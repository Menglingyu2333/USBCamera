/**
  ******************************************************************************
  * @file    USB_Device/Composiet_Device/inc/usb_desc.h
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Descriptor Header for Composite Device Demo
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
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define HID_DESCRIPTOR_TYPE                     0x21
#define Composite_DATA_SIZE                     64
#define Composite_INT_SIZE                      8

#define MOUSE_SIZ_HID_DESC                      9
#define MOUSE_OFF_HID_DESC                      92
#define MOUSE_SIZ_REPORT_DESC                   74
#define Composite_SIZ_DEVICE_DESC               18
#define Composite_SIZ_CONFIG_DESC               108
#define Composite_SIZ_STRING_LANGID             4
#define Composite_SIZ_STRING_VENDOR             36
#define Composite_SIZ_STRING_PRODUCT            44
#define Composite_SIZ_STRING_SERIAL             26

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

/* Exported functions ------------------------------------------------------- */
extern const uint8_t Composite_DeviceDescriptor[Composite_SIZ_DEVICE_DESC];
extern const uint8_t Composite_ConfigDescriptor[Composite_SIZ_CONFIG_DESC];

extern const uint8_t MOUSE_ReportDescriptor[MOUSE_SIZ_REPORT_DESC];
extern const uint8_t Composite_StringLangID[Composite_SIZ_STRING_LANGID];
extern const uint8_t Composite_StringVendor[Composite_SIZ_STRING_VENDOR];
extern const uint8_t Composite_StringProduct[Composite_SIZ_STRING_PRODUCT];
extern uint8_t Composite_StringSerial[Composite_SIZ_STRING_SERIAL];


#endif /* __USB_DESC_H */


