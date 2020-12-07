/**
  ******************************************************************************
  * @file    USB_Device/Composite_Device/src/usb_desc.c
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description for Composite Device Demo
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
#include "usb_desc.h"

/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup USB_Composite_Device
  * @{
  */ 
/* USB Standard Device Descriptor */
const uint8_t Composite_DeviceDescriptor[] =
{
  0x12,   /* bLength */
  USB_DEVICE_DESCRIPTOR_TYPE,     /* bDescriptorType */
  0x00,
  0x02,   /* bcdUSB = 2.00 */
  0xEF,   /* bDeviceClass: CDC */
  0x02,   /* bDeviceSubClass */
  0x01,   /* bDeviceProtocol */
  0x40,   /* bMaxPacketSize0 */
  0x3C,
  0x2E,   /* idVendor = 0x2E3C */
  0x50,
  0x57,   /* idProduct = 0x5750 */
  0x00,
  0x02,   /* bcdDevice = 2.00 */
  1,      /* Index of string descriptor describing manufacturer */
  2,      /* Index of string descriptor describing product */
  3,      /* Index of string descriptor describing the device's serial number */
  0x01    /* bNumConfigurations */
};

const uint8_t Composite_ConfigDescriptor[] =
{
  /*Configuration Descriptor*/
  0x09,   /* bLength: Configuration Descriptor size */
  USB_CONFIGURATION_DESCRIPTOR_TYPE,      /* bDescriptorType: Configuration */
  Composite_SIZ_CONFIG_DESC,       /* wTotalLength:no of returned bytes */
  0x00,
  0x03,   /* bNumInterfaces: 2 interface */
  0x01,   /* bConfigurationValue: Configuration value */
  0x00,   /* iConfiguration: Index of string descriptor describing the configuration */
  0xC0,   /* bmAttributes: self powered */
  0xFA,   /* MaxPower 0 mA */
  /* Interface Association Descriptor(IAD Descriptor) */
  0x08,   /*blength */
  0x0B,   /*bDescriptorType */
  0x00,   /*bFirstInterface */
  0x02,   /*bInterfaceCount */
  0x02,   /*bFunctionclass -- CDC */
  0x02,   /*bFunctionSubClass */
  0x01,   /*bFunctionProtocoll */
  0x00,   /*iFunction */
  /********** Descriptor of virtual com port *****************/
  /*Interface Descriptor*/
  0x09,   /* bLength: Interface Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: Interface */
  /* Interface descriptor type */
  0x00,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x01,   /* bNumEndpoints: One endpoints used */
  0x02,   /* bInterfaceClass: Communication Interface Class */
  0x02,   /* bInterfaceSubClass: Abstract Control Model */
  0x01,   /* bInterfaceProtocol: Common AT commands */
  0x00,   /* iInterface: */
  /*Header Functional Descriptor*/
  0x05,   /* bLength: Endpoint Descriptor size */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x00,   /* bDescriptorSubtype: Header Func Desc */
  0x10,   /* bcdCDC: spec release number */
  0x01,
  /*Call Management Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x01,   /* bDescriptorSubtype: Call Management Func Desc */
  0x00,   /* bmCapabilities: D0+D1 */
  0x01,   /* bDataInterface: 1 */
  /*ACM Functional Descriptor*/
  0x04,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x02,   /* bDescriptorSubtype: Abstract Control Management desc */
  0x02,   /* bmCapabilities */
  /*Union Functional Descriptor*/
  0x05,   /* bFunctionLength */
  0x24,   /* bDescriptorType: CS_INTERFACE */
  0x06,   /* bDescriptorSubtype: Union func desc */
  0x00,   /* bMasterInterface: Communication class interface */
  0x01,   /* bSlaveInterface0: Data Class Interface */
  /*Endpoint 2 Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
  0x82,   /* bEndpointAddress: (IN2) */
  0x03,   /* bmAttributes: Interrupt */
  Composite_INT_SIZE,      /* wMaxPacketSize: */
  0x00,
  0xFF,   /* bInterval: */
  /*Data class interface descriptor*/
  0x09,   /* bLength: Endpoint Descriptor size */
  USB_INTERFACE_DESCRIPTOR_TYPE,  /* bDescriptorType: */
  0x01,   /* bInterfaceNumber: Number of Interface */
  0x00,   /* bAlternateSetting: Alternate setting */
  0x02,   /* bNumEndpoints: Two endpoints used */
  0x0A,   /* bInterfaceClass: CDC */
  0x00,   /* bInterfaceSubClass: */
  0x00,   /* bInterfaceProtocol: */
  0x00,   /* iInterface: */
  /*Endpoint 3 Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
  0x03,   /* bEndpointAddress: (OUT3) */
  0x02,   /* bmAttributes: Bulk */
  Composite_DATA_SIZE,             /* wMaxPacketSize: */
  0x00,
  0x00,   /* bInterval: ignore for Bulk transfer */
  /*Endpoint 1 Descriptor*/
  0x07,   /* bLength: Endpoint Descriptor size */
  USB_ENDPOINT_DESCRIPTOR_TYPE,   /* bDescriptorType: Endpoint */
  0x81,   /* bEndpointAddress: (IN1) */
  0x02,   /* bmAttributes: Bulk */
  Composite_DATA_SIZE,             /* wMaxPacketSize: */
  0x00,
  0x00,   /* bInterval */
  
  /* Interface Association Descriptor(IAD Descriptor) */
  0x08,   /*blength */
  0x0B,   /*bDescriptorType */
  0x02,   /*bFirstInterface */
  0x01,   /*bInterfaceCount */
  0x03,   /*bFunctionclass -- HID*/
  0x00,   /*bFunctionSubClass */
  0x00,   /*bFunctionProtocoll */
  0x05,   /*iFunction */
  /********** Descriptor of virtual com port *****************/
  
  /*Mouse interface descriptor*/
  0x09,   /*bLength: Interface Descriptor size*/
  USB_INTERFACE_DESCRIPTOR_TYPE,/*bDescriptorType: Interface descriptor type*/
  0x02,   /*bInterfaceNumber: Number of Interface*/
  0x00,   /*bAlternateSetting: Alternate setting*/
  0x01,   /*bNumEndpoints*/
  0x03,   /*bInterfaceClass: HID*/
  0x01,   /*bInterfaceSubClass : 1=BOOT, 0=no boot*/
  0x02,   /*nInterfaceProtocol : 0=none, 1=keyboard, 2=mouse*/
  0,      /*iInterface: Index of string descriptor*/
  /******************** Descriptor of Joystick Mouse HID ********************/
  0x09,   /*bLength: HID Descriptor size*/
  HID_DESCRIPTOR_TYPE, /*bDescriptorType: HID*/
  0x00,   /*bcdHID: HID Class Spec release number*/
  0x01,
  0x00,   /*bCountryCode: Hardware target country*/
  0x01,   /*bNumDescriptors: Number of HID class descriptors to follow*/
  0x22,   /*bDescriptorType*/
  MOUSE_SIZ_REPORT_DESC,/*wItemLength: Total length of Report descriptor*/
  0x00,
  /******************** Descriptor of Joystick Mouse endpoint ********************/
  0x07,    /*bLength: Endpoint Descriptor size*/
  USB_ENDPOINT_DESCRIPTOR_TYPE, /*bDescriptorType:*/
  0x84,    /*bEndpointAddress: Endpoint Address (IN)*/
  0x03,    /*bmAttributes: Interrupt endpoint*/
  0x04,    /*wMaxPacketSize: 4 Byte max */
  0x00,
  0x20,    /*bInterval: Polling Interval (32 ms)*/
};
  /* MOUSE_ConfigDescriptor */
const u8 MOUSE_ReportDescriptor[MOUSE_SIZ_REPORT_DESC] =
{
  0x05,          /*Usage Page(Generic Desktop)*/
  0x01,
  0x09,          /*Usage(Mouse)*/
  0x02,
  0xA1,          /*Collection(Logical)*/
  0x01,
  0x09,          /*Usage(Pointer)*/
  0x01,
  /* 8 */
  0xA1,          /*Collection(Linked)*/
  0x00,
  0x05,          /*Usage Page(Buttons)*/
  0x09,
  0x19,          /*Usage Minimum(1)*/
  0x01,
  0x29,          /*Usage Maximum(3)*/
  0x03,
  /* 16 */
  0x15,          /*Logical Minimum(0)*/
  0x00,
  0x25,          /*Logical Maximum(1)*/
  0x01,
  0x95,          /*Report Count(3)*/
  0x03,
  0x75,          /*Report Size(1)*/
  0x01,
  /* 24 */
  0x81,          /*Input(Variable)*/
  0x02,
  0x95,          /*Report Count(1)*/
  0x01,
  0x75,          /*Report Size(5)*/
  0x05,
  0x81,          /*Input(Constant,Array)*/
  0x01,
  /* 32 */
  0x05,          /*Usage Page(Generic Desktop)*/
  0x01,
  0x09,          /*Usage(X axis)*/
  0x30,
  0x09,          /*Usage(Y axis)*/
  0x31,
  0x09,          /*Usage(Wheel)*/
  0x38,
  /* 40 */
  0x15,          /*Logical Minimum(-127)*/
  0x81,
  0x25,          /*Logical Maximum(127)*/
  0x7F,
  0x75,          /*Report Size(8)*/
  0x08,
  0x95,          /*Report Count(3)*/
  0x03,
  /* 48 */
  0x81,          /*Input(Variable, Relative)*/
  0x06,
  0xC0,          /*End Collection*/
  0x09,
  0x3c,
  0x05,
  0xff,
  0x09,
  /* 56 */
  0x01,
  0x15,
  0x00,
  0x25,
  0x01,
  0x75,
  0x01,
  0x95,
  /* 64 */
  0x02,
  0xb1,
  0x22,
  0x75,
  0x06,
  0x95,
  0x01,
  0xb1,
  /* 72 */
  0x01,
  0xc0
}
; /* Joystick_ReportDescriptor */
  
/* USB String Descriptors */
const uint8_t Composite_StringLangID[Composite_SIZ_STRING_LANGID] =
{
  Composite_SIZ_STRING_LANGID,
  USB_STRING_DESCRIPTOR_TYPE,
  0x09,
  0x04 /* LangID = 0x0409: U.S. English */
};

const uint8_t Composite_StringVendor[Composite_SIZ_STRING_VENDOR] =
{
  Composite_SIZ_STRING_VENDOR,     /* Size of Vendor string */
  USB_STRING_DESCRIPTOR_TYPE,             /* bDescriptorType*/
  /* Manufacturer: "Artery " */
  'A', 0, 'r', 0, 't', 0, 'e', 0, 'r', 0, 'y', 0, ' ', 0, 
  'T', 0, 'e', 0, 'c', 0, 'h', 0, 'n', 0, 'o', 0, 'l', 0, 'o', 0, 'g', 0, 'y', 0
};

const uint8_t Composite_StringProduct[Composite_SIZ_STRING_PRODUCT] =
{
  Composite_SIZ_STRING_PRODUCT,          /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,        /* bDescriptorType */
  /* Product name: "AT32 Composite CDC + HID" */
  'A', 0, 'T', 0, '3', 0, '2', 0, ' ', 0, 'C', 0,
  'o', 0, 'm', 0, 'p', 0, 'o', 0, 's', 0, 'i', 0, 't', 0,
  'e', 0,' ',0, 'C', 0, 'D', 0, 'C',  0, 'H', 0, 'I', 0, 'D', 0
};


uint8_t Composite_StringSerial[Composite_SIZ_STRING_SERIAL] =
{
  Composite_SIZ_STRING_SERIAL,           /* bLength */
  USB_STRING_DESCRIPTOR_TYPE,            /* bDescriptorType */
  'A', 0, 'T', 0, '3', 0, '2', 0
};

/**
  * @}
  */

/**
  * @}
  */


  
  

  

