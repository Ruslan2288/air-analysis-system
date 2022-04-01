#include "CCS811.h"

//Check Firmware and Init the MEASURE MODE
HAL_StatusTypeDef CCS811_init(I2C_HandleTypeDef *hi2c) {
    HAL_StatusTypeDef CHECK_ALIVE = HAL_ERROR;
    uint8_t START_data = 0xF4;
    uint8_t CHECK_FIRMWARE;
    //Check firmware if not available then PULL DOWN RST pin to reset the sensor
    while(CHECK_ALIVE != HAL_OK) {
        CHECK_ALIVE = HAL_I2C_IsDeviceReady(&hi2c1,CJMCU811_ADDRESS,3,50);
        if (CHECK_ALIVE== HAL_BUSY || CHECK_ALIVE == HAL_ERROR) {
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_RESET);
            HAL_GPIO_WritePin(GPIOA,GPIO_PIN_1,GPIO_PIN_SET);
        }
    }
    //Check ERROR bit
    HAL_I2C_Mem_Read(hi2c, CJMCU811_ADDRESS, CJMCU811_STATUS, 1, &CHECK_FIRMWARE, 1, 50);
    if((CHECK_FIRMWARE & 1u) == 0 ) {
        HAL_I2C_Master_Transmit(hi2c, CJMCU811_ADDRESS, &START_data, 1, 50);
        uint8_t MEAS_mode = MEASURE_MODE;
        HAL_I2C_Mem_Write(hi2c, CJMCU811_ADDRESS, CJMCU811_MEAS_MODE, 1, &MEAS_mode, 1,50);
        return HAL_OK;
    }
    else return HAL_ERROR;
}


HAL_StatusTypeDef CCS811_ReadAlgorithmData (I2C_HandleTypeDef *hi2c,uint16_t *co2, int16_t *tvoc) {
    uint8_t Received_Data[4];
    uint8_t Status_Register = 0u;
    HAL_I2C_Mem_Read(hi2c, CJMCU811_ADDRESS,CJMCU811_STATUS,1, &Status_Register,1,50);
    if ((Status_Register >> 3u & 1u) == 1) {
        HAL_I2C_Mem_Read(hi2c, CJMCU811_ADDRESS,0x02,1, &Received_Data[0],4,50);
        *co2 = (uint16_t)((Received_Data[0] << 8)| Received_Data[1]);
        *tvoc = (uint16_t)((Received_Data[2] << 8) | Received_Data[3]);
        return HAL_OK;
    }
    return HAL_ERROR;
}
