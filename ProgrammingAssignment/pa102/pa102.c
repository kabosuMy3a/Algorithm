#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int row_num =0 ;
int col_num =0 ;
void receiveInputChar(char inputArray[][1000]);
void input2Grid(char inputArray[][1000]);
void printOutput(int outputArray[][1000]);

int main(int argc, char* args[]){
	char inputArray[1000][1000];
	int outputArray[1000][1000];
	
	receiveInputChar(inputArray);
	input2Grid(inputArray);
	
	FILE * fp = fopen("./pa102/Z3apix","w");
	int i, j ;
	for(i=0 ; i< row_num ; i++){
		for(j=0 ; j< col_num ; j++){
			fprintf(fp,"(declare-const a%d%d Int)\n",i,j);
		}
	}
	fprintf(fp, "\n;all x| x= 0 or x= 1, 0 is white, 1 is black\n");
	fprintf(fp, "(assert (and ");
	for(i = 0 ; i < row_num ; i++){
		for(j=0 ; j < col_num ; j++){
			fprintf(fp, "(or (= a%d%d 0) (= a%d%d 1))",i,j,i,j);
		}
	}
	fprintf(fp, "))\n\n");


	fprintf(fp, "(check-sat)\n");
	fprintf(fp, "(get-model)");
	fclose(fp);
	

	//printOutput(outputArray);
	return 0;
}

void receiveInputChar(char inputArray[][1000]){
	
	char * lines = 0x0;
	size_t buf = 0;

	while(getline(&lines, &buf, stdin)!=EOF){
		int len = strlen(lines);
		int coldex=0;
		for(int i = 0 ; i<len ; i++){
			//pa103에서는 valid 다르게 받아야 함. 10이상의 숫자도 받기 때문. 
			char c = lines[i];
			if((c>=48 && c<= 57) || c==63){
				inputArray[row_num][coldex++] = c;
			}
		}
		if(row_num==0) col_num = coldex;
		(coldex==col_num) ? row_num++ : 
			(coldex!=0) ? printf("please input correct grid, same column number before you enter\n") : 0x0;
			//가장자리 처리 제대로 안하면 쓰레기값 생길 수 있음 배열 넘겨서도 받기 때문.
	}
}

void input2Grid(char inputArray[][1000]){
	for(int i=0 ; i< row_num ; i++){
		for(int j=0 ; j< col_num ; j++){
			printf("%c ",inputArray[i][j]);	
		}
		printf("\n");
	}
}

void printOutput(int outputArray[][1000]){
	for(int i=0 ; i< row_num ; i++){
		for(int j=0 ; j< col_num ; j++){
			printf("%c ", outputArray[i][j]);	
		}
		printf("\n");
	}
}
