/**
 **************************************************************************
 * @File Name    : readme.txt
 * @brief        : USB Aduio Composite Speaker and Micphone.
 * @Date         : 2018-01-09
 * @Version      : V1.0.0
 **************************************************************************
 */

Description
****************
  The AT32 USB_Device Audio Composite Speaker and Micphone gives example how
  to use the AT32F4xx USB peripheral to communicate with PC host in the 
  isochronous transfer mode.
  This example implementation composite device include USB Speaker and USB
  Micphone.It can play voice and record voice.   
  
  The Demo software implementation is given in the "UM_USB_Audio_Composite_Speaker_Micphone_V1.0".

Directory contents
******************
  + \inc: contains the demo header files
  + \MDK-ARM: contains preconfigured projects for MDK-ARM toolchain
  + \src: contains the demo source files
  + \wm8988-driver: contains the drivers of wm8988

Hardware Board
******************
  This demo runs on Artery AT-START-F403A,AT-START-F413 evaluation boards, more
  details of this board is given in the "AT_START_F403" and "AT_START_F413".

  The Demo Base on the "Audio Arduino Demo Board",More details about this
  demo board implementation is given in the "UM_Audio_Arduino_Daughter_Board_V1.0".

How to use it
*****************
  \MDK-ARM
  - Open the Audio_Composite project
  - Rebuild all files
  - Load project iamge
  - Run program
  - Connect USB device to PC Host
  - Play voice on PC or record voice use windows recorder
  More details of how to use is given in "AT32 USB Audio Development_NoteV1.0.0.pdf"
/****************** (C) COPYRIGHT 2017 ArteryTek *********END OF FILE*********/
