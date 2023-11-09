/*
 * tick.c
 *
 *  Created on: Jan 7, 2020
 *      Author: ipopa
 */

#include "_tos_inc/tick.h"

TOS_TICKS __tos_wall_tick;
TOS_CLOCK_HZ __tos_tick_clock;


TOS_TICKS tos_ticks()
{
	return __tos_wall_tick;
}

TOS_CLOCK_HZ tos_hz_per_tick()
{
	return __tos_tick_clock;
}

void tos_set_hz_per_tick(const TOS_CLOCK_HZ hz)
{
	__tos_tick_clock = hz;
}
