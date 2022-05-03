/*
 * pmu.c
 *
 *  Created on: May 3, 2022
 *      Author: sergioventeo
 */


void Init_PMU(void) {  //Empezamos PMU
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET); //habilitamos PMU
}
