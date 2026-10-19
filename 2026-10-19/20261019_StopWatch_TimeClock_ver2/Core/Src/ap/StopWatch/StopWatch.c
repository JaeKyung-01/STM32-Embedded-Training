/*
 * StopWatch.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: rhoblack
 */

#include "StopWatch.h"
#include "../../driver/button/button.h"
#include "../../driver/led/led.h"
#include "../../driver/fnd/fnd.h"
#include "../TimeClock/TimeClock.h"

static void StopWatch_stop();
static void StopWatch_run();
static void StopWatch_clear();
static void StopWatch_setState(stopWatch_t s);
stopWatch_t StopWatch_getState(void);
static void StopWatch_incCounter(void);
static void StopWatch_clearCounter(void);
static void StopWatch_setCounter(uint16_t c);
static uint16_t StopWatch_getCounter(void);

extern TIM_HandleTypeDef htim4;

static TimeClock_t stopWatchData;

static stopWatch_t stopWatch_state;
static uint16_t counter;

led_t led_run;
led_t led_stop;

static button_t btn_run_stop;
static button_t btn_clear;

extern TIM_HandleTypeDef htim3;

static void StopWatch_setState(stopWatch_t s)
{
	stopWatch_state = s;
}

stopWatch_t StopWatch_getState(void)
{
	return stopWatch_state;
}

static void StopWatch_incCounter(void)
{
	counter++;
}

static void StopWatch_clearCounter(void)
{
	counter = 0;
}

static void StopWatch_setCounter(uint16_t c)
{
	counter = c;
}

static uint16_t StopWatch_getCounter(void)
{
	return counter;
}

void StopWatch_incMsec(void)
{
	if (stopWatchData.msec < 99) {
		stopWatchData.msec++;
		return;
	}
	stopWatchData.msec = 0;
	if (stopWatchData.sec < 59) {
		stopWatchData.sec++;
		return;
	}
	stopWatchData.sec = 0;
	if (stopWatchData.min < 59) {
		stopWatchData.min++;
		return;
	}
	stopWatchData.min = 0;
	if (stopWatchData.hour < 23) {
		stopWatchData.hour++;
		return;
	}
	stopWatchData.hour = 0;
}

void StopWatch_ISR(void)
{
	if (stopWatch_state == RUN) {
		StopWatch_incMsec();
	}
}


void StopWatch_init(void)
{
	stopWatch_state = STOP;
	stopWatchData.hour = 0;
	stopWatchData.min = 0;
	stopWatchData.sec = 0;
	stopWatchData.msec = 0;

	led_init(&led_stop, GPIOB, GPIO_PIN_13);
	led_init(&led_run, GPIOC, GPIO_PIN_4);

	button_init(&btn_run_stop, GPIOC, GPIO_PIN_1);
	button_init(&btn_clear, GPIOC, GPIO_PIN_3);

	HAL_TIM_Base_Start_IT(&htim3);
}

void StopWatch_task(void)
{
	switch(stopWatch_state)
	{
	case STOP:
		led_off(&led_run);
		led_on(&led_stop);
		StopWatch_stop();
		break;
		break;
	case RUN:
		led_on(&led_run);
		led_off(&led_stop);
		StopWatch_run();
		break;
	case CLEAR:
		led_off(&led_run);
		led_on(&led_stop);
		StopWatch_clear();
		break;
	default:
		led_off(&led_run);
		led_on(&led_stop);
		StopWatch_stop();
		break;
	}
}

static void StopWatch_stop()
{
	if (button_getState(&btn_run_stop) == ACT_PUSH) {
		StopWatch_setState(RUN);
	}
	else if (button_getState(&btn_clear) == ACT_RELEASE) {
		StopWatch_setState(CLEAR);
	}

	uint16_t num = (stopWatchData.sec * 100) + stopWatchData.msec;
	fnd_set_num(num);
	fnd_set_dp(FND_DIGIT_2, FND_DP_OFF);
}

static void StopWatch_run()
{
	if (button_getState(&btn_run_stop) == ACT_PUSH) {
		StopWatch_setState(STOP);
	}

	uint16_t num = (stopWatchData.sec * 100) + stopWatchData.msec;
	fnd_set_num(num);

	if (stopWatchData.msec < 50) {
		fnd_set_dp(FND_DIGIT_2, FND_DP_ON);
	}
	else {
		fnd_set_dp(FND_DIGIT_2, FND_DP_OFF);
	}
}

static void StopWatch_clear()
{
	stopWatchData.hour = 0;
	stopWatchData.min = 0;
	stopWatchData.sec = 0;
	stopWatchData.msec = 0;

	fnd_set_num(0);
	fnd_set_dp(FND_DIGIT_2, FND_DP_OFF);

	StopWatch_setState(STOP);
}
