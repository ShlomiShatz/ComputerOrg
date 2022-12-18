# 316093202 Shlomo Shatz
# Part of the third exercise in Computer Organization course
.include    "func_select.s"


    .data
cinstr: .string "%s"    # format for string input
cinnum: .string "%d"    # format for number input

    .text	# text section
.globl		run_main			# making run_main global
    .type	run_main, 	@function	# defining run_main a function
run_main:   # the function that runs the begining of the program
    pushq   %rbp    # saving old frame pointer
    movq    %rsp,   %rbp    # creat the new frame pointer

    subq    $768,       %rsp    # allocating memory for all the things
    leaq    -512(%rbp), %rsi    # passing the address to scanf
    movq    $cinstr,    %rdi    # passing the input format to scanf - first number
    xor     %rax,       %rax    # zeroing %rax
    call    scanf   # taking in the input

    leaq    -511(%rbp), %rsi    # passing the address to scanf
    movq    $cinstr,    %rdi    # passing the input format to scanf - first string
    xor     %rax,       %rax    # zeroing %rax
    call    scanf   # taking in the input

    leaq    -256(%rbp), %rsi    # passing the address to scanf
    movq    $cinstr,    %rdi    # passing the input format to scanf - second number
    xor     %rax,       %rax    # zeroing %rax
    call    scanf   # taking in the input

    leaq    -255(%rbp), %rsi    # passing the address to scanf
    movq    $cinstr,    %rdi    # passing the input format to scanf - second string
    xor     %rax,       %rax    # zeroing %rax
    call    scanf   # taking in the input

    leaq    -768(%rbp), %rsi    # passing the address to scanf
    movq    $cinnum,    %rdi    # passing the input format to scanf - menu option
    xor     %rax,       %rax    # zeroing %rax
    call    scanf   # taking in the input

    movzbq    -768(%rbp), %rdi    # passing the first argument to the menu - menu option
    leaq    -512(%rbp), %rsi    # passing the second argument to the menu - first pstring
    leaq    -256(%rbp), %rdx    # passing the third argument to the menu - second pstring
    call     func_select    # calling the menu function


    movq    %rbp,       %rsp   # restores the old stack pointer - release all used memory
    popq    %rbp    # restore old frame pointer
    ret     # return to caller function
    