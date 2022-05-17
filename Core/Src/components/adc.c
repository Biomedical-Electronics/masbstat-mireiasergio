/*
 * adc.c
 *
 *  Created on: 4 may. 2022
 *      Author: sergioventeo
 */
#include "stm32f4xx_hal.h"

extern	ADC_HandleTypeDef	hadc1;

double ADCv;
double ADCi;

void ADC_v(void){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 200);
	return HAL_ADC_GetValue(&hadc1);
}

void ADC_i(void){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 200);
	return HAL_ADC_GetValue(&hadc1);
}

