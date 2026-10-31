
/*
 * display.c
 *
 *  Created on: 2026. 10. 28.
 *      Author: rhoblack
 */

#include "display.h"
#include "../temp_humid/temp_humid.h"
#include "../../../driver/lcd/lcd.h"
#include "../../../driver/DS3231/DS3231.h"
#include "../distance/distance.h"
#include "../../StopWatch/StopWatch.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

void display_init(void)
{
	Lcd_init(&hi2c1);
}

// LCD 1행: 현재 시간
void display_timeTask(void)
{
	static uint32_t prevTime = 0;

	if (HAL_GetTick() - prevTime < 1000) {
		return;
	}

	prevTime = HAL_GetTick();

	char buff[30];
	ds3231_time_t timeData;

	ds3231_getTime(&timeData);

	sprintf(buff, "TIME %02u:%02u:%02u   ",
			timeData.hour, timeData.min, timeData.sec);

	Lcd_writeStringXY(0, 0, buff);
}

// LCD 2행: 날짜 및 요일
void display_calendarTask(void)
{
	static uint32_t prevTime = 0;

	if (HAL_GetTick() - prevTime < 1000) {
		return;
	}

	prevTime = HAL_GetTick();

	char buff[30];
	char dayStr[4] = "---";
	ds3231_date_t dateData;

	ds3231_getDate(&dateData);

	switch (dateData.day)
	{
	case 1:
		sprintf(dayStr, "MON");
		break;

	case 2:
		sprintf(dayStr, "TUE");
		break;

	case 3:
		sprintf(dayStr, "WED");
		break;

	case 4:
		sprintf(dayStr, "THU");
		break;

	case 5:
		sprintf(dayStr, "FRI");
		break;

	case 6:
		sprintf(dayStr, "SAT");
		break;

	case 7:
		sprintf(dayStr, "SUN");
		break;

	default:
		break;
	}

	sprintf(buff, "20%02u-%02u-%02u %s  ",
			dateData.year,
			dateData.month,
			dateData.date,
			dayStr);

	Lcd_writeStringXY(1, 0, buff);
}

// LCD 2행: 온도 및 습도
void display_THTask(void)
{
	static uint32_t prevTime = 0;

	if (HAL_GetTick() - prevTime < 1000) {
		return;
	}

	prevTime = HAL_GetTick();

	char buff[30];
	dht11_data_t thData;

	TH_getTempHumid(&thData);

	sprintf(buff, "T:%2u.%uC H:%2u%%   ",
			thData.t_int,
			thData.t_dec,
			thData.h_int);

	Lcd_writeStringXY(1, 0, buff);
}

// LCD 2행: 초음파 거리 측정
void display_distanceTask(void)
{
	uint16_t dist_cm;
	char buff[30];

	if (distance_get(&dist_cm) == DISTANCE_OK) {
		sprintf(buff, "DIST: %3u cm    ", dist_cm);
		Lcd_writeStringXY(1, 0, buff);
	}
}

void display_distanceClear(void)
{
	Lcd_writeStringXY(1, 0, "DIST: --- cm    ");
}

// LCD 2행: 스톱워치
void display_stopWatchTask(void)
{
	static uint32_t prevTime = 0;

	if (HAL_GetTick() - prevTime < 50) {
		return;
	}

	prevTime = HAL_GetTick();

	char buff[30];
	stopWatch_t *timeData = StopWatch_getTimeData();

	sprintf(buff, "SW %02u:%02u:%02u.%02u  ",
			timeData->hour,
			timeData->min,
			timeData->sec,
			timeData->msec);

	Lcd_writeStringXY(1, 0, buff);
}
