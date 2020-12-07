/**
  ******************************************************************************
  * @file    CortexM4/FPU/at32f4xx_julia.h 
  * @author  Artery Technology
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   at32f4xx julia header file
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
#ifndef __AT32F4xx_JULIA_H
#define __AT32F4xx_JULIA_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"

/** @addtogroup AT32F403A_StdPeriph_Examples
  * @{
  */

/** @addtogroup CortexM4_FPU
  * @{
  */ 

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define  ITERATION      	((uint32_t)128)
#define	 REAL_CONSTANT		(0.285f)
#define	 IMG_CONSTANT		(0.01f)
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void GenerateJulia_fpu(uint16_t size_x,
                       uint16_t size_y,
					   uint16_t offset_x,
					   uint16_t offset_y,
					   uint16_t zoom,
					   uint8_t * buffer);

#ifdef __cplusplus
}
#endif

#endif /* __AT32F4xx_JULIA_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/ 
