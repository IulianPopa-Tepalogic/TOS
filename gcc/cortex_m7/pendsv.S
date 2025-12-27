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

.syntax unified
.cpu cortex-m7

.section .text.TOS.PendSV_Handler

.type __tos_prepare_for_ctx_switch, %function
.type TOSPendSV_Handler, %function

.global TOSPendSV_Handler
TOSPendSV_Handler:
		push {r2, r3, lr}
		add r0, sp, 0
		add r1, sp, 4

		bl __tos_prepare_for_ctx_switch
		pop {r2, r3, lr}

		mrs r1, psp
		stmdb r1!, {r4-r11}
#ifdef TOS_FPU_ENABLED
		vstmdb r1!, {s16-s31}
#endif
		str r1, [r2]

		ldr r1, [r3]
#ifdef TOS_FPU_ENABLED
		vldm r1!, {s16-s31}
#endif
		ldm r1!, {r4-r11}
		msr psp, r1
		cpsie if
		bx lr
