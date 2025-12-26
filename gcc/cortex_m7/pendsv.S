
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
