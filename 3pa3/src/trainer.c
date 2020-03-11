#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <glib.h>
#include "../include/libstemmer.h"
#include <gmodule.h>

const int MSG_NUM = 5500;

void  
print_counter (gpointer key, gpointer value, gpointer userdata)  
{ 
        char * t = key ; 
        int * d = value ; 
 
        printf("(%s, %d)\n", t, *d) ; 
} 


void
prob_printer(gpointer key, gpointer value, gpointer userdata){
	char * t = key;
	double * d = value;
	FILE * f;
	f= fopen("./model.csv","aw");
	fprintf(f,"%s, %f\n", t, *d);
	fclose(f);
}

void
get_prob(gpointer key, gpointer value, gpointer userdata){

	char* t = key;
	int * d = value;
		
	
 	double * prob = (double *) malloc(sizeof(double));
	*prob = (double)*d/MSG_NUM ;
	if(*prob > 1){
		printf("WWWWWW");
		return;
	}	
	GHashTable * model = userdata;
	g_hash_table_insert(model, t,prob);
}	


int 
getStopWordList(char * stopWordList[]){

	FILE * f = fopen("../data/stopword.txt","r");
	char * line = 0x0;
	size_t n=0;
	int listnum =0;
	while(getline(&line, &n, f) >= 0){
		line[strlen(line)-1]=0x0;
		stopWordList[listnum++] = line;
		line=0x0;
	}
	fclose(f);
	return listnum;
}

char*
makeBetterWord(char * his){
//	char * nw = (char*) malloc(sizeof(char)* 50) ;
	char * nw = strdup(his);
	int i=0;
	int k=0;
	for( i ; his[i] ; ){
		if(isalpha(his[i]) ==0){
			int j = 1;
			while(isalpha(his[i+j]) == 0
				&& his[i+j] != 0x0 	
			) j++;
			i= i+j;
		}else{
			if(his[i]==0x0) nw[k]= 0x0;
			nw[k++] = tolower(his[i++]);
		}
	}
	nw[k] = 0x0;
	return nw;
}

int  
main ()  
{ 
        FILE * f = fopen("../data/train.negative.csv", "r") ; 
 
        GHashTable * counter = g_hash_table_new(g_str_hash, g_str_equal) ; 
 	GHashTable * nonModel = g_hash_table_new(g_str_hash, g_str_equal);
	
        char * line = 0x0 ; 
        int msg_num ;  
        size_t n = 0 ;

	struct sb_stemmer * stemmer;
	stemmer = sb_stemmer_new("english",0x0);

	char * stop_words[1000];
	int stop_words_num = 0 ;
	stop_words_num = getStopWordList(stop_words);

        while (getline(&line, &n, f) >= 0) { 
                char * t ; 
                char * _line = line ; 
		msg_num++;
		if(msg_num == MSG_NUM) break; 
		/*
		const char * redundantCheck[100] ;
		for(int i=0 ; i< 100 ; i++){
			redundantCheck[i] =0x0;
		}
		int rnum =0;*/
		
                for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) { 
			/*trim & lowercase*/
			t = makeBetterWord(t);
			/*delete basic stop words*/
			for(int i =0 ; i<stop_words_num ; i++){
				if(strcmp(stop_words[i],t)==0){
					t= 0x0;
					break;
				}
			}
			if(t==0x0) continue;
			/*use stemmer*/
			const char * st;
			st = sb_stemmer_stem(stemmer, t, strlen(t));
			/*redundant check*/	
			/*Isn't it really need?
			int redundantFlag=0;
			if(rnum==0) redundantCheck[rnum++] = st;
			else{
				for(int i =0 ; i<rnum ; i++){
					if(strcmp(st,redundantCheck[i])==0){
						printf("%s : %s\n",st,redundantCheck[i]);
						redundantFlag=1;
						break;
					}
					else{
						redundantCheck[rnum++] = strdup(st);
					}
				}
			}
			if(redundantFlag==1) continue;*/

			int * d ;
                        d = g_hash_table_lookup(counter, st) ; 
                        if (d == NULL) { 
                                d = malloc(sizeof(int)) ; 
                                *d = 1 ; 
                                g_hash_table_insert(counter, strdup(st), d) ; 
                        } 
                        else { 
                                *d = *d + 1 ; 
                        }
                } 
		
                free(_line) ; 
                line = 0x0 ; 
        } 
       	sb_stemmer_delete(stemmer);

        //g_hash_table_foreach(counter, print_counter, 0x0) ; 
	g_hash_table_foreach(counter, get_prob, nonModel);
	g_hash_table_foreach(nonModel,prob_printer,0x0);

        printf("worst: %d\n", *((int *) g_hash_table_lookup(counter, "suck"))) ; 
	fclose(f); 
}

