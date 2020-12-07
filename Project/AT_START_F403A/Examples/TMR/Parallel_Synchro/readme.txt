/**
  @page TMR_Parallel_Synchro example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    TMR/Parallel_Synchro/readme.txt 
  * @author  ArteryTek Technology
  * @version V1.0.4
  * @date    2019-01-04
  * @brief   Description of the TMR_Parallel_Synchro example.
  ******************************************************************************
   @endverbatim

@par Example Description 
    This demo is based on the AT-START-F403A board,in this demo,PA6 ,PA0 and PB6 output pwm waveform.
    The TMRxCLK is fixed to 240   MHz, the TMR2 counter clock is 240 MHz.
    The Master TMRer TMR2 is running at 750 KHz and the duty cycle
    is equal to 25%
    The TMR3 is running:
    - At (TMR2 frequency)/ (TMR3 period + 1) = 75 KHz and a duty cycle
    equal to TMR3_CCR1/(TMR3_ARR + 1) = 30%
    The TMR4 is running:
    - At (TMR2 frequency)/ (TMR4 period + 1) = 150 KHz and a duty cycle
    equal to TMR4_CCR1/(TMR4_ARR + 1) = 60%  
	
* <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */

