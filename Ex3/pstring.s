# 316093202 Shlomo Shatz
# Part of the third exercise in Computer Organization course
    .section .rodata:
strerr: .string  "Invalid Input!\n"

    .text	# text section
    .globl pstrlen
    .type pstrlen, @function    # psrtlen is the beginning of a function
pstrlen:
    pushq	%rbp			# save the old frame pointer
	movq	%rsp,		%rbp	# create the new frame pointer
    movzbq  (%rdi),   %rax  # moves the first byte of the pstring to %rax
    movq	%rbp,   	%rsp	# restores the old stack pointer - release all used memory
	popq	%rbp			# restore old frame pointer
	ret				# return to caller function

    .globl replaceChar
    .type	replaceChar, 	@function	# replaceChar is the beginning of a function
replaceChar:    # replaces the old char with new char in a pstring
    pushq	%rbp			# save the old frame pointer
	movq	%rsp,		%rbp	# create the new frame pointer 

    xor %rcx,       %rcx    # zeros %rcx to be used as a counter
    leaq    1(%rdi),	%r8	# loads the pointer to different register
    cmpb   (%r8),   %sil # checks if the chars are equals
    je  swapChar    # if so, jumps to the relevant part

LOOP0:	# the LOOP function for this part
    cmpb (%rdi), %cl   # checks if the string is finished
    je  END0    # if so, jumps to the end
    inc %cl # increments the counter
    leaq	(%rcx, %rdi),	%r8	# advances the pointer by a byte
    cmpb   (%r8),   %sil # compares the two chars
    je  swapChar    # if it is equal, jumps to the relevant location
    jmp LOOP0   # jumps back to the loop

swapChar:   # the part that swaps the character
    movb    %dl,    (%r8)    # moves the new char to the old's place
    jmp LOOP0   # jumps back to the loop
    
END0:   # ending section
    leaq    1(%rdi),   %rax    # moves the result to %rax
    movq	%rbp,   %rsp	# restores the old stack pointer - release all used memory
	popq	%rbp			# restore old frame pointer
	ret				# return to caller function

    .globl pstrijcpy
    .type	pstrijcpy, 	@function	# pstrijcpy is the beginning of a function
pstrijcpy:
    pushq	%rbp			# save the old frame pointer
	movq	%rsp,		%rbp	# create the new frame pointer

    leaq    (%rdi),    %r10
    
    cmpb    $0,    %dl  # checks if the index is smaller than 0
    jb ERR
    cmpb    %dl,   %cl  # checks if i > j
    jb  ERR
    cmpb    (%rsi),   %dl # checks if j >= src.size
    jae  ERR
    cmpb    (%rdi),   %dl # checks if j >= dst.size
    jae  ERR
    cmpb    (%rsi),   %cl # checks if i >= src.size
    jae  ERR
    cmpb    (%rdi),   %cl # checks if i >= dst.size
    jae  ERR

    leaq    1(%rdx, %rsi),    %r8
    leaq    1(%rdx, %rdi),    %r9

LOOP1:
    cmpb    %dl,    %cl
    jl  END1
    movb    (%r8),   %r10b
    movb    %r10b,   (%r9)
    leaq    1(%r8), %r8
    leaq    1(%r9), %r9
    inc %dl
    jmp LOOP1

ERR:
    movq	$stringput,	%rdi	# the string to be passed to printf
	xor	%rax,		%rax 	# zeroing %rax
    pushq   %r10
	call	printf			# calling printf to print
    popq    %r10
    leaq    (%r10),    %rdi
    jmp END1

END1:
    leaq    (%rdi),     %rax
    movq	%rbp,   	%rsp	# restores the old stack pointer - release all used memory
	popq	%rbp			# restore old frame pointer
	ret				# return to caller function

    
    .globl swapCase
    .type swapCase, @function
swapCase:
    pushq	%rbp			# save the old frame pointer
	movq	%rsp,		%rbp	# create the new frame pointer
    xor %cl,    %cl
    movb    %dil,   %cl
    inc %cl
    addb	$1,	    (%rdi)	# advances the pointer by a first byte
LOOP3:
    cmpb %dil, 0   # if the first byte of %rdi is null, jump to the end
    je  END3
    cmpb    $65,    %dil
    jl  NOTL
    cmpb    $90,     %dil
    jle UPPER
    cmpb    $97,    %dil
    jl  NOTL
    cmpb    $122,   %dil
    jle LOWER
    jmp NOTL
UPPER:
    addb    $32,    %dil
    addb	$1,	    (%rdi)	# advances the pointer by a first byte
    jmp LOOP3
LOWER:
    subb    $32,    %dil
    addb	$1,	    (%rdi)	# advances the pointer by a first byte
    jmp LOOP3
NOTL:
    addb	$1,	    (%rdi)	# advances the pointer by a first byte
    jmp LOOP3
END3:
    subb    %cl,   (%rdi)
    movq    %rdi,   %rax
    movq	%rbp,   	%rsp	# restores the old stack pointer - release all used memory
	popq	%rbp			# restore old frame pointer
	ret				# return to caller function


     .globl pstrijcmp
     .type pstrijcmp, @function
 pstrijcmp:
    