#include "components/stm32main.h"
#include "components/masb_comm_s.h"
#include "components/chrono_amperometry.h"
#include "components/cyclic_voltammetry.h"
#include "components/ad5280_driver.h" //
#include "components/mcp4725_driver.h"
#include "components/i2c_lib.h"

struct CV_Configuration_S cvConfiguration; // assign the CV configuration structure to cvConfiguration variable
struct CA_Configuration_S caConfiguration; //we assign the CA configuration structure to caConfiguration variable
struct Data_S data;
MCP4725_Handle_T hdac = NULL;


// setup function, to be called only when the program starts
void setup(struct Handles_S *handles) {  //takes as parameter the Handles_S structure
	HAL_GPIO_WritePin(EN_GPIO_Port, EN_Pin, GPIO_PIN_SET);
	HAL_Delay(500);

	// pass the pointers
	MASB_COMM_S_setUart(handles->huart2);
	DAC_setup();
	I2C_init(handles->hi2c);

	// potentiometer
	AD5280_Handle_T hpot = NULL;
	hpot = AD5280_Init();
	AD5280_ConfigSlaveAddress(hpot, 0x2C);
	AD5280_ConfigNominalResistorValue(hpot, 50e3f);
	AD5280_ConfigWriteFunction(hpot, I2C_write);
	AD5280_SetWBResistance(hpot, 50e3f);

	// DAC
	hdac = MCP4725_Init();
	MCP4725_ConfigSlaveAddress(hdac, 0x66);
	MCP4725_ConfigVoltageReference(hdac, 4.0f);
	MCP4725_ConfigWriteFunction(hdac, I2C_write);

	MASB_COMM_S_waitForMessage();

	Estado = IDLE;
}

// loop function, repeated until the program stops
void loop(void) {
	if (MASB_COMM_S_dataReceived()) { // If a message has been received

		switch (MASB_COMM_S_command()) { // Check what has been received
			case START_CV_MEAS: // If START_CV_MEAS was received

				cvConfiguration = MASB_COMM_S_getCvConfiguration(); //we read the received configuration and store it in the cvConfiguration variable

				Estado = CV; // Change the Estado variable to CV (0)
				__NOP(); // To place a breakpoint, can be erased

				break;

			case START_CA_MEAS: // If START_CA_MEAS was received

				caConfiguration = MASB_COMM_S_getCaConfiguration(); //we read the received configuration and store it in the caConfiguration variable

				Estado = CA; // Change the Estado variable to CA (1)
				__NOP(); // To place a breakpoint, can be erased

				break;

			case STOP_MEAS: // If STOP_MEAS was received

				Estado = IDLE;
				__NOP();


				break;

			default: // If an unknown command is received, wait for another message

				break;

		}
		// We wait for another message

		MASB_COMM_S_waitForMessage();
	} else { // In case no command is not received, meaning that we have already received one
		//the Estado variable is checked

		switch (ESTADO) {
		case CV:  // in case of CV, the CV_meas function is called
			CV_meas(cvConfiguration);
			Estado = IDLE;  // change the Estado variable to IDLE (2)
			__NOP();
			break;


		case CA:
			CA_meas(caConfiguration);
			Estado = IDLE;
			__NOP();
			break;


		default:
			break;

		}

	}


}

