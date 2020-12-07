/**
  @page I2C example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    I2C/Two_Board_Communication/readme.txt 
  * @author  Artery Technology 
  * @version V1.0.0
  * @date    2019-07-02
  * @brief   Description of the I2C Communication example.
  ******************************************************************************
   @endverbatim

@par Example Description 

  This demo is based on the AT-START-F403A board,in this demo,use hardware I2C1 
  with two board communication and USART1 printf the result. change the macro 
  definition IIC_MODE 0-3 can switch among slave-receive/slave-tranmit/master-
  receive and master-transmit operation.

  ATTENTION:
    1. I2C bus must pull-up
	2. Press slave button first, and then press master button.

 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
