#ifndef __FLASH_H__
#define __FLASH_H__
#include "sys.h"  

// 所选AT32的FLASH SIZE(单位为KByte)
#define FLASH_SIZE  (*((uint32_t*)0x1FFFF7E0))    /* Read from flash capacity register */

// 所选AT32的PAGE SIZE(单位为Byte)
#if defined (AT32F403xx) || defined (AT32F403Axx) || defined (AT32F407xx)
  #if  defined (AT32F403Cx_MD)
    #define PAGE_SIZE 		1024
		#define SRAM_SIZE 		32
	#else 
    #define PAGE_SIZE 		2048
		#define SRAM_SIZE 		224
	#endif
#endif

#if defined AT32F413xx
	#if defined (AT32F413Kx_HD) || defined (AT32F413Cx_HD) || defined (AT32F413Rx_HD) || defined (AT32FEBKCx_HD)
    #define PAGE_SIZE 		2048
		#define SRAM_SIZE 		64
	#elif defined (AT32F413Kx_MD) || defined (AT32F413Cx_MD) || defined (AT32F413Rx_MD) || defined (AT32FEBKCx_MD) || defined (AT32FEBKCx_MD)
    #define PAGE_SIZE 		1024
		#define SRAM_SIZE 		64
	#endif
#endif

#if defined AT32F415xx
  #if defined (AT32F415CCT7)  || defined (AT32F415CCU7)  || defined (AT32F415RCT7)  || defined (AT32F415KCU7_4)|| defined (AT32F415RCT7_7)
    #define PAGE_SIZE 		2048
	#else
    #define PAGE_SIZE 		1024	
	#endif	
		#define SRAM_SIZE 		32
#endif

u8 FLASH_Read_Upgrade_Flag(void);
void FLASH_Write_2KB(u32 WriteAddr, u8 *pBuffer);

#endif
