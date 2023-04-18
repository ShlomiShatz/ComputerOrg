// 316093202 Shlomo Shatz
# This is the second exercise in Computer Organization course
	.data	# data section
id:	.quad	123456789	# the id stored in this memory

    .section	.rodata	# read only data section
strnum: .string	"%lu\n" 	# the number is passed to the string
strtr:	.string	"True\n"	# string that holds True
strfl:	.string	"False\n"	# string that holds False

    .text	# text section
.globl		main			# main is the initial point of this program
	.type	main, 	@function	# main is the beginning of a function
main:	# the main function (and the first part of the exercise):
	pushq	%rbp			# save the old frame pointer
	movq	%rsp,		%rbp	# create the new frame pointer
	movq 	id, 		%rsi 	# assigning id (316093202) to %rsi
	movq	$strnum,	%rdi	# the string to be passed to printf
	xor	%rax,		%rax 	# zeroing %rax
	call	printf			# calling printf to print

SEC:	# the second part of the exercise:
	xor	%rbx,		%rbx	# zeros %rbx
	xor	%rsi,		%rsi	# zeros %rsi
	movq	$id,		%rsi	# assigns the address of id to %rsi
	movsbq	1(%rsi),	%rbx	# assigns the second signed byte to %rbx
	movq 	$1, 		%rcx	# assigns the number 1 to %rcx
	andq	%rcx,		%rbx	# uses bitwise 'and' to check if %rbx is odd or even (based on the LSB)
	je	EVEN			# if it is even, jumps to the propriate function

ODD:	# the function if it is odd:
	movq	id,		%rsi	# assigning id (316093202) to %rsi
	leaq	(%rsi, %rsi, 2),	%rsi	# multiplies %rsi by 3
	movq	$strnum,	%rdi	# the string to be passed to printf
	xor	%rax,		%rax 	# zeroing %rax
	call	printf			# calling prinf to print
	jmp	THRD			# jump to the next part of the exercise

EVEN:	# the function if it is even:
	xor	%rax, 		%rax	# zeros %rax
	xor	%rdx,		%rdx	# zeros %rdx
	movq	id,		%rax	# assigning id (316093202) to %rax
	idivq 	3			# divides by 3 (now %rdx contains the remainder)
	movq	%rdx,		%rsi	# assigns %rdx (the remainder) to %rsi (to be passed on printf)
	movq	$strnum,	%rdi	# the string to be passed to printf
	xor	%rax,		%rax 	# zeroing %rax
	call	printf			# calling printf to print

THRD:	# the third part of the exercise:
	movq	$id,		%rsi	# assigns the address of id to %rsi
	xor	%rbx,		%rbx	# zeros %rbx
	xor	%rcx,		%rcx	# zeros %rcx
	movsbq	(%rsi),	%rbx	# assigns the first signed byte to %rbx
	movsbq	2(%rsi),	%rcx	# assigns the third signed byte to %rcx
	xor	%rcx,		%rbx	# calculates xor and moving it to %rbx
	movq	$127, 		%rcx	# assigns 127 to %rcx
	cmpq	%rcx,		%rbx	# compares %rbx (xor result) with %rcx (127)
	ja	GRTR			# if it is bigger (unsigned), jump to relevant label

LESR:	# the function if it is equal or lesser:
	movq	$strfl,	%rdi	# the string to be passed to printf
	xor	%rax,		%rax 	# zeroing %rax
	call	printf			# calling printf to print
	jmp	UNTNUM			# jumps to the next part of the exercise

GRTR:	# the function if it is greater:
	movq	$strtr,	%rdi	# the string is the first parameter pass to the function
	xor	%rax,		%rax 	# zeroing %rax
	call	printf			# calling printf to print

UNTNUM:	# the fourth part of the exercise
	movq	$id,		%rsi	# assigning id (316093202) to %rsi
	xor	%rcx,		%rcx	# zeros %rcx
	xor 	%rdx,		%rdx	# zeros %rdx
	movq	$1,		%rdx	# assigns 1 to %rdx
	movsbq	3(%rsi),	%rbx	# assigns the fourth signed byte to %rbx
	testq	$1,		%rbx	# tests 1 and %rbx(to compare the LSB with 1)
	je 	LOOP			# if zero flag is on, jumps to the LOOP function
	inc	%rcx			# if not, increments %rcx
LOOP:	# the LOOP function for this part
	cmpb	$0, 		%bl	# checks if the number is zero
	je 	RSLT			# if so, jumps to the end
	shrb	$1, 		%bl	# shifts logically the one step to the right
	testq	$1,		%rbx	# test if the LSB is 1
	je	LOOP			# if not, jumps back to the beginning of the loop
	inc	%rcx			# if it is 1, increments %rcx
	jmp 	LOOP			# jumps back to the beginning of the loop
RSLT:	# the printing of the result:
	movq 	%rcx,		%rsi	# moves the result (%rcx counter) to %rsi to be passed to printf
	movq	$strnum,	%rdi	# the string is to be passed to printf
	xor	%rax,		%rax 	# zeroing %rax
	call	printf			# calling printf to print

END:	# the end of the program:
	xor	%rax,		%rax	# zeros the return value
	movq	%rbp,   	%rsp	# restores the old stack pointer - release all used memory
	popq	%rbp			# restore old frame pointer
	ret				# return to caller function
