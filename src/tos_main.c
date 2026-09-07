/*
MIT License

Copyright (c) 2019 Tepalogic SRL

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "tos_startup.h"
#include "tos.h"
#include "_tos_inc/process.h"
#include "_tos_inc/exception.h"

extern void stm_main(void);

static void prepare_procs_to_start()
{
	for (uint_t p = 0; p < __tos_processes_count; ++p)
	{
		/* When the process is configured,  some values are not ready at compile time
		 * Following is a fix to handle those situations and yet keep the mechanism to
		 * configure the processes statically. */
		void *patch = *((void**)__tos_processes[p].stackStart);
		__tos_processes[p].stackStart = patch;

		patch = *((void**)__tos_processes[p].stackEnd);
		__tos_processes[p].stackEnd = patch;

		patch = *((void**)__tos_processes[p].processEntryPoint);
		__tos_processes[p].processEntryPoint = patch;

		struct TOS_IPC_SIGNAL* signals = __tos_processes[p].nextFreeSignal;
		const int signals_count = signals->id; //Use this field for initial step to avoid extra definitions
		for (int i = 0; i < signals_count; ++i)
			signals[i].next = (i + 1) < signals_count ? &signals[i + 1] : NULL;

		__tos_prepare_proc_to_start(__tos_processes + p);
	}
}


void __tos_start_stage1()
{
	__tos_disable_exceptions();

	__tos_clocks_init_stage_hook();

	__tos_bsp_init_stage_1_hook();

	__tos_bsp_init_stage_2_hook();
}

extern uint32_t _sitos_data;
extern uint32_t _stos_data;
extern uint32_t _etos_data;
extern uint32_t _stos_bss;
extern uint32_t _etos_bss;

void __tos_start_stage2()
{
	/* The linker script must define the section boundaries used below. For
	 * GNU ld, adapt the memory regions in this example to the target:
	 *
	 *   .tos_data : {
	 *     . = ALIGN(4);
	 *     _stos_data = .;
	 *     *(.tos_data)
	 *     _etos_data = .;
	 *   } > RAM AT> FLASH
	 *   _sitos_data = LOADADDR(.tos_data);
	 *
	 *   .tos_bss : {
	 *     . = ALIGN(4);
	 *     _stos_bss = .;
	 *     *(.tos_bss)
	 *     _etos_bss = .;
	 *   } > RAM
	 *
	 * Declare the symbols in C as shown above with `extern uint32_t`.
	 * _sitos_data is the flash load address; the other symbols delimit the
	 * RAM ranges copied and cleared during this startup stage. */
	uint32_t* src = &_sitos_data;
	uint32_t* dest  =&_stos_data;
	while (dest < &_etos_data)
		*dest++ = *src++;

	dest = &_stos_bss;
	while (dest < &_etos_bss)
		*dest++ = 0;

	__tos_bsp_init_stage_3_hook();
}

void tos_start()
{
	__tos_disable_exceptions();
	 prepare_procs_to_start();

	__tos_disable_exceptions();
	__tos_irqs_setup_stage_hook();

	__tos_disable_exceptions();
	stm_main();

	__tos_disable_exceptions();
	__tos_final_init_stage_hook();

	__tos_disable_exceptions();
	__tos_specific_OS_related_hook();

	__tos_enable_exceptions();
	__tos_start_idle();
}



