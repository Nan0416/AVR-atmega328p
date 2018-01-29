	.file	"interrupt1.c"
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__SREG__ = 0x3f
__tmp_reg__ = 0
__zero_reg__ = 1
	.text
.global	powerDown
	.type	powerDown, @function
powerDown:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	ldi r18,lo8(319999)
	ldi r24,hi8(319999)
	ldi r25,hlo8(319999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	in r24,0x33
	andi r24,lo8(-15)
	ori r24,lo8(4)
	out 0x33,r24
	in r24,0x33
	ori r24,lo8(1)
	out 0x33,r24
/* #APP */
 ;  13 "interrupt1.c" 1
	sleep
 ;  0 "" 2
/* #NOAPP */
	in r24,0x33
	andi r24,lo8(-2)
	out 0x33,r24
	ret
	.size	powerDown, .-powerDown
.global	blink
	.type	blink, @function
blink:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sbi 0x5,4
	ldi r18,lo8(3199999)
	ldi r24,hi8(3199999)
	ldi r25,hlo8(3199999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	cbi 0x5,4
	ldi r18,lo8(3199999)
	ldi r24,hi8(3199999)
	ldi r25,hlo8(3199999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	ret
	.size	blink, .-blink
.global	on_board_led
	.type	on_board_led, @function
on_board_led:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	sbi 0x5,5
	ldi r18,lo8(3199999)
	ldi r24,hi8(3199999)
	ldi r25,hlo8(3199999)
1:	subi r18,1
	sbci r24,0
	sbci r25,0
	brne 1b
	rjmp .
	nop
	cbi 0x5,5
	ret
	.size	on_board_led, .-on_board_led
.global	__vector_1
	.type	__vector_1, @function
__vector_1:
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
	call blink
	sbi 0x1c,0
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
	.size	__vector_1, .-__vector_1
.global	main
	.type	main, @function
main:
/* prologue: function */
/* frame size = 0 */
/* stack size = 0 */
.L__stack_usage = 0
	in r24,0x4
	ori r24,lo8(48)
	out 0x4,r24
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
 ;  41 "interrupt1.c" 1
	sei
 ;  0 "" 2
/* #NOAPP */
.L6:
	call powerDown
	call on_board_led
	rjmp .L6
	.size	main, .-main
	.ident	"GCC: (GNU) 4.9.2"
