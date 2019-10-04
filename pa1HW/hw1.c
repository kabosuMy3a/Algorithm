#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int row_num = 0 ;
int col_num = 0 ;

void receiveInputChar(int inputArray[][51], int * rlabel, int * clabel);
void input2Grid(int inputArray[][51], int * rlabel, int * clabel);
void printOutput(int outputArray[][50]);

int main(int argc, char* args[]){
	int inputArray[51][51];
	int rlabel[51]; 
	int clabel[50];
	int outputArray[50][50];
	

	receiveInputChar(inputArray, rlabel, clabel);
	row_num--;

	input2Grid(inputArray,rlabel, clabel);
	
	int i, j ;
	int grid_num = row_num * col_num ;
	
	FILE * fp = fopen("./Z3Ncross","w");

	for(i=0; i<row_num ; i ++){
		for(j=0 ; j<col_num ; j++){
			fprintf(fp,"(declare-const r%dc%d Int)\n",i,j);
		}
	}	
	fprintf(fp,"\n(assert (and ");
	for(i=0 ; i<row_num ; i++){
		for(j=0 ; j <col_num ; j++){
			fprintf(fp, "(or (= r%dc%d 0) (= r%dc%d 1))",i,j,i,j); 
		}
	}
	fprintf(fp,"))\n");
		
	fprintf(fp,"\n(assert (and \n");
	for(j=0 ; j <col_num ; j++){
		fprintf(fp, "(= \n");
		fprintf(fp, "(+ ");
		for(i = 0 ; i < col_num ; i++){
			fprintf(fp, "(* %d r%dc%d)",inputArray[i][j],i,j);
		}
			fprintf(fp, ")");
			fprintf(fp, "%d",rlabel[j]);
			fprintf(fp, ")\n");

	}
	fprintf(fp,"))\n");
	
	fprintf(fp,"\n(assert (and \n");
	for(i=0 ; i <row_num ; i++){
		fprintf(fp, "(= \n");
		fprintf(fp, "(+ ");
		for(j=0 ; j <col_num ; j++){
			fprintf(fp, "(* %d (* (+ r%dc%d -1) -1))",inputArray[i][j],i,j);
		}
			fprintf(fp, ")");
			fprintf(fp, "%d",clabel[i]);
			fprintf(fp, ")\n");

	}
	fprintf(fp,"))\n");

	fprintf(fp, "\n(check-sat)\n");
	fprintf(fp, "(get-model)");
	fclose(fp);
	
	
	FILE * fin = popen("z3 ./Z3Ncross > ./rawNcross", "r");
	pclose(fin);

	FILE * rst = fopen("./rawNcross", "r");
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

void receiveInputChar(int inputArray[][51], int * rlabel, int * clabel){
	
	char * lines = 0x0;
	size_t buf = 0;

	while(getline(&lines, &buf, stdin)!=EOF){
		int len = strlen(lines);
		int coldex=0;
		int flagforClabel = 0; 
		for(int i = 0 ; i<len ; i++){
			if (coldex==51) break;
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
			else{
				continue;
			}
			if (row_num==0) rlabel[coldex++] = element ;
			else{			
				if(coldex < col_num){
					if(element >= 10){
						printf("Error: Each input except label should be less 10\n");
						return; 
					}
					inputArray[row_num-1][coldex++] = element;
				}else{
					if(flagforClabel==1){
						coldex++;
					}
					clabel[row_num-1] = element ;
					flagforClabel = 1;
					coldex++;
				}	
			}
		}
		if(row_num==0){
		       	col_num = coldex;
			coldex++;
		}
		if(row_num==50){
			row_num++; 
			break;
		}
		(coldex==col_num+1) ? row_num++ : 
			(coldex!=0) ? printf("please input correct grid, same column number before you enter\nthe number of column is %d, and you should add a label\n", col_num) : 0x0;
	}
}

void input2Grid(int inputArray[][51], int * rlabel, int * clabel){
	FILE *fip = fopen("./input2Grid.txt","w");
	for(int i= 0 ; i<col_num ; i++){
		fprintf(fip, "%d ", rlabel[i]);
	}
	fprintf(fip,"\n");
	for(int i=0 ; i< row_num ; i++){
		for(int j=0 ; j< col_num ; j++){
			fprintf(fip, "%d ", inputArray[i][j]);
		}
		fprintf(fip, "%d", clabel[i]) ; 
		fprintf(fip,"\n");
	}
	fclose(fip);
}

void printOutput(int outputArray[][50]){
	FILE * output = fopen("./parsedOutput.txt","w");
	for(int i=0 ; i< row_num ; i++){ 
		for(int j=0 ; j< col_num ; j++){
			printf("%d ", outputArray[i][j]);
			fprintf(output,"%d ", outputArray[i][j]);	
		}
		fprintf(output,"\n");
		printf("\n");
	}
	fclose(output);
}
