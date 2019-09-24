#include <stdio.h>

int main(int argc, char* args[]){
	
	char inputArray[9][9] ;
	int outputArray[9][9] ;
	//printf("%d,%d",'/','*'); =63,42
	for(int i=0 ; i< 9 ; i++){	
		for(int j =0 ; j< 9 ; j++){
			char c ;
			scanf("%c",&c);
			while(1) {
				if(c>=48 && c<=57) break;
				if(c==63) break;
				if(c==42) break;
				scanf("%c",&c);
			}
			inputArray[i][j]= c;
			//printf("%d",c);
		}
	}
	for(int i = 0 ; i < 9 ; i++){
		for(int j = 0; j< 9 ; j++){
			printf("%c ",inputArray[i][j]);
		}
		printf("\n");
	}
      	return 0;	

}



