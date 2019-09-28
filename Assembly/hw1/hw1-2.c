#include <stdio.h>

int main(){

	int i, v[10];
	int min;
	int k= 0;
	for(i = 0 ; i< 10 ; i++){
		scanf("%d", &v[i]);
	}	
	min = v[0];
	for(i= 0 ; i<10 ; i++){
		if (v[i] < min){
			min = v[i];
			k= i;
		}
	}

	printf("%d=> %d",k,min);
	
	return 0;

}
