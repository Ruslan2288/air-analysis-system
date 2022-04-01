/*
 * ledStatus.cpp
 *
 *  Created on: Sep 23, 2018
 *      Author: Itamar Eliakim
 */

#include "stm32f1xx_hal.h"
#include "nbt.h"
#include "ledStatus.h"

#define LED_GPIO_Port GPIOC
#define LED_Pin GPIO_PIN_13


static nbt_t lednbt;

extern "C" void init_ledStatus()
{
	NBT_init(&lednbt, 500);
}

extern "C" void ledStatus_handler()
{
	if (NBT_handler(&lednbt))
	{
	  HAL_GPIO_TogglePin(LED_GPIO_Port, LED_Pin);
	}
}
