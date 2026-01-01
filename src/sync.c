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

#include <stdlib.h>

#include "tos.h"
#include "_tos_inc/sync.h"
#include "_tos_inc/exception.h"
#include "_tos_inc/tick.h"

void tos_init_mutex(struct TOS_MUTEX* mutex)
{
	memset(mutex, 0, sizeof(struct TOS_MUTEX));
}

void tos_lock_mutex(struct TOS_MUTEX* mutex)
{
	struct __TOS_WAIT_NODE node;

	__tos_disable_exceptions();

__lock_mutex_again:
	if (mutex->owner == __tos_current_pid)
		mutex->count++;

	else if (mutex->owner != NULL)
	{
		if (mutex->owner->priority < __tos_current_pid->priority)
			mutex->owner->priority = __tos_current_pid->priority;

		struct __TOS_WAIT_NODE* iterator = mutex->wait_list;
		struct __TOS_WAIT_NODE* prev = NULL;
		while ((iterator != NULL)
				&& (__tos_current_pid->priority < iterator->process->priority))
		{
			prev = iterator;
			iterator = iterator->next;
		}

		node.process = __tos_current_pid;
		node.next = iterator;
		if (iterator != NULL)
		{
			node.prev = iterator->prev;
			iterator->prev = &node;
		}
		else
			node.prev = prev;

		if (node.prev == NULL)
			mutex->wait_list = &node;
		else
			node.prev->next = &node;

		__tos_current_pid->runState = WAIT;
		__tos_switch_next_ctxt();

		__tos_disable_exceptions();
		if (node.next != NULL)
			node.next->prev = node.prev;

		if (node.prev != NULL)
			node.prev->next = node.next;
		else
			mutex->wait_list = node.next;

		goto __lock_mutex_again;
	}
	else
	{
		mutex->owner = __tos_current_pid;
		mutex->count = 1;
	}

	__tos_enable_exceptions();
}

int tos_try_lock_mutex(struct TOS_MUTEX* mutex, TOS_TICKS ticks_tmo)
{
	struct __TOS_WAIT_NODE node;

	__tos_disable_exceptions();

__try_lock_mutex_again:
	if (mutex->owner == __tos_current_pid)
		mutex->count++;

	else if (mutex->owner != NULL)
	{
		if (ticks_tmo == 0)
		{
			__tos_enable_exceptions();
			return 0;
		}

		if (mutex->owner->priority < __tos_current_pid->priority)
			mutex->owner->priority = __tos_current_pid->priority;

		struct __TOS_WAIT_NODE* iterator = mutex->wait_list;
		struct __TOS_WAIT_NODE* prev = NULL;
		while ((iterator != NULL)
				&& (__tos_current_pid->priority < iterator->process->priority))
		{
			prev = iterator;
			iterator = iterator->next;
		}

		node.process = __tos_current_pid;
		node.next = iterator;
		if (iterator != NULL)
		{
			node.prev = iterator->prev;
			iterator->prev = &node;
		}
		else
			node.prev = prev;

		if (node.prev == NULL)
			mutex->wait_list = &node;
		else
			node.prev->next = &node;

		__tos_current_pid->runState = WAIT_FOR_TICK;
		__tos_current_pid->sleepTickMark = __tos_wall_tick + ticks_tmo;
		__tos_switch_next_ctxt();

		ticks_tmo = 0;

		__tos_disable_exceptions();
		if (node.next != NULL)
			node.next->prev = node.prev;

		if (node.prev != NULL)
			node.prev->next = node.next;
		else
			mutex->wait_list = node.next;

		goto __try_lock_mutex_again;
	}
	else
	{
		mutex->owner = __tos_current_pid;
		mutex->count = 1;
	}

	__tos_enable_exceptions();

	return mutex->count;
}

void tos_unlock_mutex(struct TOS_MUTEX* mutex)
{
	__tos_disable_exceptions();

	if ((mutex->owner != __tos_current_pid) || (mutex->count <= 0))
		tos_critical_fault();

	mutex->count -= 1;
	if (mutex->count == 0)
	{
		__tos_current_pid->priority = __tos_current_pid->configuredPriority;
		if (mutex->wait_list != NULL)
		{
			 struct TOS_PROCESS_DESCRIPTOR* const proc = mutex->wait_list->process;
			 proc->runState = READY;

			 mutex->owner = proc;
			 if (__tos_current_pid->priority < proc->priority)
				 __tos_switch_next_ctxt();
		}
		else
			mutex->owner = NULL;
	}
	__tos_enable_exceptions();
}


void tos_push_sem(struct TOS_SEMAPHORE* sem)
{
	__tos_disable_exceptions();

	sem->count += 1;
	if ((sem->count > 0) && (sem->wait_list != NULL))
	{
		 struct __TOS_WAIT_NODE* next = sem->wait_list->next;
		 struct TOS_PROCESS_DESCRIPTOR* proc = sem->wait_list->process;

		 sem->wait_list = next;
		 proc->runState = READY;
	}
	__tos_switch_next_ctxt();
}

void tos_pop_sem(struct TOS_SEMAPHORE* sem)
{
	struct __TOS_WAIT_NODE node;

	__tos_disable_exceptions();

__pop_sem_again:
	if (sem->count <= 0)
	{
		struct __TOS_WAIT_NODE* iterator = sem->wait_list;
		struct __TOS_WAIT_NODE* prev = NULL;
		while ((iterator != NULL)
				&& (__tos_current_pid->priority < iterator->process->priority))
		{
			prev = iterator;
			iterator = iterator->next;
		}

		node.process = __tos_current_pid;
		node.next = iterator;
		if (iterator != NULL)
		{
			node.prev = iterator->prev;
			iterator->prev = &node;
		}
		else
			node.prev = prev;

		if (node.prev == NULL)
			sem->wait_list = &node;
		else
			node.prev->next = &node;

		__tos_current_pid->runState = WAIT;

		__tos_switch_next_ctxt();

		__tos_disable_exceptions();
		if (node.next != NULL)
			node.next->prev = node.prev;

		if (node.prev != NULL)
			node.prev->next = node.next;
		else
			sem->wait_list = node.next;

		goto __pop_sem_again;
	}
	else
		sem->count -= 1;

	__tos_enable_exceptions();
}

int tos_try_pop_sem(struct TOS_SEMAPHORE* sem, TOS_TICKS ticks_tmo)
{
	struct __TOS_WAIT_NODE node;

	__tos_disable_exceptions();

__try_pop_sem_again:
	if (sem->count <= 0)
	{
		if (ticks_tmo == 0)
		{
			__tos_enable_exceptions();
			return 0;
		}

		struct __TOS_WAIT_NODE* iterator = sem->wait_list;
		struct __TOS_WAIT_NODE* prev = NULL;
		while ((iterator != NULL)
				&& (__tos_current_pid->priority < iterator->process->priority))
		{
			prev = iterator;
			iterator = iterator->next;
		}

		node.process = __tos_current_pid;
		node.next = iterator;
		if (iterator != NULL)
		{
			node.prev = iterator->prev;
			iterator->prev = &node;
		}
		else
			node.prev = prev;

		if (node.prev == NULL)
			sem->wait_list = &node;
		else
			node.prev->next = &node;

		__tos_current_pid->runState = WAIT_FOR_TICK;
		__tos_current_pid->sleepTickMark = __tos_wall_tick + ticks_tmo;

		__tos_switch_next_ctxt();

		ticks_tmo = 0;
		__tos_disable_exceptions();
		if (node.next != NULL)
			node.next->prev = node.prev;

		if (node.prev != NULL)
			node.prev->next = node.next;
		else
			sem->wait_list = node.next;

		goto __try_pop_sem_again;
	}
	else
		sem->count -= 1;

	__tos_enable_exceptions();
	return 1;
}
