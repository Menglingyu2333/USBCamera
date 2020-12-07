/**
  @page DMA SPI_RAM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    DMA/SPI_RAM/readme.txt 
  * @author  ArteryTek Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description of the DMA SPI_RAM example.
  ******************************************************************************
   @endverbatim

@par Example Description
  This demo provides a description of how to use four DMA channels to transfer a 
  data buffer from memory to SPI_SLAVE through SPI_MASTER and a second data buffer
  from memory to SPI_MASTER through SPI_SLAVE in full-duplex mode.
  
  Set-up 
    - Connect SPI1 SCK pin (PA.05) to SPI2 SCK pin	(PB.13)
    - Connect SPI1 MISO pin (PA.06) to SPI2 MISO pin (PB.14)
    - Connect SPI1 MOSI pin (PA.07) to SPI2 MOSI pin (PB.15)
    
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */



