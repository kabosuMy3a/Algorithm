#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int row_num =0 ;
int col_num =0 ;
void receiveInputChar(char inputArray[][1000]);
void input2Grid(char inputArray[][1000]);

int main(int argc, char* args[]){
	char inputArray[1000][1000];
	int outputArray[1000][1000];
	
	receiveInputChar(inputArray);
	input2Grid(inputArray);

	return 0;
}

void receiveInputChar(char inputArray[][1000]){
	
	char * lines = 0x0;
	size_t buf = 0;

	while(getline(&lines, &buf, stdin)!=EOF){
		int len = strlen(lines);
		int coldex=0;
		for(int i = 0 ; i<len ; i++){
			char c = lines[i];
			if((c>=48 && c<= 57) || c==63){
				inputArray[row_num][coldex++] = c;
			}
		}
		if(row_num==0) col_num = coldex;	
		(coldex==col_num) ? row_num++ : 
			printf("please input correct grid, same column number before you enter\n");
			//가장자리 처리 제대로 안하면 쓰레기값 입력될 수 있음. 일단 받았기 때문.
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
