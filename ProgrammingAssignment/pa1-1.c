#include <stdio.h>

int receiveInputChar(char inputArray[][9]);
void testInput(char inputArray[][9]);
void printOutput(int outputArray[][9]);


int main(int argc, char* args[]){
	
	char inputArray[9][9] ;
	int outputArray[9][9] ;
	//printf("%d,%d",'?','*'); =63,42
	int num_of_star ;
	
	num_of_star = receiveInputChar(inputArray);
	if(num_of_star > 9){
		printf("Star should not be more than 9\n");
		return -1;
	}
	testInput(inputArray);
	
	FILE * fp = fopen("Z3udoku","w");
	int i,j ;

	for(i = 0 ; i< 9 ; i++){
		for(j= 0 ; j<9 ; j++){
			fprintf(fp, "(declare-const a%d%d Int)\n", i,j);
		}
	}
	fprintf(fp, "\n;all x| 0<x<10)\n");
	fprintf(fp, "(assert (and ");
	for(i= 0 ; i< 9 ; i++){
		for(j=0 ; j< 9 ; j++){
			fprintf(fp, "(and (> a%d%d 0) (< a%d%d 10))",i,j,i,j);
		}
	}
	fprintf(fp, "))\n\n");
	//assign initial value and find star
	int index_of_star[10];
	int index_of_index= 0;

	fprintf(fp, ";assign the intial value\n");
	fprintf(fp, "(assert (and ");
	for(i= 0 ; i <9 ; i++){
		for(j=0 ; j<9 ; j++){
			char c = inputArray[i][j];
			if(c>48 && c<=57){
				fprintf(fp, "(= a%d%d %c)",i,j,c); 
			}
			if(c==42){
				index_of_star[index_of_index] = i*10+j;
				index_of_index++;
			}	
		}
	}
	fprintf(fp, "))\n\n");
	
	fprintf(fp, ";each row has disctinct value\n");
	for(i =0 ; i < 9 ; i++){
		fprintf(fp, "(assert (distinct ");
		for(int j=0 ; j<9; j++){
			fprintf(fp, "a%d%d ", i,j); 
		}
		fprintf(fp, "))\n");
	}
	
	fprintf(fp, "\n;each col has disctinct value\n");
	for(j =0 ; j < 9 ; j++){
		fprintf(fp, "(assert (distinct ");
		for(int i=0 ; i<9; i++){
			fprintf(fp, "a%d%d ", i,j); 
		}
		fprintf(fp, "))\n");
	}
	
	fprintf(fp, "\n;3x3 grid has disctinct value\n");
	for(int m = 0 ; m <3 ; m++){
		for(int n= 0 ; n< 3 ; n ++){
			fprintf(fp, "(assert (distinct ");
			for( i=0; i <3 ; i++){
				for(j=0; j<3 ; j++){
					fprintf(fp,"a%d%d ",3*m+i,3*n+j);
				}
			}
			fprintf(fp,"))\n");
		}	
	}

	fprintf(fp, "\n;where * is given should have different value\n");
	if(index_of_index >1){ 
		fprintf(fp, "(assert (distinct ");
		for(i=0 ; i < index_of_index ; i++){
			int star = index_of_star[i] ;	
			if(star < 10){
				fprintf(fp, "a%d%d ",0,star);
			}
			else{
				fprintf(fp, "a%d ",star);
			}
		}
		fprintf(fp, "))\n");
	}



	fprintf(fp,"\n(check-sat)\n(get-model)\n");
	//printOutput(char outputArray);
	fclose(fp);

	return 0;
}


int receiveInputChar(char inputArray[][9]){
	int num_of_star =0;
	for(int i=0 ; i< 9 ; i++){	
		for(int j =0 ; j< 9 ; j++){
			char c ;
			scanf("%c",&c);
			while(1) {
				if(c>48 && c<=57) break;
				if(c==63) break;
				if(c==42){
					num_of_star++;
				       	break;
				}
				scanf("%c",&c);
			}
			inputArray[i][j]= c;
			//printf("%d",c);
		}
	}
	return num_of_star;
}
void testInput(char inputArray[][9]){
	for(int i = 0 ; i < 9 ; i++){
		for(int j = 0; j< 9 ; j++){
			printf("%c ",inputArray[i][j]);
		}
		printf("\n");
	}
}
void printOutput(int outputArray[][9]){
	for(int i = 0 ; i < 9 ; i++){
		for(int j = 0; j< 9 ; j++){
			printf("%c ",outputArray[i][j]);
		}
		printf("\n");
	}
}
