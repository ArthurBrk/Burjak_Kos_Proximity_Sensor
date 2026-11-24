/*
 * i2c_utils.h
 *
 *  Created on: Nov 28, 2025
 *      Author: Stefan Kos
 */

#ifndef MIDDLEWARE_I2C_UTILS_H_
#define MIDDLEWARE_I2C_UTILS_H_

#include "main.h"
#include "i2c.h"

#define PS_CONF1 0x03
#define PS_CONF2 0x04
#define PS_CONF3 0x05
#define SENSOR_ADDR 0x60

typedef struct{
    uint16_t reg;
    uint8_t value;
} I2C_InitReg_t;

/**
 * @brief  Initialisiert einen I2C‑Slave über eine Registertabelle.
 *         Jedes Element: 1 Byte Registeradresse, 1 Byte Wert.
 * @param  hi2c        I2C Handle (z.B. &hi2c1)
 * @param  devAddr7bit 7‑bit Slave‑Adresse (z.B. 0x60 für VCNL4035X01)
 * @param  table       Pointer auf Init‑Tabelle
 * @param  count       Anzahl Einträge in der Tabelle
 * @param  timeout     Timeout in ms
 * @retval HAL_StatusTypeDef
 */
HAL_StatusTypeDef I2C_ChipInitialize(I2C_HandleTypeDef *hi2c,uint16_t devAddr7bit,const I2C_InitReg_t *table,uint16_t count,uint32_t timeout);

/**
 * @brief  Liest beliebig viele Bytes ab einer Registeradresse.
 * @param  hi2c        I2C Handle
 * @param  devAddr7bit 7‑bit Slave‑Adresse (z.B. 0x60)
 * @param  regAddr     Registeradresse (8‑ oder 16‑bit, je nach regAddrSize)
 * @param  regAddrSize 1 oder 2 (Bytes für die Registeradresse)
 * @param  pBuf        Zielpuffer
 * @param  size        Anzahl zu lesender Bytes
 * @param  timeout     Timeout in ms
 * @retval HAL_StatusTypeDef
 */
HAL_StatusTypeDef I2C_ReadBytes(I2C_HandleTypeDef *hi2c,uint16_t devAddr7bit,uint16_t regAddr,int16_t regAddrSize,uint8_t *pBuf,uint16_t size,uint32_t timeout);

#endif /* MIDDLEWARE_I2C_UTILS_H_ */
