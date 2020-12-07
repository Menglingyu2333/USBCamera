/**
 @page  CAN Normal mode Transmit and receive data example
 @verbatim
 ******************************** (C) COPYRIGHT 2018 ArteryTek ******************************
 * @file      : CAN/CAN_Normal/readme.txt
 * @author    : Artery Technology
 * @version   : V1.0.4
 * @date      : 2018-12-26
 * @brief     : Description of the CAN Normal mode Transmit and receive data example.
 ********************************************************************************************
 @endverbatiom

@par Example Desctiption
  This demo is based on the AT-START-F403A board,in this demo,CAN work in Normal mode.
  LED4 toggle when transmit CAN data.LED2 toggle when receive CAN data
  User can use USBCAN to transmit and receive data.
 
@note
 ¡î¡î¡î	In main.c line 46:The following one of the define AT32_USE_CAN1 must be choose
 ¡î¡î¡î	In main.c line 51:The following one of the define CAN1_NOREMAP and CAN1_REMAP10 and CAN1_REMAP11 must be choose
 
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */

