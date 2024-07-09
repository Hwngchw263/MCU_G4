#ifndef DRIVER_LPIT_H_
#define DRIVER_LPIT_H_
#include "S32K144.h"
#include <stdio.h>

typedef enum{
	LPIT_TIMER_CHANNEL0 = 0,
	LPIT_TIMER_CHANNEL1 = 1,
	LPIT_TIMER_CHANNEL2 = 2,
	LPIT_TIMER_CHANNEL3 = 3,
} Lpit_timer_t;
void Timer_Init(Lpit_timer_t channel);
void Reset_timer_value(Lpit_timer_t channel,uint32_t value);
void Start_timer(Lpit_timer_t channel);
void Stop_timer(Lpit_timer_t channel);
#endif /*DRIVER_LPIT_H_*/
