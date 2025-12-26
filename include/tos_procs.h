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
	void* processEntryPoint;
	void* stackStart;
	void* stackEnd;
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
#define TOS_PROC_DECL_PROC_FUNC_EP(p)  __tos_process_ ## p ## _ep
#define TOS_PROC_DECL_PROC_STACK(p)  __tos_process_stack_ ## p
#define TOS_PROC_DECL_PROC_STACK_S(p)  __tos_process_stack_ ## p ## _start
#define TOS_PROC_DECL_PROC_STACK_E(p)  __tos_process_stack_ ## p ## _end
#define TOS_PROC_DECL_PROC_SIGNALS_A(p) __tos_process_signals_ ## p ## _a
#define TOS_PROC_DECL_PROC_SIGNALS(p) __tos_process_signals_ ## p

#define TOS_PROC_DECL(p)\
	_TOS_EXTERN_DECL_ void (*TOS_PROC_DECL_PROC_FUNC_EP(p))(void); \
	_TOS_EXTERN_DECL_ uint64_t TOS_PROC_DECL_PROC_STACK(p)[];\
	_TOS_EXTERN_DECL_ void* TOS_PROC_DECL_PROC_STACK_S(p);\
	_TOS_EXTERN_DECL_ void* TOS_PROC_DECL_PROC_STACK_E(p);\
	_TOS_EXTERN_DECL_ struct TOS_IPC_SIGNAL TOS_PROC_DECL_PROC_SIGNALS_A(p)[];\
	_TOS_EXTERN_DECL_ struct TOS_IPC_SIGNAL* TOS_PROC_DECL_PROC_SIGNALS(p);

TOS_PROC_DECL(__tos_idle)

#define TOS_PROC_CTX_DEF_2(n,p) {{0,},\
								#n,\
								&TOS_PROC_DECL_PROC_FUNC_EP(n),\
								&TOS_PROC_DECL_PROC_STACK_S(n),\
								&TOS_PROC_DECL_PROC_STACK_E(n),\
								&(TOS_PROC_DECL_PROC_SIGNALS_A(n)[0]),\
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
		_TOS_EXTERN_C_START \
		const uint32_t __tos_processes_count = (sizeof(__tos_processes_arr_) / sizeof (__tos_processes_arr_[0])); \
		struct TOS_PROCESS_DESCRIPTOR* const __tos_processes = __tos_processes_arr_; \
		_TOS_EXTERN_C_END


#define TOS_PROCESS_EXT_2(name,stack,signals) \
		static void TOS_PROC_DECL_PROC_FUNC(name)(void); \
		_TOS_EXTERN_C_START \
		void (*TOS_PROC_DECL_PROC_FUNC_EP(name))(void) = TOS_PROC_DECL_PROC_FUNC(name); \
		struct TOS_IPC_SIGNAL TOS_PROC_DECL_PROC_SIGNALS_A(name)[signals] = {{signals,}}; \
		struct TOS_IPC_SIGNAL* TOS_PROC_DECL_PROC_SIGNALS(name) = TOS_PROC_DECL_PROC_SIGNALS_A(name); \
		uint64_t TOS_PROC_DECL_PROC_STACK(name)[(STACK_ADD_CTX_SAVE_SIZE(stack) + 7) / 8]; \
		void* TOS_PROC_DECL_PROC_STACK_E(name) = TOS_PROC_DECL_PROC_STACK(name); \
		void* TOS_PROC_DECL_PROC_STACK_S(name) = TOS_PROC_DECL_PROC_STACK(name) + ((STACK_ADD_CTX_SAVE_SIZE(stack) + 7) / 8) - 1; \
		_TOS_EXTERN_C_END \
		static void TOS_PROC_DECL_PROC_FUNC(name)(void)

#define TOS_PROCESS_EXT(name,stack,signals) TOS_PROCESS_EXT_2(name,stack,signals)
#define TOS_PROCESS(name,stack) TOS_PROCESS_EXT(name,stack,1)

#define TOS_IRQ_HANDLER(x) _TOS_EXTERN_ void x ()

#endif /* INCLUDE_TOS_PROCS_H_ */

