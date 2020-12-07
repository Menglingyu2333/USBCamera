#include "flash.h"
#include "at32_board.h"


#if FLASH_SIZE<256
#define SECTOR_SIZE 1024 //�ֽ�
#else 
#define SECTOR_SIZE	2048
#endif		 
u16 FLASH_BUF[SECTOR_SIZE/2];//�����2K�ֽ�

//��ȡָ����ַ�İ���(16λ����)
//faddr:����ַ(�˵�ַ����Ϊ2�ı���!!)
//����ֵ:��Ӧ����.
u16 FLASH_ReadHalfWord(u32 faddr)
{
	return *(vu16*)faddr; 
}


//������д��
//WriteAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��   
void FLASH_Write_NoCheck(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)   
{ 			 		 
	u16 i;
	for(i=0;i<NumToWrite;i++)
	{
		FLASH_ProgramHalfWord(WriteAddr,pBuffer[i]);
		WriteAddr+=2;//��ַ����2.
	}  
} 

//��ָ����ַ��ʼд��ָ�����ȵ�����
//WriteAddr:��ʼ��ַ(�˵�ַ����Ϊ2�ı���!!)
//pBuffer:����ָ��
//NumToWrite:����(16λ)��(����Ҫд���16λ���ݵĸ���.)
void FLASH_Write(u32 WriteAddr,u16 *pBuffer,u16 NumToWrite)	
{
	u32 secpos;	   //������ַ
	u16 secoff;	   //������ƫ�Ƶ�ַ(16λ�ּ���)
	u16 secremain; //������ʣ���ַ(16λ�ּ���)	   
	u16 i;    
	u32 offaddr;   //ȥ��0X08000000��ĵ�ַ
	
	if(WriteAddr<FLASH_BASE||(WriteAddr>=(FLASH_BASE+1024*FLASH_SIZE)))
		return;		//�Ƿ���ַ
	FLASH_Unlock();						//����
	offaddr=WriteAddr-FLASH_BASE;		//ʵ��ƫ�Ƶ�ַ.
	secpos=offaddr/SECTOR_SIZE;			//������ַ  0~512
	secoff=(offaddr%SECTOR_SIZE)/2;		//�������ڵ�ƫ��(2���ֽ�Ϊ������λ.)
	secremain=SECTOR_SIZE/2-secoff;		//����ʣ��ռ��С   
	if(NumToWrite<=secremain)
		secremain=NumToWrite;//�����ڸ�������Χ
	while(1) 
	{
		FLASH_Read(secpos*SECTOR_SIZE+FLASH_BASE,FLASH_BUF,SECTOR_SIZE/2);//������������������
		for(i=0;i<secremain;i++)//У������
		{
			if(FLASH_BUF[secoff+i]!=0XFFFF)
				break;//��Ҫ����
		}
		if(i<secremain)//��Ҫ����
		{
			FLASH_ErasePage(secpos*SECTOR_SIZE+FLASH_BASE);//�����������
			for(i=0;i<secremain;i++)//����
			{
				FLASH_BUF[i+secoff]=pBuffer[i];	  
			}
			FLASH_Write_NoCheck(secpos*SECTOR_SIZE+FLASH_BASE,FLASH_BUF,SECTOR_SIZE/2);//д����������  
		}
		else
		{
			FLASH_Write_NoCheck(WriteAddr,pBuffer,secremain);//д�Ѿ������˵�,ֱ��д������ʣ������. 		
		}
		if(NumToWrite==secremain)
			break;//д�������
		else//д��δ����
		{
			secpos++;				//������ַ��1
			secoff=0;				//ƫ��λ��Ϊ0 	 
			pBuffer+=secremain;  	//ָ��ƫ��
			WriteAddr+=(secremain*2);	//д��ַƫ��	   
			NumToWrite-=secremain;	//�ֽ�(16λ)���ݼ�
			if(NumToWrite>(SECTOR_SIZE/2))
				secremain=SECTOR_SIZE/2;//��һ����������д����
			else 
				secremain=NumToWrite;//��һ����������д����
		}
	}
	FLASH_Lock();//����
}

//��ָ����ַ��ʼ����ָ�����ȵ�����
//ReadAddr:��ʼ��ַ
//pBuffer:����ָ��
//NumToWrite:����(16λ)��
void FLASH_Read(u32 ReadAddr,u16 *pBuffer,u16 NumToRead)   	
{
	u16 i;
	
	for(i=0;i<NumToRead;i++)
	{
		pBuffer[i]=FLASH_ReadHalfWord(ReadAddr);//��ȡ2���ֽ�.
		ReadAddr+=2;//ƫ��2���ֽ�.	
	}
}

