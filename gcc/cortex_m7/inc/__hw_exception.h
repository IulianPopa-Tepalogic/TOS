/*
 * __hw_exceptions.h
 *
 *  Created on: Jan 3, 2020
 *      Author: ipopa
 */

#ifndef __HW_EXCEPTION_H_
#define __HW_EXCEPTION_H_

static inline void __tos_hw_disable_exceptions() { asm ("cpsid if"); }
static inline void __tos_hw_enable_exceptions() { asm ("cpsie if"); }
static inline void __tos_hw_wait_for_event() { asm ("wfi"); }

#endif /* __HW_EXCEPTION_H_ */
