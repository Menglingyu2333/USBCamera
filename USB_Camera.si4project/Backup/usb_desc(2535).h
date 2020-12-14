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

//UVC relevant define

#define  USB_LEN_DEV_QUALIFIER_DESC                     0x0A
#define  USB_LEN_DEV_DESC                               0x12
#define  USB_LEN_CFG_DESC                               0x09
#define  USB_LEN_IF_DESC                                0x09
#define  USB_LEN_EP_DESC                                0x07
#define  USB_LEN_OTG_DESC                               0x03
#define  USB_LEN_LANGID_STR_DESC                        0x04
#define  USB_LEN_OTHER_SPEED_DESC_SIZ                   0x09

#define  USBD_IDX_LANGID_STR                            0x00
#define  USBD_IDX_MFC_STR                               0x01
#define  USBD_IDX_PRODUCT_STR                           0x02
#define  USBD_IDX_SERIAL_STR                            0x03
#define  USBD_IDX_CONFIG_STR                            0x04
#define  USBD_IDX_INTERFACE_STR                         0x05

#define  USB_REQ_TYPE_STANDARD                          0x00
#define  USB_REQ_TYPE_CLASS                             0x20
#define  USB_REQ_TYPE_VENDOR                            0x40
#define  USB_REQ_TYPE_MASK                              0x60

#define  USB_REQ_RECIPIENT_DEVICE                       0x00
#define  USB_REQ_RECIPIENT_INTERFACE                    0x01
#define  USB_REQ_RECIPIENT_ENDPOINT                     0x02
#define  USB_REQ_RECIPIENT_MASK                         0x03

#define  USB_REQ_GET_STATUS                             0x00
#define  USB_REQ_CLEAR_FEATURE                          0x01
#define  USB_REQ_SET_FEATURE                            0x03
#define  USB_REQ_SET_ADDRESS                            0x05
#define  USB_REQ_GET_DESCRIPTOR                         0x06
#define  USB_REQ_SET_DESCRIPTOR                         0x07
#define  USB_REQ_GET_CONFIGURATION                      0x08
#define  USB_REQ_SET_CONFIGURATION                      0x09
#define  USB_REQ_GET_INTERFACE                          0x0A
#define  USB_REQ_SET_INTERFACE                          0x0B
#define  USB_REQ_SYNCH_FRAME                            0x0C

#define  USB_DESC_TYPE_DEVICE                              1
#define  USB_DESC_TYPE_CONFIGURATION                       2
#define  USB_DESC_TYPE_STRING                              3
#define  USB_DESC_TYPE_INTERFACE                           4
#define  USB_DESC_TYPE_ENDPOINT                            5
#define  USB_DESC_TYPE_DEVICE_QUALIFIER                    6
#define  USB_DESC_TYPE_OTHER_SPEED_CONFIGURATION           7
#define  USB_DESC_TYPE_INTERFACE_ASSOCIATION               11
#define  USB_DESC_TYPE_BOS                                 0x0F

#define USB_CONFIG_REMOTE_WAKEUP                           0xA0
#define USB_CONFIG_SELF_POWERED                            0xC0
#define USB_CONFIG_BUS_POWERED                             0x80

 // Video Interface Class Codes
 // (USB_Video_Class_1.1.pdf, A.1 Video Interface Class Code)
 #define CC_VIDEO                                   0x0E

 // Video Interface Subclass Codes
 // (USB_Video_Class_1.1.pdf, A.2 Video Interface Subclass Code)
 #define SC_UNDEFINED                               0x00
 #define SC_VIDEOCONTROL                            0x01
 #define SC_VIDEOSTREAMING                          0x02
 #define SC_VIDEO_INTERFACE_COLLECTION              0x03

 // Video Interface Protocol Codes
 // (USB_Video_Class_1.1.pdf, A.3 Video Interface Protocol Codes)
 #define PC_PROTOCOL_UNDEFINED                      0x00
 #define PC_PROTOCOL_15                             0x01
 // Video Class-Specific Descriptor Types
 // (USB_Video_Class_1.1.pdf, A.4 Video Class-Specific Descriptor Types)
 #define CS_UNDEFINED                               0x20
 #define CS_DEVICE                                  0x21
 #define CS_CONFIGURATION                           0x22
 #define CS_STRING                                  0x23
 #define CS_INTERFACE                               0x24
 #define CS_ENDPOINT                                0x25

 // Video Class-Specific VideoControl Interface Descriptor Subtypes
 // (USB_Video_Class_1.1.pdf, A.5 Video Class-Specific VC Interface Descriptor Subtypes)
 #define VC_DESCRIPTOR_UNDEFINED                    0x00
 #define VC_HEADER                                  0x01
 #define VC_INPUT_TERMINAL                          0x02
 #define VC_OUTPUT_TERMINAL                         0x03
 #define VC_SELECTOR_UNIT                           0x04
 #define VC_PROCESSING_UNIT                         0x05
 #define VC_EXTENSION_UNIT                          0x06

 // Video Class-Specific VideoStreaming Interface Descriptor Subtypes
 // (USB_Video_Class_1.1.pdf, A.6 Video Class-Specific VS Interface Descriptor Subtypes)
 #define VS_UNDEFINED                               0x00
 #define VS_INPUT_HEADER                            0x01
 #define VS_OUTPUT_HEADER                           0x02
 #define VS_STILL_IMAGE_FRAME                       0x03
 #define VS_FORMAT_UNCOMPRESSED                     0x04
 #define VS_FRAME_UNCOMPRESSED                      0x05
 #define VS_FORMAT_MJPEG                            0x06
 #define VS_FRAME_MJPEG                             0x07
 #define VS_FORMAT_MPEG2TS                          0x0A
 #define VS_FORMAT_DV                               0x0C
 #define VS_COLORFORMAT                             0x0D
 #define VS_FORMAT_FRAME_BASED                      0x10
 #define VS_FRAME_FRAME_BASED                       0x11
 #define VS_FORMAT_STREAM_BASED                     0x12



 // Video Class-Specific Request Codes
 // (USB_Video_Class_1.1.pdf, A.8 Video Class-Specific Request Codes)
 #define RC_UNDEFINED                               0x00
 #define UVC_SET_CUR                                0x01
 #define UVC_GET_CUR                                0x81
 #define UVC_GET_MIN                                0x82
 #define UVC_GET_MAX                                0x83
 #define UVC_GET_RES                                0x84
 #define UVC_GET_LEN                                0x85
 #define UVC_GET_INFO                               0x86
 #define UVC_GET_DEF                                0x87



 // USB Terminal Types
 // (USB_Video_Class_1.1.pdf, B.1 USB Terminal Types)
 #define TT_VENDOR_SPECIFIC         	             0x0100
 #define TT_STREAMING               	             0x0101

 // Input Terminal Types
 // (USB_Video_Class_1.1.pdf, B.2 Input Terminal Types)
 #define ITT_VENDOR_SPECIFIC                      0x0200
 #define ITT_CAMERA                               0x0201
 #define ITT_MEDIA_TRANSPORT_INPUT                0x0202

 // Output Terminal Types
 // (USB_Video_Class_1.1.pdf, B.3 Output Terminal Types)
 #define OTT_VENDOR_SPECIFIC                      0x0300
 #define OTT_DISPLAY                              0x0301
 #define OTT_MEDIA_TRANSPORT_OUTPUT               0x0302

 // External Terminal Types
 // (USB_Video_Class_1.1.pdf, B.4 External Terminal Types)
 #define EXTERNAL_VENDOR_SPECIFIC                 0x0400
 #define COMPOSITE_CONNECTOR                      0x0401
 #define SVIDEO_CONNECTOR                         0x0402
 #define COMPONENT_CONNECTOR                      0x0403

#define MAKE_WORD(x) (x & 0xFF),((x >> 8) & 0xFF)
#define MAKE_DWORD(x) (x & 0xFF),((x >> 8) & 0xFF),((x >> 16) & 0xFF),((x >> 24) & 0xFF)

#define WBVAL(x) (x & 0xFF),((x >> 8) & 0xFF)
#define DBVAL(x) (x & 0xFF),((x >> 8) & 0xFF),((x >> 16) & 0xFF),((x >> 24) & 0xFF)
/* bMaxPower in Configuration Descriptor */
#define USB_CONFIG_POWER_MA(mA)                ((mA)/2)
/* bEndpointAddress in Endpoint Descriptor */
#define USB_ENDPOINT_DIRECTION_MASK            0x80
#define USB_ENDPOINT_OUT(addr)                 ((addr) | 0x00)
#define USB_ENDPOINT_IN(addr)                  ((addr) | 0x80)

#define UVC_STATE_OFF 0
#define UVC_STATE_READY 1
#define UVC_STATE_NEED_FRAME 2
#define UVC_STATE_BUSY 3

#define UVC_USR_STR_CONFIG 0x11
#define UVC_USR_STR_IAD 0x12
#define UVC_USR_STR_VC_ITF 0x13
#define UVC_USR_STR_VC_IT 0x14
#define UVC_USR_STR_VC_OT 0x15
#define UVC_USR_STR_VS_ITF_AS0 0x16
#define UVC_USR_STR_VS_ITF_AS1 0x17

/* Exported define -----------------------------------------------------------*/
#define USB_DEVICE_DESCRIPTOR_TYPE              0x01
#define USB_CONFIGURATION_DESCRIPTOR_TYPE       0x02
#define USB_STRING_DESCRIPTOR_TYPE              0x03
#define USB_INTERFACE_DESCRIPTOR_TYPE           0x04
#define USB_ENDPOINT_DESCRIPTOR_TYPE            0x05

#define USB_ASSOCIATION_DESCRIPTOR_TYPE         0x0B

#define IMG_MJPG_FRAMERATE      10           //预定义MJPEG视频帧率

#define PACKET_SIZE                             0xB0        //176
#define MIN_BIT_RATE                        0x000DEC00//(20*1024*IMG_MJPG_FRAMERATE)
#define MAX_BIT_RATE                        0x000DEC00//(80*1024*IMG_MJPG_FRAMERATE)

#define MAX_FRAME_SIZE          (40*1024)      //最大每帧JPEG Byte数，对应Host要求的Buffer Size

#define FRAME_INTERVEL          (10000000ul/IMG_MJPG_FRAMERATE)     //帧间间隔时间，单位100ns

#define CAMERA_SIZ_CONFIG_DESC                  USB_UVC_CONFIG_DESC_TOTAL_SIZE         //!!

#define CAMERA_SIZ_DEVICE_DESC                  18
#define CAMERA_SIZ_STRING_LANGID                4
#define CAMERA_SIZ_STRING_VENDOR                38
#define CAMERA_SIZ_STRING_PRODUCT               50
#define CAMERA_SIZ_STRING_SERIAL                26

//#define VC_DESC_TOTAL_SIZE ((12+1)+(15+2)+8+9)
////#define VS_DESC_TOTAL_SIZE ((13+1*1)+27+30+6)
//#define VS_DESC_TOTAL_SIZE ((13+1*1)+11+38)
//#define USB_UVC_CONFIG_DESC_TOTAL_SIZE (9+8+9+VC_DESC_TOTAL_SIZE+9+VS_DESC_TOTAL_SIZE+9+7)

#define VC_DESC_TOTAL_SIZE ((12+1)+(15+2)+9)
//#define VC_DESC_TOTAL_SIZE ((12+1)+8+9)
#define VS_DESC_TOTAL_SIZE ((13+1*1)+11+38)
//#define VS_DESC_TOTAL_SIZE ((13+1*1)+27+30+6)
#define USB_UVC_CONFIG_DESC_TOTAL_SIZE (9+8+9+VC_DESC_TOTAL_SIZE+9+VS_DESC_TOTAL_SIZE+9+7)

/* Exported functions ------------------------------------------------------- */
extern const u8 Camera_DeviceDescriptor[CAMERA_SIZ_DEVICE_DESC];
//extern const u8 Camera_ConfigDescriptor[CAMERA_SIZ_CONFIG_DESC];
extern const u8 Camera_ConfigDescriptor[USB_UVC_CONFIG_DESC_TOTAL_SIZE];
extern const u8 Camera_StringLangID[CAMERA_SIZ_STRING_LANGID];
extern const u8 Camera_StringVendor[CAMERA_SIZ_STRING_VENDOR];
extern const u8 Camera_StringProduct[CAMERA_SIZ_STRING_PRODUCT];
extern u8 Camera_StringSerial[CAMERA_SIZ_STRING_SERIAL];

#endif /* __USB_DESC_H */
/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
