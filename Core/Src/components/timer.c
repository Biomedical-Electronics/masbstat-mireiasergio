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

static _Bool wait = TRUE;


void Timer3_CV(Period) {
	HAL_TIM_Base_DeInit(&htim3);
	TIM_ClockConfigTypeDef sClockSourceConfig = { 0 };
	TIM_MasterConfigTypeDef sMasterConfig = { 0 };

	htim3.Instance = TIM3;
	htim3.Init.Prescaler = 8399;
	htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
	htim3.Init.Period = Period;
	htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
	htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;

	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIM_Base_Init(&htim3);
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


	sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;

	sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIM_Base_Start_IT(&htim3);
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim3) {
	Timer3_SetFlag();
}

void Timer3_ResetFlag() {
	wait = TRUE;
}

void Timer3_SetFlag(){
	wait =  FALSE;
}

_Bool Timer3_GetFlag(){
	return wait;
}
