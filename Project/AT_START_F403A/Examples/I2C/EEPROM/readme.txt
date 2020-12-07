/**
  @page MCO example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    I2C/EEPROM/readme.txt 
  * @author  Artery Technology 
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description of the MCO example.
  ******************************************************************************
   @endverbatim

@par Example Description 

  This demo is based on the AT-START-F403A board,in this demo,use hardware I2C1 to
  write/read EEPROM AT24C02 and USART1 printf the result. change the macro definition
  PROCESS_MODE 0-2 can switch among polling,interrupt,DMA operation.
  
  Note: After the EEPROM receives the data, it doesn't respond to the host's request
  within the time period when the internal cycle write data is started. So after writing
  a page of data, the EEPROM address will be sent until the EEPROM response is
  detected.

 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */