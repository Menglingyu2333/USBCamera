/*
 **************************************************************************
 * File Name    : hw_config.h
 * Description  : Hardware Configuration & Setup
 * Date         : 2016-12-14
 * Version      : V1.0.0
 **************************************************************************
 */


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __HW_CONFIG_H
#define __HW_CONFIG_H

/* Includes ------------------------------------------------------------------*/
#include <at32f4xx.h>
#include "usb_type.h"


/* Exported types ------------------------------------------------------------*/
/* Codec Control defines */
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported define -----------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
/* External variables --------------------------------------------------------*/
#define USBCLK_FROM_HSE  1
#define USBCLK_FROM_HSI  2

void Set_USBClock(u8 Clk_Source);
void Enter_LowPowerMode(void);
void Leave_LowPowerMode(void);
void Get_SerialNum(void);
void USB_Interrupts_Config(void);
#endif  /*__HW_CONFIG_H*/


