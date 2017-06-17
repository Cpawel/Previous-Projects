# Used for Assignment 6

.text

main:

	li $t0, 0	# Initialize counter to 0
	li $t1, 0	# Holds offset from address
	li $t2, 4	# Holds size of a Word

loop:
	add $t3, $t1, $a0	# $t3 holds the current address to access
	ulw $v1, 0($t3)		# Go to address in $t3 and store in $v1
	beqz $v1, return	# If the word in v1 is equal to zero, jump to the end

	add $t3, $t1, $a1	# Update address for storing
	usw $v1, 0($t3)		# Store the word in $v1 at the address in $t3

	addi $t0, $t0, 1	# Increment counter by 1
	mul $t1, $t0, $t2	# Offset = counter * size of a Word

	j loop

return:
	li $v0, 10		# Load exit code 10 into v0
	syscall			# Exit


# NOTE: Address in a0 is NOT guaranteed to be divisible by 4, and thus will cause an exception when trying to read and write.
