/**
 @page  PWR_SLEEP example
 @verbatim
 ******************************** (C) COPYRIGHT 2018 ArteryTek ******************************
 * @file      : PWR/SLEEP/readme.txt
 * @author    : Artery Technology
 * @version   : V1.0.4
 * @date      : 2019-12-18
 * @brief     : Description of the PWR_SLEEP mode example.
 ********************************************************************************************
 @endverbatiom

@par Example Desctiption
  This demo is based on the AT-START-F403A board.
  The system enters and exits SLEEP mode as following:
    The Timer2 is configured to generate an interrupt in 2 second. 
    System enters SLEEP mode,and then the Timer2 interrupt will wake-up it.This behavior is repeated in an infinite loop.

  Three leds LED2,LED3 and LED4 are used to monitor the system state as following:
    - LED2 on: system in RUN mode
    - LED2 off: system in STOP mode
    - LED4 is toggled if an interrupt generate with Timer2.
 
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */


