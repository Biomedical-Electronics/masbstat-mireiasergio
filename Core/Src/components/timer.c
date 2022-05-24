/*
 * timer.c
 *
 *  Created on: 4 may. 2022
 *      Author: sergioventeo
 */

#include "components/timer.h"
#include "components/masb_comm_s.h"

extern TIM_HandleTypeDef htim3;

extern struct CA_Configuration_S caConfiguration;
extern struct CV_Configuration_S cvConfiguration;

void Timer3_CV(void) {
	HAL_TIM_Base_DeInit(&htim3);
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 8399;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period =cvConfiguration.eStep/cvConfiguration.scanRate;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim3) != HAL_OK){
		_Error_Handler(__FILE__, __LINE__);
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
		 _Error_Handler(__FILE__, __LINE__);
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) {
		 _Error_Handler(__FILE__, __LINE__);
	}
	HAL_TIM_Base_Start_IT(&htim3);

}

void Timer3_CA(void) {
	HAL_TIM_Base_DeInit(&htim3);
	TIM_ClockConfigTypeDef sClockSourceConfig;
	TIM_MasterConfigTypeDef sMasterConfig;

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 8399;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period =caConfiguration.samplingPeriodMs;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	if (HAL_TIM_Base_Init(&htim3) != HAL_OK){
		_Error_Handler(__FILE__, __LINE__);
	}

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK) {
		 _Error_Handler(__FILE__, __LINE__);
	}
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK) {
		 _Error_Handler(__FILE__, __LINE__);
	}

	HAL_TIM_Base_Start_IT(&htim3);
}
