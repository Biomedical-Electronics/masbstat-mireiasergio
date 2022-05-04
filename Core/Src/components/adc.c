/*
 * adc.c
 *
 *  Created on: 4 may. 2022
 *      Author: sergioventeo
 */

extern	ADC_HandleTypeDef	hadc1;

double ADCv;
double ADCi;

void ADC_v(void){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 200);
	ADCv = HAL_ADC_GetValue(&hadc1);
}

void ADC_i(void){
	HAL_ADC_Start(&hadc1);
	HAL_ADC_PollForConversion(&hadc1, 200);
	ADCi = HAL_ADC_GetValue(&hadc1);
}

