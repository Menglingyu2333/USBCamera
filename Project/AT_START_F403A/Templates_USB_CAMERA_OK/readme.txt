/**
 @page  EXTI Config example
 @verbatim
 ******************************** (C) COPYRIGHT 2018 ArteryTek ******************************
 * @file      : EXTI\EXTI_Config/readme.txt
 * @author    : Artery Technology
 * @version   : V1.0.4
 * @date      : 2018-12-28
 * @brief     : Description of the EXTI Config example.
 ********************************************************************************************
 @endverbatiom

@par Example Desctiption
  This demo is based on the AT-START-F403A board.
  This example shows how to configure external interrupt lines.
  In this example, 2 EXTI lines (EXTI0 and EXTI9) are configured to generate an interrupt on 
  each rising or falling edge. In the interrupt routine a led connected to a specific GPIO pin is toggled.
  LED2-->GPIOD Pin13
  LED3-->GPIOD Pin14
  LED4-->GPIOD Pin15
 
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */

