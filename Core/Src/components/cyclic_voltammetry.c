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



}
