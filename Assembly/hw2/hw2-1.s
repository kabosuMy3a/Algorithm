#int accum(int final){
#	int sum=0;
#	for(int I=1; I <= final ; I = I+1 ) sum = sum+I;
#	return sum;
#}
#
#int accum_range(int a, int b){
#	return accum(b) - accum(a);
#}
#void main(){
#	int i, sum, begin, end;
#	for(i=0 ; i<5 ; i++){
#		scanf("%d", &begin);
#		scanf("%d", &end);
#		sum = accum_range(begin, end);
#		printf("sum[%d: %d] = %d\n", begin, end, sum);
#	}
#}
#21500800 HongSungyu

	.data
msg1: .asciiz "sum["
msg2: .asciiz ": "
msg3: .asciiz "] = "
msg4: .asciiz "\n"

	.text
	.globl main

main: 
	addi $s0, $0, 0

mainLoop:	
	slti $t0, $s0, 5
	beq $t0, $0, Exit

	li $v0, 5
	syscall		
	move $s1, $v0
	
	li $v0, 5
	syscall
	move $s2, $v0
	
	move $a0, $s1
	move $a1, $s2
	jal accum_range
	move $s3 $v0

#print
	li $v0, 4
	la $a0, msg1
	syscall
	
	li $v0, 1
	move $a0, $s1
	syscall

	li $v0, 4
	la $a0, msg2
	syscall

	li $v0, 1
	move $a0, $s2
	syscall
	
	li $v0, 4
	la $a0, msg3
	syscall

	li $v0, 1
	move $a0, $s3
	syscall	
	
	li $v0, 4
	la $a0, msg4
	syscall
	
	addi $s0, $s0, 1
	j mainLoop

accum_range:
	addi $sp, $sp, -12
	sw $ra, 8($sp)
	sw $s1, 4($sp)
	sw $s2, 0($sp)

	move $s1, $a1
	
	jal accum
	move $s2, $v0
	
	move $a0, $s1
	jal accum
	
	sub $v0, $v0, $s2
	
	lw $s2, 0($sp)
	lw $s1, 4($sp) 
	lw $ra, 8($sp)
	addi $sp, $sp, 12
	j $ra

accum:
	li $t0, 0 #sum
	li $t1, 1 #I
	addi $a0, $a0, 1

accumLp:
	slt $t2, $t1, $a0			 	
	beq $t2, 0, acrt 
	add $t0, $t0, $t1	
	addi $t1, $t1, 1
	j accumLp

acrt:	move $v0, $t0
	j $ra
	
Exit : li $v0, 10
	syscall 

