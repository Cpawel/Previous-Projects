# Assignment 5 for Lab 3
# This program works in single precision, not double precision.

.data
	ask: .asciiz "\nEnter in the height: "
	ask2: .asciiz "\nEnter in the radius: "
	ans: .asciiz "\nThe resulting volume is: "
	err: .asciiz "\nNegative numbers are not allowed. Try again.\n"

.text

main:
	li.s $f12, 0.0	# Store zero into f12 for error checking
height:
	## Display first ask string - height
	li $v0, 4
	la $a0, ask
	syscall

	## Read input - automatically stored in $f0 -> transfer to $f2 so next float can be read
	li $v0, 6
	syscall
	mov.s $f2, $f0	# Transferring

	## Check if non-negative and branch to radius if so
	c.le.s $f12, $f2
	bc1t radius
	
	## Display error string
	li $v0, 4
	la $a0, err
	syscall
	j height	# if height < 0, prompt user for new height until positive

radius:
	## Display second ask string - radius
	li $v0, 4
	la $a0, ask2
	syscall

	## Read input - automatically stored in $f0
	li $v0, 6
	syscall

	## Check if non-negative and branch to continue if so
	c.le.s $f12, $f0
	bc1t continue
	
	## Display error string
	li $v0, 4
	la $a0, err
	syscall
	j radius	# if radius < 0, prompt user for new radius until positive

continue:

	## By this point, height and radius have been successfully read into f2 and f0

	mul.s $f4, $f0, $f0	# Performing radius^2 and moving to new float register f4

	mul.s $f6, $f4, $f2	# Multiply r^2 by height and store in f6

	li.s $f8, 3.141592653589	# Store pi into f8

	mul.s $f6, $f6, $f8	# f6 now holds r^2 * h * pi

	li.s $f8, 3.0		# Replace pi by 3 in order to be able to finish calculation

	div.s $f6, $f6, $f8	# f6 = r^2 * h * pi / 3, which is the answer (volume of circular cone)

end:
	## Display answer string
	li $v0, 4
	la $a0, ans
	syscall

	## Display answer
	li $v0, 2
	mov.s $f12, $f6		# Print float uses f12 to output - moving answer to f12
	syscall

	## Quit
	li $v0, 10
	syscall
