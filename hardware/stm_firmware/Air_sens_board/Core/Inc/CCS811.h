#ifndef CCS811_H
#define CCS811_H


#include "i2c.h"

#ifdef __cplusplus
extern "C" {
#endif
	
#define MEASURE_MODE 0x18u // Mode 1 measure every second with interrupt
//#define MEASURE_MODE 0x28u // Mode 2 measure every 10 seconds with interrupt
//#define MEASURE_MODE 0x38u // Mode 3 measure every 60 seconds with interrupt
//Interrupt connects to INT pin help you to recognize if INT is HIGH means NO DATA otherwise there is DATA (INT is active-low)

#define CJMCU811_ADDRESS (0x5Au) << 1u //Default Address when ADD pin pull down to LOW
//#define CJMCU811_ADDRESS (0x5Bu) << 1u //Address when ADD pin pull up to HIGH
#define CJMCU811_STATUS 0x00u
#define CJMCU811_SW_RESET 0xFFu  //Enter BOOT mode
#define CJMCU811_MEAS_MODE 0x01u
#define CJMCU811_ALG_RESULT_DATA 0x02u
#define CJMCU811_ENV_DATA 0x05u  //Temperature and Humidity of ENV
#define CJMCU811_NTC 0x06u

HAL_StatusTypeDef CCS811_init(I2C_HandleTypeDef *hi2c);
HAL_StatusTypeDef CCS811_ReadAlgorithmData (I2C_HandleTypeDef *hi2c,uint16_t *co2, int16_t *tvoc);

	
#ifdef __cplusplus
}
#endif
	
#endif // CCS811
