/**
  ******************************************************************************
<<<<<<< HEAD
  * @file		chronoamperometry.h
=======
  * @file		chrono_amperometry.h
>>>>>>> 15baa01 (Cronoamperometria)
  * @brief		Archivo de encabezado del componente encargado de la gestión de
  *             la cronoamperometría.
  * @author		Mireia Alibau, Sergio Venteo
  * @copyright	Copyright 2022 Mireia Alibau, Sergio Venteo. All rights reserved.
  ******************************************************************************
  */

#ifndef INC_COMPONENTS_CHRONO_AMPEROMETRY_H_
#define INC_COMPONENTS_CHRONO_AMPEROMETRY_H_

#include "components/mcp4725_driver.h"
#include "components/masb_comm_s.h"
#include "components/ad5280_driver.h"
#include "components/i2c_lib.h"
#include "components/stm32main.h"
#include "main.h"


// Macros
#define FALSE	0
#define TRUE	1


// CA configuration structure that stores the parameters received from viSense-S
struct CA_Configuration_S {

	double eDC;
	uint32_t samplingPeriodMs;
	uint32_t measurementTime;

};


//Prototypes
void CA_start(struct CA_Configuration_S caConfiguration);
void CA_sendData(void);


#endif /* INC_COMPONENTS_CHRONO_AMPEROMETRY_H_ */
