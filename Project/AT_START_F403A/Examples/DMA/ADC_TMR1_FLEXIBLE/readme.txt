/**
  @page DMA ADC_TMR1 example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    DMA/ADC_TMR1/readme.txt 
  * @author  ArteryTek Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description of the DMA ADC_TMR1_FLEXIBLE example.
  ******************************************************************************
   @endverbatim
 
@par Example Description 
  This demo provides a description of how to use DMA flexible function with DMA1 channle1 to 
  transfer continuously a data from a peripheral (ADC1) to another peripheral (TIM1) supporting 
  DMA transfer.The ADC channel14 is configured to be converted continuously. TIM1_CH1 is 
  configured to generate a PWM signal on its output.The duty cycle variation can be visualized on 
  oscilloscope on the TIM1_CH1 pin PA.08 while changing the analog input on ADC channel14 
  using the potentiometer.
  
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
