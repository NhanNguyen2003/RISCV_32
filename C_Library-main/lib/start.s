.section .text
.global _start

_start:
    # SP is already set by Kernel
    call main
    
    # If main returns, call exit(0)
    li a7, 93
    li a0, 0
    ecall
1:  j 1b
