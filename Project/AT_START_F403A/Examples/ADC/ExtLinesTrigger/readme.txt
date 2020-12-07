/**
  @page ADC_ExtLinesTrigger ADC external lines trigger example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    ADC/ExtLinesTrigger/readme.txt 
  * @author  Artery Technology 
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description of the ADC external lines trigger example.
  ******************************************************************************
   @endverbatim

@par Example Description

This example describes how to trigger ADC regular and injected groups channels 
conversion using two external line events. Discontinuous mode is enabled for 
regular group channel conversion and configured to convert one regular channel 
on each external trigger.

ADC1 is configured to start regular group channel conversion on EXTI11 event.
On detection of the first rising edge on PB.11 pin, the conversion of the first 
regular channel (ADC channel4) is done and its converted value is transferred by 
DMA to ADC_RegularConvertedValueTab table. On the following edge detection, the 
second regular channel (ADC channel14) is automatically converted and its converted 
value is stored by DMA in the same table. The number of transmitted data by DMA, 
in this example is limited to 64 data.

The procedure is repeated for both regular channels on each EXTI11 event. ADC1 is 
configured to start injected group channel conversion on EXTI15 event. On detection 
of the first rising edge on PB.15 pin all selected injected channels, which are 
two in this example (ADC channel11 and channel12), are converted and an interrupt 
is generated on JEOC flag rising at the end of all injected channels conversion. 
Both injected channels converted results are stored in ADC_InjectedConvertedValueTab 
table inside the interrupt routine. The procedure is repeated for injected channels 
on each EXTI15 event. The ADCs clock is set to 24 MHz.

 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */

