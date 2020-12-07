/**
  ******************************************************************************
  * @file    USB_Device/USB_Audio_Demo/src/main.c
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   USB Audio Demo main file
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
#include "at32_audio_conf.h"
#include "wm8988.h"

/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup USB_Audio_Demo
  * @{
  */ 
  
int main(void)
{	
    /*AT START F4xx board initialize
    * Include LED, Button
    */
    AT32_Board_Init();
    
    /*USB GPIO configure*/
    AT32_USB_GPIO_init();
    
    /*Enable USB Interrut*/
    USB_Interrupts_Config();    
    
    /*Set USB Clock, USB Clock must 48MHz and clock source is HSE or HSI*/
    Set_USBClock(USBCLK_FROM_HSE);
    
    /*WM8988 initialize*/
    if ( Audio_Init(AT32_DEFAULT_AUDIO_FREQ, AT32_DEFAULT_AUDIO_BITW) != AUDIO_OK )
    {
        /*WM8988 init Failed*/
        AT32_LEDn_Toggle(LED2);
    }      
    
    /*if use USB SRAM_Size = 768 Byte, default is 512 Byte*/
    Set_USB768ByteMode();
    
    /* USB protocol and register initialize*/
    USB_Init();         
    while(1)
    {
          AT32_LEDn_Toggle(LED4);
          WM8988_Poll();
          Delay_ms(200);

    }	   										    			    
}
/**
  * @}
  */

/**
  * @}
  */






















