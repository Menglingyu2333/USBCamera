/*============================================================================
//  File Name       : Pld_Intf.h
//  Author          : HECC. DuckWeed Tech.
//                  @HuiZhou, 2011 
//  email:  hcc21cn@163.com	
//  Version         :   
//      V0.1    2011-10-16 ��������
//      
//  Description     : CPLD��չ�ӿ�
//
==============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLD_INTF_H_
#define __PLD_INTF_H_

#include "AppPlatForm.h"
//----------------------------------------------------------------------------
//  ��������
#define DRAM_REFRESH_ADDONS     1

//----------------------------------------------------------------------------
/* Public typedef -----------------------------------------------------------*/
    //����XBUF��д�����Ļ�������
typedef struct  tagXBufContext
{
    u16 fsmcpage;
    u16 fsmcrow;
    u16 colbase;
    u16 rowbase;
}   PldXBufContext;

/* Public define ------------------------------------------------------------*/
    //FSMC��չ�ڴ�Ļ���ַ����
    //0x60000000~0x6fffffff 4x64M�ռ�Ϊ�̶���NOR/SRAM��չ��
    //_NE[4:1]�ֱ�Ѱַ4�飬ÿ���ַ��FSMC_A[25:0]Ѱַ64M��
    // PLD���ʹ����FSMC_AD[15..0], �ø��÷�ʽѰַ��û���õ��ĸ�λ��ַ��(����Ϊ
    // GPIO_Mode_AF_PP)״̬����Ӱ�죬������ͨGPIO�á� 
    //  8Bit FSMC_MemoryDataWidthʱ��ϵͳ��ַ����HADDR[25..0]����FSMC_A[25..0]��
    //16Bit FSMC_MemoryDataWidthʱ, ϵͳ��ַ����HADDR[25..1]����FSMC_A[24..0]����
    //���ǳ������FSMC_A�ķ�ʽ������16Bit��ʽѰַ�ⲿMemoryʱ����ֵַҪ����һλ��
    //ѰַPixel��16Bit��ȣ�����1Pixel��ӦUInt16����(UInt16 *)ptr��Ϊ��ַ��ֱ��
    //��λ������UInt32ת��Ϊ��ַ��ÿPixelӦ��2�� 
#define FSMC_Bank1_SRAM1_BASE       ((u32)0x60000000)
#define FSMC_Bank1_SRAM3_BASE       ((u32)0x68000000)
//---------------
//  CPLD�Զ���Ĵ���
//---------------
//Reg-0: �ۺϿ��ƼĴ���
#define PLD_REG_CAPCTRL         0x8000          //->0x10000
    //Bits
    //b0: Enable Capture��ÿ֮֡���Զ����㣬Ҫ��VSYN rise edgeд1
#define PLD_REGBIT_CAPEN        0x0001         
    //b1: RGB565(1)��ʽ or YUV(0)��ʽ. YUV��ʽ��LCDֻ��ʾY
#define PLD_REGBIT_RGB565       0x0002
    //b2: ��ǰFrameͼ��洢�õ�Mem Page
#define PLD_REGBIT_CAPWRPAGE    0x0004      //
//x    //b3: Cameraʱ��Ƶ��ѡ��MT9M111=20M(1), ov7670=40M(0)
    //b3: Cameraʱ��Ƶ��ѡ��MT9M111=20M(1), ov7670=10M(0)
#define PLD_REGBIT_XCLK20M      0x0008      //
    //b4: SDRAMˢ�´���
#define PLD_REGBIT_REFTRIG      0x0010
    //b8: LCD
#define PLD_REGBIT_LCDVIDEOEN   0x0100      //Enable /disable Video��ʾ
#define PLD_REGBIT_LCDSCALE     0x0600      //2Bit Scale, 00->2:1, 01->4:1, 10->5:1, 11����
    //Scale����
    #define PLDMSK_LCDSCALE2        0x0000
    #define PLDMSK_LCDSCALE4        0x0200
    #define PLDMSK_LCDSCALE5        0x0400

//Reg-1: FSMC�ӿ�SDRAMҳѡ��
#define PLD_REG_FSMCPAGE        0xA000      //->0x14000
    //��ЧBits: (1)
    //b2: ��ǰFSMC��ȡ��Mem Page
#define PLD_REGBIT_FSMCPAGE     0x0004
    #define PLD_FSMC_PAGE0          0x0000
    #define PLD_FSMC_PAGE1          0x0004
    
//Reg-2: ��д��ͼ��/Memory�е�ַ
#define PLD_REG_ROWSEL          0xC000      //->0x18000

//Reg-3: ��չLCD���Ƹ���
#define PLD_REG_LCDAUX          0xE000      //->0x1C000
//#define PLD_REG_EXPORT          0xE000      //->0x1C000
    //��ЧBits: (1)
    //b2: =1 LCD DataPort, LCD_WR output Enable; = 0�������̬
#define PLD_REGBIT_LCDPORTOE    0x0004

    //-------------
#define PLD_XBUF_STARTCOL       (1280+256)      //�����XBUF Column���
#define PLD_XBUF_ROWLIM         512             //����XBUF Row number����
/* Public macro -------------------------------------------------------------*/
//-------------------------------------
//	�꺯��
#define Pld_UpdateCtrlReg()         {*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x10000) = pld_RegCtrl;  }
    //����FSMC�ӿڶ�д��SDRAMҳ
#define Pld_SetFsmcPage(page)       {*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x14000) = page;  \
            pld_RegFsmcPage = page; }
#define PLD_SetXBufPage             Pld_SetFsmcPage

    //���ö�д��ͼ��/Memory�мĴ�����4Mx16 DRAM�汾֧��0~1023, 8Mx16�汾֧��0~2047
#define Pld_SelectImgRow(row)       {*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x18000) = row;   }
    //���ö�д��ͼ��/Memory�������ص�ַ��ÿ������16Bit��ÿ�г���֧��2048Pixel��
#define Pld_PixelPtr(xaddr)         ((vu16 *)(FSMC_Bank1_SRAM1_BASE + (u32)(xaddr<<1)))
#define Pld_MemPtrBase()            ((vu16 *)FSMC_Bank1_SRAM1_BASE)

#define Pld_LCDPort_OutEn()         {*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x1C000) = 0xff;       }
#define Pld_LCDPort_OutDis()        {*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x1C000) = 0x00;       }

    //����PLD�ڻ����ͼ��/Memory�мĴ���ֵ(Pld_SelectImgRow()д��ֵ)
#define Pld_ReadImgRowAddr()        (*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x18000))
/* Public variables ---------------------------------------------------------*/
//  ���ñ���
extern volatile Bool    g_Img_CapEnable;
extern volatile Bool    LCD_VideoEnable;

extern vu16 pld_RegFsmcPage;

/* Public function prototypes -----------------------------------------------*/
/* Publice functions ---------------------------------------------------------*/
//Application ���ú���
void CAMERA_Ctrl_Init(void);
void FSMC_Mem_Init(void);
void PLD_WriteCtrlReg(u16 mask, u16 value);
void PLD_Init(Bool cap_enable);

void PLD_CapFrameServe_Irq(void);
u16 PLD_LockImageFrame(Bool on);
void PLD_DRAM_Refresh(void);

void PLD_CaptureRun(Bool on);
void PLD_SetVideo2LCD(Bool on, u16 lcd_scale);

u32 PLD_SetupXBuf(u16 rowbase, u16 colbase);
u32 PLD_WriteToXBuf(u8 *srcbuf, s32 byteNum, u32 XBufPtr);
u32 PLD_ReadFromXBuf(u8 *dstbuf, s32 byteNum, u32 XBufPtr);

PldXBufContext PLD_GetXBufContext(void);
void PLD_SetXBufContext(PldXBufContext contx);

s32 PLD_TestDRam(u16 pagesel, s32 yaddr);

#endif /* __PLD_INTF_H */
