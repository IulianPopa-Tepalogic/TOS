/*
 * idle.c
 *
 *  Created on: Jan 14, 2020
 *      Author: ipopa
 */

#include "tos_procs.h"
#include "_tos_inc/exception.h"

TOS_PROCESS(__tos_idle,128)
{
	__tos_switch_next_ctxt();
	__tos_enable_exceptions();

	while (1)
	{
		if (! tos_yield())
		{
			__tos_wait_for_event();
		}
	}
}


