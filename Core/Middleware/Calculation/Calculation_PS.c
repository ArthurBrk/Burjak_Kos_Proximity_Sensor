/*
 * Calculation_PS.c
 *
 *  Created on: Dec 28, 2025
 *      Author: Stefan Kos
 */

#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <i2c.h>
#include "i2c_utils.h"
#include "Calculation_PS.h"

// Sensor Init Tabelle - Werte aus Datasheet
// PS_CONF1: Duty 1/40, PERS 1, IT = 3T, PS_SD=0
I2C_InitReg_t vcnl_init[] = {
	      { 0x03, 0x20 },      		// PS_CONF1_L: PS an, Duty=00, IT=100, SD=0
	      { 0x03 | 0x80, 0x08 },	// PS_CONF2_H: 16-bit (Bit3=1)
	      { 0x04, 0x00 },      		// PS_CONF3_L: IRED1, defaul
		  };

float Proximity_Conversion(uint16_t pData){

	float conv_data = 0.0;

//Koeffizienten für die Kalibrierung gemessen und via Excel bestimmt
	if(pData > 50){
		conv_data = (float)((log(pData/5518.9))/-0.415);

	}else if(pData == 0){
		conv_data = 50.0f;

	}else{
		conv_data = (float)((log(pData/169.07))/-0.079);
	}

	if(conv_data > 50){
		conv_data = 50.0f;
	}

	return conv_data;
}



void PS_StateConfig_ADC(uint16_t PotiDigital){

	if (PotiDigital < 1024) {
	    // 1/40 Duty
	    vcnl_init[0].reg   = 0x03;
	    vcnl_init[0].value = 0x20;      // Duty=00, IT=3T

	} else if (PotiDigital >= 1024 && PotiDigital < 2048) {
	    // 1/80 Duty
	    vcnl_init[0].reg   = 0x03;
	    vcnl_init[0].value = 0x60;      // Duty=01, IT=3T

	} else if (PotiDigital >= 2048 && PotiDigital < 3072) {
	    // 1/160 Duty
	    vcnl_init[0].reg   = 0x03;
	    vcnl_init[0].value = 0xA0;      // Duty=10, IT=3T

	} else { // PotiDigital >= 3072
	    // 1/320 Duty
	    vcnl_init[0].reg   = 0x03;
	    vcnl_init[0].value = 0xE0;      // Duty=11, IT=3T
	}
	vcnl_init[1].reg   = 0x03 | 0x80;
	vcnl_init[1].value = 0x08;
	vcnl_init[2].reg   = 0x04;
	vcnl_init[2].value = 0x00;

	I2C_ChipInitialize(&hi2c1, SENSOR_ADDR, vcnl_init, 3, HAL_MAX_DELAY);
}

