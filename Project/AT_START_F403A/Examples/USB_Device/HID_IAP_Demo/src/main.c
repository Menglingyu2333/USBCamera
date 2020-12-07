/**
  ******************************************************************************
  * @file    USB_Device/HID_IAP_Demo/src/main.c
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   HID IAP  demo main file
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
#include "iap_user.h"
#include "at_spi_flash.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup USB_HID_IAP_Demo
  * @{
  */ 
/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{	
    uint32_t LedTimer = 0, LedTog = SystemCoreClock/40;
    /*AT START F4xx board initialize
    * Include LED, Button
    */
    AT32_Board_Init();
    /*Check if the key Push button*/
    if ( AT32_BUTTON_State(BUTTON_WAKEUP) == 0x00)
    {
        /*Check Flag*/
        if (IAP_get_flag() == IAP_SUCCESS)
        {
            /*Jump to User Code*/
            IAP_Jump(FLASH_APP1_ADDR);
        }
    }
     
    /*USB GPIO configure*/
    AT32_USB_GPIO_init();
    
    /*Flash SPI init*/
    Flash_spi_init();
    
    /*Enable USB Interrut*/
 	  USB_Interrupts_Config();    
    
    /*Set USB Clock, USB Clock must 48MHz and clock source is HSE or HSI*/
    Set_USBClock(USBCLK_FROM_HSI);
    
    /* USB protocol and register initialize*/
 	  USB_Init();
    
 	  while(1)
	  {
        IAP_handle_loop();
        if ( LedTimer == LedTog)
        {
            AT32_LEDn_Toggle(LED4);
            LedTimer = 0;
        }
        LedTimer ++;
        
        
	  }	   										    			    
}

/**
  * @}
  */ 

/**
  * @}
  */





















