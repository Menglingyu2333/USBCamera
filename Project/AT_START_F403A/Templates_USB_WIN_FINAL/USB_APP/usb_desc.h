/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : usb_desc.h
* Author             : MCD Application Team
* Version            : V2.2.1
* Date               : 09/22/2008
* Description        : Descriptor Header for Virtual COM Port Device
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USB_DESC_H
#define __USB_DESC_H

/* Includes ------------------------------------------------------------------*/

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
#define   UNCOMPRESS        0

#define MAKE_WORD(x)        (u8)((x)&0xFF),(u8)(((x)>>8)&0xFF)
#define MAKE_DWORD(x)       (u8)((x)&0xFF),(u8)(((x)>>8)&0xFF),(u8)(((x)>>16)&0xFF),(u8)(((x)>>24)&0xFF)


/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define USB_ASSOCIATION_DESCRIPTOR_TYPE         0x0B

#define IMG_MJPG_FRAMERATE      10           //Ԥ����MJPEG��Ƶ֡��

#define PACKET_SIZE                             0xB0        //176
#define MIN_BIT_RATE                        (20*1024*IMG_MJPG_FRAMERATE)
#define MAX_BIT_RATE                        (80*1024*IMG_MJPG_FRAMERATE)

#define MAX_FRAME_SIZE          (40*1024*2)      //���ÿ֡JPEG Byte������ӦHostҪ���Buffer Size

#define FRAME_INTERVEL          (10000000ul/IMG_MJPG_FRAMERATE)     //֡����ʱ�䣬��λ100ns

#define CAMERA_SIZ_CONFIG_DESC                  144         //!!

#define CAMERA_SIZ_DEVICE_DESC                  18
#define CAMERA_SIZ_STRING_LANGID                4
#define CAMERA_SIZ_STRING_VENDOR                38
#define CAMERA_SIZ_STRING_PRODUCT               50
#define CAMERA_SIZ_STRING_SERIAL                26

#define STANDARD_ENDPOINT_DESC_SIZE             0x09

/* Exported functions ------------------------------------------------------- */
extern const u8 Camera_DeviceDescriptor[CAMERA_SIZ_DEVICE_DESC];
extern const u8 Camera_ConfigDescriptor[CAMERA_SIZ_CONFIG_DESC];
extern const u8 Camera_StringLangID[CAMERA_SIZ_STRING_LANGID];
extern const u8 Camera_StringVendor[CAMERA_SIZ_STRING_VENDOR];
extern const u8 Camera_StringProduct[CAMERA_SIZ_STRING_PRODUCT];
extern u8 Camera_StringSerial[CAMERA_SIZ_STRING_SERIAL];

#endif /* __USB_DESC_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
