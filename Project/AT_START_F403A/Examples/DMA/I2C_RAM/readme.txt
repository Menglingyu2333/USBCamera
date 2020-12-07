/**
  @page DMA I2C_RAM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    DMA/I2C_RAM/readme.txt 
  * @author  ArteryTek Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description of the DMA I2C_RAM example.
  ******************************************************************************
   @endverbatim

@par Example Description 
  This demo provides a description of how to use two DMA channels to transfer a 
  data buffer from memory to I2C2 through I2C1.If received data right ,the Three 
  leds will turn on.
  
  - Connect I2C1 SCL pin (PB.06) to I2C2 SCL pin (PB.10)
  - Connect I2C1 SDA pin (PB.07) to I2C2 SDA pin (PB.11)
  - Check that a pull-up resistor is connected on one I2C SDA pin
  - Check that a pull-up resistor is connected on one I2C SCL pin   
  
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */