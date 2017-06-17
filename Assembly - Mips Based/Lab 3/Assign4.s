# Assignment 4 for Lab 3

.data

	ask: .asciiz "\n Please enter the number you wish the calculate the cube root of: "
	ans: .asciiz "\n Result: "

.text

main:
	## Display first ask string
	li $v0, 4
	la $a0, ask
	syscall

	## Read input - automatically stored in $f0
	li $v0, 6
	syscall

	## Setting initial conditions
	# register f0 holds N
	li.s $f2, 1.0	# f2 will act as Xi, initially 1
	li.s $f4, 3.0	# f4 holds 3 for the division
	li.s $f6, 2.0	# f6 holds 2 for the multiplication
	# f8 holds 2 * Xi, f10 holds delta (f2 - f12), f12 holds X(i+1) eventually
	li.s $f14, 0.00001	# f14 holds epsilon
	
	
loop:
	## Keep looping until delta is < epsilon
	
	mul.s $f8, $f6, $f2	# update 2*Xi
	div.s $f12, $f0, $f2	# f12 = usernum / Xi
	div.s $f12, $f12, $f2	# f12 = usernum / Xi / Xi = usernum / Xi^2
	add.s $f12, $f12, $f8	# f12 = 2 * Xi + N/Xi^2
	div.s $f12, $f12, $f4	# f12 = (2 * Xi + N/Xi^2)/3 = X(i+1)


	sub.s $f10, $f2, $f12	# Get delta
	abs.s $f10, $f10	# Get absolute value of f10, in case it is negative
	c.le.s $f10, $f14	# Check if f10 <= epsilon
	bc1t end		# If true, branch to end
	mov.s $f2, $f12		# Else, update f2 and continue
	j loop

end:
	## By this point delta < epsilon, therefore f12 holds the answer
	## Display answer string
	li $v0, 4
	la $a0, ans
	syscall

	## Display answer
	li $v0, 2
	syscall

	## Quit
	li $v0, 10
	syscall
