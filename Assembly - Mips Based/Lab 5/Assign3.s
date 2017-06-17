.data 0x10000860
	Vector_A: .word 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25
.data 0x1000A000	
	Matrix_B: .word 26, 27, 28, 29, 30
.data 0x1000A100
	.word 31, 32, 33, 34, 35
.data 0x1000A200
	.word 36, 37, 38, 39, 40
.data 0x1000A300
	.word 41, 42, 43, 44, 45
.data 0x1000A400
	.word 46, 47, 48, 49, 50
.data 0x1000B000
	Vector_C: .space 100

.text 0x00400000
.globl main

main:
	# Initialization
	la $a1, Vector_A 	# a1 holds element address of A
	la $a2, Matrix_B 	# a2 holds element address of B
	la $a3, Vector_C 	# a3 holds element address of C

	la $t0, Matrix_B	# t0 holds base row address of B
	li $t1, 0	# t2 holds index for elements of A
	li $t2, 0 	# t1 holds index for the loop in B
	
loop:
	beq $t1, 25, exit	# If loop is on 25th element, exit
	lw $t3, 0($a1)	# Load element of A into t3
	lw $t4, 0($a2)	# Load element of B into t4
	
	mulo $t5, $t3, $t4	# t5 holds A[x] * B[x] with overflow
	sw $t5, 0($a3)	# Store the above result in C[x]
	
	addi $t1, $t1, 1	# Increment index of A
	addi $a1, $a1, 4	# Update address for element of A
	addi $a3, $a3, 4	# Update address for element of C
	
	addi $t2, $t2, 1	# Increment index of B
	beq $t2, 5, newrow	# If loop is on 5th element, new row in B
	addi $a2, $a2, 4	# Update address for element of B
	
	j loop
	
newrow:
	# New row of B - have to reset t2 and put new address in a2
	li $t2, 0	# reset
	addi $t0, $t0, 256	# Offset between rows is 256 bytes
	move $a2, $t0	# Copy number in t0 to a2
	
	j loop

exit: 
	li $v0, 10
	syscall
