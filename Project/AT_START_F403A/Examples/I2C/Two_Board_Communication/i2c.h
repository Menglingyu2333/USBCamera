/**
  ******************************************************************************
  * @file    I2C/Two_Board_Communication/i2c_eeprom.h
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
#include <stdio.h>
#include "at32f4xx.h"

/* Exported types ------------------------------------------------------------*/
typedef enum 
{
  PROC_OK       = 0x00,
  PROC_ERROR    = 0x01,
  PROC_BUSY     = 0x02,
  PROC_TIMEOUT  = 0x03
} PROC_StatusType;

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

#define I2Cx    I2C1

#define I2Cx_SCL_PIN    GPIO_Pins_6 
#define I2Cx_SDA_PIN    GPIO_Pins_7 
#define GPIOx           GPIOB 

#define I2Cx_peripheral_clock()  RCC_APB1PeriphClockCmd(RCC_APB1PERIPH_I2C1, ENABLE)
#define I2Cx_scl_pin_clock()     RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE)
#define I2Cx_sda_pin_clock()     RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOB, ENABLE)

#define WAIT_TIMEOUT      9000000

#define I2C_SPEED         100000
#define I2C_ADDRESS       0xB0
#define I2C_DUTYCYCLE     I2C_FmDutyCycle_2_1

/** Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define FALSE 0
#define TRUE  1

/* Exported functions ------------------------------------------------------- */
void AT32_I2C_Init(void);

void I2C1_Evt_handle(void);
void I2C1_Err_handle(void);

PROC_StatusType I2C_Master_Transmit_INT(uint8_t Addr, uint8_t *tx_buf, uint32_t size);
PROC_StatusType I2C_Slave_Receive_INT(uint8_t *rx_buf, uint32_t size);
PROC_StatusType I2C_Master_Receive_INT(uint8_t Addr, uint8_t *rx_buf, uint32_t size);
PROC_StatusType I2C_Slave_Transmit_INT(uint8_t *tx_buf, uint32_t size);

#endif /* __I2C_EEPROM_H */

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
