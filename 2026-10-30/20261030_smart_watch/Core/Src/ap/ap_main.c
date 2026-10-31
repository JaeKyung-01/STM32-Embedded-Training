
/*
 * ap_main.c
 *
 *  Created on: 2026. 10. 31.
 *      Author: Moly
 */

#include "ap_main.h"
#include "../driver/led/led.h"
#include "../driver/DS3231/DS3231.h"
#include "../driver/button/button.h"
#include "../common/queue/que_buffer.h"
#include "service/display/display.h"
#include "service/temp_humid/temp_humid.h"
#include "service/distance/distance.h"
#include "service/sound/sound.h"
#include "StopWatch/StopWatch.h"

static smart_watch_mode_t smartWatchMode = CALENDAR;

static led_t ledCalendar;
static led_t ledTempHumid;
static led_t ledDistance;
static led_t ledStopWatch;

static button_t btnMode;
static button_t btn2;
static button_t btn3;

typedef enum
{
	BTN_MODE = 0,
	BTN_2,
	BTN_3
} btn_event_t;

#define BTN_EVENT_Q_SIZE 6

static btn_event_t btnEventBuffer[BTN_EVENT_Q_SIZE];
static que_buf_t que_btnEvent;

// HC-SR04 초기화 전 인터럽트 처리 방지
static volatile int distanceReady = 0;

extern I2C_HandleTypeDef hi2c1;
extern TIM_HandleTypeDef htim4;

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
	if (GPIO_Pin == GPIO_PIN_6 && distanceReady == 1) {
		distance_ISR();
	}
}

// TIM4 10ms 스톱워치 인터럽트
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	if (htim->Instance == TIM4) {
		StopWatch_ISR();
	}
}

static void ap_init(void)
{
	led_init(&ledCalendar, GPIOB, GPIO_PIN_15);
	led_init(&ledTempHumid, GPIOB, GPIO_PIN_14);
	led_init(&ledDistance, GPIOB, GPIO_PIN_13);
	led_init(&ledStopWatch, GPIOC, GPIO_PIN_4);

	led_on(&ledCalendar);
	led_off(&ledTempHumid);
	led_off(&ledDistance);
	led_off(&ledStopWatch);

	ds3231_init(&hi2c1);

	display_init();

	button_init(&btnMode, GPIOC, GPIO_PIN_0);
	button_init(&btn2, GPIOC, GPIO_PIN_1);
	button_init(&btn3, GPIOC, GPIO_PIN_3);

	que_buf_init(&que_btnEvent, btnEventBuffer,
			sizeof(btn_event_t), BTN_EVENT_Q_SIZE);

	TH_init();

	distance_init();
	distanceReady = 1;

	StopWatch_init();
	HAL_TIM_Base_Start_IT(&htim4);

	sound_init();
	sound_power();
}

int ap_main(void)
{
	ap_init();

	btn_event_t btnEvent;

	while (1)
	{
		if (smartWatchMode == STOPWATCH) {
			StopWatch_task();
		}

		if (button_getState(&btnMode) == ACT_RELEASE) {
			btnEvent = BTN_MODE;
			que_put(&que_btnEvent, &btnEvent);
		}

		if (button_getState(&btn2) == ACT_RELEASE) {
			btnEvent = BTN_2;
			que_put(&que_btnEvent, &btnEvent);
		}

		if (button_getState(&btn3) == ACT_RELEASE) {
			btnEvent = BTN_3;
			que_put(&que_btnEvent, &btnEvent);
		}

		if (!que_is_empty(&que_btnEvent)) {
			que_get(&que_btnEvent, &btnEvent);

			switch (btnEvent)
			{
			case BTN_MODE:
				sound_button();

				switch (smartWatchMode)
				{
				case CALENDAR:
					smartWatchMode = TEMP_HUMID;
					break;

				case TEMP_HUMID:
					smartWatchMode = DISTANCE;
					break;

				case DISTANCE:
					smartWatchMode = STOPWATCH;
					break;

				case STOPWATCH:
					smartWatchMode = CALENDAR;
					break;
				}

				led_off(&ledCalendar);
				led_off(&ledTempHumid);
				led_off(&ledDistance);
				led_off(&ledStopWatch);

				switch (smartWatchMode)
				{
				case CALENDAR:
					led_on(&ledCalendar);
					break;

				case TEMP_HUMID:
					led_on(&ledTempHumid);
					break;

				case DISTANCE:
					led_on(&ledDistance);
					display_distanceClear();
					break;

				case STOPWATCH:
					led_on(&ledStopWatch);
					break;
				}
				break;

				case BTN_2:
					switch (smartWatchMode)
					{
					case DISTANCE:
						distance_request();
						sound_button();
						break;

					case STOPWATCH:
						// RUN / STOP은 StopWatch_task()에서 처리
						sound_button();
						break;

					default:
						break;
					}
					break;

					case BTN_3:
						switch (smartWatchMode)
						{
						case STOPWATCH:
							// CLEAR는 StopWatch_task()에서 처리
							if (StopWatch_getState() == STOP ||
									StopWatch_getState() == CLEAR) {
								sound_button();
							}
							break;

						default:
							break;
						}
						break;
			}
		}

		TH_task();

		distance_task();

		display_timeTask();

		switch (smartWatchMode)
		{
		case CALENDAR:
			display_calendarTask();
			break;

		case TEMP_HUMID:
			display_THTask();
			break;

		case DISTANCE:
			display_distanceTask();
			break;

		case STOPWATCH:
			display_stopWatchTask();
			break;
		}
	}

	return 0;
}
