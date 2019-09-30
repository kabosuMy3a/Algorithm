#include <stdio.h>
#include <stdlib.h>

int receiveInputChar(char inputArray[][9]);
void input2Grid(char inputArray[][9]);
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
	input2Grid(inputArray);
	
	FILE * fp = fopen("./pa101/Z3udoku","w");
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
	fclose(fp);

	FILE * fin = popen("z3 ./pa101/Z3udoku > ./pa101/rawdoku", "r");
	char buf[128] ;
	fscanf(fin, "%s %s", buf, buf);
	while(!feof(fin)){
		fscanf(fin, "%s", buf) ; 
	}
	pclose(fin);

	FILE * rst = fopen("./pa101/rawdoku","r");
	char bury[128] ;
	char gi[128] ;
	char nv[128];

	fscanf(rst, "%s", bury);
	if(bury[0]=='u'){
		/*
		printf("%s\n",bury);
		while(1){
			fscanf(rst,"%s",bury); 
			if(!feof(rst)) printf("%s ",bury);
			else break;
		}
		printf("\n");*/
		printf("No Solution\n");
		pclose(rst);
		return 0;
	}
	else{
		fscanf(rst, "%s",bury);
		for(int k =0 ; k<81 ; k++){	
			fscanf(rst,"%s %s %s %s %s", bury, gi, bury, bury, nv);
			
			int gridRow, gridCol, numVal;
			char rt[2], ct[2], nt[2]  ;
			rt[0]= gi[1]; rt[1]= 0x0;
			ct[0]= gi[2]; ct[1]= 0x0;
			nt[0]= nv[0]; nt[1]= 0x0;
			gridRow= atoi(rt);
			gridCol= atoi(ct);
			numVal= atoi(nt);

			outputArray[gridRow][gridCol] = numVal;
		}
	}
	pclose(rst);

	printOutput(outputArray);

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
void input2Grid(char inputArray[][9]){
	FILE * fip = fopen("./pa101/input2Grid.txt","w");

	for(int i = 0 ; i < 9 ; i++){
		for(int j = 0; j< 9 ; j++){
			fprintf(fip, "%c ",inputArray[i][j]);
		}
		fprintf(fip, "\n");
	}
	fclose(fip);
}
void printOutput(int outputArray[][9]){
	FILE * output = fopen("./pa101/parsedOutput.txt","w");
	for(int i = 0 ; i < 9 ; i++){
		for(int j = 0; j< 9 ; j++){
			fprintf(output,"%d ",outputArray[i][j]);
			printf("%d ",outputArray[i][j]);
		}
		fprintf(output,"\n");
		printf("\n");
	}
	fclose(output);
}
