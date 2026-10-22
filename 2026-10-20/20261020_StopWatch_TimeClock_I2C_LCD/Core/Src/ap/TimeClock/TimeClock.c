/*
 * TimeClock.c
 *
 *  Created on: 2026. 10. 19.
 *      Author: kccistc
 */
#include "TimeClock.h"
#include "../../driver/fnd/fnd.h"

static TimeClock_t timeClockData; // 중심 데이터

void TimeClock_init(void){
	timeClockData.hour = 0;
	timeClockData.min = 0;
	timeClockData.sec = 0;
	timeClockData.msec = 0;
}

TimeClock_t *TimeClock_getTimeData(void)
{
	return &timeClockData;
}

static void TimeClock_incMsec(void)
{
	if (timeClockData.msec < 99) {
		timeClockData.msec++;
		return;
	}
	timeClockData.msec = 0;
	if (timeClockData.sec < 59) {
		timeClockData.sec++;
		return;
	}
	timeClockData.sec = 0;
	if (timeClockData.min < 59) {
		timeClockData.min++;
		return;
	}
	timeClockData.min = 0;
	if (timeClockData.hour < 23) {
		timeClockData.hour++;
		return;
	}
	timeClockData.hour = 0;
}

void TimeClock_ISR(void)
{
	TimeClock_incMsec();
}

void TimeClock_task(void)
{

}
