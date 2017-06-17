# Assignment 5 for Lab 2

.data
	ask: .asciiz "\n Set a password (8 to 12 characters): "
	confirm: .asciiz "\n Re-enter the password: "
	err: .asciiz "\n Failed. Please enter a password with the size of 8 to 12!\n"
	redo: .asciiz "\n Incorrect, you have 2 chances left! Please re-enter the password: "
	redo2: .asciiz "\n Incorrect, last chance! Please re-enter the password: "
	success: .asciiz "\n Password is setup.\n"
	bad: .asciiz "\n Password has not been setup.\n"
	str1: .space 12		# Initial password
	str2: .space 12		# Confirming password

.text

main:
	ori $t5, $0, 12		# Holds 12 to help signify end of strings
	j good

fail:
	## Display error message
	li $v0, 4
	la $a0, err
	syscall

good:
	## Display ask string
	li $v0, 4
	la $a0, ask
	syscall

	## Retrieve input string 1
	li $v0, 8
	la $a0, str1
	ori $a1, $0, 12
	syscall

	ori $t0, $0, 0		# t0 will act as index from 0 to 12 to read string chars
	la $t1, str1		# Holds element addr of str1
	la $t2, str2		# Holds element addr of str2

	## Check if input password is valid
check:
	beq $t0, $t5, resume	# Reached end of string
	lb $t3, 0($t1)		# Get next char from str1
	addi $t0, $t0, 1	# Increment index
	addi $t1, $t1, 1	# Update pointer for str1
	beqz $t3, check2	# End of string found - check if password is long enough
	j check

check2:
	blt $t0, 8, fail	# Too short a password was input


resume:
	## Display confirming string
	li $v0, 4
	la $a0, confirm
	syscall

	## Retrieve input string 2
	li $v0, 8
	la $a0, str2
	ori $a1, $0, 12 
	syscall

	## Now to check that the two strings agree

	ori $t0, $0, 0		# t0 will act as index from 0 to 12 to read string chars
	la $t1, str1		# Holds element addr of str1
	la $t2, str2		# Holds element addr of str2

cmploop:
	beq $t0, $t5, end	# Reached end of strings
	lb $t3, 0($t1)		# Get next char from str1
	lb $t4, 0($t2)		# Get next char from str2
	addi $t0, $t0, 1	# Increment index
	addi $t1, $t1, 1	# Update pointer for str1
	addi $t2, $t2, 1	# Update pointer for str2
	bne $t3, $t4, mismatch1	# discrepancy found between strings
	j cmploop

mismatch1:
	## Display fail string
	li $v0, 4
	la $a0, redo
	syscall

	## Clear string 2 to be ready for new input
	la $t2, str2		# Holds element addr of str2
	ori $t0, $0, 0		# Refresh index
wiping:
	beq $t0, $t5, resume2	# Reached end of wipe
	sb $0, 0($t2)
	addi $t0, $t0, 1	# Increment index
	addi $t2, $t2, 1	# Update pointer for str2
	j wiping

resume2:
	
	## Retrieve input again
	li $v0, 8
	la $a0, str2
	ori $a1, $0, 12
	syscall

	ori $t0, $0, 0		# t0 will act as index from 0 to 12 to read string chars
	la $t1, str1		# Holds element addr of str1
	la $t2, str2		# Holds element addr of str2

cmploop2:
	beq $t0, $t5, end	# Reached end of strings
	lb $t3, 0($t1)		# Get next char from str1
	lb $t4, 0($t2)		# Get next char from str2
	addi $t0, $t0, 1	# Increment index
	addi $t1, $t1, 1	# Update pointer for str1
	addi $t2, $t2, 1	# Update pointer for str2
	bne $t3, $t4, mismatch2	# discrepancy found between strings
	j cmploop2

mismatch2:
	## Display fail string
	li $v0, 4
	la $a0, redo2
	syscall

	## Clear string 2 to be ready for new input
	la $t2, str2		# Holds element addr of str2
	ori $t0, $0, 0		# Refresh index
wiping2:
	beq $t0, $t5, resume3	# Reached end of wipe
	sb $0, 0($t2)
	addi $t0, $t0, 1	# Increment index
	addi $t2, $t2, 1	# Update pointer for str2
	j wiping2

resume3:
	
	## Retrieve input again
	li $v0, 8
	la $a0, str2
	ori $a1, $0, 12
	syscall

	ori $t0, $0, 0		# t0 will act as index from 0 to 12 to read string chars
	la $t1, str1		# Holds element addr of str1
	la $t2, str2		# Holds element addr of str2

cmploop3:
	beq $t0, $t5, end	# Reached end of strings
	lb $t3, 0($t1)		# Get next char from str1
	lb $t4, 0($t2)		# Get next char from str2
	addi $t0, $t0, 1	# Increment index
	addi $t1, $t1, 1	# Update pointer for str1
	addi $t2, $t2, 1	# Update pointer for str2
	bne $t3, $t4, mismatch3	# discrepancy found between strings
	j cmploop2

mismatch3:
	## Too many tries taken. Ending program.
	li $v0, 4
	la $a0, bad
	syscall

	j fin

end:
	## Password set. Ending program.
	li $v0, 4
	la $a0, success
	syscall

fin:
	## End program
	li $v0, 10
	syscall
