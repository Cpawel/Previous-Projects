# Used in Assignment 5
# Registers used:
# $t0 - used to hold the x coordinate of the first pair.
# $t1 - used to hold the y coordinate of the first pair.
# $t2 and $t5 - used to help compute and hold the distance between the coordinates.
# $t3 - used to hold the x coordinate of the second pair.
# $t4 - used to hold the y coordinate of the second pair.
# $t6 - used to hold -1 in order to flip negative distances.
# $v0 - syscall parameter and return value.
# $a0 - syscall parameter.

.data
	initial:
	.asciiz "\n Initializing Manhattan Distance calculator.\n Please enter the first X-coordinate. NOTE: Integers only!\n"

	init2:
	.asciiz "\n Please enter the Y-coordinate of the first pair.\n"

	second:
	.asciiz "\n Please enter the X-coordinate of the second pair.\n"

	second2:
	.asciiz "\n Please enter the Y-coordinate of the second pair.\n"
.text

main:
	li $t6, -1 	# Load -1 into $t6 for future use to get abs. value

	## Printing initial message
	li $v0, 4	# system call code for writing is 4
	la $a0, initial	# load address of the message into a0
	syscall		# print message

	## Get x-coord from user, put into $t0.
	li $v0, 5 	# load syscall read_int into $v0.
	syscall 	# make the syscall.
	move $t0, $v0 	# move the number read into $t0.

	## Printing second message
	li $v0, 4	# system call code for writing is 4
	la $a0, init2	# load address of the message into a0
	syscall		# print message

	## Get y-coord from user, put into $t0.
	li $v0, 5 	# load syscall read_int into $v0.
	syscall 	# make the syscall.
	move $t1, $v0 	# move the number read into $t1.

	## Printing third message
	li $v0, 4	# system call code for writing is 4
	la $a0, second	# load address of the message into a0
	syscall		# print message

	## Get second x-coord from user, put into $t0.
	li $v0, 5 	# load syscall read_int into $v0.
	syscall 	# make the syscall.
	move $t3, $v0 	# move the number read into $t3.

	## Printing fourth message
	li $v0, 4	# system call code for writing is 4
	la $a0, second2	# load address of the message into a0
	syscall		# print message

	## Get second y-coord from user, put into $t0.
	li $v0, 5 	# load syscall read_int into $v0.
	syscall 	# make the syscall.
	move $t4, $v0 	# move the number read into $t4.

	## Compute the distances for x and y
	sub $t2, $t0, $t3 	# Compute the difference between X coords and put in $t2
	sub $t5, $t1, $t4	# Compute the difference between Y coords and put in $t5

	## Making sure both values are positive

check1:
	bgez $t2, check2 	# If $t2 >= 0, continue the program
	mul $t2, $t2, $t6	# Else, multiply $t2 by -1
check2:
	bgez $t5, final		# If $t5 >= 0, continue
	mul $t5, $t5, $t6	# Else, multiply $t5 by -1
final:
	## Should be dealing with only positive distances now - Compute and Print answer
	add $t2, $t2, $t5	# Get final distance
	move $a0, $t2		# Store the distance into a0 for printing
	li $v0, 1		# Inserted printing code for integers
	syscall			# Print the integer distance

	## Exit
	li $v0, 10
	syscall
	
















