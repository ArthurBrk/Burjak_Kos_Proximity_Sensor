/*
 * Calculation_PS.h
 *
 *  Created on: Dec 28, 2025
 *      Author: Stefan Kos
 */

#ifndef MIDDLEWARE_CALCULATION_CALCULATION_PS_H_
#define MIDDLEWARE_CALCULATION_CALCULATION_PS_H_

#include <stdint.h>
#include "i2c_utils.h"

extern I2C_InitReg_t vcnl_init[];

/* Proimity_Conversion
 * @Function:
 * Calculates the corresponding Distance to the measured Data
 * @Input:
 * pData: The measured Data from the PS
 * resolution: The Amount of Bits the Sensor puts out
 */
float Proximity_Conversion(uint16_t pData);

/* PS_StateConfig_ADC
 * @Function:
 * Decides which PS_Duty needs to be set, according to the Potentiometer State
 * @Input:
 * PotiDigital: Is the Digital Value of the ADC from the Potentiometer Input
 */
void PS_StateConfig_ADC(uint16_t PotiDigital);

#endif /* MIDDLEWARE_CALCULATION_CALCULATION_PS_H_ */
