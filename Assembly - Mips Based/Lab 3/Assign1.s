# Assignment 1 for Lab 3

.data

	ask: .asciiz "\n Please enter the first integer: \n"
	ask2: .asciiz "\n Please enter the second integer: \n"
	ask3: .asciiz "\n Please enter the third, and last, integer: \n"
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

	## Display third ask string
	li $v0, 4
	la $a0, ask3
	syscall

	## Read and store input 3
	li $v0, 5
	syscall
	move $t2, $v0

	## All input has been stored in t0, t1 and t2 respectively - Time to calculate

	or $t0, $t0, $t2
	and $t1, $t1, $t2
	not $t0, $t0
	not $t1, $t1
	and $t0, $t0, $t1	#Final answer stored in t0

	## Output answer and quit
	li $v0, 4
	la $a0, ans	# Loads the ans string
	syscall
	move $a0, $t0	# Loads the answer
	li $v0, 1
	syscall
	li $v0, 10	# Exiting
	syscall
