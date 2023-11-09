.syntax unified
.cpu cortex-m4
.thumb

.section .text.TOS.Atomic

.type tos_add_fetch_atomic, %function
tos_add_fetch_atomic:
__try_again:
	ldrex r2, [r0]
	add r2, r2, r1
	strex r2, r3, [r0]
	cmp r3, 0
	beq __trygain
	add r0, r2, 0
	bx lr
