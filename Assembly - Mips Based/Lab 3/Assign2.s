# Assignment 2 for Lab 3

.data

	ask: .asciiz "\n Please enter the first integer: "
	ask2: .asciiz "\n Please enter the second integer: "
	ans: .asciiz "\n Result: "

.text

main:
	## Display first ask string
	li $v0, 4
	la $a0, ask
	syscall

	## Read and store input 1
	li $v0, 5
	syscall
	move $t0, $v0

	## Display second ask string
	li $v0, 4
	la $a0, ask2
	syscall

	## Read and store input 2
	li $v0, 5
	syscall
	move $t1, $v0

	add $v0, $0, $0		# Store result in v0

	## Check sign of numbers
	slt $t2, $t0, $0	# t2 now holds 1 if t0 is negative
	beq $t2, $0, pos1
	nor $t0, $t0, $t0	# one's complement of t0
	addi $t0, $t0, 1	# two's complement of t0

pos1:
	slt $t3, $t1, $0	# t3 now holds 1 if t1 is negative, both t2 and t3 used later
	beq $t3, $0, continue
	nor $t1, $t1, $t1	# one's complement of t1
	addi $t1, $t1, 1	# two's complement of t1

continue:
	slt $t4, $t0, 32
	bne $t4, $0, loop1	# If 32 is larger than t0, go to loop 1
	slt $t4, $t1, 32
	bne $t4, $0, loop2	# Same as above. This is due to the fact that adding would take less time than going through the bits. Shortens execution time.
	j long			# If both t0 and t1 are larger than 32, do bitwise.

loop1:
	beq $t0, $0, end
	add $v0, $v0, $t1	# v0 will get t1 added to it t0 times
	addi $t0, $t0, -1	# decrement t0
	j loop1

loop2:
	beq $t1, $0, end
	add $v0, $v0, $t0	# v0 will get t0 added to it t1 times
	addi $t1, $t1, -1	# decrement t1
	j loop2

long:
	beq $t1, $0, end
	andi $t5, $t1, 1	# grab LSB of t1, to know whether to add or not
	beq $t5, $0, shift	# LSB is 0, no addition, just shift
	add $v0, $v0, $t0
shift:
	sll $t0, $t0, 1
	srl $t1, $t1, 1
	j long

end:
	beq $t2, $t3, fin	# The input numbers had the same sign, output is positive
	nor $v0, $v0, $v0
	addi $v0, $v0, 1	# Two's complement performed on answer

fin:
	## Output answer and quit
	or $t0, $v0, $0		# Move answer to t0 so v0 can be used for syscalls
	li $v0, 4
	la $a0, ans	# Loads the ans string
	syscall
	move $a0, $t0	# Loads the answer
	li $v0, 1
	syscall
	li $v0, 10	# Exiting
	syscall
	
