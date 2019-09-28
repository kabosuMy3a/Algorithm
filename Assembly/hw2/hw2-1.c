#include <stdio.h>


int accum(int final){
	int sum=0;
	for(int I=1; I <= final ; I = I+1 ) sum = sum+I;
	return sum;
}

int accum_range(int a, int b){
	return accum(b) - accum(a);
}
void main(){
	int i, sum, begin, end;
	for(i=0 ; i<5 ; i++){
		scanf("%d", &begin);
		scanf("%d", &end);
		sum = accum_range(begin, end);
		printf("sum[%d: %d] = %d\n", begin, end, sum);
	}
}
