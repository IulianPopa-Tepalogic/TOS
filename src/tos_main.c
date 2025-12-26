/*
 * main_tos.c
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#include "tos_startup.h"
#include "tos.h"
#include "_tos_inc/process.h"
#include "_tos_inc/exception.h"

static void prepare_procs_to_start()
{
	for (uint_t p = 0; p < __tos_processes_count; ++p)
	{
		/* When the process is configure some, values are not ready at compile time
		 * Following is a fix to handle those situations and yet neet mechanism to
		 * confgure the processes statically. */
		void *patch = *((void**)__tos_processes[p].stackStart);
		__tos_processes[p].stackStart = patch;

		patch = *((void**)__tos_processes[p].stackEnd);
		__tos_processes[p].stackEnd = patch;

		patch = *((void**)__tos_processes[p].processEntryPoint);
		__tos_processes[p].processEntryPoint = patch;

		struct TOS_IPC_SIGNAL* signals = __tos_processes[p].nextFreeSignal;
		const int signals_count = signals->id; //Use this field for initial step to avoid extra definitions
		for (int i = 0; i < signals_count; ++i)
			signals[i].next = (i + 1) < signals_count ? &signals[i + 1] : NULL;

		__tos_prepare_proc_to_start(__tos_processes + p);
	}
}


void __tos_start_stage1()
{
	__tos_disable_exceptions();
		__tos_clocks_init_stage_hook();
		__tos_bsp_init_stage_1_hook();
}


void tos_start()
{
	 prepare_procs_to_start();

	__tos_irqs_setup_stage_hook();
	__tos_enable_exceptions();
	__tos_bsp_init_stage_2_hook();
	__tos_disable_exceptions();

	__tos_final_init_stage_hook();
	__tos_specific_OS_related_hook();

	__tos_start_idle();
}



