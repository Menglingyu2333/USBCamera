/**
  @page TMR_Gated_mode example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    TMR/Gated_mode/readme.txt 
  * @author  ArteryTek Technology
  * @version V1.0.4
  * @date    2019-01-04
  * @brief   Description of the TMR_Gated_mode example.
  ******************************************************************************
   @endverbatim

@par Example Description 
  This demo shows how to test Gated mode with an external trigger.
  
  Gated mode test -----
    1/TMR1 is configured as Master Timer:
     - Toggle Mode is used

    2/TMR1 is configured as Slave Timer for an external Trigger connected
     to TMR1 TI2 pin (TMR1 CH2 configured as input pin):
     - The TMR1 TI2FP2 is used as Trigger Input
     - Rising edge is used to start and stop the TMR1: Gated Mode.

    * The TIMxCLK is fixed to 240 MHZ, the Prescaler is equal to 2 so the TMRx clock 
      counter is equal to 80 MHz.
      The Three Timers are running at: 
      TMRx frequency = TMRx clock counter/ 2*(TIMx_Period + 1) = 500 KHz. 
      
  Set-up 
  - Connect the following pins to an oscilloscope to monitor the different waveforms:
      - TIM1 CH1 (PA.08)
 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
