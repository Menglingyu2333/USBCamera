/**
  ******************************************************************************
  * @file    touch.h
  * @author  Artery
  * @version V4.5.0
  * @date    07-March-2011
  * @brief   This file contains all the functions prototypes for the 
  *          touch firmware driver.
  ******************************************************************************
  * @attention
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, ARTERY SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2011 Artery</center></h2>
  ******************************************************************************  
  */ 
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __TOUCH_H
#define __TOUCH_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
   
#include "at32f4xx.h"
#include "at32_board.h"
   
/** @addtogroup XMC_LCD&TOUCH_16BIT
  * @{
  */ 
  
/** @addtogroup XMC_Utilities
  * @{
  */
 

/** @defgroup AT32F403A_XMC_LCD_Exported_Functions
  * @{
  */ 
   
   
typedef struct
{
  void (*init)(void);                /* PIN init */
  u8 (*touch_read_xy)(u16 *x,u16 *y);/* read x/y coordinate */
  u8 (*touch_scan)(void);            /* touch scan func */
  u8 (*touch_adjust)(void);          /* touch adjust func */
  u16 X_P[6];                        /* save x coordinate */
  u16 Y_P[6];                        /* save y coordinate */
  
}_touch_dev;

extern _touch_dev  touch_dev;
extern _touch_dev *touch;

#define TCLK_HIGH GPIO_SetBits(GPIOB,GPIO_Pins_3);
#define TCLK_LOW GPIO_ResetBits(GPIOB,GPIO_Pins_3);

#define TDOUT GPIO_ReadInputDataBit(GPIOB,GPIO_Pins_4);

#define TDIN_HIGH GPIO_SetBits(GPIOB,GPIO_Pins_5);
#define TDIN_LOW GPIO_ResetBits(GPIOB,GPIO_Pins_5);

#define PEN_HIGH GPIO_ReadInputDataBit(GPIOD,GPIO_Pins_5);

#define TCS_HIGH GPIO_SetBits(GPIOB,GPIO_Pins_8);
#define TCS_LOW GPIO_ResetBits(GPIOB,GPIO_Pins_8);

void TOUCH_PIN_Init(void);
u8 TOUCH_Read_XY(u16 *x,u16 *y);
u8 TOUCH_Scan(void);
u16 TOUCH_Read_X_OR_Y(u8 xy);
u16 TOUCH_Read_AD(u8 xy);
void TOUCH_Write_1byte(u8 CMD);
u8 TOUCH_Read_1byte(void);
u8 TOUCH_Adjust(void); 

#ifdef __cplusplus
}
#endif

#endif /* __AT_START_XMC_LCD&TOUCH_16BIT_H */
/**
  * @}
  */

/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2011 Artery *****END OF FILE****/



