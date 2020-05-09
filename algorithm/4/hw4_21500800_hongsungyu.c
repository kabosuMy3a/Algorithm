//This program works for Greedy, DP, and Branch and Bound soultions.

#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct item Item ;

//for keeping memory, don't store b/w
struct item{
	int benefit ;
	int weight ;
};

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

void greedy_solution(){
	
}

int main(){
	srand(time(0x0));
	Item ** items_10000 = create_item_list(10000);
	Item ** sorted_items_10000 = make_sorted_item_list(items_10000, 10000);

	delete_item_list(items_10000,10000);
	delete_item_list(sorted_items_10000,10000);
	return 0;
	

}
