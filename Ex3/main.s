.include "run_main.s"

    .text	# text section
.globl		main			# main is the initial point of this program
	.type	main, 	@function	# main is the beginning of a function
main:
    pushq   %rbp
    movq    %rsp,   %rbp
    call    run_main
    movq    %rbp,    %rsp
    popq    %rbp
        