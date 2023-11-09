/*
 * hw_process.c
 *
 *  Created on: Jan 14, 2020
 *      Author: ipopa
 */

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
