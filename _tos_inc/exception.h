/*
 * irq.h
 *
 *  Created on: Jan 3, 2020
 *      Author: ipopa
 */

#ifndef TOS_INC_EXCEPTION_H_
#define TOS_INC_EXCEPTION_H_

#include "__hw_exception.h"

static inline void __tos_disable_exceptions()
{
	__tos_hw_disable_exceptions();
}

static inline void __tos_enable_exceptions()
{
	__tos_hw_enable_exceptions();
}

static inline void __tos_wait_for_event()
{
	__tos_hw_wait_for_event();
}

#endif /* TOS_INC_EXCEPTION_H_ */
