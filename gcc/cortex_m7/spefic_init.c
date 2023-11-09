/*
 * spefic_init.c
 *
 *  Created on: 14 Mar 2020
 *      Author: IulianPopa
 */

#include "tos.h"

void __tos_specific_OS_related_hook()
{
	tos_enable_irq(14, 0xFF); //Make PendSV one of the lowest priorities IRQs
	tos_enable_irq(15, 0x00); //Just keep the systick handler as higher as possible
}


