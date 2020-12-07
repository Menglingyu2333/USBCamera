/**
  @page FLASH_run_in_SPIM run in SPIM example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    FLASH/run_in_SPIM/readme.txt 
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description of the run in SPIM example.
  ******************************************************************************
   @endverbatim

@par Example Description 

External SPI flash pin:
SPIF_SCK    : PB1
SPIF_CS     : PA8
SPIF_TX     : PB10
SPIF_RX     : PB11
SPIF_HOLD_N : PB6
SPIF_WP_N   : PB7

This demo is based on the AT-START-F403A board, using on-board EN25QH128A as ext.flash. 
In this demo, code in bank1 run usart for print message and init SPIM, while code in SPIM run LED toggle.   

 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
