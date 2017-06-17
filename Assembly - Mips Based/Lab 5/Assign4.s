.data 0x10000800
	OrinRow_0: .word 1, 2, 3, 4, 5, 6
	OrinRow_1: .word 7, 8, 9, 10, 11, 12
	OrinRow_2: .word 13, 14, 15, 16, 17, 18
	OrinRow_3: .word 19, 20, 21, 22, 23, 24
	OrinRow_4: .word 25, 26, 27, 28, 29, 30
	OrinRow_5: .word 31, 32, 33, 34, 35, 36
.data 0x10001000
	TransRow_0: .word 0, 0, 0, 0, 0, 0
	TransRow_1: .word 0, 0, 0, 0, 0, 0
	TransRow_2: .word 0, 0, 0, 0, 0, 0
	TransRow_3: .word 0, 0, 0, 0, 0, 0
	TransRow_4: .word 0, 0, 0, 0, 0, 0
	TransRow_5: .word 0, 0, 0, 0, 0, 0
	
.text 0x00400000
.globl main

main:
	# First Initialization - Upper Transform
	
	la $a0, OrinRow_3	# Load Orin addresses
	la $a1, OrinRow_4
	la $a2, OrinRow_5
	
	la $t0, TransRow_0	# Load Trans addresses
	la $t1, TransRow_1
	la $t2, TransRow_2
	
	addi $a0, $a0, 20	# Adjust so that Orin start from end
	addi $a1, $a1, 20
	addi $a2, $a2, 20
	
	li $t3, 0	# Current index
	
loop:
	beq $t3, 6, nextinit	# If t3 = 6, first half done
	
	lw $t5, 0($a0)	# Load element from first row
	lw $t6, 0($a1)	# Load element from second row
	lw $t7, 0($a2)	# Load element from third row
	
	sw $t5, 0($t0)	# Store element from first row into fourth row
	sw $t6, 0($t1)	# Store element from second row into fifth row
	sw $t7, 0($t2)	# Store element from third row into sixth row
	
	addi $a0, $a0, -4	# Go to next element in Orin
	addi $a1, $a1, -4
	addi $a2, $a2, -4
	
	addi $t0, $t0, 4	# Go to next storage point in Trans
	addi $t1, $t1, 4
	addi $t2, $t2, 4
	
	addi $t3, $t3, 1	# Increment index
	j loop
	
nextinit:
	# Second Initialization - Lower Transform
	
	la $a0, OrinRow_0	# Load Orin addresses
	la $a1, OrinRow_1
	la $a2, OrinRow_2
	
	la $t0, TransRow_3	# Load Trans addresses
	la $t1, TransRow_4
	la $t2, TransRow_5
	
	addi $a0, $a0, 20	# Adjust so that Orin start from end
	addi $a1, $a1, 20
	addi $a2, $a2, 20
	
	li $t3, 0	# Current index
	
loop2:
	beq $t3, 6, exit	# If t3 = 6, second half done
	
	lw $t5, 0($a0)	# Load element from fourth row
	lw $t6, 0($a1)	# Load element from fifth row
	lw $t7, 0($a2)	# Load element from sixth row
	
	sw $t5, 0($t0)	# Store element from fourth row into first row
	sw $t6, 0($t1)	# Store element from fifth row into second row
	sw $t7, 0($t2)	# Store element from sixth row into third row
	
	addi $a0, $a0, -4	# Go to next element in Orin
	addi $a1, $a1, -4
	addi $a2, $a2, -4
	
	addi $t0, $t0, 4	# Go to next storage point in Trans
	addi $t1, $t1, 4
	addi $t2, $t2, 4
	
	addi $t3, $t3, 1	# Increment index
	j loop2
	
exit:
	li $v0, 10
	syscall
	