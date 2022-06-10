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
#include "stdbool.h"
#include "math.h"

extern TIM_HandleTypeDef htim3;
extern MCP4725_Handle_T hdac;

bool equalsf(float A, float B, float margin) {
	return fabs(A - B) < margin;
}

void cyclic_voltammetry(struct CV_Configuration_S cvConfiguration) {
	float Vcell = cvConfiguration.eBegin;
	float VDAC = calculateDacOutputVoltage(Vcell);
	MCP4725_SetOutputVoltage(hdac, VDAC);

	double vObjetivo = cvConfiguration.eVertex1;
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);

	uint8_t cycles = cvConfiguration.cycles;
	double scanRate = cvConfiguration.scanRate;
	double eStep = cvConfiguration.eStep;
	double SamplingPeriod = eStep / (scanRate / 1000);

	__HAL_TIM_SET_AUTORELOAD(&htim3, SamplingPeriod * 10); // Fijamos el periodo
	// ⚠ Solo si se fija una frecuecia de trabajo de 10 KHz para el timer,
	// mutliplicar samplingPeriodMs por 10 ⚠

	__HAL_TIM_SET_COUNTER(&htim3, 0); //Setting the counter

	HAL_TIM_Base_Start_IT(&htim3); // E iniciamos el timer
	Timer3_ResetFlag();
	uint32_t point = 1;
	uint32_t counter = 0;
	uint8_t numbercycles = 0;

	while (numbercycles < cycles) {

		while (Timer3_GetFlag() == TRUE) {
		};

		uint32_t vADC = ADC_v();
		double Vreal = calculateVrefVoltage(vADC);
		uint32_t iADC = ADC_i();
		double Ireal = calculateIcellCurrent(iADC);

		struct Data_S data;
		data.point = point;
		data.timeMs = counter;
		data.voltage = Vreal;
		data.current = Ireal;
		MASB_COMM_S_sendData(data);

		point++;
		counter += SamplingPeriod;

		Timer3_ResetFlag();

		if (equalsf(Vcell, vObjetivo, 0.000001)) {
			if (equalsf(vObjetivo, cvConfiguration.eVertex1, 0.000001)) {
				vObjetivo = cvConfiguration.eVertex2;
			} else if (equalsf(vObjetivo, cvConfiguration.eVertex2, 0.000001)) {
				vObjetivo = cvConfiguration.eBegin;
			} else {
				vObjetivo = cvConfiguration.eVertex1;
				numbercycles++;
			}
		} else { // Solo funcionara si vertex1 > vertex2
			if (equalsf(vObjetivo, cvConfiguration.eVertex1, 0.000001)) {
				if (Vcell + eStep > vObjetivo) {
					Vcell = vObjetivo;
				} else {
					Vcell = Vcell + eStep;
				}
			}
			if (equalsf(vObjetivo, cvConfiguration.eVertex2, 0.000001)) {
				if (Vcell - eStep < vObjetivo) {
					Vcell = vObjetivo;
				} else {
					Vcell = Vcell - eStep;
				}
			}
			if (equalsf(vObjetivo, cvConfiguration.eBegin, 0.000001)) {
				if (Vcell + eStep > vObjetivo) {
					Vcell = vObjetivo;

				} else {
					Vcell = Vcell + eStep;

				}
			}
		}


		MCP4725_SetOutputVoltage(hdac, calculateDacOutputVoltage(Vcell));

	}
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET); // Abrimos el rele
	HAL_TIM_Base_Stop_IT(&htim3); // Paramos el timer

}
