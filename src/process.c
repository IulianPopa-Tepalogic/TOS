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

#include <string.h>
#include <memory.h>

#include "_tos_inc/process.h"
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
