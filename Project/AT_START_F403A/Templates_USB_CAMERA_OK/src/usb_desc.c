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
    0x85,                                 /* idVendor = 0x1985*/
    0x19,
    0x17,                                 /* idProduct  = 0x1017*/
    0x10,
    0x00,                                 /* 1.00 */             /* bcdDevice */
    0x01,
    1,                                 /* iManufacturer */
    2,                                 /* iProduct */
    3,                                 /* iSerialNumber */
    0x01                                  /* bNumConfigurations */
  };

const u8 Camera_ConfigDescriptor[] =
  {
    /* Configuration Descriptor */
    0x09,                                /* bLength */
    USB_CONFIGURATION_DESCRIPTOR_TYPE,   /* bDescriptorType */
    CAMERA_SIZ_CONFIG_DESC,              /* wTotalLength  0x90 bytes*/
    0x00,
    0x02,                                 /* bNumInterfaces */
    0x01,                                 /* bConfigurationValue */
    0x00,                                 /* iConfiguration */
    0x80,                                 /* bmAttributes  BUS Powred, no remote wakeup*/
    0xFA,                                 /* bMaxPower = 500 mA*/
    /* 09 byte, total size 9*/

    /* 1. Standard Video Interface Collection IAD */
    0x08,                                 /* bLength */
    USB_ASSOCIATION_DESCRIPTOR_TYPE,      /* bDescriptorType */
    0x00,                                 /* bFirstInterface: Interface number of the VideoControl interface that is associated with this function*/
    0x02,                                 /* Number of contiguous Video interfaces that are associated with this function */
    0x0E,                                 /* bFunction Class: CC_VIDEO*/
    0x03,                                 /* bFunction sub Class: SC_VIDEO_INTERFACE_COLLECTION */
    0x00,                                 /* bFunction protocol : PC_PROTOCOL_UNDEFINED*/
    0x02,                                 /* iFunction */
    /* 08 bytes, total size 17*/

    /* 2. Standard VideoControl Interface Descriptor */
    0x09,                                 /* bLength */
    0x04,                                 /* bDescriptorType */
    0x00,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints */
    0x0e,                                 /* bInterfaceClass : CC_VIDEO */
    0x01,                                 /* bInterfaceSubClass : SC_VIDEOCONTROL */
    0x00,                                 /* bInterfaceProtocol : PC_PROTOCOL_UNDEFINED */
    0x02,                                 /* iInterface */
    /* 09 bytes, total size 26*/

    /* 2.1. Class-specific VideoControl Interface Descriptor */
    0x0d,                                 /* bLength */
    0x24,                                 /* bDescriptorType : CS_INTERFACE */
    0x01,                                 /* bDescriptorSubType : VC_HEADER subtype */
    0x00,                                 /* bcdUVC : Revision of class specification that this device is based upon. For this example, the device complies with Video Class specification version 1.0 */
    0x01,
    0x1e,                                 /* wTotalLength : Total size of class-specific descriptors*/
    0x00,
    0x80, 0x8d, 0x5b, 0x00,               /* dwClockFrequency : 0x005b8d80 -> 6,000,000 == 6MHz*/
    0x01,                                 /* bInCollection : Number of streaming interfaces. */
    0x01,                                 /* baInterfaceNr(1) : VideoStreaming interface 1 belongs to this VideoControl interface.*/
    /* 13 Bytes, totoal size 39 */

    /* 2.2. Video Input Terminal Descriptor (Composite) */
    0x08,                                 /* bLength */
    0x24,                                 /* bDescriptorType : CS_INTERFACE */
    0x02,                                 /* bDescriptorSubType : VC_INPUT_TERMINAL subtype */
    0x02,                                 /* bTerminalID: ID of this input terminal */
    0x01, 0x04,                           /* wTerminalType: 0x0401 COMPOSITE_CONNECTOR type. This terminal is the composite connector. */
    0x00,                                 /* bAssocTerminal: No association */
    0x00,                                 /* iTerminal: Unused*/
    /* 8 Bytes, totoal size 47 */

    /* 2.3. Video Output Terminal Descriptor */
    0x09,                                 /* bLength */
    0x24,                                 /* bDescriptorType : CS_INTERFACE */
    0x03,                                 /* bDescriptorSubType : VC_OUTPUT_TERMINAL subtype */
    0x03,                                 /* bTerminalID: ID of this output terminal */
    0x01, 0x01,                           /* wTerminalType: 0x0101 TT_STREAMING type. This terminal is a USB streaming terminal. */
    0x00,                                 /* bAssocTerminal: No association */
    0x02,                                 /* bSourceID: The input pin of this unit is connected to the output pin of unit 2. */
    0x00,                                 /* iTerminal: Unused*/
    /* 9 bytes, total size 56 */

    /* 3. Standard VideoStream Interface Descriptor*/
    0x09,                                 /* bLength */
    0x04,                                 /* bDescriptorType : INTERFACE */
    0x01,                                 /* bInterfaceNumber */
    0x00,                                 /* bAlternateSetting */
    0x00,                                 /* bNumEndpoints : 0 endpoints �C no bandwidth used*/
    0x0e,                                 /* bInterfaceClass : CC_VIDEO */
    0x02,                                 /* bInterfaceSubClass : SC_VIDEOSTREAMING */
    0x00,                                 /* bInterfaceProtocol : PC_PROTOCOL_UNDEFINED */
    0x00,                                 /* iInterface : unused */
    /* 9 bytes, total size 65 */

    /* 3.1 Class-specific VideoStream Header Descriptor (Input) */
    0x0e,                                 /* bLength */
    0x24,                                 /* bDescriptorType : CS_INTERFACE */
    0x01,                                 /* bDescriptorSubType : VC_HEADER subtype */
    0x01,                                 /* bNumFormats : One format descriptor follows. */
    0x3f, 0x00,                           /* wTotalLength : Total size of class-specific descriptors*/
    0x81,                                 /* bEndpointAddress : 0x81 */
    0x00,                                 /* bmInfo : No dynamic format change supported. */
    0x03,                                 /* bTerminalLink : This VideoStreaming interface supplies terminal ID 3 (Output Terminal). */
    0x00,                                 /* bStillCaptureMethod : Device supports still image capture method 0. */
    0x00,                                 /* bTriggerSupport : Hardware trigger supported for still image capture */
    0x00,                                 /* bTriggerUsage : Hardware trigger should initiate a still image capture. */
    0x01,                                 /* bControlSize : Size of the bmaControls field */
    0x00,                                 /* bmaControls : No VideoStreaming specific controls are supported.*/
    /* 14 Bytes, totoal size 79 */

    /* 3.2 Class-specific VideoStream Format(MJPEG) Descriptor */
    0x0b,                                 /* bLength */
    0x24,                                 /* bDescriptorType : CS_INTERFACE */
    0x06,                                 /* bDescriptorSubType : VS_FORMAT_MJPEG subtype */
    0x01,                                 /* bFormatIndex : First (and only) format descriptor */
    0x01,                                 /* bNumFrameDescriptors : One frame descriptor for this format follows. */
    0x01,                                 /* bmFlags : Uses fixed size samples.. */
    0x01,                                 /* bDefaultFrameIndex : Default frame index is 1. */
    0x00,                                 /* bAspectRatioX : Non-interlaced stream �C not required. */
    0x00,                                 /* bAspectRatioY : Non-interlaced stream �C not required. */
    0x00,                                 /* bmInterlaceFlags : Non-interlaced stream */
    0x00,                                 /* bCopyProtect : No restrictions imposed on the duplication of this video stream. */
    /* 11 bytes, total size 90 */

    /* 3.3 Class-specific VideoStream Frame Descriptor */
    0x26,                                 /* bLength */
    0x24,                                 /* bDescriptorType : CS_INTERFACE */
    0x07,                                 /* bDescriptorSubType : VS_FRAME_MJPEG */
    0x01,                                 /* bFrameIndex : First (and only) frame descriptor */
    0x02,                                 /* bmCapabilities : Still images using capture method 0 are supported at this frame setting.D1: Fixed frame-rate. */
    MAKE_WORD((ImageWidth/IMG_VIDEO_SCALE)),     /* wWidth : Width of frame, pixels. */
    MAKE_WORD((ImageHeight/IMG_VIDEO_SCALE)),    /* wHeight : Height of frame, pixels. */
    MAKE_DWORD(MIN_BIT_RATE),             /* dwMinBitRate : Min bit rate in bits/s  */
    MAKE_DWORD(MAX_BIT_RATE),             /* dwMaxBitRate : Max bit rate in bits/s  */
    MAKE_DWORD(MAX_FRAME_SIZE),           /* dwMaxVideoFrameBufSize : Maximum video or still frame size, in bytes. */
    MAKE_DWORD(FRAME_INTERVEL),           /* dwDefaultFrame Interval time, unit=100ns */
    0x00,                                 /* bFrameIntervalType : Continuous frame interval */
    MAKE_DWORD(FRAME_INTERVEL),           /* dwDefaultFrame Interval time, unit=100ns */
    MAKE_DWORD(FRAME_INTERVEL),           /* dwDefaultFrame Interval time, unit=100ns */
    0x00, 0x00, 0x00, 0x00,               /* dwFrameIntervalStep : No frame interval step supported. */
    /* 38 bytes, total size 128 */

    /* 4. Operational Alternate Setting 1 */
    /* 4.1 Standard VideoStream Interface Descriptor */
    0x09,                               /* Size of this descriptor, in bytes. */
    0x04,                               /* INTERFACE descriptor type */
    0x01,                               /* Index of this interface */
    0x01,                               /* Index of this alternate setting */
    0x01,                               /* endpoints, 0 �C no bandwidth used */
    0x0e,                               /* CC_VIDEO */
    0x02,                               /* SC_VIDEOSTREAMING */
    0x00,                               /* PC_PROTOCOL_UNDEFINED */
    0x00,                               /* Unused */
    /* 9 bytes, total size 137 */

    /* 4.2 Standard VideoStream Isochronous Video Data Endpoint Descriptor */
    0x07,                               /* Size of this descriptor, in bytes. */
    0x05,                               /* ENDPOINT */
    0x81,                               /* IN endpoint 1 */
    0x05,                               /* Isochronous transfer type. Asynchronous synchronization type. */
    MAKE_WORD(PACKET_SIZE),             /* Max packet size, in bytes */
    0x01,                               /* One frame interval */
    /* 7 bytes, total size 144 */
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
    'f', 0, 'C', 0, 'A', 0, 'M', 0, '1', 0, '0', 0, ' ', 0
  };

  /************************END OF FILE***************************************/
