
#include "uvcstream.h"
#include "usb_lib.h"
#include "usb_desc.h"
#include "usb_mem.h"

#include "ov2640.h"


#define CAMERA_SIZ_STREAMHD     2

u8 UVC_TxBuf[PACKET_SIZE];          //USB TX Buffer
vs32 FrameSentLen = 0;               //当前Frame已发送Byte Number

/* Private function prototypes -----------------------------------------------*/

void UVC_SendPack_Irq(void)
{
    uint32_t datalen;
    uint8_t *payload;

    AT32_LEDn_ON(LED4);

    payload = UVC_TxBuf + CAMERA_SIZ_STREAMHD;

    if (FrameSentLen >= FrameLen)
    {
        FrameSentLen = 0;
        //检查两个帧接收Buf哪个处于接收完成的状态
        if(FrameBuf_1_Ready)
        {
          Frame_SendPtr = FrameBuf_1_Addr;
          FrameLen = FrameBuf_1_Len;
        }
        else
        {
          Frame_SendPtr = FrameBuf_2_Addr;
          FrameLen = FrameBuf_2_Len;
        }
        //每帧图像的起始包，初始化payload
        UVC_TxBuf[0] = 0x02;
        UVC_TxBuf[1] &= 0x01;
        UVC_TxBuf[1] ^= 0x01;

        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
        bufCopy(Frame_SendPtr + FrameSentLen, payload, datalen);
        //从TX Buffer送到USB PMA Buffer
        UserToPMABufferCopy(UVC_TxBuf, ENDP1_BUF1Addr, PACKET_SIZE);
        FrameSentLen = datalen;

    }
    else
    {
        //单帧图像的后续包
        datalen = PACKET_SIZE - CAMERA_SIZ_STREAMHD;
            //判断是否最后一包
        if (FrameSentLen + datalen >= FrameLen)
        {
            datalen = FrameLen - FrameSentLen;
            UVC_TxBuf[1] |= 0x02;       //加结束包标记
        }
        bufCopy(Frame_SendPtr + FrameSentLen, payload, datalen);
        FrameSentLen += datalen;
    }

    //USB双缓冲模式数据包发送
    if(_GetENDPOINT(ENDP1) & EP_DTOG_RX)    //EP_DTOG_RX ->使用的是BUF1
    {
        // User use buffer0
        UserToPMABufferCopy(UVC_TxBuf, ENDP1_BUF0Addr, PACKET_SIZE);
        SetEPDblBuf0Count(ENDP1, EP_DBUF_IN, PACKET_SIZE);
    }
    else
    {
        // User use buffer1
        UserToPMABufferCopy(UVC_TxBuf, ENDP1_BUF1Addr, PACKET_SIZE);
        SetEPDblBuf1Count(ENDP1, EP_DBUF_IN, PACKET_SIZE);
    }
    _ToggleDTOG_RX(ENDP1);

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
