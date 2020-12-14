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
    0x00,                                 /* bcdUSB 2.10 */
    0x02,
    0xEF,                                 /* bDeviceClass */
    0x02,                                 /* bDeviceSubClass */
    0x01,                                 /* bDeviceProtocol */
    0x40,                                 /* bMaxPacketSize 40 */
    0xF2,                                 /* idVendor = 0x1985*/
    0x04,
    0x08,                                 /* idProduct  = 0x1017*/
    0xb0,
    0x00,                                 /* 1.00 */             /* bcdDevice */
    0x01,
    1,                                 /* iManufacturer */
    2,                                 /* iProduct */
    3,                                 /* iSerialNumber */
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
//  CS_INTERFACE,                                 /* bDescriptorType : CS_INTERFACE */
//  VC_INPUT_TERMINAL,                                 /* bDescriptorSubType : VC_INPUT_TERMINAL subtype */
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
  0x00,                                 /* bAspectRatioX : Non-interlaced stream 每 not required. */
  0x00,                                 /* bAspectRatioY : Non-interlaced stream 每 not required. */
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
//  {
//    /* Configuration Descriptor */
//    0x09,                                /* bLength */
//    USB_CONFIGURATION_DESCRIPTOR_TYPE,   /* bDescriptorType */
//    CAMERA_SIZ_CONFIG_DESC,              /* wTotalLength  0x90 bytes*/
//    0x00,
//    0x02,                                 /* bNumInterfaces */
//    0x01,                                 /* bConfigurationValue */
//    0x00,                                 /* iConfiguration */
//    0x80,                                 /* bmAttributes  BUS Powred, no remote wakeup*/
//    0xFA,                                 /* bMaxPower = 500 mA*/
//    /* 09 byte, total size 9*/
//
//    /* 1. Standard Video Interface Collection IAD */
//    0x08,                                 /* bLength */
//    USB_ASSOCIATION_DESCRIPTOR_TYPE,      /* bDescriptorType */
//    0x00,                                 /* bFirstInterface: Interface number of the VideoControl interface that is associated with this function*/
//    0x02,                                 /* Number of contiguous Video interfaces that are associated with this function */
//    0x0E,                                 /* bFunction Class: CC_VIDEO*/
//    0x03,                                 /* bFunction sub Class: SC_VIDEO_INTERFACE_COLLECTION */
//    0x00,                                 /* bFunction protocol : PC_PROTOCOL_UNDEFINED*/
//    0x02,                                 /* iFunction */
//    /* 08 bytes, total size 17*/
//
//    /* 2. Standard VideoControl Interface Descriptor */
//    0x09,                                 /* bLength */
//    0x04,                                 /* bDescriptorType */
//    0x00,                                 /* bInterfaceNumber */
//    0x00,                                 /* bAlternateSetting */
//    0x00,                                 /* bNumEndpoints */
//    0x0e,                                 /* bInterfaceClass : CC_VIDEO */
//    0x01,                                 /* bInterfaceSubClass : SC_VIDEOCONTROL */
//    0x00,                                 /* bInterfaceProtocol : PC_PROTOCOL_UNDEFINED */
//    0x02,                                 /* iInterface */
//    /* 09 bytes, total size 26*/
//
//    /* 2.1. Class-specific VideoControl Interface Descriptor */
//    0x0d,                                 /* bLength */
//    0x24,                                 /* bDescriptorType : CS_INTERFACE */
//    0x01,                                 /* bDescriptorSubType : VC_HEADER subtype */
//    0x00,                                 /* bcdUVC : Revision of class specification that this device is based upon. For this example, the device complies with Video Class specification version 1.0 */
//    0x01,
//    0x1e,                                 /* wTotalLength : Total size of class-specific descriptors*/
//    0x00,
//    0x80, 0x8d, 0x5b, 0x00,               /* dwClockFrequency : 0x005b8d80 -> 6,000,000 == 6MHz*/
//    0x01,                                 /* bInCollection : Number of streaming interfaces. */
//    0x01,                                 /* baInterfaceNr(1) : VideoStreaming interface 1 belongs to this VideoControl interface.*/
//    /* 13 Bytes, totoal size 39 */
//
//    /* 2.2. Video Input Terminal Descriptor (Composite) */
//    0x08,                                 /* bLength */
//    0x24,                                 /* bDescriptorType : CS_INTERFACE */
//    0x02,                                 /* bDescriptorSubType : VC_INPUT_TERMINAL subtype */
//    0x02,                                 /* bTerminalID: ID of this input terminal */
//    0x01, 0x04,                           /* wTerminalType: 0x0401 COMPOSITE_CONNECTOR type. This terminal is the composite connector. */
//    0x00,                                 /* bAssocTerminal: No association */
//    0x00,                                 /* iTerminal: Unused*/
//    /* 8 Bytes, totoal size 47 */
//
//    /* 2.3. Video Output Terminal Descriptor */
//    0x09,                                 /* bLength */
//    0x24,                                 /* bDescriptorType : CS_INTERFACE */
//    0x03,                                 /* bDescriptorSubType : VC_OUTPUT_TERMINAL subtype */
//    0x03,                                 /* bTerminalID: ID of this output terminal */
//    0x01, 0x01,                           /* wTerminalType: 0x0101 TT_STREAMING type. This terminal is a USB streaming terminal. */
//    0x00,                                 /* bAssocTerminal: No association */
//    0x02,                                 /* bSourceID: The input pin of this unit is connected to the output pin of unit 2. */
//    0x00,                                 /* iTerminal: Unused*/
//    /* 9 bytes, total size 56 */
//
//    /* 3. Standard VideoStream Interface Descriptor*/
//    0x09,                                 /* bLength */
//    0x04,                                 /* bDescriptorType : INTERFACE */
//    0x01,                                 /* bInterfaceNumber */
//    0x00,                                 /* bAlternateSetting */
//    0x00,                                 /* bNumEndpoints : 0 endpoints 每 no bandwidth used*/
//    0x0e,                                 /* bInterfaceClass : CC_VIDEO */
//    0x02,                                 /* bInterfaceSubClass : SC_VIDEOSTREAMING */
//    0x00,                                 /* bInterfaceProtocol : PC_PROTOCOL_UNDEFINED */
//    0x00,                                 /* iInterface : unused */
//    /* 9 bytes, total size 65 */
//
//    /* 3.1 Class-specific VideoStream Header Descriptor (Input) */
//    0x0e,                                 /* bLength */
//    0x24,                                 /* bDescriptorType : CS_INTERFACE */
//    0x01,                                 /* bDescriptorSubType : VC_HEADER subtype */
//    0x01,                                 /* bNumFormats : One format descriptor follows. */
//    0x3f, 0x00,                           /* wTotalLength : Total size of class-specific descriptors*/
//    0x81,                                 /* bEndpointAddress : 0x81 */
//    0x00,                                 /* bmInfo : No dynamic format change supported. */
//    0x03,                                 /* bTerminalLink : This VideoStreaming interface supplies terminal ID 3 (Output Terminal). */
//    0x00,                                 /* bStillCaptureMethod : Device supports still image capture method 0. */
//    0x00,                                 /* bTriggerSupport : Hardware trigger supported for still image capture */
//    0x00,                                 /* bTriggerUsage : Hardware trigger should initiate a still image capture. */
//    0x01,                                 /* bControlSize : Size of the bmaControls field */
//    0x00,                                 /* bmaControls : No VideoStreaming specific controls are supported.*/
//    /* 14 Bytes, totoal size 79 */
//
//    /* 3.2 Class-specific VideoStream Format(MJPEG) Descriptor */
//    0x0b,                                 /* bLength */
//    0x24,                                 /* bDescriptorType : CS_INTERFACE */
//    0x06,                                 /* bDescriptorSubType : VS_FORMAT_MJPEG subtype */
//    0x01,                                 /* bFormatIndex : First (and only) format descriptor */
//    0x01,                                 /* bNumFrameDescriptors : One frame descriptor for this format follows. */
//    0x01,                                 /* bmFlags : Uses fixed size samples.. */
//    0x01,                                 /* bDefaultFrameIndex : Default frame index is 1. */
//    0x00,                                 /* bAspectRatioX : Non-interlaced stream 每 not required. */
//    0x00,                                 /* bAspectRatioY : Non-interlaced stream 每 not required. */
//    0x00,                                 /* bmInterlaceFlags : Non-interlaced stream */
//    0x00,                                 /* bCopyProtect : No restrictions imposed on the duplication of this video stream. */
//    /* 11 bytes, total size 90 */
//
//    /* 3.3 Class-specific VideoStream Frame Descriptor */
//    0x26,                                 /* bLength */
//    0x24,                                 /* bDescriptorType : CS_INTERFACE */
//    0x07,                                 /* bDescriptorSubType : VS_FRAME_MJPEG */
//    0x01,                                 /* bFrameIndex : First (and only) frame descriptor */
//    0x02,                                 /* bmCapabilities : Still images using capture method 0 are supported at this frame setting.D1: Fixed frame-rate. */
//    MAKE_WORD(ImageWidth),     /* wWidth : Width of frame, pixels. */
//    MAKE_WORD(ImageHeight),    /* wHeight : Height of frame, pixels. */
//    MAKE_DWORD(MIN_BIT_RATE),             /* dwMinBitRate : Min bit rate in bits/s  */
//    MAKE_DWORD(MAX_BIT_RATE),             /* dwMaxBitRate : Max bit rate in bits/s  */
//    MAKE_DWORD(MAX_FRAME_SIZE),           /* dwMaxVideoFrameBufSize : Maximum video or still frame size, in bytes. */
//    MAKE_DWORD(FRAME_INTERVEL),           /* dwDefaultFrame Interval time, unit=100ns */
//    0x00,                                 /* bFrameIntervalType : Continuous frame interval */
//    MAKE_DWORD(FRAME_INTERVEL),           /* dwDefaultFrame Interval time, unit=100ns */
//    MAKE_DWORD(FRAME_INTERVEL),           /* dwDefaultFrame Interval time, unit=100ns */
//    0x00, 0x00, 0x00, 0x00,               /* dwFrameIntervalStep : No frame interval step supported. */
//    /* 38 bytes, total size 128 */
//
//    /* 4. Operational Alternate Setting 1 */
//    /* 4.1 Standard VideoStream Interface Descriptor */
//    0x09,                               /* Size of this descriptor, in bytes. */
//    0x04,                               /* INTERFACE descriptor type */
//    0x01,                               /* Index of this interface */
//    0x01,                               /* Index of this alternate setting */
//    0x01,                               /* endpoints, 0 每 no bandwidth used */
//    0x0e,                               /* CC_VIDEO */
//    0x02,                               /* SC_VIDEOSTREAMING */
//    0x00,                               /* PC_PROTOCOL_UNDEFINED */
//    0x00,                               /* Unused */
//    /* 9 bytes, total size 137 */
//
//    /* 4.2 Standard VideoStream Isochronous Video Data Endpoint Descriptor */
//    0x07,                               /* Size of this descriptor, in bytes. */
//    0x05,                               /* ENDPOINT */
//    0x81,                               /* IN endpoint 1 */
//    0x05,                               /* Isochronous transfer type. Asynchronous synchronization type. */
//    MAKE_WORD(PACKET_SIZE),             /* Max packet size, in bytes */
//    0x01,                               /* One frame interval */
//    /* 7 bytes, total size 144 */
//  };

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
