/**
  @page ADC_ADC1_DMA ADC1 DMA example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    ADC/ADC1_DMA/readme.txt 
  * @author  Artery Technology 
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description of the ADC1 DMA example.
  ******************************************************************************
   @endverbatim

@par Example Description 

This example describes how to use the ADC1 and DMA to transfer continuously 
converted data from ADC1 to memory.
The ADC1 is configured to convert continuously channel14.
Each time an end of conversion occurs the DMA transfers, in circular mode, the
converted data from ADC1 DR register to the ADCConvertedValue variable.
The ADCs clock is set to 24 MHz.

 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
