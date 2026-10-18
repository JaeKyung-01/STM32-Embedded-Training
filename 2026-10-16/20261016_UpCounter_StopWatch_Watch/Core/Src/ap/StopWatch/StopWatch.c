/*
 * StopWatch.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#include "StopWatch.h"
#include "../../driver/button/button.h"
#include "../../driver/led/led.h"
#include "../../driver/fnd/fnd.h"

typedef enum {STOP, RUN, CLEAR} stopWatch_t;

static void StopWatch_stop();
static void StopWatch_run();
static void StopWatch_clear();
static void StopWatch_setState(stopWatch_t s);
static stopWatch_t StopWatch_getState(void);
static void StopWatch_incCounter(void);
static void StopWatch_clearCounter(void);
static void StopWatch_setCounter(uint16_t c);
static uint16_t StopWatch_getCounter(void);

static stopWatch_t stopWatch_state;
static uint16_t counter;

static led_t led_watch;
static led_t led_stopwatch;
static led_t led_stop;
static led_t led_run;

static button_t btn_run_stop;
static button_t btn_clear;

extern TIM_HandleTypeDef htim3;

static void StopWatch_setState(stopWatch_t s)
{
	stopWatch_state = s;
}

static stopWatch_t StopWatch_getState(void)
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

// StopWatch 초기화 함수
void StopWatch_init(void)
{
	// FSM 상태 및 카운터 초기화
	stopWatch_state = STOP;
	counter = 0;

	// led 초기화
	led_init(&led_watch, GPIOB, GPIO_PIN_15);
	led_init(&led_stopwatch, GPIOB, GPIO_PIN_14);
	led_init(&led_stop, GPIOB, GPIO_PIN_13);
	led_init(&led_run, GPIOC, GPIO_PIN_4);

	// button 초기화
	button_init(&btn_run_stop, GPIOC, GPIO_PIN_1);
	button_init(&btn_clear, GPIOC, GPIO_PIN_3);

	// TIM3 타이머 인터럽트 시작 (1ms 주기)
	HAL_TIM_Base_Start_IT(&htim3);
}


void StopWatch_task(void)
{
	switch(stopWatch_state)
	{
	case STOP:
		StopWatch_stop();
		break;
	case RUN:
		StopWatch_run();
		break;
	case CLEAR:
		StopWatch_clear();
		break;
	default:
		StopWatch_stop();
		break;
	}
}


static void StopWatch_stop()
{
	// 처음 STOP상태 LED 출력
	led_off(&led_watch);
	led_on(&led_stopwatch);
	led_on(&led_stop);
	led_off(&led_run);

	if (button_getState(&btn_run_stop) == ACT_PUSH){
		StopWatch_setState(RUN);
		// stopwatch에서 상태가 run일때 led 동작
		led_off(&led_watch);
		led_on(&led_stopwatch);
		led_off(&led_stop);
		led_on(&led_run);
	}
	else if(button_getState(&btn_clear) == ACT_RELEASE){
		StopWatch_setState(CLEAR);
	}

	uint16_t num = StopWatch_getCounter();
	fnd_set_num(num);
}

static void StopWatch_run()
{
	if (button_getState(&btn_run_stop) == ACT_PUSH) {
		StopWatch_setState(STOP);
		// stopwatch에서 상태가 stop일때 led 동작
		led_off(&led_watch);
		led_on(&led_stopwatch);
		led_on(&led_stop);
		led_off(&led_run);
	}

	static uint32_t prevTime = 0;
	uint32_t curTime = HAL_GetTick();
	if (curTime - prevTime < 90){
		return;
	}
	prevTime = curTime;

	uint16_t num = StopWatch_getCounter();

	if (num == 9999) {
		StopWatch_clearCounter();
	}
	else {
		StopWatch_incCounter();
	}
	num = StopWatch_getCounter();
	fnd_set_num(num);
}

static void StopWatch_clear()
{
	uint16_t num;

	StopWatch_clearCounter();
	num = StopWatch_getCounter();

	fnd_set_num(num);
	StopWatch_setState(STOP);
}
