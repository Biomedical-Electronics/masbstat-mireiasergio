/*
 * adc.c
 *
 *  Created on: 4 may. 2022
 *      Author: sergioventeo
 */
#include "stm32f4xx_hal.h"

extern ADC_HandleTypeDef hadc1;

uint32_t ADC_v(void){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 200);
	uint32_t adcVal = HAL_ADC_GetValue(&hadc1);
	return adcVal;
}

uint32_t ADC_i(void){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 200);
	uint32_t adcVal = HAL_ADC_GetValue(&hadc1);
	return adcVal;
}

