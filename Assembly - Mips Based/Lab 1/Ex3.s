# Used in assignment 4
# Registers used: $t0 - used to hold the first number.
# $t1 - used to hold the second number.
# $t2 - used to hold the difference of the $t1 and $t0.
# $v0 - syscall parameter and return value.
# $a0 - syscall parameter.

 .text

main:
	## Get first number from user, put into $t0.
	li $v0, 5 	# load syscall read_int into $v0.
	syscall 	# make the syscall.
	move $t0, $v0 	# move the number read into $t0.

	## Get second number from user, put into $t1.
	li $v0, 5 	# load syscall read_int into $v0.
	syscall 	# make the syscall.
	move $t1, $v0 	# move the number read into $t1.
