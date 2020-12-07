/**
  @page TMR_6STEPS example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    TMR/6STEPS/readme.txt 
  * @author  ArteryTek Technology
  * @version V1.0.4
  * @date    2019-01-04
  * @brief   Description of the TMR_6STEPS example.
  ******************************************************************************
   @endverbatim

@par Example Description 
  This demo shows how to configure the TMR1 peripheral to generate 6 Steps.
  In this example, a software COM event is generated each 100 ms: using the SysTick 
  interrupt.
  The break Polarity is used at High level.

  The following Table describes the TMR1 Channels states:
              -----------------------------------------------
             | Step1 | Step2 | Step3 | Step4 | Step5 | Step6 |
   ----------------------------------------------------------
  |Channel1  |   1   |   0   |   0   |   0   |   0   |   1   |
   ----------------------------------------------------------
  |Channel1N |   0   |   0   |   1   |   1   |   0   |   0   |
   ----------------------------------------------------------
  |Channel2  |   0   |   0   |   0   |   1   |   1   |   0   |
   ----------------------------------------------------------
  |Channel2N |   1   |   1   |   0   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |Channel3  |   0   |   1   |   1   |   0   |   0   |   0   |
   ----------------------------------------------------------
  |Channel3N |   0   |   0   |   0   |   0   |   1   |   1   |
   ----------------------------------------------------------
   
  Set-up 
    - Connect the TMR1 pins to an oscilloscope to monitor the different waveforms:
      - TMR1_CH3  pin (PA.10)
      - TMR1_CH1N pin (PB.13)  
      - TMR1_CH2  pin (PA.09)  
      - TMR1_CH3N pin (PB.15)
      - TMR1_CH1  pin (PA.08)  
      - TMR1_CH2N pin (PB.14) 
  
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
