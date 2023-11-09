/*
 * tos_startup.h
 *
 *  Created on: Jan 11, 2020
 *      Author: ipopa
 */

#ifndef INCLUDE_TOS_STARTUP_H_
#define INCLUDE_TOS_STARTUP_H_

#include "tos.h"

#ifdef __cplusplus
extern "C" {
#endif

void __tos_clocks_init_stage_hook();
void __tos_clocks_tick_callback();
void __tos_bsp_init_stage_1_hook();
void __tos_irqs_setup_stage_hook();
void __tos_bsp_init_stage_2_hook();
void __tos_final_init_stage_hook();
void __tos_specific_OS_related_hook();

void tos_start();

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_TOS_STARTUP_H_ */
