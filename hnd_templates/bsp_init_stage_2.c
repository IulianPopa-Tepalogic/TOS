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

void __tos_bsp_init_stage_2_hook()
{
	/* TODO: Add code to initialize the specific devices of
	 * Intent: Initialize the board peripherals that must be ready before C
	 * runtime initialization, such as external RAM used by .data or .bss.
	 *
	 * C/C++ global and static storage has not been initialized yet. Do not
	 * read or write C globals, and do not use C++ global objects. After this
	 * hook returns, the reset handler initializes .data and .bss, then
	 * __tos_start_stage2() copies .tos_data and clears .tos_bss. At exit,
	 * IRQs must remain disabled. */
}



