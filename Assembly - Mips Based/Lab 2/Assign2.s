# Assignment 2 for Lab 2

.data

	primes: 
	.word 2 3 5 7 11 13 17 19 23 29 31 37 41 43 47 53 59 61 67 71 73 79 83 89 97

	ask: .asciiz "\n Please enter an integer less than 101 and greater than 1. \n"
	ans: .asciiz "\n Result: \n"
	err: .asciiz "\n Number entered is out of program bounds.\n"

.text

main:
	## Display ask string
	li $v0, 4
	la $a0, ask
	syscall

	## Read and verify input is in bounds
	li $v0, 5
	syscall
	move $t0, $v0

	ori $t1, $0, 1		# Load 1 into t1 for error check
	ori $t2, $0, 101	# Load 101 into t2 for error check

	ble $t0, $t1, bad	# Go to bad if input <= 1
	bge $t0, $t2, bad	# Go to bad if input >= 101
	j good

bad:
	## Number is out of bounds
	li $v0, 4
	la $a0, err
	syscall		# Print error string
	j main		# Jump to main so user can enter new number

good:
	## Number recieved is good - start search for prime
	ori $t1, $0, 24		# 25 slots in primes array, t1 is the index
	ori $t2, $0, 4		# Holds 4 in order to traverse words in the array
	la $t3, primes		# Holds base address of primes
	mul $t4, $t1, $t2	# word size * index
	add $t4, $t4, $t3	# t4 now holds the address of the last element in the array
	lw $t1, 0($t4)		# t1 updated to hold the element from the array

loop:
	ble $t1, $t0, end	# Leave upon t1 being smaller or equal to input
	addi $t4, $t4, -4	# Decrementation of index, to get the next element
	lw $t1, 0($t4)		# Update t1
	j loop			# Continue the loop

end:
	## $t1 holds the prime searched for at this point - print the result
	li $v0, 4
	la $a0, ans	# Loads the ans string
	syscall
	move $a0, $t1	# Loads the answer
	li $v0, 1
	syscall
	li $v0, 10	# Exiting
	syscall












