#21500800 HongSunGyu
#	int i, v[10];
#	int min;
#	int k= 0;
#	for(i = 0 ; i< 10 ; i++){
#		scanf("%d", &v[i]);
#	}	
#	min = v[0];
#	for(i= 0 ; i<10 ; i++){
#		if (v[i] < min){
#			min = v[i];
#			k= i;
#		}
#	}
#	printf("%d=> %d",k,min);

	.data
print: .asciiz "=>" 
min: .word 0
input: .word 0:10
index: .word 0

	.text
	.globl main

main:
	addi $s0, $0, 10
	la $s1, input
	move $s2, $s1 #input differed by loop
Lovp:	
	
	beq $t0, $s0, Second
	
	li $v0, 5
	syscall
	move $t1, $v0
	sw $t1, ($s2)
	
	addi $t0, $t0, 1
	addi $s2, $s2, 4

	j Lovp

Second:
	move $s2, $s1
	li $t0, 0
	la $s3, min
	la $s4, index
	lw $t3, ($s1)
	sw $t3, ($s3)
Loop:
	beq $t0, $s0, Exit
	lw $t5, ($s2)
	lw $t6, ($s3)
	slt $t3, $t5, $t6
	bne $t3, 0, newMin
	j finally
newMin: 
	add $t4, $0, $t0
	sw $t4, ($s4)
	lw $t5 ,($s2)
	sw $t5 ,($s3)
	j finally	

finally:
	addi $t0, $t0, 1
	addi $s2, $s2, 4

	j Loop	
Exit:
	li $v0, 1
	lw $a0, index
	syscall

	li $v0, 4
	la $a0, print
	syscall
	
	li $v0, 1
	lw $a0, min
	syscall

	li $v0, 10
	syscall
