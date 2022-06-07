/*
 * cyclic_voltammetry.c
 *
 *  Created on: May 3, 2022
 *      Author: sergioventeo
 */

#include "components/cyclic_voltammetry.h"
#include "components/masb_comm_s.h"
#include "components/adc.h"
#include "components/formulas.h"
#include "components/timer.h"

extern TIM_HandleTypeDef htim3;

void cyclic_voltammetry(struct CV_Configuration_S cvConfiguration) {
	float Vcell = cvConfiguration.eBegin;
	float vObjetivo = cvConfiguration.eVertex1;
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);

	uint8_t cycles = cvConfiguration.cycles;
	double scanRate = cvConfiguration.scanRate;
	double eStep = cvConfiguration.eStep;
	double SamplingPeriod = (eStep / scanRate) * 1000;

	__HAL_TIM_SET_AUTORELOAD(&htim3,SamplingPeriod * 10); // Fijamos el periodo
		// ⚠ Solo si se fija una frecuecia de trabajo de 10 KHz para el timer,
		// mutliplicar samplingPeriodMs por 10 ⚠

		__HAL_TIM_SET_COUNTER(&htim3,0); //Setting the counter


		HAL_TIM_Base_Start_IT(&htim3); // E iniciamos el timer

	uint8_t point = 1;
	uint8_t counter = 0;
	uint8_t numbercycles = 0;

	Timer3_ResetFlag();

	while(numbercycles < cycles) {

		while (Timer3_GetFlag() == TRUE) {
		};

		uint32_t vADC = ADC_v();
		float Vreal = calculateVrefVoltage(vADC);
		uint32_t iADC = ADC_i();
		float Ireal = calculateIcellCurrent(iADC);

		struct Data_S data;
		data.point = point;
		data.timeMs = counter;
		data.voltage = Vreal;
		data.current = Ireal;
		MASB_COMM_S_sendData(data);

		point++;
		counter += SamplingPeriod;

		if (Vreal == vObjetivo) {
			if (vObjetivo == cvConfiguration.eVertex1) {
				vObjetivo = cvConfiguration.eVertex2;
			} else if (vObjetivo == cvConfiguration.eVertex2) {
				vObjetivo = cvConfiguration.eBegin;
			} else if (numbercycles == cycles) {
				HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
			} else {
				numbercycles++;
				vObjetivo = cvConfiguration.eVertex1;
			}

		} else {
			if (Vreal + eStep > vObjetivo) {
				Vreal = vObjetivo;
			} else {
				if (Vcell > vObjetivo) {
					Vreal -= eStep;
				} else {
					Vreal += eStep;
				}
			}
		}
		Timer3_ResetFlag();
	}
}
