	.file	"interrupt1.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	delay_ms
	.type	delay_ms, @function
delay_ms:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	cp __zero_reg__,r24
	cpc __zero_reg__,r25
	brge .L1
	ldi r18,0
	ldi r19,0
.L3:
	ldi r30,lo8(3999)
	ldi r31,hi8(3999)
1:	sbiw r30,1
	brne 1b
	rjmp .
	nop
	subi r18,-1
	sbci r19,-1
	cp r18,r24
	cpc r19,r25
	brne .L3
.L1:
	ret
	.size	delay_ms, .-delay_ms
.global	on_board_led
	.type	on_board_led, @function
on_board_led:
	push r28
	push r29
/* prologue: function */
/* frame size = 0 */
/* stack size = 2 */
.L__stack_usage = 2
	movw r28,r24
	sbi 0x5,4
	call delay_ms
	cbi 0x5,4
	movw r24,r28
	call delay_ms
/* epilogue start */
	pop r29
	pop r28
	ret
	.size	on_board_led, .-on_board_led
.global	__vector_1
	.type	__vector_1, @function
__vector_1:
	sei
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r28
	push r29
	push r30
	push r31
/* prologue: Interrupt */
/* frame size = 0 */
/* stack size = 17 */
.L__stack_usage = 17
	ldi r28,lo8(100)
	ldi r29,0
.L7:
	lds r24,time
	lds r25,time+1
	call on_board_led
	sbi 0x1c,0
	sbiw r28,1
	brne .L7
/* epilogue start */
	pop r31
	pop r30
	pop r29
	pop r28
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_1, .-__vector_1
.global	__vector_2
	.type	__vector_2, @function
__vector_2:
	push r1
	push r0
	in r0,__SREG__
	push r0
	clr __zero_reg__
	push r18
	push r19
	push r20
	push r21
	push r22
	push r23
	push r24
	push r25
	push r26
	push r27
	push r30
	push r31
/* prologue: Signal */
/* frame size = 0 */
/* stack size = 15 */
.L__stack_usage = 15
	ldi r24,lo8(50)
	ldi r25,0
	call on_board_led
	ldi r24,lo8(50)
	ldi r25,0
	call on_board_led
	sbi 0x1c,1
	lds r24,time
	lds r25,time+1
	asr r25
	ror r24
	sts time+1,r25
	sts time,r24
	lds r24,time
	lds r25,time+1
	sbiw r24,50
	brge .L9
	ldi r24,lo8(-48)
	ldi r25,lo8(7)
	sts time+1,r25
	sts time,r24
.L9:
/* epilogue start */
	pop r31
	pop r30
	pop r27
	pop r26
	pop r25
	pop r24
	pop r23
	pop r22
	pop r21
	pop r20
	pop r19
	pop r18
	pop r0
	out __SREG__,r0
	pop r0
	pop r1
	reti
	.size	__vector_2, .-__vector_2
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sbi 0x4,4
	in r24,0xa
	andi r24,lo8(-13)
	out 0xa,r24
	ldi r30,lo8(105)
	ldi r31,0
	ld r24,Z
	ori r24,lo8(15)
	st Z,r24
	in r24,0x1d
	ori r24,lo8(3)
	out 0x1d,r24
/* #APP */
 ;  40 "interrupt1.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
	ldi r18,lo8(3199999)
	ldi r24,hi8(3199999)
	ldi r25,hlo8(3199999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
.L12:
	rjmp .L12
	.size	main, .-main
.global	time
	.data
	.type	time, @object
	.size	time, 2
time:
	.word	2000
	.ident	"GCC: (GNU) 4.9.2"
.global __do_copy_data
