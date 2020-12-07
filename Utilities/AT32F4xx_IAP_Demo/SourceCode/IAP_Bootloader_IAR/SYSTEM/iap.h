#ifndef __IAP_H__
#define __IAP_H__
#include "at32f4xx.h"

#define APP_START_ADDR					0x08004000  						// 0x08004000~0x0800FFFF�Ŀռ�ΪAPPʹ��
#define IAP_UPGRADE_FLAG_ADDR		(APP_START_ADDR-2048)		// IAP��ַ���һ��Page��Flag 
#define IAP_UPGRADE_FLAG 				0x41544B38 

typedef struct{
	uint8_t cmd_head;
	uint8_t cmd_addr[4];
	uint8_t cmd_buf[2048];
	uint8_t cmd_check;	
}CMD_DATA_STRUCT;

typedef enum{
	CMD_DATA_IDLE,
	CMD_DATA_ADDR,
	CMD_DATA_BUF,
	CMD_DATA_CHACK,
	CMD_DATA_DONE,
	CMD_DATA_ERR,
}CMD_DATA_STEP;

typedef enum{
	CMD_CTR_IDLE,
	CMD_CTR_INIT,
	CMD_CTR_DONE,
	CMD_CTR_ERR,
	CMD_CTR_APP,
}CMD_CTR_STEP;

typedef enum{
	UPDATE_PRE,
	UPDATE_CLEAR_FLAG,
	UPDATE_ING,
	UPDATE_DONE,
}UPDATE_STATUS;

extern CMD_DATA_STRUCT cmd_data_struct;
extern CMD_DATA_STEP cmd_data_flag;
extern CMD_CTR_STEP cmd_ctr_flag;
extern u8 update_status;
void Command_Handle(void);
void Back_Err(void);
void Back_Ok(void);
typedef void (*iapfun)(void);				//����һ���������͵Ĳ���.
void IAP_Upgrade_APP_Handle(void);
void IAP_Load_APP(u32 appxaddr);
	
#endif
