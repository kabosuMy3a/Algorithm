#21500800 HongSunGyu
#sum =0;
# for(i=0 ; i< 1000 ; i++)
#	sum = sum +i ;
#printf("sum= %d",sum);

	.data
msg: .asciiz "sum = "
sum: .word 0

	.text
	.globl main

main:
	addi $s0, $0, 1000

Loop:
	addi $t0, $t0, 1 #i
	beq $t0, $s0, Exit
	add $t1, $t1, $t0 #sum = sum+i
	j Loop

Exit:	
	sw $t1, sum

	li $v0, 4
	la $a0, msg
	syscall
	
	li $v0, 1
	lw $a0, sum
	syscall
	
	li $v0, 10
	syscall

	

