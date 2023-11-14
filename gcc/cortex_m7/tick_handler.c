/*
 * tick_handler.c
 *
 *  Created on: Jan 7, 2020
 *      Author: ipopa
 */

#include "_tos_inc/tick.h"
#include "_tos_inc/process.h"
#include "_tos_inc/exception.h"

#include "tos_startup.h"

void TOSSysTick_Handler()
{
	__tos_disable_exceptions();
	__tos_context_processes_tick_updates(++__tos_wall_tick);
	__tos_clocks_tick_callback();
	__tos_enable_exceptions();
}



