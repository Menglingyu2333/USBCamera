/**
  ******************************************************************************
  * @file    USB_Device/HID_IAP_Demo/inc/usb_desc.h
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Descriptor Header for HID IAP Demo
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
#define IAPHID_SIZ_HID_DESC                  0x09
#define IAPHID_OFF_HID_DESC                  0x12

#define IAPHID_SIZ_DEVICE_DESC               18
#define IAPHID_SIZ_CONFIG_DESC               41
#define IAPHID_SIZ_REPORT_DESC               32
#define IAPHID_SIZ_STRING_LANGID             4
#define IAPHID_SIZ_STRING_VENDOR             36
#define IAPHID_SIZ_STRING_PRODUCT            30
#define IAPHID_SIZ_STRING_SERIAL             26

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

/*USB Vendor ID*/
#define USB_VENDER_ID                           0x2E3C
#define USB_PRODUCT_ID                          0xAF01
//#define USB_VENDER_ID                           0x0483
//#define USB_PRODUCT_ID                          0x5750
//#define USB_VENDER_ID                           0xC251
//#define USB_PRODUCT_ID                          0xF001

/* Exported functions ------------------------------------------------------- */
extern const uint8_t IapHID_DeviceDescriptor[IAPHID_SIZ_DEVICE_DESC];
extern const uint8_t IapHID_ConfigDescriptor[IAPHID_SIZ_CONFIG_DESC];
extern const uint8_t IapHID_ReportDescriptor[IAPHID_SIZ_REPORT_DESC];
extern const uint8_t IapHID_StringLangID[IAPHID_SIZ_STRING_LANGID];
extern const uint8_t IapHID_StringVendor[IAPHID_SIZ_STRING_VENDOR];
extern const uint8_t IapHID_StringProduct[IAPHID_SIZ_STRING_PRODUCT];
extern uint8_t IapHID_StringSerial[IAPHID_SIZ_STRING_SERIAL];



#endif /* __USB_DESC_H */


