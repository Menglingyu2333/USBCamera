/**
 @page  CAN Loopback mode Transmit data example
 @verbatim
 ******************************** (C) COPYRIGHT 2018 ArteryTek ******************************
 * @file      : CAN/CAN_Loopback/readme.txt
 * @author    : Artery Technology
 * @version   : V1.0.4
 * @date      : 2018-12-26
 * @brief     : Description of the CAN Loopback mode Transmit data example.
 ********************************************************************************************
 @endverbatiom

@par Example Desctiption
 This demo is based on the AT-START-F403A board,in this demo,CAN work in LoopBack mode.
 LED4 toggle when transmit CAN data.
 User can use USBCAN to receive data.
 User can check register CAN->ESTS and CAN->TSTS to affirm no error causing when disconnect CAN_RX pin.
 
@note
 ¡î¡î¡î	In main.c line 46:The following one of the define AT32_USE_CAN1 must be choose
 ¡î¡î¡î	In main.c line 51:The following one of the define CAN1_NOREMAP and CAN1_REMAP10 and CAN1_REMAP11 must be choose
 
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
 

