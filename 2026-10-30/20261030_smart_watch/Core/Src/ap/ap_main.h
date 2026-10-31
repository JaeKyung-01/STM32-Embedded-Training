/*
 * ap_main.h
 *
 *  Created on: 2026. 10. 31.
 *      Author: Moly
 */

#ifndef SRC_AP_AP_MAIN_H_
#define SRC_AP_AP_MAIN_H_

typedef enum
{
	CALENDAR,
	TEMP_HUMID,
	DISTANCE,
	STOPWATCH
} smart_watch_mode_t;


int ap_main(void);

#endif /* SRC_AP_AP_MAIN_H_ */
