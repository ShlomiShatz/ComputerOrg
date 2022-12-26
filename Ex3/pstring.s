# 316093202 Shlomo Shatz
# Part of the third exercise in Computer Organization course
    .section .rodata			# read-only section
strerr:	.string  "invalid input!\n"	# string format to be used later

    .text			# text section
.globl		pstrlen	# making pstrlen global
    .type	pstrlen,	@function    # defining pstrlen as a function
pstrlen:    			# returns the length of a given pstring
    pushq   %rbp		# save the old frame pointer
    movq    %rsp,	%rbp	# create the new frame pointer

    movzbq  (%rdi),	%rax	# moves the first byte of the pstring to %rax

    movq    %rbp,	%rsp	# restores the old stack pointer - release all used memory
    popq    %rbp		# restore old frame pointer
    ret			# return to caller function

.globl		replaceChar	# making replaceChar global
    .type	replaceChar, 	@function	# defining replaceChar as a function
replaceChar:    		# replaces the old char with new char in a pstring
    pushq   %rbp		# save the old frame pointer
    movq    %rsp,	%rbp	# create the new frame pointer 

    xorq    %rcx,	%rcx 	# zeros %rcx to be used as a counter
    leaq    1(%rdi),	%r8	# loads the pointer to different register
    cmpb    (%r8),	%sil 	# checks if the chars are equals
    je      swapChar    	# if so, jumps to the relevant part

LOOP0:				# the LOOP function for this part
    cmpb    (%rdi),	%cl	# checks if the string is finished
    je      END0		# if so, jumps to the end

    inc     %cl 		# increments the counter
    leaq    (%rcx, %rdi),	%r8	# advances the pointer by a byte(using the counter)
    cmpb    (%r8),	%sil 	# compares the two chars
    je      swapChar		# if it is equal, jumps to the relevant location
    jmp     LOOP0		# jumps back to the loop

swapChar:   			# the part that swaps the character
    movb    %dl,	(%r8)	# moves the new char to the old's place
    jmp LOOP0   		# jumps back to the loop
    
END0:   			# ending section
    leaq    (%rdi),	%rax	# moves the result pstring to %rax
    movq    %rbp,	%rsp	# restores the old stack pointer - release all used memory
    popq    %rbp		# restore old frame pointer
    ret			# return to caller function

.globl		pstrijcpy	# making pstrijcpy global
    .type	pstrijcpy, 	@function	# defining pstrijcpy as a function
pstrijcpy:			# copies part of a pstring to another
    pushq   %rbp		# save the old frame pointer
    movq    %rsp,	%rbp	# create the new frame pointer
    pushq   %r12    		# saves the callee-saved convention register

    leaq    (%rdi),	%r12	# saves the dst pstring address in a register
    
    cmpb    $0,	%dl 	# checks if the index is smaller than 0
    jb      ERR  		# if so, jumps to error section
    cmpb    %dl,	%cl  	# checks if i > j
    jb      ERR 		# if so, jumps to error section
    cmpb    (%rsi),	%dl 	# checks if i >= src.size
    jae     ERR    		# if so, jumps to error section
    cmpb    (%rdi),	%dl 	# checks if i >= dst.size
    jae     ERR    		# if so, jumps to error section
    cmpb    (%rsi),	%cl 	# checks if j >= src.size
    jae     ERR    		# if so, jumps to error section
    cmpb    (%rdi),	%cl 	# checks if j >= dst.size
    jae     ERR    		# if so, jumps to error section

    leaq    1(%rdx, %rsi),    %r8   # moves the src string starting at location i to the register
    leaq    1(%rdx, %rdi),    %r9   # moves the dst string starting at location i to the register

LOOP1:				# the LOOP part of the funciton
    cmpb    %dl,	%cl 	# checks if i > j (index passed the limit)
    jb      END1    		# if so, jumps to the finish section
    movb    (%r8),	%r12b	# copies the first byte of the string to a register
    movb    %r12b,	(%r9)	# copies the copied byte of the old string
    leaq    1(%r8),	%r8 	# advances the pointer by 1
    leaq    1(%r9),	%r9 	# advances the pointer by 1
    inc     %dl		# increments the index
    jmp     LOOP1		# jumps back to the loop

ERR:				# for invalid parameters
    movq    $strerr,	%rdi	# the string to be passed to printf
    xorq    %rax,	%rax 	# zeroing %rax
    call    printf		# calling printf to print
    leaq    (%r12),    %rdi 	# loading the original pstring back

END1:				# ending part of the function
    leaq    (%rdi),	%rax 	# moves the result to the return address
    popq    %r12    		# restores the callee-saved convention register
    movq    %rbp,   	%rsp	# restores the old stack pointer - release all used memory
    popq    %rbp		# restore old frame pointer
    ret			# return to caller function

    
.globl		swapCase	# making swapCase global
    .type	swapCase,	@function   # defining swapCase as a function
swapCase:			# swaps the characters of a pstring from lower to upper and the other way around
    pushq   %rbp		# save the old frame pointer
    movq    %rsp,	%rbp	# create the new frame pointer

    xorq    %rcx,	%rcx 	# zeros a counter 
    leaq    1(%rdi),   %r10	# moves the string to a register
    movzbq  (%rdi),    %r11	# moves the length to a register

LOOP3:				# the LOOP part of this function
    cmpb    %r11b,	%cl	# checks if the string is finished
    je      END3		# if so, jumps to the end
    cmpb    $65,	(%r10)	# checks if the current char is not a letter
    jb      NOTL		# jumps to the relevant section
    cmpb    $90,	(%r10)	# checks if the current char is an upper case
    jbe     UPPER		# jumps to the relevant section
    cmpb    $97,	(%r10)	# checks if the current char is not a letter
    jb      NOTL		# jumps to the relevant section
    cmpb    $122,	(%r10)	# checks if the current lower case
    jbe     LOWER		# jumps to the relevant section
    jmp     NOTL		# jumps to the relevant section in case it is not a letter

UPPER:				# handling upper case letters
    addb    $32,	(%r10)	# adding 32 to change to lower
    leaq    1(%r10),	%r10	# advances the pointer by a first byte
    inc     %cl		# increments the counter
    jmp     LOOP3		# jumps back to the loop

LOWER:				# handling lower case letters
    subb    $32,	(%r10)	# reducing 32 to change to upper
    leaq    1(%r10),	%r10	# advances the pointer by a first byte
    inc     %cl		# increments the counter
    jmp     LOOP3		# jumps back to the loop

NOTL:   			# handling not a letter case
    leaq    1(%r10),	%r10	# advances the pointer by a first byte
    inc     %cl 		# increments the counter
    jmp     LOOP3		# jumps back to the loop

END3:   			# the end section of this function
    leaq    (%rdi),	%rax	# passes the result to the return value
    movq    %rbp,   	%rsp	# restores the old stack pointer - release all used memory
    popq    %rbp		# restore old frame pointer
    ret			# return to caller function


.globl		pstrijcmp	# making pstrijcmp global
     .type	pstrijcmp,	@function	# defining pstrijcmp as a function
 pstrijcmp:			# compares two pstring lexicographically and returns 1, 0, -1, -2 based on the result
    pushq   %rbp		# save the old frame pointer
    movq    %rsp,	%rbp	# create the new frame pointer
    pushq   %r12    		# saves the callee-saved convention register
    pushq   %r13    		# saves the callee-saved convention register
    
    cmpb    $0,    	%dl	# checks if the index is smaller than 0
    jb      ERR0	 	# if so, jumps to error section
    cmpb    %dl,	%cl	# checks if i > j
    jb      ERR0		# if so, jumps to error section
    cmpb    (%rsi),	%dl	# checks if i >= src.size
    jae     ERR0		# if so, jumps to error section
    cmpb    (%rdi),	%dl	# checks if i >= dst.size
    jae     ERR0		# if so, jumps to error section
    cmpb    (%rsi),	%cl	# checks if j >= src.size
    jae     ERR0		# if so, jumps to error section
    cmpb    (%rdi),	%cl	# checks if j >= dst.size
    jae     ERR0		# if so, jumps to error section

    leaq    1(%rdx, %rdi),	%r8	# moves the first string starting at location i to the register
    leaq    1(%rdx, %rsi),	%r9	# moves the second string starting at location i to the register

LOOP2:				# the LOOP part of this section
    cmpb    %dl,	%cl	# checks if the index has reached its limits (i > j)
    jb      EQL		# if so, jumps to the relevant section
    
    movzbq  (%r8),	%r12	# moves the first byte to a different register
    movzbq  (%r9),	%r13	# moves the first byte to a different register
    cmpq    %r12,	%r13	# compares the current letters
    jb      GRTR		# if the first is the bigger, jump to relevent section
    ja      LWR   		# if the first is the smaller, jump to relevent section
    
    leaq    1(%r8),	%r8	# advances the pointer by 1
    leaq    1(%r9),	%r9	# advances the pointer by 1
    inc     %dl 		# increments the counter
    jmp     LOOP2  		# jumps back to the loop

GRTR:   			# handles if first > second
    movq    $1,	%rax	# moves 1 to the return value
    jmp     END2		# jumps to the end

EQL:				# handles if first = second
    xorq    %rax,	%rax	# moves 0 to the return value
    jmp     END2		# jumps to the end

LWR:				# handles if first < second
    movq    $-1,	%rax	# moves -1 to the return value
    jmp     END2		# jumps to the end


ERR0:				# invalid input section
    movq    $strerr,	%rdi	# the string to be passed to printf
    xorq    %rax,	%rax 	# zeroing %rax
    call    printf		# calling printf to print
    movq    $-2,	%rax	# moves -2 to the return value

END2:				# the end section of the function
    popq    %r13    		# restores the callee-saved convention register
    popq    %r12   		# restores the callee-saved convention register
    movq    %rbp,	%rsp	# restores the old stack pointer - release all used memory
    popq    %rbp		# restore old frame pointer
    ret			# return to caller function
    
