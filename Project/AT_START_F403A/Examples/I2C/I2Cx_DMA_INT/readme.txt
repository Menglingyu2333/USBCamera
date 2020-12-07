/**
  @page I2C example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    I2C/I2Cx_DMA_INT/readme.txt 
  * @author  Artery Technology 
  * @version V1.0.0
  * @date    2019-07-02
  * @brief   Description of the I2Cx DMA INT example.
  ******************************************************************************
   @endverbatim

@par Example Description 

  This demo is based on the AT-START-F403A board,in this demo,use hardware I2C1
  or I2C2 with device communication, both send and receive are transmitted 
  through DMA. After the transfer is complete, it enters the DMA IRQHandler.
  PS: Use EEPROM as slave device for testing, slave address is 0xA0.

  ATTENTION:
    1. I2C bus must pull-up

 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
