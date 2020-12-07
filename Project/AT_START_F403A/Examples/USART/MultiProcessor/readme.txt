/**
  @page USART MultiProcessor example
  
  @verbatim
  ******************** (C) COPYRIGHT 2018 ArteryTek ****************************
  * @file    USART/MultiProcessor/readme.txt 
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Description of the USART MultiProcessor example.
  ******************************************************************************
   @endverbatim

@par Example Description 

  This demo provides a description of how to use the USART in multi-processor mode.
  
  First, the USART2 and USART3 address are set to 0x1 and 0x2. The USART2 send 
  continuously the character 0x33 to the USART3. The board toggle LED2, LED3 and
  LED4 pins while receiving 0x33.
  
  When a falling edge is applied on EXTI Line13, an interrupt is generatedand in 
  the EXTI15_10_IRQHandler routine, the USART3 is entered in mute mode and still
  in this mode (no LED toggling) until a rising edge is applied on BUTTON_WAKEUP 
  EXTI Line 0.
  
  In this interrupt routine the USART2 send the character of address mark (0x102)
  to wakeup USART3. The LED restart toggling.
  
  USART2 and USART3 configured as follow:
  - BaudRate = 9600 baud  
  - Word Length = 9 Bits
  - One Stop Bit
  - No parity
  - Hardware flow control disabled (RTS and CTS signals)
  - Receive and transmit enabled
  
  Set-up 
    - Connect USART2 Tx pin (PD.05) to USART3 Rx pin (PC.11)
    - Connect USART2 Rx pin (PD.06) to USART3 Tx pin (PC.10)
    - Use Key push-button connected to pin PC.13 (EXTI Line13)
    - Use Wakeup push-button connected to pin PA.00 (EXTI Line0) 

 * <h3><center>&copy; COPYRIGHT 2018 ArteryTek</center></h3>
 */
