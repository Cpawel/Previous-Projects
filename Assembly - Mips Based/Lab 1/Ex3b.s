# Used in assignment 4
# Registers used: $t0 - used to hold the first number.
# $t1 - used to hold the second number.
# $t2 - used to hold the difference of the $t1 and $t0.
# $v0 - syscall parameter and return value.
# $a0 - syscall parameter.

 .data
	choice1:
	.asciiz "\n Please enter first number you wish to calculate the difference of. #1 - #2\n"
	choice2:
	.asciiz "\n Please enter the second number.\n"
	custom:
	.asciiz "\n Please enter the sentence you wish trail the answer. Max 20 characters!\n"
	string: .space 20
 .text

main:
	## Printing initial message
	li $v0, 4	# system call code for writing is 4
	la $a0, choice1	# load address of the message into a0
	syscall		# print message

	## Get first number from user, put into $t0.
	li $v0, 5 	# load syscall read_int into $v0.
	syscall 	# make the syscall.
	move $t0, $v0 	# move the number read into $t0.

	## Printing the second message
	li $v0, 4	# system call code for writing is 4
	la $a0, choice2	# load address of the message into a0
	syscall		# print message

	## Get second number from user, put into $t1.
	li $v0, 5 	# load syscall read_int into $v0.
	syscall 	# make the syscall.
	move $t1, $v0 	# move the number read into $t1.

	## Printing the third message
	li $v0, 4	# system call code for writing is 4
	la $a0, custom	# load address of the message into a0
	syscall		# print message

	## Read string from input
	li $v0, 8	# prep for string reading
	la $a0, string	# give start address to a0 for string
	li $a1, 20	# Specify how long the string will be, stored in a1
	syscall		# Get string

	## Computing the difference and printing
	sub $t2, $t0, $t1	# Computing
	move $a0, $t2		# Move the difference into a0
	li $v0, 1		# Prep for printing int
	syscall			# Print the difference

	li $v0, 4		# Prep for printing string
	la $a0, string		# Give address for string
	syscall			# Print string

	li $v0, 10		# Load code to exit program
	syscall			# exit
	
