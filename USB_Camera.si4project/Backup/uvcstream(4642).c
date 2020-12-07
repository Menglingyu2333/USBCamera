/*============================================================================
//
//	UVCStream.c
//		by: HECC. @HuiZhou, 2011	
//
//   email: hcc21cn@163.com	
//		
**==============================================================================*/
#include "uvcstream.h"
#include "AppPlatForm.h"
//#include "Pld_Intf.h"
#include "ejpeg.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
#define DEF_JPGBUF_SIZE         2048

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u8 jpg_codebuf[DEF_JPGBUF_SIZE];    //JPEG编码器bit stream buffer。由JPEG Encoder管理
vu32 xbuf_WrFramePtr;               //写入XBUF用的复合地址
    
    //开启2个XBUF区P0/P1，各对应存储1帧JPEG编码的图像，一个用于JPEG Encoder写入，
    //一个用于USB读出发送，动态交换。两个XBUF区的读写上下文参数放在数组中，读写程序
    //引用xbuf_WrFrame/xbuf_RdFrame作为数组ID，获得当前读写XBUF区。
    //  两个XBUF块所在的页是动态的, 与图像所在页相同。起点在不同的ROW上，这样不管
    //页是否相同，都不会冲突。每一块设为512行，每行512Word, 有512KBytes足以存储一幅
    //JPEG图像了。
volatile PldXBufContext  xbuf_context[2];   //XBUF区的读写上下文参数
vs32 jpg_codesize[2] = {0, 0};  //已编码数据长度。中断可能在UVC_streamStart之前执行，应初始化
vu8 xbuf_WrFrame;               //Encoder writing frame ID
vu8 xbuf_RdFrame = 0;           //USB reading frame ID。应初始化!

    //UVC payload head
#define CAMERA_SIZ_STREAMHD     2
u8 uvc_StreamHead[CAMERA_SIZ_STREAMHD] = {0x02, 0x01};
//#define CAMERA_SIZ_STREAMHD     12
//#u8 uvc_StreamHead[CAMERA_SIZ_STREAMHD] = {0x0C, 0x01, 0,0,0,0,0, 0,0,0,0,0};
    
    //XBUF读到USB发送Buffer
u8 uvc_rdbuf[PACKET_SIZE];          //USB TX Buffer
vu32 xbuf_RdFramePtr;               //读出XBUF用的复合地址
vs32 uvc_rdsize = 0;                //当前Frame已发送Byte Number

s32 m_frameNo = 0;
/* Private function prototypes -----------------------------------------------*/

/* Private functions ---------------------------------------------------------*/
/*******************************************************************************
* Function Name  : [private] CallBack
* Description    :  从JPEG Encoder输出数据到目标File Stream的应用处理函数 
*       JPEG Encoder将bit stream写入一个自管理的Buffer，此Buffer满后，调用jpgStreamManage()
*   函数将数据写到最终的输出Stream，刷新这个bit stream buffer。这是内定的标准处理模式。
* Input          :  JPEG编码数据指针(bufptr)及数据长度
* Output         : 
* Return         :  刷新后的bit stream buffer指针
*******************************************************************************/
u8* jpgStreamManage(UInt8 *bufptr, Int32 byteNum)
{
    Pld_SetFsmcPage(xbuf_context[xbuf_WrFrame].fsmcpage);   //非中断读写，只重设页就可以了
    xbuf_WrFramePtr = PLD_WriteToXBuf(bufptr, byteNum, xbuf_WrFramePtr);
    return(jpg_codebuf);
}

/*******************************************************************************
* Function Name  : [Public]
* Description    :  初始化USB Video Class Stream
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void UVC_streamStart(void)
{
//#if 0
    s32 datalen;   
//#endif

    //初始化JPEG编码器
    JPG_initImgFormat(IMG_WIDTH, IMG_HEIGHT, IMG_VIDEO_SCALE, JPG_IMGFMT_YUV420, 
                2, IMG_WIDTH, 0, 0);
    JPG_initOutStream(jpgStreamManage, jpg_codebuf, DEF_JPGBUF_SIZE);

    //初始化UVC payload head
    uvc_rdbuf[0] = 0x02;
    uvc_rdbuf[1] = 0x01;        //0x81也行
    
    uvc_rdsize = 0;             //初始化单帧数据传输计数

    //预先编码两帧图像，存在XBUF，同时初始化两个XBUF块(P0/P1)
    xbuf_WrFrame = 0;
    PLD_LockImageFrame(True);                                   //锁定图像帧P0
    xbuf_WrFramePtr = PLD_SetupXBuf(0, PLD_XBUF_STARTCOL);      //设定P0帧JPEG数据存储区
    xbuf_context[0] = PLD_GetXBufContext();         //保存XBUF关联的读写环境
    JPG_WriteHeader(0);                                         //启动JPEG Encoder
    jpg_codesize[0] = JPG_Encode();                             //等帧编码完成
        //
    xbuf_WrFrame = 1;
    PLD_LockImageFrame(True);                                   //锁定图像帧P1
    xbuf_WrFramePtr = PLD_SetupXBuf(512, PLD_XBUF_STARTCOL);    //设定P1帧JPEG数据存储区
    xbuf_context[1] = PLD_GetXBufContext();         //保存XBUF关联的读写环境
    JPG_WriteHeader(0);
    jpg_codesize[1] = JPG_Encode();
    
    //
    PLD_LockImageFrame(False);      //解除帧存储锁定，SDRAM图像自动刷新
    xbuf_RdFrame = 0;               //初始化XBUF读出指针：写入指向P1，读出指向P0。应在                                        
                                    //此程序之前也做初始化，防止中断提前执行
//#if 0        
    //从SDRAM中读出一个Packet，预写入USB TX Buffer。没有这个也可以工作，但PC收到的
    //第一个数据包是垃圾数据.
        //read fram xbuf-p0
    Pld_SetFsmcPage(xbuf_context[0].fsmcpage);
    xbuf_RdFramePtr = PLD_SetupXBuf(0, PLD_XBUF_STARTCOL);
        //ISO Double Buffer, send
    datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
    xbuf_RdFramePtr = PLD_ReadFromXBuf(uvc_rdbuf+CAMERA_SIZ_STREAMHD, datalen, xbuf_RdFramePtr);                
    UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF0Addr, PACKET_SIZE);
    _SetEPDblBuf0Count(ENDP1, EP_DBUF_IN, PACKET_SIZE);
    uvc_rdsize += datalen;
//#endif

    //数据准备完成，再设置EP_TX_VALID，这很重要! 设置EP_TX_VALID后，一旦收到IN令牌，
    //USB即发送PACKET_SIZE长度的数据包，不管Buf0Count/Buf1Count设置如何。仅凭BufxCount=0
    //设置控制不住发送。
    _SetEPTxStatus(ENDP1, EP_TX_VALID);
}

/*******************************************************************************
* Function Name  : [Public/INT]
* Description    :  USB数据包发送。由USB EndPoint中断调用。
*       从XBUF中读出数据并发送，每次为PACKET_SIZE长度的数据包。加入payload信息，
*   自动切换XBUF块并推动JPEG Encoder处理下一帧。
* Input          : 
* Output         : 
* Return         : 
*******************************************************************************/
void UVC_SendPack_Irq(void)
{
    s32 datalen;    
    u8 *payload;
    PldXBufContext contx;

    if (jpg_codesize[xbuf_RdFrame] ==0)     
    {
        //当前帧没有JPEG数据，不读出。实际发送冗余空包
        return;
    }

    //从SDRAM中读出
    payload = uvc_rdbuf + CAMERA_SIZ_STREAMHD;
    if (uvc_rdsize==0)
    {
        //每帧图像的起始包
        uvc_rdbuf[1] &= 0x01;
        uvc_rdbuf[1] ^= 0x01;

        //从XBUF中读出数据包，送给USB TX Buffer。这是每帧图像的开始，要取得XBUF读指针
        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
        contx = PLD_GetXBufContext();           //push 保存读写环境
        PLD_SetXBufContext(xbuf_context[xbuf_RdFrame]);     //设置读写环境。中断中必须用这个!        
            //取得XBUF块读出的复合地址。这个必须在保存Context之后做.
        if (xbuf_RdFrame ==0)
            xbuf_RdFramePtr = PLD_SetupXBuf(0, PLD_XBUF_STARTCOL);            
        else
            xbuf_RdFramePtr = PLD_SetupXBuf(512, PLD_XBUF_STARTCOL);
            //读出
        xbuf_RdFramePtr = PLD_ReadFromXBuf(payload, datalen, xbuf_RdFramePtr);                
        PLD_SetXBufContext(contx);              //pop 恢复主线程的读写环境

        //从TX Buffer送到USB PMA Buffer
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF1Addr, PACKET_SIZE);
        uvc_rdsize = datalen;
        datalen += CAMERA_SIZ_STREAMHD;
    }
    else
    {
        //单帧图像的后续包
        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
            //判断是否最后一包                
        if (uvc_rdsize + datalen >= jpg_codesize[xbuf_RdFrame])
        {
            datalen = jpg_codesize[xbuf_RdFrame] - uvc_rdsize;
            uvc_rdbuf[1] |= 0x02;       //加结束包标记
        }
        contx = PLD_GetXBufContext();           //push
        PLD_SetXBufContext(xbuf_context[xbuf_RdFrame]);     //setup context
        xbuf_RdFramePtr = PLD_ReadFromXBuf(payload, datalen, xbuf_RdFramePtr);
        PLD_SetXBufContext(contx);              //pop

        uvc_rdsize += datalen;    
        datalen += CAMERA_SIZ_STREAMHD;

#if 0
            //包不满，用JPEG规定的冗余字节0xFF填充。这不是必须的，如果发送的数据包
            //<PACKET_SIZE，会自动发冗余Byte，实际也发PACKET_SIZE长度
        if (datalen < PACKET_SIZE)         
        {
            datalen--;
            while(datalen<PACKET_SIZE-1)
            {
                uvc_rdbuf[datalen++] = 0xff;
            }
            uvc_rdbuf[datalen++] = 0xD9;        //新构造JPEG结束符 
        }
#endif
    }

    //USB双缓冲模式数据包发送
    if(_GetENDPOINT(ENDP1) & EP_DTOG_RX)    //EP_DTOG_RX ->使用的是BUF1
    {
        // User use buffer0
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF0Addr, datalen);
        SetEPDblBuf0Count(ENDP1, EP_DBUF_IN, datalen);
    }
    else
    {
        // User use buffer1
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF1Addr, datalen);
        SetEPDblBuf1Count(ENDP1, EP_DBUF_IN, datalen);
    }
    _ToggleDTOG_RX(ENDP1);    

    //判断本帧图像是否发送完成, 一帧发送完成，切换到下一帧
    if (uvc_rdsize >= jpg_codesize[xbuf_RdFrame])
    {
        uvc_rdsize = 0;         // 
        if ((xbuf_RdFrame != xbuf_WrFrame)&&(jpg_codesize[xbuf_WrFrame]>0)) 
        {
            xbuf_RdFrame = xbuf_WrFrame;    //下一帧已写好，读出指向当前写入块，推动帧切换。
        }
    }
}

/*******************************************************************************
* Function Name  : [Public]
* Description    :  序列图像JPEG编码。在主循环中调用此函数，实现UVC连续视频编码
*       预编码有2帧图像，写入/读出指向不同XBUF块; 当读出发送一帧完成，检查到写入指向
*   的XBUF块有数据(即已编码完成)，切换换到此块读; 主程序检查到读出指针切换，相应的
*   切换写入指针，到已发送的XBUF块，启动新图像帧的编码，以此循环实现连续视频编码输出。
*       由于单帧图像捕捉、编码、USB发送时间不同，编码要与帧捕捉同步，此函数要在VSYN
*   中断后执行，或执行后等下一个VSYN才继续，否则会出现编码超前的情况，打乱视频的时间
*   序列。可以要求此函数内部检查帧同步，如外部主循环是在帧同步后调用UVC_MovieEncoder，
*   则无需再次同步。
*
* Input          : 是否要求在此函数内部进行帧同步
* Output         : 
* Return         : 成功编码一帧，返回Byte数，否则返回0
*******************************************************************************/
s32 UVC_MovieEncoder(bool reqSync)
{
    u16 page;
    
    if (reqSync)
    {        
        //帧同步
        if (cam_FrameNo == m_frameNo)
            return(0);        
        m_frameNo = cam_FrameNo;
    }
    if (xbuf_RdFrame == xbuf_WrFrame)   //读出帧已赶上来了，切换XBUF，开始新帧编码
    {
        xbuf_WrFrame++;                     //XBUF块读写交换
        xbuf_WrFrame &= 0x01;
        jpg_codesize[xbuf_WrFrame] = 0;     //当前写入块数据清除，屏蔽读出

        //编码一帧图像
        page = PLD_LockImageFrame(True);    //锁定图像，动态获得XBUF块所在页。
                //@停止图像Capture使当前帧完成后，下一帧不自动存储入SDRAM，这时对SDRAM
                //的读写速度要快一些，JPEG编码速度相应快一些，但当前帧会继续写完，仅在
                //JPEG编码时间超过一帧图像时间才有效。不用这个功能也可以。
                //Capture Stop必须在PLD_LockImageFrame()之后运行，它会干扰_LockImageFrame.
        PLD_CaptureRun(False);  

        if (xbuf_WrFrame ==0)               //设置XBUF块在SDRAM中的位置
            xbuf_WrFramePtr = PLD_SetupXBuf(0, PLD_XBUF_STARTCOL);
        else
            xbuf_WrFramePtr = PLD_SetupXBuf(512, PLD_XBUF_STARTCOL);
        xbuf_context[xbuf_WrFrame].fsmcpage = page;     //动态更新XBUF块所在页        
            //JPEG编码
        JPG_WriteHeader(0);
        jpg_codesize[xbuf_WrFrame] = JPG_Encode();
        
        PLD_LockImageFrame(False);          //释放帧锁定，继续自动捕捉
        PLD_CaptureRun(True);               //恢复图像自动捕捉

#if 0
        //Test
        if (xbuf_WrFrame ==0)
            GPIO_ResetBits(PORT_LED,PIN_LED1);
        else
            GPIO_SetBits(PORT_LED,PIN_LED1); 
#endif
        return(jpg_codesize[xbuf_WrFrame]);
    }    
    else
        return(0);
}
