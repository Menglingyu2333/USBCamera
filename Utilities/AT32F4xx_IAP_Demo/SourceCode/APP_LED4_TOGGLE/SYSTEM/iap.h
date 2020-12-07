#ifndef __IAP_H__
#define __IAP_H__
#include "at32f4xx.h"

#define APP_START_ADDR						0x08004000
#define IAP_UPGRADE_FLAG_ADDR			(APP_START_ADDR-2048)
#define IAP_UPGRADE_FLAG 					0x41544B38 

void IAP_Command_Handle(void);
void IAP_Init(void);

#endif

