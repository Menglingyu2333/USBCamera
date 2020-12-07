/**
  ******************************************************************************
  * @file    USB_Device/Mouse/src/main.c
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Mouse demo main file
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ARTERYTEK SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2018 ArteryTek</center></h2>
  ******************************************************************************
  */

#include "usb_lib.h"
#include "hw_config.h"
#include "usb_pwr.h"
#include "at32_board.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup USB_Mouse
  * @{
  */
  
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{		
    BUTTON_Type Button_Press;
    u8 keysta = 0;	
    
    /*AT START F4xx board initialize
    * Include LED, Button
    */
    AT32_Board_Init();
    
    /*USB GPIO configure*/
    AT32_USB_GPIO_init();
    
    /*Enable USB Interrut*/
 	  USB_Interrupts_Config();    
    
    /*Set USB Clock, USB Clock must 48MHz and clock source is HSE or HSI*/
    Set_USBClock(USBCLK_FROM_HSI);
    
    /*if use USB SRAM_Size = 768 Byte, default is 512 Byte*/
    //Set_USB768ByteMode();
    
    /* USB protocol and register initialize*/
 	  USB_Init();
    
    while(1)
	  {
        Button_Press = AT32_BUTTON_Press();
        switch (Button_Press)
        {

            case BUTTON_WAKEUP:
                Joystick_Send(2, 0, 0, 0); /*send mouse right button*/
                keysta = 1;
                break;
            default:
                break;
        }
        if ( keysta != 0 )
        {
            Delay_ms(30);
            keysta=0;
            Joystick_Send(0,0,0,0);       /*release the button*/
        }
        Delay_ms(40);
        AT32_LEDn_Toggle(LED4);
	  }
    
 							    			    
}

/**
  * @}
  */

/**
  * @}
  */


















