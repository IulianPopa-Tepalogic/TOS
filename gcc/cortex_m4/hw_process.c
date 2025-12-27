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

#include "_tos_inc/process.h"
#include "_tos_inc/process.h"


void __tos_prepare_proc_to_start(struct TOS_PROCESS_DESCRIPTOR* desc)
{
	desc->ctxt.contextStackPointer = (uint32_t)desc->stackStart;
#ifndef TOS_FPU_ENABLED
	desc->ctxt.contextStackPointer -= 8 * sizeof (uint32_t);

	desc->ctxt.contextStackPointer -= 8 * sizeof (uint32_t);
	((uint32_t*)desc->stackStart)[-1] = (1 << 24);
	((uint32_t*)desc->stackStart)[-2] = (uint32_t)desc->processEntryPoint;
#else
	desc->ctxt.contextStackPointer -= (26 * sizeof (uint32_t));

	desc->ctxt.contextStackPointer -= (24 * sizeof (uint32_t));
	((uint32_t*)desc->stackStart)[-2] = 0;
	((uint32_t*)desc->stackStart)[-19] = (1 << 24);
	((uint32_t*)desc->stackStart)[-20] = (uint32_t)desc->processEntryPoint;
#endif

	desc->runState = READY;
}

void __tos_start_idle()
{
	const uint32_t sp = (uint32_t)__tos_processes[0].stackStart;
	void (*entry)() = __tos_processes[0].processEntryPoint;

	__tos_current_pid = __tos_next_pid = __tos_processes;

	asm ("mrs r0, CONTROL\n"
		 "orr r0, r0, #2\n"
		 "msr CONTROL, r0\n"
		 "isb\n"
		 "mov sp, %0\n"
		 "mov lr, %1\n"
		 "bx lr\n"
		:
		: "r" (sp), "r" (entry)
		:);
}
