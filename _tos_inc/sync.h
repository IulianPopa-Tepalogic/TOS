/*
 * sync.h
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#ifndef TOS_INC_SYNC_H_
#define TOS_INC_SYNC_H_

#include "process.h"

struct __TOS_WAIT_NODE {
	struct TOS_PROCESS_DESCRIPTOR* process;
	struct __TOS_WAIT_NODE* prev;
	struct __TOS_WAIT_NODE* next;
};



#endif /* TOS_INC_SYNC_H_ */
