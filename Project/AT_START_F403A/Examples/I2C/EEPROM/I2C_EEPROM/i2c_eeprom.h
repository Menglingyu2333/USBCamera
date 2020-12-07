/**
  ******************************************************************************
  * @file    I2C/EEPROM/I2C_EEPROM/i2c_eeprom.h
  * @author  Artery Technology 
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   The header file of eeprom driver.
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
#ifndef __I2C_EEPROM_H
#define __I2C_EEPROM_H

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"
#include <stdio.h>

/* Exported types ------------------------------------------------------------*/
typedef enum i2c_state
{
  COMM_DONE  = 0,    ///done successfully
  COMM_PRE = 1,
  COMM_IN_PROCESS = 2,
  COMM_EXIT = 3     ///exit since failure
}I2C_STATE;

typedef enum i2c_direction
{
  Transmitter=0x00,
  Receiver = 0x01
}I2C_DIRECTION;

/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/

/**
 * PROCESS MODE 
 * 0=polling 
 * 1=interrupt
 * 2=DMA
 */
#define PROCESS_MODE  2

#define I2C1_TEST
//#define I2C1_REMAP

#ifdef I2C1_TEST
  #define I2Cx    I2C1
  #ifdef I2C1_REMAP
  #define I2Cx_SCL_PIN    GPIO_Pins_8 
  #define I2Cx_SDA_PIN    GPIO_Pins_9 
  #define GPIOx           GPIOB 
  #else
  #define I2Cx_SCL_PIN    GPIO_Pins_6 
  #define I2Cx_SDA_PIN    GPIO_Pins_7 
  #define GPIOx           GPIOB 
  #endif
  #define I2Cx_peripheral_clock()  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_I2C1, ENABLE)
  #define I2Cx_scl_pin_clock()     RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE)
  #define I2Cx_sda_pin_clock()     RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE)
#endif

#define TEST_EEPROM_SIZE      256
#define TEST_EEPROM_ADDR      0x00
#define I2C_Speed             400000
#define EEPROM_ADDRESS        0xA0
#define I2C_PageSize          8    ///eeprom IC type AT24C02
#define sEE_FLAG_TIMEOUT      ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT      ((uint32_t)(100 * sEE_FLAG_TIMEOUT))

/** Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER   150
#define FALSE 0
#define TRUE  1

/* Exported functions ------------------------------------------------------- */
void I2C_EE_Init(void);
void I2C_EE_WriteBuffer(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_WriteOnePage(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_PageWrite(u8* pBuffer, u16 WriteAddr, u16 NumByteToWrite);
void I2C_EE_WriteOnePageCompleted(void);

void I2C_EE_ReadBuffer(u8* pBuffer, u16 ReadAddr, u16 NumByteToRead);
void I2C_EE_WaitOperationIsCompleted(void);
void I2C_EE_WaitEepromStandbyState(void);

void i2c1_evt_handle(void);
void i2c1_err_handle(void);
void i2c1_send_dma_handle(void);
void i2c1_receive_dma_handle(void);

#endif /* __I2C_EEPROM_H */

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
