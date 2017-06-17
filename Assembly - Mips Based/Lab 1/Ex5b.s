#ex5b.s
 .data 0x10000000
	ask: .asciiz "\nEnter a number between 0 and 50000: "
	ans: .asciiz "\nAnswer: "

 .text 0x00400000
 .globl main
main:
	li $v0, 4
	la $a0, ask	# Loads the ask string
	syscall		# Display the ask string
	li $v0, 5	# Read the input
	syscall
	move $t0, $v0	# n = $vo, Move the user input to t0
	addi $t1, $0, 0	# i = 0
	addi $t2, $0, 0 # ans = 0, Starting case (n=0) is 0
	li $t3, 2	# we store two in $t3
loop:
   beq $t0, 0, END 		# if we reach 0 we stop
	div $t0, $t3		# divide $t0 with 2
	mfhi $t1		# store the remainder to $t1
   bne $t1, $0, ADD	 	# if the reminder is not 0 then we go to ADD
	sub $t0, $t0, 1 	# we reduce $t0
   j loop			# go back to loop
ADD:
	add $t2, $t2, $t0 	# add the odd number to the $t2
	sub $t0, $t0, 1 	# reduce $t0
   j loop			# go back
END:
	li $v0, 4
	la $a0, ans	# Loads the ans string
	syscall
	move $a0, $t2	# Loads the answer
	li $v0, 1
	syscall
	li $v0, 10	# Exiting
	syscall
