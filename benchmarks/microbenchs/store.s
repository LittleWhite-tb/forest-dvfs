	.file	"stride.c"
	.text
	.p2align 4,,15
.globl entryPoint
	.type	entryPoint, @function
entryPoint:
.LFB22:
	.cfi_startproc
    sub $32, %rsp
    #Store r10~r13
    mov %r10, 0(%rsp)
    mov %r11, 8(%rsp)
    mov %r12, 16(%rsp)
    mov %r13, 24(%rsp)
    #Load r10~r12
    mov 40(%rsp), %r10
    mov 48(%rsp), %r11
    mov 56(%rsp), %r12
    #Reset eax, r13
	xorl	%eax, %eax
    xor %r13, %r13
    #To be safe, remove a bit...
    sub     $4, %rdi
    testq	%rdi, %rdi
    je	.L3
    .p2align 4,,10
    .p2align 3


.L6:

	#Unroll beginning
	#Unrolled factor 8
	#Unrolling, iteration 1 out of 8
	movaps %xmm0, 0(%rsi)
	#Unrolling, iteration 2 out of 8
	movaps %xmm1, 16(%rsi)
	#Unrolling, iteration 3 out of 8
	movaps %xmm2, 32(%rsi)
	#Unrolling, iteration 4 out of 8
	movaps %xmm3, 48(%rsi)
	#Unrolling, iteration 5 out of 8
	movaps %xmm4, 64(%rsi)
	#Unrolling, iteration 6 out of 8
	movaps %xmm5, 80(%rsi)
	#Unrolling, iteration 7 out of 8
	movaps %xmm6, 96(%rsi)
	#Unrolling, iteration 8 out of 8
	movaps %xmm7, 112(%rsi)
	#Unroll ending
	#Induction variables
	##Induction variable: 1 , 1 , 1
	add $1, %eax
	##Induction variable: 1 , 16 , 1
	add $16, %rsi
	sub $4, %rdi
	jge .L6


.L3:
    mov 0(%rsp), %r10
    mov 8(%rsp), %r11
    mov 16(%rsp), %r12
    mov 24(%rsp), %r13
    add $32, %rsp

	ret
	.cfi_endproc
.LFE22:
	.size	entryPoint, .-entryPoint
	.ident	"GCC: (Ubuntu 4.4.3-4ubuntu5) 4.4.3"
	.section	.note.GNU-stack,"",@progbits

