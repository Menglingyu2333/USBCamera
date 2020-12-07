
/**
  @page RTC_LSI_Calib RTC LSI_Calib example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    RTC/LSI_Calib/readme.txt 
  * @author  Artery Technology 
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description of the RTC LSI_Calib example.
  ******************************************************************************
   @endverbatim

@par Example Description 

This example demonstrates and explains how to use the LSI clock source auto
calibration to get a precise RTC clock. 
As an application example, it demonstrates how to configure the TIM5 timer
internally connected to LSI clock output, in order to adjust the RTC prescaler. 

The Low Speed External (LSI) clock is used as RTC clock source.
After reset, the RTC prescaler is set with the default value (40000). 
The inaccuracy of the LSI clock causes the RTC Second signal to be inaccurate. This
signal is output on the Tamper pin (PC.13) and can be measured by on oscilloscope
or a frequencymeter.
    
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
