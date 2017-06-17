# Exercise1 is used in assignments 1 and 2 
 .data 0x10000000
	Number1:
	.align 2
	.word 18 #Start data at designated hex location. Number is the number to undergo factorial.
 .text 0x00400000
 .globl main 

main:
	lw $10, Number1($0) # Load number into register 10
	ori $11, $0, 1
	ori $9, $0, 1  # both ORIs load "1" into register 11 and 9, respectively
	# The following is the binary: (op->) 001101 (rs->) 00000 (rt->) 01001 (const->) 0000000000000001
#compute the factorial of Number ($10)!

factloop:
	bge $11, $10, factexit #Once reg10 hits 1 (the value in reg11), leave the loop
	mul $9, $10, $9 #Reg 9 holds the factorial by use of multiplication
	sub $10, $10, 1 #Decrement 10 by 1, to procede with the factorial
	j factloop # Keep looping until bge condition met

factexit:
	#the computation of the factorial is over
	#Is the result in $9 correct? The result in $9 is in hexadecimal form

 	li $2, 10 #Loads "10" into reg 2 in order to give the syscall command "exit"
	syscall


