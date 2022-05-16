/**
  ******************************************************************************
  * @file		chrono_amperometry.c
  * @brief		Gestión de la cronoamperometría.
  * @author		Mireia Alibau, Sergio Venteo
  * @copyright	Copyright 2022 Mireia Alibau, Sergio Venteo. All rights reserved.
  ******************************************************************************
  */

#include "components/chrono_amperometry.h"
#include "components/formulas.h"
#include "components/mcp4725_driver.h"
#include "components/adc.h"

extern MCP4725_Handle_T hdac;
extern ADC_HandleTypeDef hadc1;
extern TIM_HandleTypeDef htim3;

volatile _Bool samplingPeriod = FALSE;

void CA_start(struct CA_Configuration_S caConfiguration) {

	double eDC = caConfiguration.eDC;
	float VDAC = calculateDacOutputVoltage(eDC); // Calculamos la tension
	MCP4725_SetOutputVoltage(hdac, VDAC); // Fijamos Vcell a eDC

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_SET); // Cerramos el rele


	samplingPeriodMs = caConfiguration.samplingPeriodMs; // Periodo de muestreo
	measurementTime = caConfiguration.measurementTime*1000; // En ms


	__HAL_TIM_SET_AUTORELOAD(&htim3,samplingPeriodMs * 10); // Fijamos el periodo
	// ⚠ Solo si se fija una frecuecia de trabajo de 10 KHz para el timer,
	// mutliplicar samplingPeriod por 10 ⚠

	__HAL_TIM_SET_COUNTER(&htim3,0); // Reiniciamos el counter del timer
	HAL_TIM_Base_Start_IT(&htim3); // E iniciamos el timer

	uint32_t point = 0; // Punto
	uint32_t counter = 0; // Contador

	CA_sendData(); // Enviamos la primera medida

	while (counter <= measurementTime) { // Mientras que no se haya superado el tiempo total del experimento...
		if (samplingPeriod == TRUE){ // y el tiempo entre muestras haya pasado...
			CA_sendData(); // enviamos los datos
		}
	}
	HAL_TIM_Base_Stop_IT(&htim3); // Paramos el timer

	HAL_GPIO_WritePin(RELAY_GPIO_Port, RELAY_Pin, GPIO_PIN_RESET); // Abrimos el rele
}

void CA_sendData(void){ // Funcion para enviar datos
	samplingPeriod = FALSE;
	point++;

	uint32_t vADC=ADC_v(); // Compute vADC
	double VCell = calculateVrefVoltage(vADC); // Calibracion

	uint32_t iADC=ADC_i(); // Compute iADC
	double ICell = calculateIcellCurrent(iADC);


	struct Data_S data; // Enviamos datos a la estructura
	data.point = point;
	data.timeMs = counter;
	data.voltage = VCell;
	data.current = ICell;
	counter = counter + samplingPeriodMs;

	MASB_COMM_S_sendData(data);
}
