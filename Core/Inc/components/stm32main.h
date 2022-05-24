#ifndef INC_COMPONENTS_STM32MAIN_H_
#define INC_COMPONENTS_STM32MAIN_H_

#include "stm32f4xx_hal.h"
#include "components/masb_comm_s.h"
#include "components/chrono_amperometry.h"
#include "components/cyclic_voltammetry.h"
#include "components/mcp4725_driver.h"
#include "components/dac.h"
#include "components/ad5280_driver.h"


// Macros
#define	CV	0
#define CA	1
#define IDLE	2


// Variable initialization
uint32_t Estado;


struct Handles_S {  // structure to handle the pointers
	UART_HandleTypeDef *huart2;
	I2C_HandleTypeDef *hi2c;
	ADC_HandleTypeDef *hadc1;
	TIM_HandleTypeDef *htim3;

};

// Prototypes
void setup(struct Handles_S *handles);
void loop(void);

#endif /* INC_COMPONENTS_STM32MAIN_H_ */
