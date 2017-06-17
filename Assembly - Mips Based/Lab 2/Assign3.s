# Assignment 3 for Lab 2

.data
	ask: .asciiz "\n Enter a number between 0 and 1000: "
	err: .asciiz "\n Number entered is out of program bounds.\n"
	ans: .asciiz "\n Result of even summation: "
	wer: .asciiz "\n Result of odd summation: "

.text

main:

	## Display ask string
	li $v0, 4
	la $a0, ask
	syscall

	## Read and verify input is in bounds
	li $v0, 5
	syscall
	move $t0, $v0		# t0 holds the input

	ori $t1, $0, 1000	# Load 1000 into t1 for error check
	
	bltz $t0, bad		# Go to bad if input < 0
	bgt $t0, $t1, bad	# Go to bad if input > 1000
	j good

bad:
	## Number is out of bounds
	li $v0, 4
	la $a0, err
	syscall		# Print error string
	j main		# Jump to main so user can enter new number

	addi $t1, $0, 0	# i = 0
	addi $t2, $0, 0 # ans = 1, Starting case (n=0) is 1
	li $t3, 2	# we store two in $t3
good:
	ori $t1, $0, 0	# t1 will hold the even sum
	ori $t2, $0, 0	# t2 will hold the odd sum
	ori $t3, $0, 0	# t3 is the index, rises to the input number

	## Right before the loop, t0 holds the input, t1 will hold the evens, t2 will hold the odds, t3 will increment by 1 until reaching input number

loop:
	beq $t3, $t0, END 	# if t3 reaches t0, go to end
	addi $t3, $t3, 1	# increment t3 by 1
	add $t2, $t2, $t3	# add the odd to t2

	beq $t3, $t0, END	# recheck t3 against t0
	addi $t3, $t3, 1	# increment t3 by 1
	add $t1, $t1, $t3	# add the even to t2

	j loop			# go back to loop

END:
	li $v0, 4
	la $a0, ans	# Loads the even ans string
	syscall
	move $a0, $t1	# Loads the even answer
	li $v0, 1
	syscall

	li $v0, 4
	la $a0, wer	# Loads the odd ans string
	syscall
	move $a0, $t2	# Loads the even answer
	li $v0, 1
	syscall

	li $v0, 10	# Exiting
	syscall
