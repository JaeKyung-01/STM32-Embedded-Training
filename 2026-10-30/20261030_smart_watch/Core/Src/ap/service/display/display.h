
/*
 * display.h
 *
 *  Created on: 2026. 10. 28.
 *      Author: rhoblack
 */

#ifndef SRC_AP_SERVICE_DISPLAY_DISPLAY_H_
#define SRC_AP_SERVICE_DISPLAY_DISPLAY_H_

void display_init(void);

void display_timeTask(void);
void display_calendarTask(void);
void display_THTask(void);
void display_distanceTask(void);
void display_distanceClear(void);
void display_stopWatchTask(void);

#endif /* SRC_AP_SERVICE_DISPLAY_DISPLAY_H_ */
