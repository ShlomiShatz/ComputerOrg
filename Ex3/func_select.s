    .section .rodata
charInput:      .string   "%s"  # format for char input
numInput:       .string   "%d"  # format for int input
firstFormat:    .string "first pstring length: %d, second pstring length: %d\n" # format to be used later
secondFormat:   .string "old char: %c, new char: %c, first string: %s, second string: %s\n" # format to be used later
thirdFormat:    .string "length: %d, string: %s\n"  # format to be used later
fourthFormat:   .string "compare result: %d\n"  # format to be used later
invalFormat:    .string "invalid option!\n" # format to be used when invalid input has been taken

    .align  16  # aligns the data
.OPTIONS:   # the options for the menu
    .quad .L31  # the option for inserting 31
    .quad .L33  # the option for inserting 32/33
    .quad .L35  # the option for inserting 35
    .quad .L36  # the option for inserting 36
    .quad .L37  # the option for inserting 37

    .text   # starting the text section
.globl func_select  # making func_select global
    .type func_select, @function    # defining func_select as a function
func_select:    # the menu function that calls the requested function. %rdi - option, %rsi - first pstring, %rdx - second pstring
    pushq	%rbp			# save the old frame pointer
	movq	%rsp,		%rbp	# create the new frame pointer
                            # checking for the option's validity
    cmpq    $31,    %rdi    # check if the option is lower than 31
    jl INVALID  # if so, go to invalid section
    je .L31     # if it equals 31, jump to relevant funciton
    cmpq    $33,    %rdi    # check if the option is lower or equal to 33
    jle .L33    # if it equals 32/33, jump to relevant funciton
    cmpq    $35,    %rdi    # check if the option equals 35
    je  .L35    # if it equals 35, jump to relevant funciton
    cmpq    $36,    %rdi    # check if the option equals 36
    je  .L36    # if it equals 36, jump to relevant funciton
    cmpq    $37,    %rdi    # check if the option equals 37
    je  .L37    # if it equals 37, jump to relevant funciton
    jmp INVALID # if so, go to invalid section

.L31:   # option 31
    movq    %rsi,   %rdi    # moves the first pstring to the pstrlen funciton
    call    pstrlen # checks the pstring's length
    movq    %rax,   %r8 # moves the result to backup register
    movq    %rdx,   %rdi    # moves the second pstring to the pstrlen funciton
    call    pstrlen # checks the pstring's length
    movq    $firstFormat,   %rdi    # passes the printing format to printf
    movq    %r8,    %rsi    # passes the first result
    movq    %rax,   %rdx    # passes the second result
    xorq    %rax,   %rax    # zeros %rax
    call    printf  # calling printf
    jmp END # jumps to the end of the program

.L33:   # options 32-33
    leaq    (%rsi),   %r12 # moving the first pstring to %r12
    leaq    (%rdx),   %r13 # moving the second pstring to %r13
    subq    $32, %rsp   # allocating memory to be used

    leaq    -16(%rbp),    %rsi   # passing the address to scanf
    movq    $charInput, %rdi    # passing the format to scanf
    xorq    %rax,   %rax    # zeros %rax
    call    scanf   # taking the input
    movzbq    -16(%rbp),    %r14 # moves the input to %r14

    leaq    -32(%rbp),    %rsi  # passing the address to scanf
    movq    $charInput, %rdi    # passing the format to scanf
    xorq    %rax,   %rax    # zeros %rax
    call    scanf   # taking the input
    movzbq    -32(%rbp),    %r15    # moves the input to %r15

    leaq    (%r12),    %rdi    # passing the first pstring
    movq    %r14,   %rsi  # passing the 'old char'
    movq    %r15,   %rdx  # passing the 'new char'
    xorq    %rax,   %rax    # zeros %rax
    call    replaceChar # calls the relevant function
    leaq    (%rax),  %r12  # saves the result in %r12

    leaq    (%r13),    %rdi    # passing the second pstring
    movq    %r14,   %rsi  # passing the 'old char'
    movq    %r15,   %rdx  # passing the 'new char'
    xorq    %rax,   %rax    # zeros %rax
    call    replaceChar # calls the relevant function
    leaq    (%rax),  %r13  # saves the result in %r13

    movq    $secondFormat,  %rdi    # passing the printing format for printf
    movq    %r14,   %rsi  # passing the 'old char'
    movq    %r15,   %rdx  # passing the 'new char'
    movq    %r12,    %rcx    # passing the first pstring
    movq    %r13,    %r8 # passing the second pstring
    xorq    %rax,   %rax    # zeros %rax
    call printf # calls printf
    jmp END # jumps to the end of the program

.L35:   # option 35
    leaq    (%rsi),   %r12 # moving the first pstring to %r12 - dst
    leaq    (%rdx),   %r13 # moving the second pstring to %r13 - src
    subq    $32, %rsp   # allocating memory to be used

    leaq    -16(%rbp),    %rsi   # passing the address to scanf
    movq    $numInput, %rdi    # passing the format to scanf
    xorq    %rax,   %rax    # zeros %rax
    call    scanf   # taking the input
    leaq    -16(%rbp),    %r14 # moves the input to %r14 - i

    leaq    -32(%rbp),    %rsi  # passing the address to scanf
    movq    $numInput, %rdi    # passing the format to scanf
    xorq    %rax,   %rax    # zeros %rax
    call    scanf   # taking the input
    leaq    -32(%rbp),    %r15    # moves the input to %r15 - j

    leaq    (%r12),    %rdi    # passes the dst pstring
    leaq    (%r13),    %rsi    # passes the src pstring
    movzbq    (%r14),   %rdx    # passes the i value
    movzbq    (%r15),   %rcx    # passes the j value
    call    pstrijcpy   # calls relevant function

    movzbq  (%rax), %rsi    # passes the length to printf
    leaq    1(%rax), %rdx   # passes the string to printf
    movq    $thirdFormat,   %rdi    # passes the needed format
    xorq    %rax,   %rax    # zeros %rax
    call    printf  # calls printf

    movzbq  (%r13), %rsi    # passes the length to printf
    leaq    1(%r13), %rdx   # passes the string to printf
    movq    $thirdFormat,   %rdi    # passes the needed format
    xorq    %rax,   %rax    # zeros %rax
    call    printf  # calls printf
    jmp END # jumps to the end of the program

.L36:
    leaq    (%rsi),   %rdi    # passes the first pstring
    call    swapCase    # swaps cases
    leaq    (%rax),   %r12 # moves the result to %r12

    leaq    (%rdx),   %rdi    # passes the second pstring
    call    swapCase    # swaps cases
    leaq    (%rax),   %r13 # moves the result to %r13
    
    movzbq  (%r12),   %rsi  # passes the length
    leaq    1(%r12),  %rdx  # advances the pointer by 1
    movq    $thirdFormat,   %rdi    # passes the needed format
    xorq    %rax,   %rax    # zeros %rax
    call    printf  # calls printf

    movzbq  (%r13),   %rsi  # passes the length
    leaq    1(%r13),  %rdx  # advances the pointer by 1
    movq    $thirdFormat,   %rdi    # passes the needed format
    xorq    %rax,   %rax    # zeros %rax
    call    printf  # calls printf

    jmp END # jumps to the end of the program

.L37:
    leaq    (%rsi),   %r12 # moving the first pstring to %r12 - pstr1
    leaq    (%rdx),   %r13 # moving the second pstring to %r13 - pstr2
    subq    $32, %rsp   # allocating memory to be used

    leaq    -16(%rbp),    %rsi   # passing the address to scanf
    movq    $numInput, %rdi    # passing the format to scanf
    xorq    %rax,   %rax    # zeros %rax
    call    scanf   # taking the input
    leaq    -16(%rbp),    %r14 # moves the input to %r14 - i

    leaq    -32(%rbp),    %rsi  # passing the address to scanf
    movq    $numInput, %rdi    # passing the format to scanf
    xorq    %rax,   %rax    # zeros %rax
    call    scanf   # taking the input
    leaq    -32(%rbp),    %r15    # moves the input to %r15 - j

    movq    %r12,    %rdi    # passes the dst pstring
    movq    %r13,    %rsi    # passes the src pstring
    movzbq    (%r14),   %rdx    # passes the i value
    movzbq    (%r15),   %rcx    # passes the j value
    call    pstrijcmp   # calls relevant function

    movq    %rax, %rsi  # passes the result to printf
    movq    $fourthFormat,   %rdi    # passes the needed format
    xorq    %rax,   %rax    # zeros %rax
    call    printf  # calls printf

    jmp END # jumps to the end of the program

INVALID:
    movq	$invalFormat,	%rdi	# passes the needed format for printf
	xorq    %rax,   %rax    # zeros %rax
	call	printf			# calling printf to print
    
END:
    movq	%rbp,   	%rsp	# restores the old stack pointer - release all used memory
	popq	%rbp			# restore old frame pointer
	ret				# return to caller function
