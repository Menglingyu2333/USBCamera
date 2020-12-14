/******************** xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx ********************
* File Name          : usb_desc.c
* Author             :
* Version            :
* Date               :
* Description        : Usb Camera Descriptors
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "usb_lib.h"
#include "usb_desc.h"
#include "ov2640.h"

/* USB Standard Device Descriptor */
const u8 Camera_DeviceDescriptor[] =
  {
    CAMERA_SIZ_DEVICE_DESC,               /* bLength */
    USB_DEVICE_DESCRIPTOR_TYPE,           /* bDescriptorType */
    WBVAL(0x0200),                        /* bcdUSB 2.00 */
    0xEF,                                 /* bDeviceClass */
    0x02,                                 /* bDeviceSubClass */
    0x01,                                 /* bDeviceProtocol */
    0x40,                                 /* bMaxPacketSize 40 */
    WBVAL(0x2020),                        /* idVendor */
    WBVAL(0x2121),                        /* idProduct */
    WBVAL(0x0100),                        /* bcdDevice */
    1,                                    /* iManufacturer */
    2,                                    /* iProduct */
    3,                                    /* iSerialNumber */
    0x01                                  /* bNumConfigurations */
  };

const u8 Camera_ConfigDescriptor[] =
{
  /* Configuration 1 */
  9,   /* bLength: Configuation Descriptor size */
  USB_DESC_TYPE_CONFIGURATION,   /* bDescriptorType: Configuration */
  WBVAL(USB_UVC_CONFIG_DESC_TOTAL_SIZE), // wTotalLength
  2,   /* bNumInterfaces: 2 interfaces */
  0x01,   /* bConfigurationValue: Configuration's Id */
  0x00,//UVC_USR_STR_CONFIG,   /* iConfiguration: */
  USB_CONFIG_BUS_POWERED,   /* bmAttributes: */
  USB_CONFIG_POWER_MA(500),   /* bMaxPower: 500 mA */

  /* Interface Association Descriptor */
  8, /* bLength */
  USB_DESC_TYPE_INTERFACE_ASSOCIATION, /* bDescriptorType */
  0x00, /* bFirstInterface */
  2, /* bInterfaceCount */
  CC_VIDEO,                      /* bFunctionClass     */
  SC_VIDEO_INTERFACE_COLLECTION, /* bFunctionSubClass  */
  PC_PROTOCOL_UNDEFINED,         /* bInterfaceProtocol */
  0x02,//UVC_USR_STR_IAD, /* iFunction */

  /* VideoControl Interface Descriptor */

  /* Standard VC Interface Descriptor = interface 0 */
  9, /* bLength */
  USB_DESC_TYPE_INTERFACE, /* bDescriptorType */
  0x00, /* bInterfaceNumber */
  0x00, /* bAlternateSetting */
  0x00, /* bNumEndpoints */
  CC_VIDEO, /* bInterfaceClass */
  SC_VIDEOCONTROL, /* bInterfaceSubClass */
  PC_PROTOCOL_UNDEFINED, /* bInterfaceProtocol */
  0x02,//UVC_USR_STR_VC_ITF, /* iInterface */

  /* Class-specific VC Interface Descriptor */
  12+1, /* bLength */
  CS_INTERFACE, /* bDescriptorType */
  VC_HEADER, /* bDescriptorSubType */
  WBVAL(0x0100), /* bcdVDC: UVC 1.0 */
  WBVAL(VC_DESC_TOTAL_SIZE), /* wTotalLength */
  DBVAL(6000000), /* dwClockFrequency: 6.000000 MHz - why 6MHz? - don't know */
  1, /* bInCollection */
  1, /* baInterfaceNr(1) */

  /* Camera Terminal Descriptor */
  15+2, /* bLength */
  CS_INTERFACE, /* bDescriptorType */
  VC_INPUT_TERMINAL, /* bDescriptorSubtype */
  0x01, /* bTerminalID */
  WBVAL(ITT_CAMERA), /* wTerminalType */
  0x00, /* bAssocTerminal */
  0x02,//UVC_USR_STR_VC_IT, /* iTerminal */
  WBVAL(0x0000), /* wObjectiveFocalLengthMin */
  WBVAL(0x0000), /* wObjectiveFocalLengthMax */
  WBVAL(0x0000), /* wOcularFocalLength       */
  2,             /* bControlSize */
  0x00, 0x00,    /* bmControls */

  /* 2.2. Video Input Terminal Descriptor (Composite) */
//  0x08,                                 /* bLength */
//  CS_INTERFACE,                         /* bDescriptorType : CS_INTERFACE */
//  VC_INPUT_TERMINAL,                    /* bDescriptorSubType : VC_INPUT_TERMINAL subtype */
//  0x02,                                 /* bTerminalID: ID of this input terminal */
//  0x01, 0x04,                           /* wTerminalType: 0x0401 COMPOSITE_CONNECTOR type. This terminal is the composite connector. */
//  0x00,                                 /* bAssocTerminal: No association */
//  0x00,                                 /* iTerminal: Unused*/

  /* Output Terminal Descriptor */
  9, /* bLength */
  CS_INTERFACE, /* bDescriptorType */
  VC_OUTPUT_TERMINAL, /* bDescriptorSubtype */
  0x03, /* bTerminalID */
  WBVAL(TT_STREAMING), /* wTerminalType */
  0x00, /* bAssocTerminal */
  0x02, /* bSourceID: is connected to terminal 0x01 */
  0x00,//UVC_USR_STR_VC_OT, /* iTerminal */

  /* VideoStreaming Interface Descriptor */

  /* Standard VS Interface Descriptor  = interface 1 */
  // alternate setting 0 = Zero Bandwidth
  9, /* bLength */
  USB_DESC_TYPE_INTERFACE, /* bDescriptorType */
  0x01, /* bInterafaceNumber */
  0x00, /* bAlternateSetting */
  0, /* bNumEndpoints */
  CC_VIDEO, /* bInterafaceClass */
  SC_VIDEOSTREAMING, /* bInterafaceSubClass */
  PC_PROTOCOL_UNDEFINED, /* bInterafaceProtocol */
  0x00,//UVC_USR_STR_VS_ITF_AS0, /* iInterface */

  /* Class-specific VS Interface Input Header Descriptor */
  13+1*1, /* bLength */
  CS_INTERFACE,                              /* bDescriptorType */
  VS_INPUT_HEADER,                           /* bDescriptorSubtype */
  1,                                         /* bNumFormats */
  WBVAL(VS_DESC_TOTAL_SIZE), /* wTotalLength */
  USB_ENDPOINT_IN(1),                        /* bEndPointAddress */
  0x00,                                      /* bmInfo */
  0x03,                                      /* bTerminalLink: it's endpoint is connected to terminal 0x02 */
  0x00,                                      /* bStillCaptureMethod */
  0x00,                                      /* bTriggerSupport */
  0x00,                                      /* bTriggerUsage */
  1,                                         /* bControlSize */
  0x00,                                      /* bmaControls(1) */

  /* 3.2 Class-specific VideoStream Format(MJPEG) Descriptor */
  11,                                   /* bLength */
  CS_INTERFACE,                         /* bDescriptorType : CS_INTERFACE */
  VS_FORMAT_MJPEG,                      /* bDescriptorSubType : VS_FORMAT_MJPEG subtype */
  0x01,                                 /* bFormatIndex : First (and only) format descriptor */
  0x01,                                 /* bNumFrameDescriptors : One frame descriptor for this format follows. */
  0x01,                                 /* bmFlags : Uses fixed size samples.. */
  0x01,                                 /* bDefaultFrameIndex : Default frame index is 1. */
  0x00,                                 /* bAspectRatioX : Non-interlaced stream ¨C not required. */
  0x00,                                 /* bAspectRatioY : Non-interlaced stream ¨C not required. */
  0x00,                                 /* bmInterlaceFlags : Non-interlaced stream */
  0x00,                                 /* bCopyProtect : No restrictions imposed on the duplication of this video stream. */

  /* Uncompressed Video Format Descriptor */
//  27, /* bLength */
//  CS_INTERFACE,                         /* bDescriptorType */
//  VS_FORMAT_UNCOMPRESSED,               /* bDescriptorSubType */
//  0x01,                                 /* bFormatIndex */
//  1,                                    /* bNumFrameDescriptors */
//  0x59,0x55,0x59,0x32,                  /* Giud Format YUY2 {32595559-0000-0010-8000-00AA00389B71} */
//  0x00,0x00,
//  0x10,0x00,
//  0x80,0x00,0x00,0xAA,0x00,0x38,0x9B,0x71,
//  16,                                   /* bBitsPerPixel: 16 for yuy2 */
//  0x01,                                 /* bDefaultFrameIndex: Default frame index is 1 */
//  0x00,                                 /* bAspectRatioX */
//  0x00,                                 /* bAspectRatioY */
//  0x00,                                 /* bmInterlaceFlags */
//  0x00,                                 /* bCopyProtect */
  /* 3.3 Class-specific VideoStream Frame Descriptor */
  38,                                   /* bLength */
  CS_INTERFACE,                                 /* bDescriptorType : CS_INTERFACE */
  VS_FRAME_MJPEG,                                 /* bDescriptorSubType : VS_FRAME_MJPEG */
  0x01,                                 /* bFrameIndex : First (and only) frame descriptor */
  0x02,                                 /* bmCapabilities : Still images using capture method 0 are supported at this frame setting.D1: Fixed frame-rate. */
  WBVAL(ImageWidth),     /* wWidth : Width of frame, pixels. */
  WBVAL(ImageHeight),    /* wHeight : Height of frame, pixels. */
  DBVAL(MIN_BIT_RATE),             /* dwMinBitRate : Min bit rate in bits/s  */
  DBVAL(MAX_BIT_RATE),             /* dwMaxBitRate : Max bit rate in bits/s  */
  DBVAL(MAX_FRAME_SIZE),           /* dwMaxVideoFrameBufSize : Maximum video or still frame size, in bytes. */
  DBVAL(FRAME_INTERVEL),           /* dwDefaultFrame Interval time, unit=100ns */
  0x00,                                 /* bFrameIntervalType : Continuous frame interval */
  DBVAL(FRAME_INTERVEL),           /* dwDefaultFrame Interval time, unit=100ns */
  DBVAL(FRAME_INTERVEL),           /* dwDefaultFrame Interval time, unit=100ns */
  0x00, 0x00, 0x00, 0x00,               /* dwFrameIntervalStep : No frame interval step supported. */

  /* Uncompressed Video Frame Descriptor */
//  30, /* bLength */
//  CS_INTERFACE, /* bDescriptorType */
//  VS_FRAME_UNCOMPRESSED, /* bDscriptorSubtype */
//  0x01, /* bFrameIndex */
//  0x02, /* bmCapabilities: fixed frame-rate */
//  WBVAL(UVC_VIDEO_WIDTH), /* wWidth */
//  WBVAL(UVC_VIDEO_HEIGHT), /* wHeight */
//  DBVAL(UVC_VIDEO_BITRATE), /* dwMinBitRate */
//  DBVAL(UVC_VIDEO_BITRATE), /* dwMaxBitRate */
//  DBVAL(UVC_VIDEO_MAX_FRAME_BUF_SIZE), /* dwMaxVideoFrameBufSize */
//  DBVAL(UVC_VIDEO_INTERVAL), /* dwDefaultFrameInterval: for 5 FPS */
//  1, /* bFrameIntervalType */
//  DBVAL(UVC_VIDEO_INTERVAL), /* dwFrameInterval(1) */

  /* Color Matching Descriptor */
//  6, /* bLength */
//  CS_INTERFACE, /* bDescriptorType */
//  VS_COLORFORMAT, /* bDescriptorSubtype */
//  1, /* bColorPrimaries: 1 - BT.709, sRGB */
//  1, /* bTransferCharacteristics: 1 - BT.709 */
//  4, /* bMatrixCoefficients: 4 - SMPTE 170M */

  /* Standard VS Interface Descriptor  = interface 1 */
  // alternate setting 1 = operational setting
  9, /* bLength */
  USB_DESC_TYPE_INTERFACE, /* bDescriptorType */
  0x01, /* bInterafaceNumber */
  0x01, /* bAlternateSetting */
  1, /* bNumEndpoints */
  CC_VIDEO, /* bInterafaceClass */
  SC_VIDEOSTREAMING, /* bInterafaceSubClass */
  PC_PROTOCOL_UNDEFINED, /* bInterafaceProtocol */
  0x00,//UVC_USR_STR_VS_ITF_AS1, /* iInterface */

  /* Standard VS Isochronous Video Data Endpoint Descriptor */
  7, /* bLength */
  USB_DESC_TYPE_ENDPOINT, /* bDescriptorType */
  USB_ENDPOINT_IN(1), /* bEndpointAddress */
  0x05, /* bmAttributes: 1 - Isochronous, 2 - Bulk */
  WBVAL(PACKET_SIZE), /* wMaxPacketSize */
  0x01 /* bInterval */
};

/* USB String Descriptors */
const u8 Camera_StringLangID[CAMERA_SIZ_STRING_LANGID] =
  {
    CAMERA_SIZ_STRING_LANGID,
    USB_STRING_DESCRIPTOR_TYPE,
    0x09,
    0x04 /* LangID = 0x0409: U.S. English */
  };

const u8 Camera_StringVendor[CAMERA_SIZ_STRING_VENDOR] =
  {
    CAMERA_SIZ_STRING_VENDOR,       /* Size of Vendor string */
    USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
    /* Manufacturer:  */
    'A', 0, 'T', 0, '3', 0, '2', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0,
    ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0,
    ' ', 0, ' ', 0
  };

const u8 Camera_StringProduct[CAMERA_SIZ_STRING_PRODUCT] =
  {
    CAMERA_SIZ_STRING_PRODUCT,          /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,         /* bDescriptorType */
    /* Product name:  */
    'A', 0, 'T', 0, '3', 0, '2', 0, ' ', 0, 'C', 0, 'a', 0, 'm', 0,
    'e', 0, 'r', 0, 'a', 0, ' ', 0, 'V', 0, '1', 0, '.', 0, '0', 0,
    ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0, ' ', 0
  };

u8 Camera_StringSerial[CAMERA_SIZ_STRING_SERIAL] =
  {
    CAMERA_SIZ_STRING_SERIAL,           /* bLength */
    USB_STRING_DESCRIPTOR_TYPE,                   /* bDescriptorType */
    'A', 0, 'T', 0, '3', 0, '2', 0, '1', 0, '0', 0, ' ', 0
  };

  /************************END OF FILE***************************************/
