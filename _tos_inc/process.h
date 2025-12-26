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

_TOS_EXTERN_DECL_ struct TOS_PROCESS_DESCRIPTOR* __tos_next_pids;
_TOS_EXTERN_DECL_ struct TOS_PROCESS_DESCRIPTOR *__tos_next_pid;
_TOS_EXTERN_DECL_ struct TOS_PROCESS_DESCRIPTOR* __tos_current_pid;
_TOS_EXTERN_DECL_ struct TOS_PROCESS_DESCRIPTOR* const __tos_processes;
_TOS_EXTERN_DECL_ const uint32_t __tos_processes_count;

static inline int
__tos_pid_from_ctxt(const struct TOS_PROCESS_DESCRIPTOR* const desc)
{
	return (int)(desc - __tos_processes);
}


void __tos_context_processes_tick_updates(const TOS_TICKS tick);
void __tos_switch_next_ctxt();

#endif /* PROCESS_H_ */
