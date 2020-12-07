/**
 **************************************************************************
 * File Name    : I2S/I2S_FullDuplex/readme.txt
 * brief  		: Transmit between i2s2 and i2s3 in full duplex mode.
 * Date         : 2019-12-18
 * Version      : V1.0.0
 **************************************************************************
 */

  @Description
    This demo is based on the AT-START-F403A board,in this demo,LED3 lights means communication is ok.  
    pin connection:
      I2S2 Master                   I2S3 Slaver
    PB12(WS      )       <--->     PA4 (WS      )
    PB13(SCK     )       <--->     PC10(SCK     )
    PB14(MISO_Ext) Rx    <--->     PC11(MISO_Ext) Tx
    PB15(MOSI    ) Tx    <--->     PC12(MOSI    ) Rx

/****************** (C) COPYRIGHT 2017 ArteryTek *********END OF FILE*********/

