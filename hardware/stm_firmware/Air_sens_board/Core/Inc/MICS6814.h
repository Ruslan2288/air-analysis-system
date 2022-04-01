#ifndef MICS6814_H
#define MICS6814_H


#include "MICS6814.h"
#include "adc.h"
#include "stm32f1xx.h"

#ifdef __cplusplus
extern "C" {
#endif
	
typedef struct
{
	uint16_t CO;
	uint16_t NO2;
	uint16_t NH3;
} gas_t;

void MICS6814_calibrate();
//MICS6814_measure(gas_t gas);
//void MICS6814_loadCalibrationData(uint16_t baseNH3, uint16_t baseCO, uint16_t baseNO2);
//float MICS6814_measure(gas_t gas);
//uint16_t MICS6814_getResistance(channel_t channel);
//uint16_t MICS6814_getBaseResistance(channel_t channel);
//float MICS6814_getCurrentRatio(channel_t channel);


#ifdef __cplusplus
}
#endif
	
#endif
