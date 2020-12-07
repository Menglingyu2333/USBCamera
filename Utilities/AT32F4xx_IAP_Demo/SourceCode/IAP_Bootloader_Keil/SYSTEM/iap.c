#include "iap.h"
#include "usart.h"
#include "flash.h"
#include "led.h"

CMD_DATA_STEP cmd_data_flag=CMD_DATA_IDLE;
CMD_CTR_STEP cmd_ctr_flag=CMD_CTR_IDLE;
u8 update_status=UPDATE_PRE;
static u8 cmd_addr_cnt=0;
static u32 cmd_data_cnt=0;
CMD_DATA_STRUCT cmd_data_struct;
iapfun jump2app;

//跳转到应用程序段
//appxaddr:用户代码起始地址.
#pragma O0																				//IAP_Load_APP不进行优化
void IAP_Load_APP(u32 appxaddr)
{
	if( ((*(vu32*)appxaddr)-0x20000000) < (SRAM_SIZE*1024) )	//检查栈顶地址是否合法.
	{
		jump2app=(iapfun)*(vu32*)(appxaddr+4);				//用户代码区第二个字为程序开始地址(复位地址)		
		MSR_MSP(*(vu32*)appxaddr);										//初始化APP堆栈指针(用户代码区的第一个字用于存放栈顶地址)
		jump2app();																		//跳转到APP.
	}
}		

u8 take_data(void)
{
  u8 Val;
  USART_INTConfig(USART1, USART_INT_RDNE, DISABLE);
  UART1_struct.count--;
  USART_INTConfig(USART1, USART_INT_RDNE, ENABLE);
  Val = UART1_struct.buf[UART1_struct.tail++];
	
  if(UART1_struct.tail>(USART_REC_LEN-1))
    UART1_struct.tail = 0; 
	
  return Val;
}
 
void Command_Handle(void)
{
  u8 Val,checksum;
  u16 i;
	
	if(UART1_struct.count>0)   
		Val = take_data();
	else
		return;
	
	if(update_status==UPDATE_PRE)
	{
		if(cmd_ctr_flag==CMD_CTR_IDLE)
		{
			if(Val==0x5A)
				cmd_ctr_flag = CMD_CTR_INIT;
		}
		else if(cmd_ctr_flag==CMD_CTR_INIT)
		{
			if(Val==0x01)
			{
				cmd_ctr_flag = CMD_CTR_DONE;
			}
			else if(Val==0xA5)
			{
				cmd_ctr_flag = CMD_CTR_APP;
			}
			else
			{
				cmd_ctr_flag = CMD_CTR_ERR;
			}
		}
	}
	else if(update_status==UPDATE_ING)
  {
		switch(cmd_data_flag)
		{
			case CMD_DATA_IDLE:
				if(Val==0x31)
				{
					cmd_data_flag=CMD_DATA_ADDR;
					cmd_addr_cnt=0;
					cmd_data_cnt=0;
				}
				if(Val==0x5A)
				{
					if(cmd_ctr_flag==CMD_CTR_IDLE)
					{
					cmd_ctr_flag = CMD_CTR_INIT;
					update_status = UPDATE_DONE;
					}
				}
				break;
			case CMD_DATA_ADDR:
				cmd_data_struct.cmd_addr[cmd_addr_cnt]=Val;
				cmd_addr_cnt++;
				if(cmd_addr_cnt>=4)
				{
					cmd_addr_cnt=0;
					cmd_data_flag=CMD_DATA_BUF;
				}
				break;
			case CMD_DATA_BUF:
				cmd_data_struct.cmd_buf[cmd_data_cnt]=Val;
				cmd_data_cnt++;
				if(cmd_data_cnt>=2048)
				{
					cmd_data_cnt=0;
					cmd_data_flag=CMD_DATA_CHACK;
				}
				break;
			case CMD_DATA_CHACK:
				checksum=0;
				for(i=0;i<4;i++)
				{
					checksum+=cmd_data_struct.cmd_addr[i];
				}
				for(i=0;i<2048;i++)
				{
					checksum+=cmd_data_struct.cmd_buf[i];
				}
				if(checksum==Val)
				{
					cmd_data_flag=CMD_DATA_DONE;
				}
				else
				{
					cmd_data_flag=CMD_DATA_ERR;
				}
				break;
			default:
				break;  
     }      
  }
	else if(update_status==UPDATE_DONE)
	{
		if(cmd_ctr_flag==CMD_CTR_IDLE)
    {
			if(Val==0x5A)
      {
				cmd_ctr_flag = CMD_CTR_INIT;
      }
    }
		else if(cmd_ctr_flag==CMD_CTR_INIT)
		{
			if(Val==0x02)
			{
				cmd_ctr_flag = CMD_CTR_DONE;
			}
			else
			{
				cmd_ctr_flag = CMD_CTR_ERR;
			}
		}
	}
}

void Back_Ok(void)
{
	USART1->DT = 0xCC;    
	while((USART1->STS&0x40)==0); 
	USART1->DT = 0xDD;   
	while((USART1->STS&0x40)==0);
}

void Back_Err(void)
{
	USART1->DT = 0xEE;    
	while((USART1->STS&0x40)==0); 
	USART1->DT = 0xFF;  
	while((USART1->STS&0x40)==0);

	cmd_data_flag=CMD_DATA_IDLE;
	cmd_ctr_flag=CMD_CTR_IDLE;
	update_status=UPDATE_PRE;
	cmd_addr_cnt=0;
	cmd_data_cnt=0;	
	TIME2_IRA_CNT=0;
	GetDaraFromUSART_Flag=0;
}

void APP_Update_Handler(void)
{
	u32 write_addr=0;
	
	if(update_status==UPDATE_PRE)
	{
		if(cmd_ctr_flag==CMD_CTR_DONE)
		{
			cmd_ctr_flag = CMD_CTR_IDLE;
			update_status = UPDATE_CLEAR_FLAG;
		}
		else if(cmd_ctr_flag==CMD_CTR_APP)
		{
			cmd_ctr_flag = CMD_CTR_IDLE;
			Back_Ok();
		}
		else if(cmd_ctr_flag==CMD_CTR_ERR)
		{
			cmd_ctr_flag = CMD_CTR_IDLE;
			Back_Err();
		}
	}
	else if(update_status==UPDATE_CLEAR_FLAG)
	{
		GetDaraFromUSART_Flag=1;	// 作为timer out判断依据
		update_status = UPDATE_ING;
		Back_Ok();
	}
	else if(update_status==UPDATE_ING)
	{
		if(cmd_data_flag==CMD_DATA_DONE)
		{
			write_addr = (cmd_data_struct.cmd_addr[0]<<24)+(cmd_data_struct.cmd_addr[1]<<16)+(cmd_data_struct.cmd_addr[2]<<8)+cmd_data_struct.cmd_addr[3];
			if((write_addr>=APP_START_ADDR)&&(write_addr<FLASH_BASE+1024*FLASH_SIZE))
			{			
				FLASH_Write_2KB(write_addr, cmd_data_struct.cmd_buf);
				cmd_data_flag = CMD_DATA_IDLE;
				Back_Ok();
			}
			else
			{
				cmd_data_flag = CMD_DATA_IDLE;
				Back_Err();
			}
		}
		else if(cmd_data_flag==CMD_DATA_ERR)
		{
			cmd_data_flag = CMD_DATA_IDLE;
			Back_Err();
		}
	}
	else if(update_status==UPDATE_DONE)
	{
		if(cmd_ctr_flag==CMD_CTR_DONE)
		{
			//FLASH_Write_Flag();
			cmd_ctr_flag = CMD_CTR_IDLE;
			Back_Ok();
			if(((*(vu32*)(APP_START_ADDR+4))&0xFF000000)==0x08000000)	//判断是否为0x08XXXXXX.
			{
				RCC->APB1RST = 0xFFFFFFFF;
				RCC->APB2RST = 0xFFFFFFFF;
				RCC->APB1RST = 0x00000000;
				RCC->APB2RST = 0x00000000;
				IAP_Load_APP(APP_START_ADDR);//执行FLASH APP代码
			}
			else 
			{
				cmd_ctr_flag=CMD_CTR_ERR;
				//printf("非FLASH应用程序,无法执行!\r\n");
			}		
		}
		else if(cmd_ctr_flag==CMD_CTR_ERR)
		{
			cmd_ctr_flag = CMD_CTR_IDLE;
			Back_Err();
		}
	}	
}

void IAP_Upgrade_APP_Handle(void)
{
    Command_Handle();
		APP_Update_Handler();
}
