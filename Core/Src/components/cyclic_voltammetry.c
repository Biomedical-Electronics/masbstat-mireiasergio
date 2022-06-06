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


void cyclic_voltammetry(struct CV_Configuration_S cvConfiguration) {
	float Vcell = cvConfiguration.eBegin;
	float vObjetivo = cvConfiguration.eVertex1;
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);

	uint8_t cycles = cvConfiguration.cycles;
	double scanRate = cvConfiguration.scanRate;
	double eStep = cvConfiguration.eStep;
	double SamplingPeriod = eStep / scanRate;

	uint8_t point = 0;
	uint8_t i = 0;
	wait = FALSE;

	for (i = 0; i < cycles; i++) {
		Timer3_CV();
		while (wait) {

			uint32_t vADC = ADC_v();
			float Vreal = calculateVrefVoltage(vADC);
			uint32_t iADC = ADC_i();
			float Ireal = calculateIcellCurrent(iADC);

			struct Data_S data;
			data.point = point;
			data.timeMs = point*(cvConfiguration.eStep/cvConfiguration.scanRate);
			data.voltage = Vreal;
			data.current = Ireal;
			MASB_COMM_S_sendData(data);

			point++;

			if (Vreal == vObjetivo) {
				if (vObjetivo == cvConfiguration.eVertex1) {
					vObjetivo = cvConfiguration.eVertex2;
				} else if (vObjetivo == cvConfiguration.eVertex2) {
					vObjetivo = cvConfiguration.eBegin;
				} else if (i == cycles) {
					HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin,
							GPIO_PIN_RESET);
				} else {
					vObjetivo = cvConfiguration.eVertex1;
					i++;
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
		}

	}
}
