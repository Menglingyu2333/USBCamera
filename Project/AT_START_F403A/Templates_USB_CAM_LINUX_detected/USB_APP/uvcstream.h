#ifndef 	_UVCSTREAM_H_
#define		_UVCSTREAM_H_
#include "at32f4xx.h"

void UVC_SendPack_Irq(void);
void bufCopy(u8* srcPtr,u8* desPtr, u32 len);


#endif
