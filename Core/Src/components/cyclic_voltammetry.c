/*
 * cyclic_voltammetry.c
 *
 *  Created on: May 3, 2022
 *      Author: sergioventeo
 */

#include "components/cyclic_voltammetry.h"
#include "components/masb_comm_s.h"

void cyclic_voltammetry(struct CV_Configuration_S cvConfiguration) {
	Vcell=cvConfiguration.eBegin;
	vObjetivo=cvConfiguration.eVertex1;
	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET);


	uint8_t cycles = cvConfiguration.cycles;
	double scanRate = cvConfiguration.scanRate;
	samplingPeriodMs = caConfiguration.samplingPeriodMs;
	double eStep = cvConfiguration.eStep;
	period=eStep/scanRate;


	Timer3_CV(period);

	uint8_t counter = 0;
	uint8_t point = 0;

	while (i<cycles) {

		uint32_t vADC=ADC_v();
		Vreal=calculateVrefVoltage(vADC);
		uint32_t iADC=ADC_i();
		Ireal=calculateIcellCurrent(iADC);

		struct Data_S data;
		data.point = point;
		data.timeMs = counter;
		data.voltage = V_CELL;
		data.current = I_CELL;
		MASB_COMM_S_sendData(data);

		counter = counter + samplingPeriodMs;
		point++;

		if (Vreal==vObjetivo) {
			if (vObjetivo==cvConfiguration.eVertex1) {
				vObjetivo = cvConfiguration.eVertex2;
			} else if (vObjetivo == cvConfiguration.eVertex2){
				vObjetivo = cvConfiguration.eBegin;
			} else if (i==cycles) {
				HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET);
			} else {
				vObjetivo = cvConfiguration.eVertex1;
				i ++;
			}

		} else {
			if (Vreal + eStep > vObjetivo) {
				Vreal = vObjetivo;
			} else {
				if (Vcell > vObjetivo){
					Vreal -= eStep;
				}else{
					Vreal += eStep;
				}
			}
		}
	}

}
