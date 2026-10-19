/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#include "ap_main.h"
#include "Watch/Watch.h"
#include "StopWatch/StopWatch.h"
#include "../driver/button/button.h"
#include "../driver/fnd/fnd.h"

typedef enum {WATCH, STOPWATCH}  mode_t;

static button_t btn_mode;
static mode_t mode = WATCH;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM3) {
		fnd_show_number();
	}
}

int ap_main(void)
{
	Watch_init();
	StopWatch_init();
	button_init(&btn_mode, GPIOC, GPIO_PIN_0);

	while(1) //super loop
	{
		Watch_RealTime();

		switch(mode)
		{
		case WATCH:
			Watch_task();
			break;

		case STOPWATCH:
			StopWatch_task();
			break;

		default :
			Watch_task();
			break;
		}
		if(button_getState(&btn_mode) == ACT_PUSH)
		{
			if(mode == WATCH)
			{
				mode = STOPWATCH;
			}
			else
			{
				mode = WATCH;
			}
		}
	}
	return 0;
}
