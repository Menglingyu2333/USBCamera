/*
 **************************************************************************
 * File Name    : wm8988_i2c.c
 * Description  : header for i2c driver
 * Author       : Artery SW Team
 * Date         : 2018-11-26
 * Version      : V1.0.0
 **************************************************************************
 */
 
 enum I2C_State{
    I2C_SUCCESS,
    I2C_FAILED    
};
 
 void AT32_I2C_Init(void);
 uint8_t AT32_I2C_WriteReg(uint8_t u8DevAddr, uint8_t u8RegAddr, uint8_t u8RegData);
 uint8_t AT32_I2C_ReadReg(uint8_t u8DevAddr, uint8_t u8RegAddr, uint8_t *u8RegData);
 uint8_t AT32_I2C_Check(void);
