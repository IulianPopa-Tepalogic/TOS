/*
 * tos_procs.h
 *
 *  Created on: Jan 13, 2020
 *      Author: ipopa
 */

#ifndef INCLUDE_TOS_PROCS_H_
#define INCLUDE_TOS_PROCS_H_

#include "tos.h"
#include "__hw_process.h"

#ifndef _EXTERN_
	#ifdef __cplusplus
		#define _EXTERN_  extern "C"
	#else
		#define _EXTERN_  extern
	#endif
#endif /* _EXTERN_ */


enum PROCESS_RUN_STATE {
	INTERNAL_ERR = 0,
	ERROR = 1,
	ENDED  = 2,
	NOT_STARTED = 4,
	WAIT_MSG = 8,
	WAIT = 16,
	WAIT_FOR_TICK_MSG = 32,
	WAIT_FOR_TICK = 64,
	READY = 128,
	RUNNING = 256
};

struct TOS_PROCESS_DESCRIPTOR {
	struct TOS_PROCESS_CONTEXT ctxt;
	const char *const processName;
	void (*const processEntryPoint)();
	void* const stackStart;
	void* const stackEnd;
	struct TOS_IPC_SIGNAL* nextFreeSignal;
	struct TOS_IPC_SIGNAL* nextRecvSignal;
	struct TOS_IPC_SIGNAL* lastRecvSignal;
	enum PROCESS_RUN_STATE runState;
	const uint8_t configuredPriority;
	uint8_t priority;
	TOS_TICKS sleepTickMark;
};

#include "../_tos_inc/process.h"


#define TOS_PROC_DECL_PROC_FUNC(p)  __tos_process_ ## p
#define TOS_PROC_DECL_PROC_STACK(p)  __tos_process_stack_ ## p
#define TOS_PROC_DECL_PROC_STACK_S(p)  __tos_process_stack_ ## p ## _start
#define TOS_PROC_DECL_PROC_STACK_E(p)  __tos_process_stack_ ## p ## _end
#define TOS_PROC_DECL_PROC_SIGNALS(p) __tos_process_signals_ ## p

#ifdef __cplusplus
#define TOS_PROC_DECL(p)\
	_EXTERN_ void TOS_PROC_DECL_PROC_FUNC(p)(void); \
	_EXTERN_ void* const TOS_PROC_DECL_PROC_STACK_S(p);\
	_EXTERN_ void* const TOS_PROC_DECL_PROC_STACK_E(p);\
	_EXTERN_ struct TOS_IPC_SIGNAL* const TOS_PROC_DECL_PROC_SIGNALS(p);
#else
#define TOS_PROC_DECL(p)\
	extern void TOS_PROC_DECL_PROC_FUNC(p)(void); \
	extern void* const TOS_PROC_DECL_PROC_STACK_S(p);\
	extern void* const TOS_PROC_DECL_PROC_STACK_E(p);\
	extern struct TOS_IPC_SIGNAL* const TOS_PROC_DECL_PROC_SIGNALS(p);
#endif

TOS_PROC_DECL(__tos_idle)

#define TOS_PROC_CTX_DEF_2(n,p) {{0,},\
								#n,\
								TOS_PROC_DECL_PROC_FUNC(n),\
								TOS_PROC_DECL_PROC_STACK_S(n),\
								TOS_PROC_DECL_PROC_STACK_E(n),\
								TOS_PROC_DECL_PROC_SIGNALS(n),\
								NULL,\
								NULL,\
								NOT_STARTED,\
								p,\
								p,\
								TOS_TICKS_MAX},
#define TOS_PROC_CTX_DEF(n,p) TOS_PROC_CTX_DEF_2(n,p)

#define TOS_PROC_CTX_START_DEF \
	static struct TOS_PROCESS_DESCRIPTOR __tos_processes_arr_[] = {\
	TOS_PROC_CTX_DEF(__tos_idle,0)

#define TOS_PROC_CTX_END_DEF }; \
		_EXTERN_ const uint32_t __tos_processes_count = (sizeof(__tos_processes_arr_) / sizeof (__tos_processes_arr_[0]));\
		_EXTERN_ struct TOS_PROCESS_DESCRIPTOR* const __tos_processes = __tos_processes_arr_;

#define TOS_PROCESS(name,stack) TOS_PROCESS_EXT(name,stack, 1)
#define TOS_PROCESS_EXT(name,stack,signals) \
		static struct TOS_IPC_SIGNAL __tos_process_signals_ ## name ## _a[signals] = {{signals,}}; \
		_EXTERN_ struct TOS_IPC_SIGNAL* const __tos_process_signals_ ## name = __tos_process_signals_ ## name ## _a; \
		static uint64_t TOS_PROC_DECL_PROC_STACK(name)[(STACK_ADD_CTX_SAVE_SIZE(stack) + 7) / 8]; \
		_EXTERN_ void* const TOS_PROC_DECL_PROC_STACK_E(name) = TOS_PROC_DECL_PROC_STACK(name); \
		_EXTERN_ void* const TOS_PROC_DECL_PROC_STACK_S(name) = TOS_PROC_DECL_PROC_STACK(name) + ((STACK_ADD_CTX_SAVE_SIZE(stack) + 7) / 8) - 1; \
		_EXTERN_ void TOS_PROC_DECL_PROC_FUNC(name)()


#define TOS_IRQ_HANDLER(x) _EXTERN_ void x ()

#endif /* INCLUDE_TOS_PROCS_H_ */
