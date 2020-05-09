//This program works for Greedy, DP, and Branch and Bound soultions.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>

typedef struct item Item ;

//for keeping memory, don't store b/w
struct item{
	int benefit ;
	int weight ;
};

//Greedy, DP, and BB share the Test cases 
Item** items_10;
Item** items_100;
Item** items_500;
Item** items_1000;
Item** items_3000;
Item** items_5000;
Item** items_7000;
Item** items_9000;
Item** items_10000;
//Above global vars are Test Cases

Item * create_item(){
	Item * item = (Item*) malloc(sizeof(Item));
	item->benefit = rand() % 300 + 1;
	item->weight = rand()% 100 + 1 ;
	return item;
}

//for duplicate(deep copy)
Item * create_item_manual(int b, int w){
	Item * item = (Item*) malloc(sizeof(Item));
	item->benefit = b;
	item->weight = w;
	return item;
}

Item ** create_item_list(int size){
	Item ** Items = (Item**) malloc(sizeof(Item*) * size);
	for(int i = 0 ; i < size ; i++){
		Items[i] = create_item() ;
	}	
	return Items ;
}

//deep copy
Item ** dup_item_list(Item** origin, int size){
	Item ** Items = (Item**) malloc(sizeof(Item*) * size);
	for(int i = 0 ; i< size ; i++){
		Items[i] = create_item_manual(origin[i]->benefit,origin[i]->weight);
	}
	return Items ;
}

void delete_item_list(Item** items, int size){
	for(int i = 0 ; i < size ; i++){
		free(items[i]);
	}	
	free(items);
}

void print_item_list(Item** items, int size){
	for(int i = 0 ; i < size ; i++){
		Item * item = items[i]; 
		printf("%d | B: %d, W: %d, B/W: %.2f\n", i+1,item->benefit,
		    item->weight, (float)item->benefit/item->weight ); 
	}

}

//for use descendant qsort library
int static compare_descendant(const void * first, const void * second){

	const Item * one = *(Item**)first ;
	const Item * two = *(Item**)second ;

	float one_bpw = (float)one->benefit/ one->weight ;
	float two_bpw = (float)two->benefit/ two->weight ;
	
	if(one_bpw > two_bpw) return -1; 
	else if (one_bpw < two_bpw) return 1;
	else return 0;
}

//use qsort 
void qsort_desc_bpw(Item ** origin, int size){
	qsort((void*) origin, size, sizeof(Item*), &compare_descendant);
}

//duplicate the list and return sorted list 
Item** make_sorted_item_list(Item** items, int size){
	Item ** new_items = dup_item_list(items, size);
	qsort_desc_bpw(new_items, size);
	return new_items;
}

//print milisec and round nearest thousandth
int time_formatter(clock_t start, clock_t end){
	//int ms = (int)(end-start); //microseconds 
	int ms = (int)(end-start)/1000;
	if(ms >= 1000){
		ms = (ms+500)/1000 ;
		ms *= 1000 ;
	} //miliseconds
	return ms ;
} 

//for output txt file
typedef struct ms_and_max_benefit {
	int ms ;
	float max_benefit ;

} MSMB ;

MSMB * create_msmb(int ms, float mb){
	MSMB * msmb = (MSMB*) malloc(sizeof(MSMB));
	msmb->ms = ms;
	msmb->max_benefit = mb;
	return msmb ;
}

int item_num[9] = {10,100,500,1000,3000,5000,7000,9000,10000} ;
MSMB * greedy_msmb[9] ;
MSMB * DP_msmb[9] ;
MSMB * BB_msmb[9] ;

//call after make txt file
void free_solutions(){
	for(int i = 0 ; i< 9 ; i++){
		if(greedy_msmb[i] != 0x0) free(greedy_msmb[i]); 
	}
	for(int i = 0 ; i< 9 ; i++){
		if(DP_msmb[i] != 0x0) free(DP_msmb[i]); 
	}
	for(int i = 0 ; i< 9 ; i++){
		if(BB_msmb[i] != 0x0) free(BB_msmb[i]); 
	}
}

//If calculation is not end, X is marked 
void make_output_file(){
	FILE * fp ;
	fp = fopen("output.txt","w") ;
	fprintf(fp,"Number of| Processing time in miliseconds/ Maximum benefit value\n");
	fprintf(fp,"  Items  |   Greedy   |    D.P.    |   B&B.\n");
       	for(int i = 0 ; i< 9 ; i++){
		fprintf(fp,"   %d    | ", item_num[i]);
		if(greedy_msmb[i] != 0x0)
			fprintf(fp,"%d / %.2f | ", greedy_msmb[i]->ms, greedy_msmb[i]->max_benefit);
		else 
			fprintf(fp,"X / X | " );
		if(DP_msmb[i] != 0x0)
			fprintf(fp,"%d / %.0f | ", DP_msmb[i]->ms, DP_msmb[i]->max_benefit);
		else 
			fprintf(fp,"X / X | " );	
		if(BB_msmb[i] != 0x0)
			fprintf(fp,"%d / %0.f\n", BB_msmb[i]->ms, BB_msmb[i]->max_benefit);
		else 
			fprintf(fp,"X / X\n" );	
	}
	fclose(fp);
	free_solutions();
}

//Greedy Algorithm implementation
MSMB* greedy_solving(Item** items, int size){
	clock_t start = clock();
	Item ** sorted_items = make_sorted_item_list(items,size);
	int W = size * 40 ;
	int B = 0 ;
       	float lastB = 0.0 ; //Output of greedy is float
	for(int i = 0 ; i< size ; i++){
		Item * item = sorted_items[i] ;
		W -= item->weight ;
		if(W > 0 ){
			B += item->benefit ;		
		} else {
			lastB += (float)B ;
			float bpw = (float)item->benefit/item->weight;
			lastB += bpw *(W+ item->weight);
			break;		
		}
		if(i == size-1) lastB = (float)B ;
	}
	delete_item_list(sorted_items, size);	
	int pt = time_formatter(start, clock());
	MSMB * msmb = create_msmb(pt, lastB);	
	return msmb ;
}

//DP Algorithm implementation

MSMB * DP_solving(Item** items, int size){
	clock_t start = clock();
	float lastB = 0 ;
	
	int pt = time_formatter(start, clock());
	MSMB * msmb = create_msmb(pt, lastB);
	return msmb ;
}

//BB Algorithm implementation

MSMB * BB_solving(Item** items, int size){
	clock_t start = clock();
	float lastB = 0 ;
	
	int pt = time_formatter(start, clock());
	MSMB * msmb = create_msmb(pt, lastB);
	return msmb ;
}

void greedy_solver(){
	greedy_msmb[0] = greedy_solving(items_10, 10 );
	greedy_msmb[1] = greedy_solving(items_100, 100 );
	greedy_msmb[2] = greedy_solving(items_500, 500 );
	greedy_msmb[3] = greedy_solving(items_1000, 1000 );
	greedy_msmb[4] = greedy_solving(items_3000, 3000 );
	greedy_msmb[5] = greedy_solving(items_5000, 5000 );
	greedy_msmb[6] = greedy_solving(items_7000, 7000 );
	greedy_msmb[7] = greedy_solving(items_9000, 9000 );
	greedy_msmb[8] = greedy_solving(items_10000, 10000 );
}

void DP_solver(){
	DP_msmb[0] = DP_solving(items_10, 10 );
	DP_msmb[1] = DP_solving(items_100, 100 );
	DP_msmb[2] = DP_solving(items_500, 500 );
	DP_msmb[3] = DP_solving(items_1000, 1000 );
	DP_msmb[4] = DP_solving(items_3000, 3000 );
	DP_msmb[5] = DP_solving(items_5000, 5000 );
	DP_msmb[6] = DP_solving(items_7000, 7000 );
	DP_msmb[7] = DP_solving(items_9000, 9000 );
	DP_msmb[8] = DP_solving(items_10000, 10000 );
}

void BB_solver(){
	BB_msmb[0] = BB_solving(items_10, 10 );
	BB_msmb[1] = BB_solving(items_100, 100 );
	BB_msmb[2] = BB_solving(items_500, 500 );
	BB_msmb[3] = BB_solving(items_1000, 1000 );
	BB_msmb[4] = BB_solving(items_3000, 3000 );
	BB_msmb[5] = BB_solving(items_5000, 5000 );
	BB_msmb[6] = BB_solving(items_7000, 7000 );
	BB_msmb[7] = BB_solving(items_9000, 9000 );
	BB_msmb[8] = BB_solving(items_10000, 10000 );
}

void create_test_case(){
	items_10 = create_item_list(10);
	items_100 = create_item_list(100);
	items_500 = create_item_list(500);
	items_1000 = create_item_list(1000);
	items_3000 = create_item_list(3000);
	items_5000 = create_item_list(5000);
	items_7000 = create_item_list(7000);
	items_9000 = create_item_list(9000);
	items_10000 = create_item_list(10000);

}

void delete_test_case(){
	delete_item_list(items_10, 10);
	delete_item_list(items_100, 100);
	delete_item_list(items_500, 500);
	delete_item_list(items_1000, 1000);
	delete_item_list(items_3000, 3000);
	delete_item_list(items_5000, 5000);
	delete_item_list(items_7000, 7000);
	delete_item_list(items_9000, 9000);
	delete_item_list(items_10000,10000);
}

void dispose_program(){
	make_output_file();
	delete_test_case();
}

//Timeout function 
void sigalrm_handler(int sig){
	dispose_program();
	exit(0);
}

int main(){
	srand(time(0x0));
	//This program gurantee the total exucution time is not over an hour. 
	signal(SIGALRM, sigalrm_handler);
	alarm(3600);
	create_test_case();
	greedy_solver();
	DP_solver();
	BB_solver();
	dispose_program();
	return 0;
}
