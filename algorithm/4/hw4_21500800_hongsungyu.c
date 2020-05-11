// This program works for Greedy, DP, and Branch and Bound soultions.
// Target OS of this program is linux. it has dependency.
// unistd, and signal which is one of linux system calls are used.
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <assert.h>

typedef struct item Item ;

//for keeping memory, b/w is not stored
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
			fprintf(fp,"%d / %.0f | ", greedy_msmb[i]->ms, greedy_msmb[i]->max_benefit);
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

void store(Item * item, int * from, int * to, int w){
	int wi = item->weight ;
	int bi = item->benefit ;
	if(wi <= w){
		if(bi+ from[w-wi] > from[w]) /* input is better */{
			to[w] = bi + from[w-wi];
		} else {
			to[w] = from[w] ;
		}		
	} else {
		to[w] = from[w] ; 
	}
}

MSMB * DP_solving(Item** items, int size){
	clock_t start = clock();
	int MAX_W = size * 40 ;
	int first[MAX_W+1] ; //index [0 to MAX_W]
	int second[MAX_W+1] ; // index[MAX_W] is a soultion
	for(int i = 0; i <= MAX_W ; i++){
		first[i] = 0 ;
		second[i] = 0 ;
	}
	int store_guide = 0; 
	for(int i = 1 ; i <= size ; i ++){
		Item * item = items[i-1] ;
		for(int w = 1 ; w <= MAX_W ; w++){
			if(store_guide ==0) /*first to second*/ {
				store(item, first, second, w);
			} else /*second to first */{
				store(item, second, first, w);
			}
		}	
		store_guide ^= 0x01 ;
	}

	float lastB = (store_guide == 0) 
		? (float)first[MAX_W] 
		: (float)second[MAX_W] ;

	int pt = time_formatter(start, clock());
	MSMB * msmb = create_msmb(pt, lastB);
	return msmb ;
}

// BB Algorithm implementation

// Priority Max queue which is implemented by heap is used.
typedef struct element Element ;
typedef struct pqueue Pqueue ;

struct element{
	int item_index ; // 0 to size-1
	int ub ; // Benefit up to 
	int uw ; // Weight up to
	int bound ; //bound is key for pqueue, decimal point of b/w is abanadoned
};

Element * create_null_element(){
	Element * element = (Element*) malloc(sizeof(Element));
	element->item_index = 0;
	element->ub = 0;
	element->uw = 0;
	element->bound = 0;
	return element ;
}

Element * create_element(int index, int ub, int uw, int bound){
	Element * element = (Element*) malloc(sizeof(Element));
	element->item_index = index;
	element->ub = ub;
	element->uw = uw;
	element->bound = bound;
	return element;
}

void free_element(Element* element){
	assert(element!=0x0);
	free(element);
}

Element * copy_element(Element * source){
	return create_element(source->item_index, source->ub, source->uw, source->bound);
}

void swap_element(Element * A, Element * B){
	Element * temp = create_null_element();
	temp->item_index = A->item_index;
	temp->ub = A->ub;
	temp->uw = A->uw;
	temp->bound = A->bound;
	A->item_index = B->item_index;
	A->ub = B->ub;
	A->uw = B->uw;
	A->bound = B->bound;
	B->item_index = temp->item_index;
	B->ub = temp->ub;
	B->uw = temp->uw;
	B->bound = temp->bound;
	free_element(temp);
}

void print_element(Element * element){
	printf("[ix: %d ub: %d uw: %d b: %d]",
			element->item_index,
			element->ub,
			element->uw,
			element->bound);
}
 

struct pqueue{
	int index ; 
	int capacity ;
	Element ** element ;
};

Pqueue * create_pqueue(int item_size){
	Pqueue * pqueue = (Pqueue*) malloc(sizeof(Pqueue));
	assert(pqueue!=0x0);
	pqueue->index = 0;
	pqueue->capacity = item_size * 100 ; 
	pqueue->element = (Element**) malloc(sizeof(Element *) * pqueue->capacity);
	return pqueue ;
}

void free_pqueue(Pqueue* pqueue){
	for(int i= 1 ; i <= pqueue->index ; i++){
		free_element(pqueue->element[i]);
	}
	free(pqueue);
}

int isfull(Pqueue* pqueue){
	return (pqueue->index == pqueue->capacity) ? 1 : 0 ;
}


int extension_log = 0 ; //debug
void pqueue_extension(Pqueue* pqueue){
	extension_log++; //debug
	pqueue->capacity *= 2;
	pqueue->element = (Element**) realloc(pqueue->element,
			sizeof(Element*) * (pqueue->capacity));
}

void bottomup_balancing(Pqueue* pqueue, int index){
	int new_index = index;
	while(new_index > 1){
		int old_index = new_index;
		new_index  = new_index/2 ;
		if(pqueue->element[old_index]->bound > pqueue->element[new_index]-> bound){
			swap_element(pqueue->element[old_index], pqueue->element[new_index]);
		} else break;
	}
}

void pqueue_insert(Pqueue* pqueue, Element * element){
	if(isfull(pqueue)==1) pqueue_extension(pqueue);
	pqueue->element[++pqueue->index] = element;
	bottomup_balancing(pqueue, pqueue->index);
}

void topdown_balancing(Pqueue* pqueue, int index){
	int new_index = index ;
	while(1){
		int old_index = new_index ;
		new_index *= 2;
		if(new_index > pqueue->index) break;
		else if(new_index == pqueue->index){
		} else {
			if(pqueue->element[new_index]->bound
				       	< pqueue->element[new_index+1]->bound)
				new_index += 1;		
		}
		if(pqueue->element[old_index]->bound < pqueue->element[new_index]->bound){
			swap_element(pqueue->element[old_index], pqueue->element[new_index]);
		} else break;
	}
}

Element * pqueue_delete(Pqueue* pqueue){
	assert(pqueue->index !=0);
	swap_element(pqueue->element[1], pqueue->element[pqueue->index]);
	
	Element * popped = copy_element(pqueue->element[pqueue->index]);
	free(pqueue->element[pqueue->index]);
	pqueue->index -= 1;
	topdown_balancing(pqueue, 1);
	return popped ;
}

int pqueue_peek_bound(Pqueue* pqueue){
	if(pqueue->index == 0) return -1 ;
	int peek = pqueue->element[1]->bound ;
	return peek;
}

void pqueue_tree_printer(Pqueue* pqueue){
	if(pqueue->index==0){
		printf("This pqueue is empty\n");
		return;
	}
	int timing = 1; int growth = 2;
	for(int i =1 ; i<=pqueue->index ; i++){
		Element * element = pqueue->element[i];
		if(i==timing+1){
			printf("\n");
			timing+= growth;
			growth *= 2;
		}
		print_element(element);
	}
	printf("\n");
}

void calculate_bound(Item** items, int size, Element* element){
	int tot_weight = 0;
	int idx = element->item_index ;
	int W = size * 40 ;
	int bound = 0 ;
	int i = 0 ; int j = 0;
	
	tot_weight += element-> uw ;
	for(i = idx+1 ; i < size ; i++){
		if(tot_weight+items[i]->weight > W) break;
		else tot_weight += items[i]->weight ;
	}	
	
	bound += element->ub ;
	for(j = idx+1 ; j <= i-1 ; j++){
		bound += items[j]->benefit;
	}

	if(i != size){
		float temp = (float)items[i]->benefit/ items[i]->weight ;
	       	temp *= (W-tot_weight) ;
		bound += (int)temp ;
	}
	element->bound = bound;
}

MSMB * BB_solving(Item** itemV, int size){
	clock_t start = clock();
	Item ** sorted_items = make_sorted_item_list(itemV,size);
	float lastB = 0 ;
	int max_benefit = 0;
	Pqueue * pqueue = create_pqueue(size);
	Element * init1 = create_element(0,sorted_items[0]->benefit,sorted_items[0]->weight,0);
	Element * init_1 = create_element(0, 0, 0, 0);
	calculate_bound(sorted_items, size, init1);
	calculate_bound(sorted_items, size, init_1);
	pqueue_insert(pqueue, init1);
	pqueue_insert(pqueue, init_1);
	//int k = 0;
	while(1){
	//	printf("\n%d, %d \n", k++, max_benefit);
		if(max_benefit > pqueue_peek_bound(pqueue)) break;
		if(pqueue_peek_bound(pqueue) == -1) break;
		
		Element * popped = pqueue_delete(pqueue);
		int idx = popped->item_index ;
		int ub = popped->ub;
		int uw = popped->uw;
		if(max_benefit < ub) max_benefit = ub ;
		if(idx!=size -1){
			Element * choosen_element = create_element(idx+1, ub+sorted_items[idx+1]->benefit, uw+sorted_items[idx+1]->weight, 0);
			Element * not_choosen_element = create_element(idx+1, ub, uw, 0);
			calculate_bound(sorted_items, size, choosen_element);
			calculate_bound(sorted_items, size, not_choosen_element);
			if(choosen_element->uw <= size * 40) 
				pqueue_insert(pqueue, choosen_element);
			pqueue_insert(pqueue, not_choosen_element);
		}
	//	pqueue_tree_printer(pqueue);
	//	printf("\n");

		free(popped);	
	}

	free_pqueue(pqueue);
	delete_item_list(sorted_items, size);
	
	lastB = (float)max_benefit;
	int pt = time_formatter(start, clock());
	MSMB * msmb = create_msmb(pt, lastB);
	return msmb ;
}

//solvers
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
	make_output_file();
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
	free_solutions();
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
	signal(SIGINT, sigalrm_handler);
	alarm(3600);
	create_test_case();
	greedy_solver();
	DP_solver();
	make_output_file();// This program ensures DP always make answer
	BB_solver(); // In tricky case, BB is not solved.  
	dispose_program();
	return 0;
}
