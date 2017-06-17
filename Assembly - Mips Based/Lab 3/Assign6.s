# Assignment 6 for Lab 3
# This program works in single precision, not double precision.

.data
	ask: .asciiz "\nEnter in the comparing number (non-imaginary): "
	ans: .asciiz "\nSum of all numbers larger than comparator is: "
	A: .float 1.35, 2.67, 3.566, 4.56, 5.98, 9.43, 12.34, 15.54, 23.87, 34.33

.text

main:
	## Display ask string
	li $v0, 4
	la $a0, ask
	syscall

	## Read input
	li $v0, 6
	syscall
	
	## Check if user input is smaller than smallest element
	li.s $f12, 0.0	# preemptively load 0.0 into the answer register
	la $t0, A	# load address of A into t0
	lwc1 $f2,0($t0)	# load word from A[0], meaning load 1.35 into f2
	c.lt.s $f0, $f2	# compare userinput <? A[0]
	bc1t all	# If the above comparison is true, jump to all, to output total sum of array


	## Initialization of loop
	addi $t1, $t0, 36	# t1 holds address of A[9]

continue:
	## Loop until address in t1 is lower than address of t0 (entire array scanned) OR until element in A is smaller than user input

	blt $t1, $t0, end	# t1 < t0 - array scanned
	lwc1 $f2, 0($t1)	# Load next float into f2
	c.lt.s $f0, $f2		# compare input <? A[?]
	bc1f end		# If false, branch to end

	add.s $f12, $f12, $f2	# f2 holds A[?] (initially A[9], going down), f12 holds the sum
	addi $t1, $t1, -4	# decrement t1 by a word

	j continue

all:
	## If this point is reached, user entered a number smaller than 1.35, so simply output the sum from precalculation, instead of going through the entire array

	li.s $f12, 113.636	# Total sum of elements in float array A

end:
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
