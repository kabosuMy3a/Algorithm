#int data[10] = {10, -2, 5, 22 ,99, 0 ,-5, 9, 30, 7};
#void bubble_sort(int a[], int size);
#void main(){
#	bubble_sort(data,10);
#	for(int i=0 ; i<10; i++) printf("%d: %d\n",i, data[i]);
#}
#void bubble_sort(int a[], int size){
#	int j, temp;
#	if(size>0){
#		for(j=0; j<size-1; j++){
#			if(a[j] > a[j+1]){
#				temp=a[j];
#				a[j]=a[j+1];
#				a[j+1]=temp;
#			}
#		}
#		bubble_sort(a,size-1);
#	}
#}
#21500800 HongSungyu

	.data
input: .word 10, -2, 5, 22, 99, 0, -5, 9, 30, 7
msg1: .asciiz ": "
msg2: .asciiz "\n"

	.text
	.globl main

main:
	la $s1, input
	move $s2, $s1
	li $s0, 0		
mainLoop:	
	slti $t0, $s0, 10
	beq $t0 $0, Exit

	move $a0, $s1
	li $a1, 10
	jal bubble_sort
	
#print
	li $v0, 1
	move $a0, $s0
	syscall

	li $v0, 4
	la $a0, msg1
	syscall

	li $v0, 1
	lw $a0, ($s2)
	syscall

	li $v0, 4
	la $a0, msg2
	syscall
#lpctrl
	addi $s0, $s0, 1
	addi $s2, $s2, 4
	j mainLoop

bubble_sort:
	addi $sp, $sp, -12
	sw $s1, 8($sp)
	sw $s2, 4($sp)
	sw $ra, 0($sp)

	slt $t0, $0, $a1
	bne $t0, $0, more
	addi $sp, $sp, 12
	j $ra
more: 
	addi $t2, $a1, -1
	li $t1, 0
	move $s2, $s1
sort: 
	slt $t0, $t1, $t2	
	beq $t0, $0, recursion
	lw $t3, 0($s2)
	lw $t4, 4($s2)
	slt $t0, $t4, $t3
	bne $t0, $0, do
	j finally
do:
	move $t5, $t3
	move $t3, $t4
	move $t4, $t5
	sw $t3, 0($s2)
	sw $t4, 4($s2)
		
finally:
	addi $s2, $s2, 4
	addi $t1, $t1, 1	
	j sort

recursion:
	addi $a1, $a1,-1
	jal bubble_sort 
	
	lw $ra, 0($sp)
	lw $s2, 4($sp)
	lw $s1, 8($sp)
	addi $sp, $sp, 12
	j $ra

Exit:
	li $v0, 10
	syscall
