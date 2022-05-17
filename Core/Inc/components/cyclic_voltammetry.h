/*
 * cyclic_voltammetry.h
 *
 *  Created on: May 3, 2022
 *      Author: sergioventeo
 */

#ifndef INC_COMPONENTS_CYCLIC_VOLTAMMETRY_H_
#define INC_COMPONENTS_CYCLIC_VOLTAMMETRY_H_

struct CV_Configuration_S {
	double eBegin;
	double eVertex1;
	double eVertex2;
	uint8_t cycles;
	double scanRate;
	double eStep;
};

#endif /* INC_COMPONENTS_CYCLIC_VOLTAMMETRY_H_ */

