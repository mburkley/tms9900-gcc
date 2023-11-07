	pseg
LC0
	text "HELLO WORLD!"
	byte 0
	even
LC1
	text "THIS IS LINE 2"
	byte 0
	even

	def	main
main
	li r1, 64 * 256
	movb r1, @-29694
	clr r1
	movb r1, @-29694
	li r1, LC0
L2
	movb *r1+, r2
	movb r2, @-29696
	ci r1, LC0+12
	jne L2
	li r1, 96 * 256
	movb r1, @-29694
	clr r1
	movb r1, @-29694
	li r1, LC1
L3
	movb *r1+, r2
	movb r2, @-29696
	ci r1, LC1+14
	jne L3
L8
	jmp L8
