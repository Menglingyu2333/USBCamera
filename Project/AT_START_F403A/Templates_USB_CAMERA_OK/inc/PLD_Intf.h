/*============================================================================
//  File Name       : Pld_Intf.h
//  Author          : HECC. DuckWeed Tech.
//                  @HuiZhou, 2011 
//  email:  hcc21cn@163.com	
//  Version         :   
//      V0.1    2011-10-16 基本功能
//      
//  Description     : CPLD扩展接口
//
==============================================================================*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __PLD_INTF_H_
#define __PLD_INTF_H_

#include "AppPlatForm.h"
//----------------------------------------------------------------------------
//  条件配置
#define DRAM_REFRESH_ADDONS     1

//----------------------------------------------------------------------------
/* Public typedef -----------------------------------------------------------*/
    //保存XBUF读写关联的环境变量
typedef struct  tagXBufContext
{
    u16 fsmcpage;
    u16 fsmcrow;
    u16 colbase;
    u16 rowbase;
}   PldXBufContext;

/* Public define ------------------------------------------------------------*/
    //FSMC扩展内存的基地址定义
    //0x60000000~0x6fffffff 4x64M空间为固定的NOR/SRAM扩展区
    //_NE[4:1]分别寻址4块，每块地址线FSMC_A[25:0]寻址64M。
    // PLD设计使用了FSMC_AD[15..0], 用复用方式寻址。没有用到的高位地址线(不设为
    // GPIO_Mode_AF_PP)状态不受影响，可作普通GPIO用。 
    //  8Bit FSMC_MemoryDataWidth时，系统地址总线HADDR[25..0]连接FSMC_A[25..0]，
    //16Bit FSMC_MemoryDataWidth时, 系统地址总线HADDR[25..1]连接FSMC_A[24..0]，即
    //总是充分利用FSMC_A的方式。这样16Bit方式寻址外部Memory时，地址值要左移一位，
    //寻址Pixel是16Bit宽度，隐含1Pixel对应UInt16，用(UInt16 *)ptr作为地址可直接
    //进位，如用UInt32转换为地址，每Pixel应进2。 
#define FSMC_Bank1_SRAM1_BASE       ((u32)0x60000000)
#define FSMC_Bank1_SRAM3_BASE       ((u32)0x68000000)
//---------------
//  CPLD自定义寄存器
//---------------
//Reg-0: 综合控制寄存器
#define PLD_REG_CAPCTRL         0x8000          //->0x10000
    //Bits
    //b0: Enable Capture，每帧之后自动清零，要在VSYN rise edge写1
#define PLD_REGBIT_CAPEN        0x0001         
    //b1: RGB565(1)格式 or YUV(0)格式. YUV格式，LCD只显示Y
#define PLD_REGBIT_RGB565       0x0002
    //b2: 当前Frame图像存储用的Mem Page
#define PLD_REGBIT_CAPWRPAGE    0x0004      //
//x    //b3: Camera时钟频率选择，MT9M111=20M(1), ov7670=40M(0)
    //b3: Camera时钟频率选择，MT9M111=20M(1), ov7670=10M(0)
#define PLD_REGBIT_XCLK20M      0x0008      //
    //b4: SDRAM刷新触发
#define PLD_REGBIT_REFTRIG      0x0010
    //b8: LCD
#define PLD_REGBIT_LCDVIDEOEN   0x0100      //Enable /disable Video显示
#define PLD_REGBIT_LCDSCALE     0x0600      //2Bit Scale, 00->2:1, 01->4:1, 10->5:1, 11保留
    //Scale常数
    #define PLDMSK_LCDSCALE2        0x0000
    #define PLDMSK_LCDSCALE4        0x0200
    #define PLDMSK_LCDSCALE5        0x0400

//Reg-1: FSMC接口SDRAM页选择
#define PLD_REG_FSMCPAGE        0xA000      //->0x14000
    //有效Bits: (1)
    //b2: 当前FSMC读取的Mem Page
#define PLD_REGBIT_FSMCPAGE     0x0004
    #define PLD_FSMC_PAGE0          0x0000
    #define PLD_FSMC_PAGE1          0x0004
    
//Reg-2: 读写的图像/Memory行地址
#define PLD_REG_ROWSEL          0xC000      //->0x18000

//Reg-3: 扩展LCD控制辅助
#define PLD_REG_LCDAUX          0xE000      //->0x1C000
//#define PLD_REG_EXPORT          0xE000      //->0x1C000
    //有效Bits: (1)
    //b2: =1 LCD DataPort, LCD_WR output Enable; = 0输出高阻态
#define PLD_REGBIT_LCDPORTOE    0x0004

    //-------------
#define PLD_XBUF_STARTCOL       (1280+256)      //建议的XBUF Column起点
#define PLD_XBUF_ROWLIM         512             //单块XBUF Row number限制
/* Public macro -------------------------------------------------------------*/
//-------------------------------------
//	宏函数
#define Pld_UpdateCtrlReg()         {*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x10000) = pld_RegCtrl;  }
    //设置FSMC接口读写的SDRAM页
#define Pld_SetFsmcPage(page)       {*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x14000) = page;  \
            pld_RegFsmcPage = page; }
#define PLD_SetXBufPage             Pld_SetFsmcPage

    //设置读写的图像/Memory行寄存器，4Mx16 DRAM版本支持0~1023, 8Mx16版本支持0~2047
#define Pld_SelectImgRow(row)       {*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x18000) = row;   }
    //设置读写的图像/Memory行内象素地址，每个象素16Bit。每行长度支持2048Pixel。
#define Pld_PixelPtr(xaddr)         ((vu16 *)(FSMC_Bank1_SRAM1_BASE + (u32)(xaddr<<1)))
#define Pld_MemPtrBase()            ((vu16 *)FSMC_Bank1_SRAM1_BASE)

#define Pld_LCDPort_OutEn()         {*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x1C000) = 0xff;       }
#define Pld_LCDPort_OutDis()        {*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x1C000) = 0x00;       }

    //读出PLD内缓存的图像/Memory行寄存器值(Pld_SelectImgRow()写入值)
#define Pld_ReadImgRowAddr()        (*(vu16 *)(FSMC_Bank1_SRAM1_BASE + 0x18000))
/* Public variables ---------------------------------------------------------*/
//  共用变量
extern volatile Bool    g_Img_CapEnable;
extern volatile Bool    LCD_VideoEnable;

extern vu16 pld_RegFsmcPage;

/* Public function prototypes -----------------------------------------------*/
/* Publice functions ---------------------------------------------------------*/
//Application 公用函数
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
