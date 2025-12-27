/*
MIT License

Copyright (c) 2019 Tepalogic SRL

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "tos.h"
#include "_tos_inc/process.h"
#include "_tos_inc/exception.h"
#include "_tos_inc/tick.h"

#include <stdlib.h>


struct TOS_IPC_SIGNAL* tos_get_signal(int32_t id, int32_t fromPid)
{
	if (fromPid < 0)
		fromPid = __tos_current_pid - __tos_processes;

	if ((uint32_t)fromPid >= __tos_processes_count)
		tos_critical_fault();

	struct TOS_PROCESS_DESCRIPTOR* const process = __tos_processes + fromPid;

	__tos_disable_exceptions();
	struct TOS_IPC_SIGNAL* const result = process->nextFreeSignal;
	process->nextFreeSignal = result->next;
	__tos_enable_exceptions();

	if (result != NULL)
	{
		result->owner = fromPid;
		result->id = id;
	}

	return result;
}

void tos_release_signal(struct TOS_IPC_SIGNAL* signal)
{
	struct TOS_PROCESS_DESCRIPTOR* const signal_owner = __tos_processes + signal->owner;

	__tos_disable_exceptions();

	signal->next = signal_owner->nextFreeSignal;
	signal_owner->nextFreeSignal = signal;

	__tos_enable_exceptions();
}

void tos_send_signal(struct TOS_IPC_SIGNAL* signal, uint32_t toPid)
{
	if ((toPid == 0) || (toPid >= __tos_processes_count))
		tos_critical_fault();

	signal->next = NULL;
	signal->sender = __tos_current_pid - __tos_processes;

	struct TOS_PROCESS_DESCRIPTOR* const dest_process = __tos_processes + toPid;

	signal->next = NULL;
	__tos_disable_exceptions();
	if (dest_process->nextRecvSignal != NULL)
	{
		dest_process->lastRecvSignal->next = signal;
		dest_process->lastRecvSignal = signal;
	}
	else
		dest_process->nextRecvSignal = dest_process->lastRecvSignal = signal;

	if (dest_process->runState & (WAIT_MSG | WAIT_FOR_TICK_MSG))
	{
		dest_process->runState = READY;
		if (__tos_current_pid->priority < dest_process->priority)
			__tos_switch_next_ctxt();
	}

	__tos_enable_exceptions();
}

struct TOS_IPC_SIGNAL* tos_wait_signal()
{

tos_recv_any_signal_again:

	__tos_disable_exceptions();

	struct TOS_IPC_SIGNAL* result = __tos_current_pid->nextRecvSignal;
	if (result == NULL)
	{
		__tos_current_pid->runState = WAIT_MSG;
		__tos_switch_next_ctxt();

		goto tos_recv_any_signal_again;
	}

	__tos_current_pid->nextRecvSignal = result->next;
	if (result == __tos_current_pid->lastRecvSignal)
		__tos_current_pid->lastRecvSignal = NULL;

	__tos_enable_exceptions();

	return result;
}

struct TOS_IPC_SIGNAL* tos_wait_signal_tmo(TOS_TICKS ticks_tmo)
{

tos_recv_any_signal_tmo_again:

	__tos_disable_exceptions();

	struct TOS_IPC_SIGNAL* result = __tos_current_pid->nextRecvSignal;
	if (result == NULL)
	{
		if (ticks_tmo == 0)
		{
			__tos_enable_exceptions();
			return NULL;
		}

		__tos_current_pid->runState = WAIT_FOR_TICK_MSG;
		__tos_current_pid->sleepTickMark = __tos_wall_tick + ticks_tmo;

		__tos_switch_next_ctxt();
		ticks_tmo = 0;

		goto tos_recv_any_signal_tmo_again;
	}

	__tos_current_pid->nextRecvSignal = result->next;
	if (result == __tos_current_pid->lastRecvSignal)
		__tos_current_pid->lastRecvSignal = NULL;

	__tos_enable_exceptions();

	return result;
}

struct TOS_IPC_SIGNAL* tos_wait_for_signal(int32_t id)
{
tos_recv_sel_signal:
	__tos_disable_exceptions();

	struct TOS_IPC_SIGNAL* result = NULL;
	if (__tos_current_pid->nextRecvSignal != NULL)
	{
		result = __tos_current_pid->nextRecvSignal;
		struct TOS_IPC_SIGNAL* prev = NULL;

		do {
			if (result->id == id)
				break;
			prev = result;
			result = result->next;
		} while (result != NULL);

		if (result != NULL)
		{
			if (prev == NULL)
				__tos_current_pid->nextRecvSignal = result->next;
			else
			{
				prev->next = result->next;
				if (result == __tos_current_pid->lastRecvSignal)
					__tos_current_pid->lastRecvSignal = prev;
			}
		}
	}

	if (result == NULL)
	{
		__tos_current_pid->runState = WAIT_MSG;
		__tos_switch_next_ctxt();

		goto tos_recv_sel_signal;
	}

	__tos_enable_exceptions();

	return result;
}

struct TOS_IPC_SIGNAL* tos_wait_for_signal_tmo(int32_t id, TOS_TICKS ticks_tmo)
{
tos_recv_sel_signal_tmo:
		__tos_disable_exceptions();

		struct TOS_IPC_SIGNAL* result = NULL;
		if (__tos_current_pid->nextRecvSignal != NULL)
		{
			result = __tos_current_pid->nextRecvSignal;
			struct TOS_IPC_SIGNAL* prev = NULL;

			do {
				if (result->id == id)
					break;
				prev = result;
				result = result->next;
			} while (result != NULL);

			if (result != NULL)
			{
				if (prev == NULL)
					__tos_current_pid->nextRecvSignal = result->next;
				else
				{
					prev->next = result->next;
					if (result == __tos_current_pid->lastRecvSignal)
						__tos_current_pid->lastRecvSignal = prev;
				}
			}
		}

		if ((result == NULL) && (ticks_tmo > 0))
		{
			__tos_current_pid->runState = WAIT_FOR_TICK_MSG;
			__tos_current_pid->sleepTickMark = __tos_wall_tick + ticks_tmo;

			ticks_tmo = 0;
			goto tos_recv_sel_signal_tmo;
		}

		__tos_enable_exceptions();

		return result;
}
