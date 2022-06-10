/*
 * timer.h
 *
 *  Created on: 4 may. 2022
 *      Author: sergioventeo
 */

#ifndef INC_COMPONENTS_TIMER_H_
#define INC_COMPONENTS_TIMER_H_

#include "components/masb_comm_s.h"

void Timer3_CV(uint32_t Period);
void Timer3_CA(void);
void Timer3_SetFlag(void);
void Timer3_ResetFlag(void);
_Bool Timer3_GetFlag();


#endif /* INC_COMPONENTS_TIMER_H_ */

