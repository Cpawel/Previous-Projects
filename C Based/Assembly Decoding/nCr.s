.globl nCr
	.type	nCr, @function
nCr:
	movq	%rbx, -32(%rsp)
	movq	%rbp, -24(%rsp)
	movq	%r12, -16(%rsp)
	movq	%r13, -8(%rsp)
	subq	$40, %rsp
.Body:
	movl	%edi, %ebx
	movl	%esi, %ebp
	cmpl    %edi, %esi
	jg      .Error
	call	Factorial
	movl	%eax, %r13d
	movl	%ebp, %edi
	call	Factorial
	movl	%eax, %r12d
	movl	%ebx, %edi
	subl	%ebp, %edi
	call	Factorial
	imull	%eax, %r12d
	movl	%r13d, %edx
	movl	%r13d, %eax
	movl	$0, %edx
	divl	%r12d
.End:
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	addq	$40, %rsp
	ret
.Error:
	movl    $0, %eax
	movq	8(%rsp), %rbx
	movq	16(%rsp), %rbp
	movq	24(%rsp), %r12
	movq	32(%rsp), %r13
	addq	$40, %rsp
	ret
	
	
.globl Factorial
	.type	Factorial, @function
Factorial:
	movq %rdi,%rax
	movq %rax,%rcx
	decq %rcx
	cmpq $1,%rcx
	jge .LoopStart
	jmp .error
.LoopStart:
	mulq %rcx
	decq %rcx
	cmpq $0,%rcx
	je .finish
	jmp .LoopStart
.error:
	movq $1, %rax
.finish:
	ret

	
