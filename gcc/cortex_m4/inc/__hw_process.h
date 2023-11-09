/*
 * TOS_process.h
 *
 *  Created on: Dec 31, 2019
 *      Author: ipopa
 */

#ifndef __HW_PROCESS_H_
#define __HW_PROCESS_H_

#include <stdint.h>

#ifndef TOS_FPU_ENABLED
#define STACK_ADD_CTX_SAVE_SIZE(s)	((s) + ((16 + 1 + 1) * 4))
#else
#define STACK_ADD_CTX_SAVE_SIZE(s)	((s) + ((16 + 32 + 1 + 1 + 1) * 4))
#endif

struct TOS_PROCESS_DESCRIPTOR;
struct TOS_PROCESS_CONTEXT {
	uint32_t contextStackPointer;
};

void __tos_prepare_proc_to_start(struct TOS_PROCESS_DESCRIPTOR* desc);
void __tos_start_idle();

extern void tos_trigger_irq(unsigned int line);
static inline void __tos_hw_trigger_ctx_switch()
{
	tos_trigger_irq(14);
}


#endif /* TOS_PROCESS_H_ */
