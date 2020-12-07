/**
  ******************************************************************************
  * @file    FLASH/operate_SPIM/SPIM/SPIM_test.h
  * @author  Artery Technology 
  * @version V1.1.2
  * @date    2019-01-04
  * @brief   This file contains all the macros used in SPIM_test.c
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
#ifndef __SPIM_TEST_H
#define __SPIM_TEST_H

/* Define External SPI flash type, define only one ---------------------------*/
#define FLASH_SPIM_TYPE1  1
#define FLASH_SPIM_TYPE2  2

/* Define External SPI flash size --------------------------------------------*/
#define SPIM_PAGE_SIZE    4096

/* Define External SPI flash unlock key --------------------------------------*/
#define FLASH_KEY1                      ((UINT32)0x45670123)
#define FLASH_KEY2                      ((UINT32)0xCDEF89AB)

/* Define External SPI flash testing address ---------------------------------*/
#define SPIM_TEST_ADDR   0x8400000

/* Exported functions ------------------------------------------------------- */
void SPIM_test(void);

#endif /* __SPIM_TEST_H */

/******************* (C) COPYRIGHT 2018 ArteryTek *****END OF FILE****/
