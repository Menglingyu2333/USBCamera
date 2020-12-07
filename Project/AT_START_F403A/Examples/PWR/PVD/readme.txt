/**
 @page  PWR_PVD example
 @verbatim
 ******************************** (C) COPYRIGHT 2018 ArteryTek ******************************
 * @file      : PWR/PVD/readme.txt
 * @author    : Artery Technology
 * @version   : V1.0.4
 * @date      : 2018-12-28
 * @brief     : Description of the PWR_PVD example.
 ********************************************************************************************
 @endverbatiom

@par Example Desctiption
  This demo is based on the AT-START-F403A board.
  This example shows how to configure the programmable voltage detector usingan external interrupt line.
  In this example, EXTI line 16 is configured to generate an interrupt on each rising or falling edge 
  of the PVD output signal (which indicates that the Vdd voltage is below the PVD threshold).
  In the interrupt routine a led connected to a specific GPIO pin is toggled every time the voltage drops 
  below or  the target threshold.
 
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */


