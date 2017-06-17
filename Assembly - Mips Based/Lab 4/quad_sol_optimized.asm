# quad_sol.s
# This assembly program calculates the integer solutions of a quadratic polynomial.
# Inputs : The coefficients a,b,c of the equation a*x^2 + b*x + c = 0
# Output : The two integer solutions. 
#
# All numbers are 32 bit integers

.globl main

main: 					# Read all inputs and put them in floating point registers.

	li $v0, 4
	la $a0, str1			# Load actual string to register $a0
	syscall
	
	li $v0, 5			# Load read_int syscall code to register v0 for the coefficient of a quadratic polynomial
	syscall
	move $t1, $v0		# Move input from register $v0 to register $t1
	
	la $a0, str2		# Load print_string syscall code to register v0 for the 2nd string - line moved up 1 command from previous - 1 cycle saved total
	li $v0, 4			
	syscall
	
	li $v0, 5			# Load read_int syscall code to register v0 for the coefficient of a quadratic polynomial
	syscall
	move $t2, $v0
	
	la $a0, str3		# Line moved up 1 command from previous - 2 cycles saved total
	li $v0, 4
	syscall
	
	li $v0, 5
	syscall
	move $t3, $v0
	
	
	# In the following lines all the necessary steps are taken to 
	# calculate the discriminant of the quadratic equation
	# D = b^2 - 4*a*c
	
	#li $t0, 2 			# Load constant number to integer register
	# Merge with calculating the integer square root by the equation x*x = D
	mul $t5,$t1,$t3 		# t5 = t1*t3, where t1 holds a and t3 holds c - line moved up 1 command from previous - 3 cycles saved total
	mul $t4,$t2,$t2 		# t4 = t2*t2, where t2 holds b
	mul $t5,$t5,4 			# Multiply value of s0 with 4, creating 4*a*c
	li $s0, 1 			# Square Root Partial Result, sqrt(D). - line moved up 2 commands - 4 cycles saved total
	sub $t6,$t4,$t5 		# Calculate D = b^2-4*a*c
	tlt $t6,$0 			# If D is less than 0 issue an exception
	move $s1,$t6 			# Move value in register t6 to register s1 for safety purposes.
	
	sqrtloop:			# calculating the integer square root of D - don't think I can optimize
		mult $s0, $s0
		mflo $s2
		bge $s2, $s1, endsqrt
		addi $s0, 1
	
	
	endsqrt:
		neg $s2,$t2		# calculate -b and save it to s2
		li $t0, 2 			# Load constant number to integer register - line moved up 2 commands from previous - 6 cycles saved total
		add $s3,$s2,$s0 	# Calculate -b+sqrt(D) and save it to s3
		mul $s5,$t1,$t0 	# Calculate 2*a and save it to s5 - line moved up 1 command from previous - 7 cycles saved total
		sub $s4,$s2,$s0 	# Calculate -b-sqrt(D) and save it to s4
		div $s6,$s3,$s5 	# Calculate first integer solution
		div $s7,$s4,$s5 	# Calculate second integer solution
		
	#Print the calculated solutions.

	li $v0, 4			# Load print_string syscall code to register v0 for the 1st result string
	la $a0, str4 			# Load actual string to register $a0
	syscall
	
	li $v0 1			# Load print_int syscall code to register v0 for the 1st result string
	move $a0, $s6			# Load actual integer to register $a0
	syscall
	
	li $v0, 4			# Load print_string syscall code to register v0 for the 1st result string
	la $a0, str5			# Load actual string to register $a0
	syscall
	
	li $v0, 1
	move $a0, $s7
	syscall
	
	li $v0, 10
	syscall

	
			
	
.data
	str1 : .asciiz "Please enter coefficient a of equation a*x^2 + b*x + c: " 
	
	str2 : .asciiz "Please enter coefficient b of equation a*x^2 + b*x + c: "
	 
	str3 : .asciiz "Please enter coefficient c of equation a*x^2 + b*x + c: " 
	
	str4 : .asciiz "The first integer solution is: "
	
	str5 : .asciiz "\nThe second integer solution is: "