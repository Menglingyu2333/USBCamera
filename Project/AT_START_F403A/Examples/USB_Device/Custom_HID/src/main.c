/**
  ******************************************************************************
  * @file    USB_Device/Custom_HID/src/main.c
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Custom HID demo main file
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


uint8_t usb_recvBuffer[256];
uint8_t usb_sendBuffer[256];
uint16_t recvLen, sendLen;

/** @addtogroup AT32F413_StdPeriph_Examples
  * @{
  */

/** @addtogroup USB_Custom_HID
  * @{
  */ 
/**
  * @brief  Main program
  * @param  None
  * @retval None
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
    Set_USBClock(USBCLK_FROM_HSI);
    
    /*if use USB SRAM_Size = 768 Byte, default is 512 Byte*/
    //Set_USB768ByteMode();
    
    /* USB protocol and register initialize*/
 	  USB_Init();
    
 	  while(1)
	  {
      
      recvLen = CDC_Receive_DATA(usb_recvBuffer, 256);
        if ( recvLen > 0 )
        {  
          /*recvive data from USB*/
          /*Send data to PC Host*/
          sendLen = CDC_Send_DATA(usb_recvBuffer, recvLen);
        }else
        {
          /*no data recv*/
          Delay_ms(500);        
        }
        
        
	  }	   										    			    
}

/**
  * @}
  */ 

/**
  * @}
  */





















