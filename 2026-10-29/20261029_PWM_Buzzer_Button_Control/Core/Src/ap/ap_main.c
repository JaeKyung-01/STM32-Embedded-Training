/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 28.
 *      Author: rhoblack
 */

#include "ap_main.h"
#include "../driver/button/button.h"

extern TIM_HandleTypeDef htim1;

#define PWM_TIMER_CLK 1000000U
#define PWM_FREQ_MIN 20U
#define PWM_FREQ_MAX 20000U
#define PWM_FREQ_STEP 100U
#define PWM_DUTY_MAX 90U
#define PWM_DUTY_STEP 10U

static void PWM_update(uint32_t frequency, uint32_t duty)
{
	uint32_t period;
	uint32_t pulse;

	period = PWM_TIMER_CLK / frequency;
	pulse = period * duty / 100;

	__HAL_TIM_SET_AUTORELOAD(&htim1, period - 1);
	__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_4, pulse);
}

int ap_main(void)
{
	button_t btn_frequency;
	button_t btn_duty;

	uint32_t frequency = 1000;
	uint32_t duty = 50;

	button_init(&btn_frequency, GPIOC, GPIO_PIN_0);
	button_init(&btn_duty, GPIOC, GPIO_PIN_1);

	PWM_update(frequency, duty);

	while (1)
	{
		if (button_getState(&btn_frequency) == ACT_RELEASE)
		{
			frequency += PWM_FREQ_STEP;

			if (frequency > PWM_FREQ_MAX)
			{
				frequency = PWM_FREQ_MIN;
			}

			PWM_update(frequency, duty);
		}

		if (button_getState(&btn_duty) == ACT_RELEASE)
		{
			duty += PWM_DUTY_STEP;

			if (duty > PWM_DUTY_MAX)
			{
				duty = 0;
			}

			PWM_update(frequency, duty);
		}
	}

	return 0;
}
