/*
 * tos.h
 *
 *  Created on: Jan 7, 2020
 *      Author: ipopa
 */

#ifndef INCLUDE_TOS_H_
#define INCLUDE_TOS_H_

#include <stdlib.h>
#include <inttypes.h>

#ifdef __cplusplus
	#define _TOS_EXTERN_C_START extern "C" {
	#define _TOS_EXTERN_C_END }
	#define _TOS_EXTERN_DECL_ extern "C"
#else
	#define _TOS_EXTERN_C_START
	#define _TOS_EXTERN_C_END
	#define _TOS_EXTERN_DECL_ extern
#endif

#ifndef uint_t
#define uint_t unsigned int
#endif

#ifndef TOS_TICKS
#define TOS_TICKS uint64_t
#endif

#define TOS_TICKS_MAX (~(TOS_TICKS)0)

#ifndef TOS_CLOCK_HZ
#define TOS_CLOCK_HZ uint64_t
#endif

struct __TOS_WAIT_NODE;
struct TOS_MUTEX {
	struct __TOS_WAIT_NODE *wait_list;
	struct TOS_PROCESS_DESCRIPTOR* owner;
	uint32_t count;
};

struct TOS_SEMAPHORE {
	struct __TOS_WAIT_NODE *wait_list;
	uint32_t count;
};

struct TOS_IPC_SIGNAL {
	int32_t id;
	uint16_t owner;
	uint16_t sender;
	struct TOS_IPC_SIGNAL* next;
	union {
		uint_t uparam;
		int sparam;
		double dparam;
		void* pparam;
	};
};

#ifdef __cplusplus
extern "C" {
#endif

void tos_critical_fault();
void tos_reset();

TOS_TICKS tos_ticks();
TOS_CLOCK_HZ tos_hz_per_tick();
void tos_set_hz_per_tick(const TOS_CLOCK_HZ hz);

static inline TOS_TICKS tos_ms2ticks(uint_t ms)
{
	return ms * tos_hz_per_tick() / 1000;
}

static inline uint_t tos_ticks2ms(TOS_TICKS ticks)
{
	return ticks * 1000 / tos_hz_per_tick();
}

void tos_enable_irq(uint_t line, uint_t priority);
void tos_disable_irq(uint_t line);
void tos_trigger_irq(uint_t line);
void tos_ack_irq(uint_t line);

int tos_yield();
void tos_sleep_ticks(uint_t ticks);
void tos_sleep(uint_t ms);

struct TOS_MUTEX;
void tos_lock_mutex(struct TOS_MUTEX* mutex);
void tos_unlock_mutex(struct TOS_MUTEX* mutex);
int tos_try_lock_mutex(struct TOS_MUTEX* mutex, TOS_TICKS ticks_tmo);

struct TOS_SEMAPHORE;
void tos_push_sem(struct TOS_SEMAPHORE* sem);
void tos_pop_sem(struct TOS_SEMAPHORE* sem);
int tos_try_pop_sem(struct TOS_SEMAPHORE* sem, TOS_TICKS ticks_tmo);


uint_t tos_cpid();
const char* tos_cpname();
uint_t tos_pid(const char* name);
const char* tos_pname(uint_t pid);


struct TOS_IPC_SIGNAL* tos_get_signal(int32_t id, int32_t fromPid);
void tos_release_signal(struct TOS_IPC_SIGNAL* signal);

void tos_send_signal(struct TOS_IPC_SIGNAL* signal, uint32_t toPid);

struct TOS_IPC_SIGNAL* tos_wait_signal();
struct TOS_IPC_SIGNAL* tos_wait_signal_tmo(TOS_TICKS ticks_tmo);

struct TOS_IPC_SIGNAL* tos_wait_for_signal(int32_t id);
struct TOS_IPC_SIGNAL* tos_wait_for_signal_tmo(int32_t id, TOS_TICKS ticks_tmo);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_TOS_H_ */
