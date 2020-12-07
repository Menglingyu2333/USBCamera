/**
  @page I2C example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    I2C/Interrupt(Not_the_highest_priority)_MasterReceiver&SlaveTransmitter(data only)/readme.txt 
  * @author  Artery Technology 
  * @version V1.0.0
  * @date    2020-03-06
  * @brief   Description of the I2C communication example based on interrupt mode.
  *          This example only uses interrupts that are not the highest priority 
  *          in the data section.
  ******************************************************************************
   @endverbatim

@par Example Description 

  This demo is based on the AT-START-F403A board,in this demo,use hardware I2C1 
  with two board communication and USART1 printf the result. change the macro 
  definition MASTER_BOARD can switch among master-receive/slave-transmit operation.

  ATTENTION:
    1. I2C bus must pull-up
	2. Press the master button to start communication.

 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
