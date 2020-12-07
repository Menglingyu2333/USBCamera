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

#include "ov2640.h"

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
typedef struct  tagXBufContext
{
    u16 fsmcpage;
    u16 fsmcrow;
    u16 colbase;
    u16 rowbase;
}   PldXBufContext;
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
vs32 jpegSendLen = 0;                //当前Frame已发送Byte Number
vu32 frameCnt = 0;               //读出XBUF用的复合地址

s32 m_frameNo = 0;
/* Private function prototypes -----------------------------------------------*/

void UVC_SendPack_Irq(void)
{
    s32 datalen;
    u8 *payload;
    PldXBufContext contx;

    if (jpeglen ==0)
    {
        //当前帧没有JPEG数据，不读出。实际发送冗余空包
        return;
    }

    //从SDRAM中读出
    payload = uvc_rdbuf + CAMERA_SIZ_STREAMHD;
    if (jpegSendLen==0)
    {
        //每帧图像的起始包
        //初始化payload
        uvc_rdbuf[0] = 0x02;
        uvc_rdbuf[1] = 0x01;        //0x81也行

        uvc_rdbuf[1] &= 0x01;
        uvc_rdbuf[1] ^= 0x01;

        //从XBUF中读出数据包，送给USB TX Buffer。这是每帧图像的开始，要取得XBUF读指针
        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
        //
        bufCopy(ov2640_framebuf_ptr + jpegSendLen, payload, datalen);
        //从TX Buffer送到USB PMA Buffer
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF1Addr, PACKET_SIZE);
        jpegSendLen = datalen;
//        printf("222=%x,jpegSendLen=%d\n",ov2640_framebuf_ptr,jpegSendLen);
    }
    else
    {
        //单帧图像的后续包
        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
            //判断是否最后一包
        if (jpegSendLen + datalen >= jpeglen)
        {
            datalen = jpeglen - jpegSendLen;
            uvc_rdbuf[1] |= 0x02;       //加结束包标记
        }
        bufCopy(ov2640_framebuf_ptr + jpegSendLen, payload, datalen);
        jpegSendLen += datalen;
//        printf("111=%x,jpegSendLen=%d\n",ov2640_framebuf_ptr + jpegSendLen,jpegSendLen);
//        printf("jpegSendLen=%d,datalen=%d,frameCnt=%d\n",jpegSendLen,datalen,frameCnt);
//        datalen += CAMERA_SIZ_STREAMHD;

#if 0
            //包不满，用JPEG规定的冗余字节0xFF填充。这不是必须的，如果发送的数据包
            //<PACKET_SIZE，会自动发冗余Byte，实际也发PACKET_SIZE长度
        if (datalen < PACKET_SIZE-CAMERA_SIZ_STREAMHD)
        {
            printf("jpegSendLen=%d,datalen=%d,frameCnt=%d\n",jpegSendLen,datalen,frameCnt);
            datalen--;
            while(datalen<PACKET_SIZE-1-CAMERA_SIZ_STREAMHD)
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
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF0Addr, PACKET_SIZE);
        SetEPDblBuf0Count(ENDP1, EP_DBUF_IN, PACKET_SIZE);
    }
    else
    {
        // User use buffer1
        UserToPMABufferCopy(uvc_rdbuf, ENDP1_BUF1Addr, PACKET_SIZE);
        SetEPDblBuf1Count(ENDP1, EP_DBUF_IN, PACKET_SIZE);
    }
    _ToggleDTOG_RX(ENDP1);

    //判断本帧图像是否发送完成, 一帧发送完成，切换到下一帧
    if (jpegSendLen >= jpeglen)
    {
      jpegSendLen = 0;
//        if ((xbuf_RdFrame != xbuf_WrFrame)&&(jpg_codesize[xbuf_WrFrame]>0))
//        {
//            xbuf_RdFrame = xbuf_WrFrame;    //下一帧已写好，读出指向当前写入块，推动帧切换。
//        }
    }
}

void bufCopy(u8* srcPtr,u8* desPtr, u32 len)
{
  while(len)
  {
    *desPtr=*srcPtr;
    desPtr++;
    srcPtr++;
    len--;
  }
}
