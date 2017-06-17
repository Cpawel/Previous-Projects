#Exercise is used in assignment 3
 .data 0x10000000
 .align 2
 	Array: .word 2 5 6 7 12 16
	Size: .word 6
 .text 0x00400000
 .globl main
main:
	lw $a0, Size 	# Load the size of array into $ao, using lw
	li $a1, 0	# initialize index i
	li $t2, 4	# t2 contains constant 4, initialize t2
	li $t0, 1	# Initialize result to one
loop:
	mul $t1, $a1, $t2	 # t1 gets i*4
	lw $a3, Array($t1)	 # a3 = Array[i]
	mul $t0, $t0, $a3	 # result *= Array[i]
	sw $t0, Array($t1)	 # store result in the Array2 in location i (???)
	addi $a1, $a1, 1	 # i++
	blt $a0, $a1, END	 # go to END if finished
j loop

END:
	li $v0, 10
	syscall

# Index i reaches 7 due to a0 < a1, not a0 <= a1. Therefore, a1 must surpass a0.
