/*
 * scheduler.c
 *
 *  Created on: Jan 3, 2020
 *      Author: ipopa
 */

#include "tos.h"
#include "_tos_inc/process.h"
#include "_tos_inc/exception.h"

struct TOS_PROCESS_DESCRIPTOR* __tos_current_pid;
struct TOS_PROCESS_DESCRIPTOR* __tos_next_pid;

#define PENDSV_EXCEPTION 14

static inline void __tos_triger_to_ctxt_switch (struct TOS_PROCESS_DESCRIPTOR* next)
{
	if (__tos_next_pid->priority < next->priority)
		__tos_next_pid = next;

	tos_trigger_irq(PENDSV_EXCEPTION);
}

void __tos_switch_next_ctxt()
{
	struct TOS_PROCESS_DESCRIPTOR* found = __tos_processes;
	struct TOS_PROCESS_DESCRIPTOR* it;

	__tos_disable_exceptions();
	for (it = found + 1; (it - __tos_processes) < __tos_processes_count; ++it)
	{
		if ((it->runState == READY) && (found->priority < it->priority))
			found = it;
	}

	if (((found - __tos_processes) >= __tos_processes_count)
		|| (found->runState < READY))
	{
		found = __tos_processes; //0 is the system IDLE process
	}

	__tos_triger_to_ctxt_switch(found);
	__tos_enable_exceptions();
}


void __tos_prepare_for_ctx_switch(struct TOS_PROCESS_DESCRIPTOR** current, struct TOS_PROCESS_DESCRIPTOR** next)
{
	__tos_disable_exceptions();

	*current = __tos_current_pid;
	*next = __tos_next_pid;

	__tos_next_pid->runState = RUNNING;
	if (__tos_current_pid->runState == RUNNING)
		__tos_current_pid->runState = READY;

	__tos_current_pid = *next;
	__tos_next_pid = __tos_processes;
}


void __tos_context_processes_tick_updates(const TOS_TICKS tick)
{
	for (uint32_t p = 0; p < __tos_processes_count; ++p)
	{
		if (__tos_processes[p].sleepTickMark <= tick)
		{
			__tos_processes[p].sleepTickMark = TOS_TICKS_MAX;
			__tos_processes[p].runState = READY;

			if (__tos_current_pid->priority < __tos_processes[p].priority)
				__tos_triger_to_ctxt_switch(__tos_processes + p);
		}
	}
}

int tos_yield()
{
	for (uint32_t p = 0; p < __tos_processes_count; ++p)
	{
		if ((__tos_current_pid->priority <= __tos_processes[p].priority)
			&& (__tos_processes[p].runState >= READY)
			&& (__tos_current_pid != __tos_processes + p))
		{
			__tos_triger_to_ctxt_switch(__tos_processes + p);
			return 1;
		}
	}

	return 0;
}

void tos_sleep_ticks(uint_t ticks)
{
	__tos_disable_exceptions();

	__tos_current_pid->sleepTickMark = tos_ticks();
	__tos_current_pid->sleepTickMark += ticks;
	__tos_current_pid->runState = WAIT_FOR_TICK;

	__tos_enable_exceptions();

	__tos_switch_next_ctxt();
}

void tos_sleep(uint_t ms)
{
	tos_sleep_ticks(tos_ms2ticks(ms));
}

