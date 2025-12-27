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
