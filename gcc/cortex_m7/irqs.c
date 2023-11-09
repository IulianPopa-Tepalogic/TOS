/*
 * irqs.c
 *
 *  Created on: Jan 7, 2020
 *      Author: ipopa
 */

#include "tos.h"

static volatile uint32_t* const NVIC_ISER = (uint32_t*)0xE000E100;
static volatile uint32_t* const NVIC_ICER = (uint32_t*)0xE000E180;
static volatile uint32_t* const NVIC_ISPR = (uint32_t*)0xE000E200;
static volatile uint32_t* const NVIC_ICPR = (uint32_t*)0xE000E280;
static volatile uint32_t* const NVIC_IPR  = (uint32_t*)0xE000E400;

static volatile uint32_t* const SHPR3 = (uint32_t*)0xE000ED20;
static volatile uint32_t* const ICSR  = (uint32_t*)0xE000ED04;

#define PENDSV_EXCEP 	14
#define SYSTICK_EXCEP 	15

void
tos_enable_irq(uint_t line, const uint_t priority)
{
	if (line < 14) return ;
	switch (line)
	{
	case PENDSV_EXCEP:
		*SHPR3 |= ((uint32_t)priority << 20);
		return;

	case SYSTICK_EXCEP:
		*SHPR3 |= ((uint32_t)priority << 28);
		return;
	}

	line -= 16;
	const uint_t reg_idx = line / 4;
	const uint_t byte_idx = line % 4;

	NVIC_IPR[reg_idx] |= ((((priority & 0x0F) << (byte_idx * 8))) << 4);

	const uint_t idx = line / 32;
	const uint_t bit = line % 32;

	NVIC_ISER[idx] |= ((uint32_t)1 << bit);
}

void
tos_disable_irq(uint_t line)
{
	if (line < 16) return;

	line -= 16;
	const uint_t idx = line / 32;
	const uint_t bit = line % 32;

	NVIC_ICER[idx] |= (uint_t)1 << bit;
}

void tos_trigger_irq(uint_t line)
{
	if (line == PENDSV_EXCEP)
	{
		ICSR[0] |= ((uint32_t)1 << 28);
		return ;
	}
	else if (line < 16)
		return ;

	line -= 16;
	const uint_t idx = line / 32;
	const uint_t bit = line % 32;

	NVIC_ISPR[idx] |= (uint_t)1 << bit;
}

void tos_ack_irq(uint_t line)
{
	if (line < 16) return ;

	line -= 16;
	const uint_t idx = line / 32;
	const uint_t bit = line % 32;

	NVIC_ICPR[idx] |= (uint_t)1 << bit;
}
