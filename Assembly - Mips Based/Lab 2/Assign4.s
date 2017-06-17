# Assignment 4 for Lab 2

.data
	ask: .asciiz "\n Enter the first string for comparison (max 20 characters): "
	ksa: .asciiz "\n Enter the second string for comparison (max 20 characters): "
	redo: .asciiz "\n Re-enter the second string in case of error the first time around (max 20 characters):  "
	ans: .asciiz "\n The following discrepancies were found while comparing strings: \n"
	ans2: .asciiz "\n No discrepancies found. Terminating program. \n"
	str1: .space 20		# Input string 1
	str2: .space 20		# Input string 2
	str3: .space 20		# String containing discrepancies between the input strings

.text

main:
	## Display ask string
	li $v0, 4
	la $a0, ask
	syscall

	## Retrieve input string 1
	li $v0, 8
	la $a0, str1
	ori $a1, $0, 20
	syscall

	## Display second ask string
	li $v0, 4
	la $a0, ksa
	syscall

	## Retrieve input string 2
	li $v0, 8
	la $a0, str2
	ori $a1, $0, 20 
	syscall

start:
	ori $t0, $0, 0		# t0 will act as index from 0 to 19 to read string chars
	la $t1, str1		# Holds element addr of str1
	la $t2, str2		# Holds element addr of str2
	la $t6, str3		# Holds element addr of the error string
	ori $t5, $0, 20		# Holds 20 to help signify end of strings
	ori $t7, $0, 0		# Holds the amount of mismatched characters

cmploop:
	beq $t0, $t5, end	# Reached end of strings
	lb $t3, 0($t1)		# Get next char from str1
	lb $t4, 0($t2)		# Get next char from str2
	addi $t1, $t1, 1	# Update pointer for str1
	addi $t2, $t2, 1	# Update pointer for str2
	bne $t3, $t4, mismatch	# discrepancy found between strings
	addi $t0, $t0, 1	# Increment index
	j cmploop

mismatch:

	## Store mismatched characters
	sb $t4, 0($t6)		# Store bad character from string 2 into error string
	addi $t6, $t6, 1	# Update element addr to store next char
	addi $t0, $t0, 1	# Increment index
	addi $t7, $t7, 1	# Increase mismatch count by 1
	j cmploop

end:
	## At this point, strings have been iterated through. If t7 holds 0, strings are the same.

	beqz $t7, fin		# Alternate end to program - matching strings
	
	## Display mismatch detected string
	li $v0, 4
	la $a0, ans
	syscall

	## Display mismatched characters
	li $v0, 4
	la $a0, str3
	syscall

	## Display redo message
	li $v0, 4
	la $a0, redo
	syscall

	## Wipe out memory from str2 so previous input doesnt interfere
	ori $t7, $0, 0		# Help fill str2 with zeros
	la $t2, str2		# Holds element addr of str2
	ori $t0, $0, 0		# Refresh index
wiping:
	beq $t0, $t5, resume	# Reached end of wipe
	sb $t7, 0($t2)
	addi $t0, $t0, 1	# Increment index
	addi $t2, $t2, 1	# Update pointer for str2
	j wiping

resume:
	## Retrieve input string 3
	li $v0, 8
	la $a0, str2
	ori $a1, $0, 20
	syscall

	j start		# Jump to start to do it over again

fin:
	## At this point, strings have been found to be the same. End program.

	## Display ending string
	li $v0, 4
	la $a0, ans2
	syscall

	## End program
	li $v0, 10
	syscall






