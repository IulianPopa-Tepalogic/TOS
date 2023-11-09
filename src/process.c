/*
 * process.h
 *
 *  Created on: Jan 2, 2020
 *      Author: ipopa
 */

#include <string.h>
#include <memory.h>

#include "_tos_inc/process.h"

static inline struct TOS_PROCESS_DESCRIPTOR*
__tos_find_proc(const char* processName)
{
	if (processName == NULL)
		return __tos_processes;

	const size_t len = strlen(processName) + 1;
	for (int p = 0; p < __tos_processes_count; ++p)
	{
		if (memcmp(processName, __tos_processes[p].processName, len) == 0)
			return &__tos_processes[p];
	}

	return NULL;
}

uint_t tos_cpid()
{
	return __tos_current_pid - __tos_processes;
}

const char* tos_cpname()
{
	return __tos_current_pid->processName;
}

uint_t tos_pid(const char* name)
{
	struct TOS_PROCESS_DESCRIPTOR* p = __tos_find_proc(name);
	if (p == NULL)
		tos_critical_fault();

	return p - __tos_processes;
}

const char* tos_pname(uint_t pid)
{
	if (pid >= __tos_processes_count)
		tos_critical_fault();

	return __tos_processes[pid].processName;
}

void tos_critical_fault()
{
	tos_reset();
}

void tos_reset()
{
	//A generic way to reset! :)
	int* const a = (int*)((int)1 - sizeof (int));
	while (1)
		*a = 0xFF;
}
