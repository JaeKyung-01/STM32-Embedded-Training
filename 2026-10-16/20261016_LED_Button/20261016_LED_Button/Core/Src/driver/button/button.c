/*
 * button.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#include "button.h"

button_state_t button_getState(button_t *btn)
{
	int curState;

	curState = HAL_GPIO_ReadPin(btn -> GPIOx, btn ->GPIO_Pin);

	if ((btn -> prevState == RELEASED) && (curState == PUSHED)) {
		btn -> prevState = PUSHED;
		HAL_Delay(5); //debounce code
		return ACT_PUSH;
	}
	else if((btn -> prevState == PUSHED) && (curState == RELEASED)) {
		btn -> prevState = RELEASED;
		HAL_Delay(5); //debounce code
		return ACT_RELEASE;
	}
	return NO_ACT;
}
