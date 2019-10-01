#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int row_num =1 ;
int col_num =1 ;
char Output[1001][1001];

void receiveInputChar(char inputArray[][1001]);
void input2Grid(char inputArray[][1001]);
void combinationUtil(int arr[], int data[], int start, int end,  
                     int index, int r, int k, int j, int N, int M, FILE *fp); 

void printCombination(int arr[], int n, int r, int k, int j, int N, int M, FILE *fp) 
{ 
    // A temporary array to store all combination one by one 
    int data[r]; 
  
    // Print all combination using temprary array 'data[]' 
    combinationUtil(arr, data, 0, n-1, 0, r, k, j, N, M, fp); 
} 
  
/* arr[]  ---> Input Array 
   data[] ---> Temporary array to store current combination 
   start & end ---> Staring and Ending indexes in arr[] 
   index  ---> Current index in data[] 
   r ---> Size of a combination to be printed */
void combinationUtil(int arr[], int data[], int start, int end, 
                     int index, int r, int k, int j, int N, int M, FILE *fp) 
{ 
    // Current combination is ready to be printed, print it
    int x[9]={0, 0, -1, -1, -1, 0, 1, 1, 1};
    int y[9]={0, -1, -1, 0, 1, 1, -1, 0, 1}; 
    

    // (or (= (+ p15 ) 1 )(= (+ p14 ) 1 )(= (+ p24 ) 1 )(= (+ p25 ) 1 ))

    if (index == r) {
        int isgood=1;
        int check[9]={0}; 
        for (int s=0; s<r; s++){
            check[data[s]]++;
        }

        for(int s=0; s<9; s++){
            if(check[s]!=0){
                if(!(k+x[s] <= N && k+x[s] >=1 && j+y[s] <=M && j+y[s] >= 1)){
                    isgood=0;
                }
            }
        }
        if(isgood){
             fprintf(fp, "(and ");  
         for(int m=0; m<9; m++){
             //If it is true
             if(check[m]!=0){
                 if(k+x[m] <= N && k+x[m] >=1 && j+y[m] <=M && j+y[m] >= 1){
                        fprintf(fp, "(= r%dc%d 1) ", k+x[m], j+y[m]);
                        // printf("(%d, %d)\n", i+x[m], j+y[m]);
                }
             }else{
                 if(k+x[m] <= N && k+x[m] >=1 && j+y[m] <=M && j+y[m] >= 1){
                        fprintf(fp, "(= r%dc%d 0) ", k+x[m], j+y[m]);
                        // printf("(%d, %d)\n", i+x[m], j+y[m]);
                }
            }
            
        }
        fprintf(fp, ")");
        }
        return; 
    } 
     
    // replace index with all possible elements. The condition 
    // "end-i+1 >= r-index" makes sure that including one element 
    // at index will make a combination with remaining elements 
    // at remaining positions 
  
    for (int i=start; i<=end && end-i+1 >= r-index; i++) 
    { 
        data[index] = arr[i]; 
        combinationUtil(arr, data, i+1, end, index+1, r, k, j, N, M, fp); 
    }
     
} 


void basicCondition(char arr[][1001], int N, int M, FILE *fp){
    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            fprintf(fp, "(declare-const r%dc%d Int)\n", i, j);
        }
    }

    fprintf(fp, "(assert (and ");
    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            fprintf(fp, "(and (> r%dc%d -1) (< r%dc%d 2))", i, j, i, j);
        }
    }
    fprintf(fp, "))\n");
    

    int ar[] = {0, 1, 2, 3, 4, 5, 6, 7, 8}; 
    int r; 
    int s = sizeof(ar)/sizeof(ar[0]); 
   
    //Q
    fprintf(fp, "(assert (and ");
    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            if(arr[i][j]!='?'){
                fprintf(fp, "(or ");
                r=arr[i][j]-'0';
                // r= the number of center
                // i, j = the x, y coordination 
                 printCombination(ar, s, r, i, j, N, M, fp); 
                fprintf(fp, ")");
            }
        }
    }
    fprintf(fp, "))\n");
}

void receiveInputChar(char inputArray[][1001]){
	
	char * lines = 0x0;
	size_t buf = 0;

	while(getline(&lines, &buf, stdin)!=EOF){
		int len = strlen(lines);
		int coldex=1;
		for(int i = 0 ; i<len ; i++){
			if(coldex==1001) break;
			char c = lines[i];
			if((c>=48 && c<= 57) || c==63){
				inputArray[row_num][coldex++] = c;
			}
		}
		if(row_num==1) col_num = coldex;
		if(row_num==1000){
			row_num++;
			break;
		}
		(coldex==col_num) ? row_num++ : 
			(coldex!=0) ? printf("please input correct grid, same column number before you enter\n") : 0x0;
	}
}

void input2Grid(char inputArray[][1001]){
	for(int i=1 ; i<= row_num ; i++){
		for(int j=1 ; j<= col_num ; j++){
			printf("%c ",inputArray[i][j]);	
		}
		printf("\n");
	}
}




int main(){
    char arr[1001][1001];
    int i, j, m, n;
    

    char buf[1001]={};


    receiveInputChar(arr);

    FILE * fp = fopen("./pa102/pixOutput0","w");
    int M=col_num-1;
    int N=row_num-1;

    basicCondition(arr, N, M, fp);

    fprintf(fp, "(check-sat)\n(get-model)\n");

    fclose(fp);


    char solution[5][1001][1001];
    char filename[100];

    for(int n=0; n<5; n++){

        sprintf(filename, "z3 ./pa102/pixOutput%d > ./pa102/pixZ3%d", n, n);
         FILE * rs = popen(filename, "r");
        char buff[128];
        fscanf(rs, "%s %s", buff, buff);
        while(!feof(rs)){
            fscanf(rs, "%s", buff);
        }
        pclose(rs);

        sprintf(filename, "./pa102/pixZ3%d", n);
         FILE *z3 = fopen(filename, "r");

            int q, w, e;

            char first[128];
            char b[128];
            char gi[128];
            char t[128];

    //Blow up the first and second line.
    //Check if there is solution.
    fscanf(z3, "%s %s", first, b);
    if(strcmp(first, "sat")!=0){
        printf("No Solution \n");
        fclose(z3);
        return 0;
    }

    for(int k=0; k < M*N; k++){
        fscanf(z3, "%s %s %s %s %s", b, gi, b, b, t);

	int gilen = strlen(gi);
	int ii, jj;
	for( ii = 0 ; ii<gilen ; ii++){
		if(gi[ii] == 'c') break;
	}
	char subs[128]; char subs2[128];
	for(jj = 1; jj <ii ; jj++){
		subs[jj-1] = gi[jj];
	}
	subs[jj-1]=0x0;
	q = atoi(subs);
	jj= ii+1;
	int dex =0;
	while(gi[jj]!=0x0){
		subs2[dex++] = gi[jj++];
	}
	subs2[dex] = 0x0;
	w= atoi(subs2);
	/*these above are for parsing ss to q,w */

        if (strcmp(t, "0)") != 0){
            Output[q][w]=1;
        }else{
            Output[q][w]=0;
        }
    }
       printf("<<Solution %d>>\n\n", n+1);
       for(int i=1; i<=N; i++){
           for(int j=1; j<=M; j++){
               printf("%d ", Output[i][j]);
           }
           printf("\n");
       }
       printf("\n\n");

    for(int i=1; i<=N; i++){
        for(int j=1; j<=M; j++){
            solution[n][i][j]=Output[i][j];
        }
    }

    sprintf(filename, "./pa102/pixOutput%d", n+1);
    FILE *next = fopen(filename, "w");

    basicCondition(arr, N, M, next);

    
    for(int i=0; i<=n; i++){
        fprintf(next, "(assert (not (and ");
        for(int j=1; j<=N; j++){
            for(int k=1; k<=M; k++){
                fprintf(next, "(= r%dc%d %d) ", j, k, solution[i][j][k]);
            }
        }
        fprintf(next, ")))\n");
    }

    fprintf(next, "(check-sat)\n(get-model)\n");

    fclose(next);
    
    }


   

    return 0;
}
