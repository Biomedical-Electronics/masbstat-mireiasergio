/**
  ******************************************************************************
  * @file		chronoamperometry.h
  * @brief		Archivo de encabezado del componente encargado de la gestión de
  *             la cronoamperometría.
  * @author		Mireia Alibau, Sergio Venteo
  * @copyright	Copyright 2022 Mireia Alibau, Sergio Venteo. All rights reserved.
  ******************************************************************************
  */

#ifndef INC_COMPONENTS_CHRONO_AMPEROMETRY_H_
#define INC_COMPONENTS_CHRONO_AMPEROMETRY_H_

struct CA_Configuration_S {
	double eDC;
	long samplingPeriodMs;
	long measurementTime;
};

#endif /* INC_COMPONENTS_CHRONO_AMPEROMETRY_H_ */
