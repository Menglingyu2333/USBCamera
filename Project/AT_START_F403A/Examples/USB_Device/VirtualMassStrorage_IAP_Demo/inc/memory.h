/**
  ******************************************************************************
  * @file    USB_Device/VirtualMassStorage_IAP_Demo/inc/memory.h
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   Memory management layer
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
#ifndef __memory_H
#define __memory_H

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define TXFR_IDLE     0
#define TXFR_ONGOING  1

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Write_Memory (uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length);
void Read_Memory (uint8_t lun, uint32_t Memory_Offset, uint32_t Transfer_Length);
#endif /* __memory_H */

/************************ (C) COPYRIGHT Arterytek *****END OF FILE****/
