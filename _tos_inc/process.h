/*
 * process.h
 *
 *  Created on: Jan 2, 2020
 *      Author: ipopa
 */

#ifndef PROCESS_H_
#define PROCESS_H_

#include "tos.h"
#include "tos_procs.h"


#ifndef _EXTERN_
	#ifdef __cplusplus
	#define _EXTERN_	extern "C"
	#else
	#define _EXTERN_ extern
	#endif
#endif


_EXTERN_ struct TOS_PROCESS_DESCRIPTOR* __tos_next_pid;
_EXTERN_ struct TOS_PROCESS_DESCRIPTOR* __tos_current_pid;
_EXTERN_ struct TOS_PROCESS_DESCRIPTOR* const __tos_processes;
_EXTERN_ const uint32_t __tos_processes_count;

static inline int
__tos_pid_from_ctxt(const struct TOS_PROCESS_DESCRIPTOR* const desc)
{
	return (int)(desc - __tos_processes);
}


void __tos_context_processes_tick_updates(const TOS_TICKS tick);
void __tos_switch_next_ctxt();

#endif /* PROCESS_H_ */
