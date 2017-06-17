#Assignment 1 for Lab 2

 .data
	ask: .asciiz "\n Please enter a non-negative integer. \n"
	ans: .asciiz "\n Result: \n"
	err: .asciiz "\n Number entered is negative.\n"

 .text
 .globl main

main:
	## Display ask string
	li $v0, 4
	la $a0, ask
	syscall

	## Read and verify input is non-negative
	li $v0, 5
	syscall
	move $t0, $v0

	bgez $t0, loop	# Go to loop if num is >0

	## Number is negative
	li $v0, 4
	la $a0, err
	syscall		# Print error string
	j main		# Jump to main so user can enter new number

loop:
     beqz $t0, END
	add $t1, $t1, $t0 	# t1 holds the result
	sub $t0, $t0, 1	# Decrement t0
     j loop

END:
	li $v0, 4
	la $a0, ans	# Loads the ans string
	syscall
	move $a0, $t1	# Loads the answer
	li $v0, 1
	syscall
	li $v0, 10	# Exiting
	syscall



