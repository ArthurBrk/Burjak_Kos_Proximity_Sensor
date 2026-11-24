/*
 * i2c_utils.c
 *
 *  Created on: Nov 28, 2025
 *      Author: Stefan Kos
 */

#include "i2c_utils.h"
#include <string.h>
#include "i2c.h"


HAL_StatusTypeDef I2C_ChipInitialize(I2C_HandleTypeDef *hi2c,uint16_t devAddr7bit,const I2C_InitReg_t *table,uint16_t count,uint32_t timeout)
{
    if (hi2c == NULL || table == NULL || count == 0) {
        return HAL_ERROR;
    }

    HAL_StatusTypeDef status = HAL_OK;
    uint8_t buf[2];

    for (uint16_t i = 0; i < count; i++) {
    	buf[0] = (uint8_t)table[i].reg;
    	buf[1] = table[i].value;

        status = HAL_I2C_Master_Transmit(hi2c,devAddr7bit << 1,buf,2,timeout);
        if (status != HAL_OK) {
            return status;
        }
    }
    return HAL_OK;
}

HAL_StatusTypeDef I2C_ReadBytes(I2C_HandleTypeDef *hi2c,uint16_t devAddr7bit,uint16_t regAddr,int16_t regAddrSize,uint8_t *pBuf,uint16_t size,uint32_t timeout)
{
    HAL_StatusTypeDef status;
    uint8_t reg_buf[2];

    if (hi2c == NULL || pBuf == NULL || size == 0) {
        return HAL_ERROR;
    }

    // Registeradresse senden
    if (regAddrSize == 2) {
        reg_buf[0] = (regAddr >> 8) & 0xFF;
        reg_buf[1] = regAddr & 0xFF;
        status = HAL_I2C_Master_Transmit(hi2c,devAddr7bit << 1,reg_buf,2,timeout);
    } else { // 1 Byte Adresse
        reg_buf[0] = regAddr & 0xFF;
        status = HAL_I2C_Master_Transmit(hi2c,devAddr7bit << 1,reg_buf,1,timeout);
    }

    if (status != HAL_OK) {
        return status;
    }

    // Daten lesen
    status = HAL_I2C_Master_Receive(hi2c,devAddr7bit << 1,pBuf,size,timeout);
    return status;
}
