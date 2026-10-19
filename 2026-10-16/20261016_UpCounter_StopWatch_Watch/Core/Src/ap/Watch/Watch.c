/*
 * StopWatch.c
 *
 *  Created on: 2026. 10. 16.
 *      Author: kccistc
 */

#include "Watch.h"
#include "../../driver/button/button.h"
#include "../../driver/led/led.h"
#include "../../driver/fnd/fnd.h"

typedef enum {TIME} Watch_t;

static void Watch_time();
//static void Watch_setState(Watch_t s);
//static Watch_t Watch_getState(void);

static Watch_t Watch_state;
static uint8_t hour = 0;
static uint8_t minute = 0;
static uint8_t second = 0;

static led_t led_watch;
static led_t led_stopwatch;
static led_t led_stop;
static led_t led_run;

static uint32_t prevTime = 0;

extern TIM_HandleTypeDef htim3;

void Watch_RealTime(void)
{
	uint32_t curTime = HAL_GetTick();
	if (curTime - prevTime >= 1000){
		prevTime = curTime;
		second++;

		if (second >= 60)
		{
			second = 0;
			minute++;
		}

		if (minute >= 60)
		{
			minute = 0;
			hour++;
		}
		if (hour >= 24)
		{
			second = 0;
			minute = 0;
			hour = 0;
		}

	}
}

// Watch 초기화 함수
void Watch_init(void)
{
	// FSM 상태 및 카운터 초기화
	Watch_state = TIME;
	second = 0;
	minute = 0;
	hour = 0;

	// led 초기화
	led_init(&led_watch, GPIOB, GPIO_PIN_15);
	led_init(&led_stopwatch, GPIOB, GPIO_PIN_14);
	led_init(&led_stop, GPIOB, GPIO_PIN_13);
	led_init(&led_run, GPIOC, GPIO_PIN_4);
}


void Watch_task(void)
{
	switch(Watch_state)
	{
	case TIME:
		Watch_time();
		break;
	default:
		Watch_time();
		break;
	}
}

static void Watch_time()
{
	// watch에서 상태가 time일때 led 동작
	led_on(&led_watch);
	led_off(&led_stopwatch);
	led_off(&led_stop);
	led_off(&led_run);

	Watch_RealTime;

	uint16_t num = minute * 100 + second;
	fnd_set_num(num);

}
