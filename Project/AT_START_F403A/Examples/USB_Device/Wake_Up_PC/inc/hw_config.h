/**
  ******************************************************************************
  * @file    USB_Device/Mouse/inc/hw_config.h
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   USB Hardware Configuration & setup header
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

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <at32f4xx.h>
#include "usb_type.h"


/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
#define USBCLK_FROM_HSE  1
#define USBCLK_FROM_HSI  2

#define DOWN            1
#define LEFT            2
#define RIGHT           3
#define UP              4
#define CURSOR_STEP     40

/* Exported functions ------------------------------------------------------- */
void Set_System(void);
void Set_USBClock(u8 Clk_Source);
void GPIO_AINConfig(void);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void USB_Interrupts_Config(void);
void USB_Cable_Config (FunctionalState NewState);
void Joystick_Send(u8 buf0,u8 buf1,u8 buf2,u8 buf3);
u8 JoyState(void);
void Get_SerialNum(void);

#endif  /*__HW_CONFIG_H*/


