/**
  @page USB Custom HID example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    USB_Device/Custom_HID/readme.txt 
  * @author  Artery Technology 
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description of the USB Custom_HID example.
  ******************************************************************************
   @endverbatim

@par Example Description 

This Demo provides a description of how to use the USB-FS-Device on the AT32.
The AT32 device is enumerated as an USB-FS-Device HID , that uses the native 
PC Host USB-FS-Device HID driver.

To test the example, you need to:

  1)- Start the "Artery_UsbHid_Demo.exe" PC applet and connect AT32 USB to PC
  2)- The device should be detected and shown in the Device capabilities
  3)- Communicaton view: LED2 ID(0x2),LED3 ID(0x3), LED4 ID(0x4), BUTTON1_ID(0x5) and Input\Output Report.
  4)- Select Leds to switch on/off on the Artery USB HID Demo => a SET_REPORT request will be sent to AT32
  5)- Push the USER key button in AT-START => AT32 will sent SET_REPORT to PC
  7)- Write data in Output Report(on the Artery USB HID Demo) will sent the data to AT32 => the host computer will receive the same data
      transmit by AT32

 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */