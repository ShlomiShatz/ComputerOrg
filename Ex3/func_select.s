    .include "pstring.s"
    
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
    xor %rax,   %rax    # zero's %rax
    call    printf  # calling printf
    jmp END # jumps to the end of the program

.L33:   # options 32-33
    xor %r8,    %r8 # zero's register to be used
    xor %r9,    %r9 # zero's register to be used
    xor %r10,   %r10    # zero's register to be used
    xor %r11,    %r11   # zero's register to be used

    movq    %rsi,   %r8 # moving the first pstring to %r8
    movq    %rdx,   %r9 # moving the second pstring to %r9
    subq    $32, %rsp   # allocating memory to be used

    leaq    -16(%rbp),    %rsi   # passing the address to scanf
    movq    $charInput, %rdi    # passing the format to scanf
    xor %rax,   %rax    # zero's %rax
    call    scanf   # taking the input
    movzbq    -16(%rbp),    %r10 # moves the input to %r10

    leaq    -32(%rbp),    %rsi  # passing the address to scanf
    movq    $charInput, %rdi    # passing the format to scanf
    xor %rax,   %rax    # zero's %rax
    call    scanf   # taking the input
    movzbq    -32(%rbp),    %r11    # moves the input to %r11

    movq    %r8,    %rdi    # passing the first pstring
    movq    %r10,   %rsi  # passing the 'old char'
    movq    %r11,   %rdx  # passing the 'new char'
    xor %rax,   %rax    # zero's %rax
    call    replaceChar # calls the relevant function
    movq    %rax,  %r8  # saves the result in %r8

    movq    %r9,    %rdi    # passing the second pstring
    movzbq  (%r10),   %rsi  # passing the 'old char'
    movzbq  (%r11),   %rdx  # passing the 'new char'
    xor %rax,   %rax    # zero's %rax
    call    replaceChar # calls the relevant function
    movq    %rax,  %r9  # saves the result in %r9

    movq    $secondFormat,  %rdi    # passing the printing format for printf
    movzbq  (%r10),   %rsi  # passing the 'old char'
    movzbq  (%r11),   %rdx  # passing the 'new char'
    movq    %r8,    %rcx    # passing the first pstring
    movq    %r9,    %r8 # passing the second pstring
    xor %rax,   %rax    # zero's %rax
    call printf # calls printf
    jmp END # jumps to the end of the program

.L35:
    subq    $32, %rsp   # allocating memory to be used

    leaq    -16(%rbp),    %rsi   # passing the address to scanf
    movq    $numInput, %rdi    # passing the format to scanf
    xor %rax,   %rax    # zero's %rax
    call    scanf   # taking the input
    movzbq    -16(%rbp),    %r10 # moves the input to %r10

    leaq    -32(%rbp),    %rsi  # passing the address to scanf
    movq    $numInput, %rdi    # passing the format to scanf
    xor %rax,   %rax    # zero's %rax
    call    scanf   # taking the input
    movzbq    -32(%rbp),    %r11    # moves the input to %r11

.L36:

.L37:


INVALID:
    movq	$invalFormat,	%rdi	# the string is to be passed to printf
	xor	%rax,		%rax 	# zeroing %rax
	call	printf			# calling printf to print
END:
    movq	%rbp,   	%rsp	# restores the old stack pointer - release all used memory
	popq	%rbp			# restore old frame pointer
	ret				# return to caller function