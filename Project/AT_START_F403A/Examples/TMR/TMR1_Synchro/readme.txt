/**
  @page TMR1_Synchro example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    TMR/TMR1_Synchro/readme.txt 
  * @author  ArteryTek Technology
  * @version V1.0.4
  * @date    2019-01-04
  * @brief   Description of the TMR1_Synchro example.
  ******************************************************************************
   @endverbatim

@par Example Description 
    This demo is based on the AT-START-F403A board,in this demo,PA6 ,PA8 and PB6 output pwm waveform.    
    The Master TMRer TMR1 is running at:
    TMR1 frequency = TMR1 counter clock / (TMR1_Period + 1) = 750 KHz
    and the duty cycle is equal to: TMR1_CCR1/(TMR1_ARR + 1) = 50%

    The TMR3 is running at: 
    (TMR1 frequency)/ ((TMR3 period +1)* (Repetition_Counter+1)) = 49.95 KHz and
    a duty cycle equal to TMR3_CCR1/(TMR3_ARR + 1) = 33.3%

    The TMR4 is running at:
    (TMR1 frequency)/ ((TMR4 period +1)* (Repetition_Counter+1)) = 74.96 KHz and
    a duty cycle equal to TMR4_CCR1/(TMR4_ARR + 1) = 50%    

 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */