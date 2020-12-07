/**
 @page  PWR_STANDBY example
 @verbatim
 ******************************** (C) COPYRIGHT 2018 ArteryTek ******************************
 * @file      : PWR/STANDBY/readme.txt
 * @author    : Artery Technology
 * @version   : V1.0.4
 * @date      : 2018-12-28
 * @brief     : Description of the PWR_STANDBY example.
 ********************************************************************************************
 @endverbatiom

@par Example Desctiption
  This demo is based on the AT-START-F403A board.
  The system enters and exits Standby mode as following:
	Initializes and turn off LED.Check SBF/WUF,if set,turn on LED2/LED3.Within 3 second Turn on LED4,
	then within 8 second enter Standby mode,LED will turn off right now.The rising on Wakeup Pin 
	will wake-up the system.As the infinite loop.
 
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */


