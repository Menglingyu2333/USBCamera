 /**
  ******************************************************************************
  * @file    USB_Device/MassStorage/src/main.c
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   MassStorage demo main file
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
#include "mass_mal.h"
/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup USB_MassStorage
  * @{
  */ 
extern uint32_t  Data_Buffer[];

int main(void)
{
    uint8_t u8FlashLed = LED4;
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
    
    /*init SD card*/
    if ( MAL_Init(0) != MAL_OK )
    {
        /*no SD Card connect*/
        u8FlashLed = LED2;
    }
    /*if use USB SRAM_Size = 768 Byte, default is 512 Byte*/
    //Set_USB768ByteMode();
    
    /* USB protocol and register initialize*/
   	USB_Init();
    
 	  while(1)
	  {
        Delay_ms(500);
        AT32_LEDn_Toggle((LED_Type)u8FlashLed);
    }
}

/**
  * @}
  */

/**
  * @}
  */



















