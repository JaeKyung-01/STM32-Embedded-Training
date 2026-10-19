/*
 * StopWatch.h
 *
 *  Created on: 2026. 10. 16.
 *      Author: rhoblack
 */

#ifndef SRC_AP_STOPWATCH_STOPWATCH_H_
#define SRC_AP_STOPWATCH_STOPWATCH_H_

typedef enum {STOP, RUN, CLEAR} stopWatch_t;

stopWatch_t StopWatch_getState(void);
void StopWatch_ISR(void);

void StopWatch_incMsec(void);
void StopWatch_init(void);
void StopWatch_task(void);

#endif /* SRC_AP_STOPWATCH_STOPWATCH_H_ */
