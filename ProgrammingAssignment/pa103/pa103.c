#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int row_num =0 ;
int col_num =0 ;

void receiveInputChar(int inputArray[][100]);
void input2Grid(int inputArray[][100]);
void printOutput(int outputArray[][100]);

int main(int argc, char* args[]){
	int inputArray[100][100];
	int outputArray[100][100];
	
	receiveInputChar(inputArray);
	input2Grid(inputArray);
	
	FILE * fp = fopen("./pa103/Z3mbrix","w");
	int i, j ;
	int grid_num = row_num * col_num ;
	
	for(i=0 ; i< row_num ; i++){
		for(j=0 ; j< col_num ; j++){
			fprintf(fp,"(declare-const r%dc%d Int)\n",i,j);
		}
	}
	fprintf(fp, "\n;Assign initial value\n");
	fprintf(fp, "(assert (and ");
	for(i = 0 ; i< row_num ; i++){
		for(j = 0 ; j< col_num ; j++){
			int elm = inputArray[i][j];
			if(elm!=0){
				fprintf(fp, "(= r%dc%d %d)",i,j,elm);
			}
		}
	}
	fprintf(fp, "))\n\n");
	
	fprintf(fp, ";Every cell never exceed number of grid\n");
	fprintf(fp, "(assert (and ");
	for(i=0 ; i< row_num ; i++){
		for(j=0; j< col_num; j++){
			fprintf(fp, "(and (>= r%dc%d 1) (<= r%dc%d %d))",i,j,i,j,grid_num);
		}	
	}
	fprintf(fp, "))\n\n");

	fprintf(fp, ";Every cell should have different value\n");
	fprintf(fp, "(assert (distinct ");
	for(i=0 ; i< row_num ; i++){
		for(j =0; j< col_num ; j++){
			fprintf(fp, "r%dc%d ",i,j);
		}
	}
	fprintf(fp, "))\n\n");
	
	fprintf(fp, "; x| (x-1 should exist in L||R||U||R)&& x+1 should exist in L||R||U||R\n");
	fprintf(fp, "; if x==1, only x+1 && if x==grid_max, only x-1\n");
	fprintf(fp, "; if x's index has 0 or row-1 or col-1 should be considered\n");

	fprintf(fp, "\n(check-sat)\n");
	fprintf(fp, "(get-model)");
	fclose(fp);
	
	FILE * fin = popen("z3 ./pa103/Z3mbrix > ./pa103/rawbrix", "r");
	char bp[128];
	fscanf(fin, "%s %s", bp, bp);
	while(!feof(fin)){
		fscanf(fin, "%s", bp);
	}
	pclose(fin);
	
	FILE * rst = fopen("./pa103/rawbrix", "r");
	char bury[128]; char gi[128]; char nv[128];

	fscanf(rst, "%s", bury);
	if(bury[0]=='u'){
		printf("No solution\n");
		pclose(rst);
		return 0;
	}else{
		fscanf(rst, "%s", bury);
		for(int k=0; k<grid_num ; k++){
			fscanf(rst, "%s %s %s %s %s", bury, gi, bury, bury, nv);
			int gridRow, gridCol, numVal ;
			int gilen = strlen(gi);
			for(i = 0; i< gilen ; i++){
				if(gi[i] =='c') break;
			}
			char subs[128]; char subs2[128]; char subs3[128];
			for(j = 1; j< i ; j++){
				subs[j-1] = gi[j] ;
			}
			subs[j-1]=0x0;
			gridRow= atoi(subs);
			j=i+1;
			int dex=0;
			while(gi[j]!=0x0){
				subs2[dex++] = gi[j++];
			}
			subs2[dex]=0x0;
			gridCol= atoi(subs2);
			
			j=0; dex=0;
			while(nv[j]!=')'){
				subs3[dex++] = nv[j++];
			}
			subs3[dex]=0x0;
			numVal = atoi(subs3);

			outputArray[gridRow][gridCol] = numVal;
		}
	}
	fclose(rst);
	printOutput(outputArray);
	return 0;
}

void receiveInputChar(int inputArray[][100]){
	
	char * lines = 0x0;
	size_t buf = 0;

	while(getline(&lines, &buf, stdin)!=EOF){
		int len = strlen(lines);
		int coldex=0;
		for(int i = 0 ; i<len ; i++){
			if (coldex==100) break;
			char c = lines[i];
			int element;
			if(c>= 48 && c<= 57){
				if(c==48){
					printf("ERROR: 0 can't be input\n");
					return;	
				}
				char num[10] ;
				num[0] = c;
				int j=0;
				while(lines[i+j+1]>=48 && lines[i+j+1]<= 57){
					j++;
					num[j] = lines[i+j];
				}
				num[j+1] = 0x0;
				i= i+j;
				element = atoi(num);
			}
			else if(c==63){
				element = 0;
			}
			else{
				continue;
			}		
			inputArray[row_num][coldex++] = element;
		}
		if(row_num==0) col_num = coldex;
		if(row_num==99){
			row_num++; 
			break;
		}
		(coldex==col_num) ? row_num++ : 
			(coldex!=0) ? printf("please input correct grid, same column number before you enter\n") : 0x0;
	}
}

void input2Grid(int inputArray[][100]){
	FILE *fip = fopen("./pa103/input2Grid.txt","w");
	for(int i=0 ; i< row_num ; i++){
		for(int j=0 ; j< col_num ; j++){
			fprintf(fip, "%d", inputArray[i][j]);
		}
		fprintf(fip,"\n");
	}
	fclose(fip);
}

void printOutput(int outputArray[][100]){
	FILE * output = fopen("./pa103/parsedOutput.txt","w");
	for(int i=0 ; i< row_num ; i++){
		for(int j=0 ; j< col_num ; j++){
			printf("%d ", outputArray[i][j]);
			fprintf(output,"%d", outputArray[i][j]);	
		}
		fprintf(output,"\n");
		printf("\n");
	}
	fclose(output);
}
