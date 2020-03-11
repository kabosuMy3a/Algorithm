#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

typedef struct element Element;

struct element{
	int key ;
	char name[100];
};

Element * create_null_element(){
	Element* element = (Element*) malloc(sizeof(Element));
	element->key = 0;
	element->name[0] = 0x0;
	return element;
	
}

Element * create_element(int key, char* name){
	Element* element = (Element*) malloc(sizeof(Element));
	element->key = key;
	strcpy(element->name,name);
	return element;
}

void free_element(Element* element){
	assert(element!=0x0);
	free(element);
}

void swap_element(Element* A, Element* B){
	Element * temp = create_null_element();
	temp->key = A->key;
	strcpy(temp->name,A->name);
	A->key = B->key;
	strcpy(A->name, B->name);
	B->key = temp->key;
	strcpy(B->name, temp->name);
	free_element(temp);	
}

void print_element(Element * element){
	printf("[%s, %d] ", element->name, element->key);
}

void swap_test(){
	Element* A= create_element(1, "hi");
	Element* B= create_element(2, "bye");
	print_element(A);
	print_element(B);
	swap_element(A,B);
	print_element(A);
	print_element(B);
}

typedef struct arrayHeap ArrayHeap;

struct arrayHeap{
	int index ;
	Element * element[1000] ;
};

ArrayHeap * create_array_heap(){
	ArrayHeap * arrayHeap = (ArrayHeap*) malloc(sizeof(ArrayHeap));
       	assert(arrayHeap!=0x0);	
	arrayHeap->index = 0 ;
	//Index is started with 1, no use ArrayHeap's element[0].
	//Thus if index is 0, heap is empty.
	for(int i=0 ; i < 1000 ; i++) arrayHeap->element[i] = create_null_element();
	return arrayHeap;
}

void free_heap(ArrayHeap * arrayHeap){
	for(int i=0 ; i<=arrayHeap->index ; i++){
		free_element(arrayHeap->element[i]);
	}
	free(arrayHeap);
}

void bottomup_balancing(ArrayHeap * heap, int index){
	int new_index = index;
	while(new_index > 1){
		int old_index = new_index;
		new_index = new_index/2;
		if(heap->element[old_index]->key > heap->element[new_index]-> key){
			swap_element(heap->element[old_index], heap->element[new_index]);
		} else break;
	}
}

void heap_insert(ArrayHeap* heap){
	char fresh_name[100];
	int fresh_key ;
	printf("Enter name of element: ");
	fgets(fresh_name, 100, stdin);
	fresh_name[strlen(fresh_name)-1] = 0x0;
	printf("Enter key value of element: ");
	char b[100];
	while(1){	
		fgets(b, 100, stdin);
		b[strlen(b)-1] = 0x0;
		fresh_key = atoi(b);
		if(fresh_key==0){
			printf("Key value is start from 1. Try integer input again\n");	
		} else break;
	}
	Element * freshman = create_element(fresh_key,fresh_name);
	heap->element[++heap->index] = freshman;
	print_element(freshman);
	printf("is inserted.\n");
	bottomup_balancing(heap, heap->index);
}

void topdown_balancing(ArrayHeap * heap, int index){
	int new_index = index;
	while(1){
		int old_index = new_index;
		new_index *= 2 ;
		if(new_index > heap->index) break;
		else if(new_index == heap->index){
			//new_index = new_index;
		} else {
			if(heap->element[new_index]->key < heap->element[new_index+1]->key)
			       	new_index += 1; 
		}//compare left child key and right child key
		if(heap->element[old_index]->key < heap->element[new_index]->key){
			swap_element(heap->element[old_index],heap->element[new_index]);	
		} else break;
	}
}

void heap_delete(ArrayHeap * heap){
	if(heap->index==0){
		printf("This heap is empty.\n");
		return;
	}//heap empty check
	swap_element(heap->element[1], heap->element[heap->index]);
	print_element(heap->element[heap->index]);
	printf("is deleted.\n");
	heap->element[heap->index] = create_null_element();
	heap->index = heap->index -1;
	//swap first element and last element
	topdown_balancing(heap, 1);	
}

void heap_retrieve(ArrayHeap* heap){
	if(heap->index==0){
		printf("This heap is empty.\n");
		return;
	} else {
		print_element(heap->element[1]);
		printf("\n");
	}
	
}

void heap_increase(ArrayHeap* heap){
	int index;
	printf("Enter index of element: ");
	char b[100];
	while(1){	
		fgets(b, 100, stdin);
		b[strlen(b)-1] = 0x0;
		index = atoi(b);
		if(index==0){
			printf("Heap index is start from 1. Try integer input again\n");	
		} else if(index> heap->index){
			printf("input index exceed current exist index.\n");
		       	return;
		}else break;
	}
	int key;
	printf("Enter new key value: ");
	while(1){	
		fgets(b, 100, stdin);
		b[strlen(b)-1] = 0x0;
		key = atoi(b);
		if(key==0){
			printf("Key value is start from 1. Try integer input again\n");	
		}else break;
	}
	heap->element[index]->key = key;
	bottomup_balancing(heap,index);
	topdown_balancing(heap,index);
}

void heap_print(ArrayHeap* heap){
	if(heap->index==0){
		printf("This heap is empty.\n");
		return;
	}
	for(int i=1 ; i<=heap->index ; i++){
		Element * element= heap->element[i];
		print_element(element);
	}
	printf("\n");
}

void heap_tree_printer(ArrayHeap* heap){
	if(heap->index==0){
		printf("This heap is empty\n");
		return;
	}
	int timing= 1; int growth = 2; 
	for(int i=1 ; i<=heap->index ; i++){
		Element * element= heap->element[i];
		if(i==timing+1){
			printf("\n");
			timing += growth;
			growth *= 2;
		}
		print_element(element);
	}
	printf("\n");
}

void print_console(){
	printf("************ Menu *************\n");
	printf("I: Insert new element into queue.\n");
	printf("D: Delete element with largest key from queue.\n");
	printf("R: Retrieve element with largest key from queue.\n");
	printf("K: Increase key of element in queue.\n");
	printf("P: Print out all elements in queue.\n");
	printf("Q: Quit.\n");
	printf("\nChoose Menu: ");	
}


void console(char key, ArrayHeap* heap){
	switch(key){
		case 'q':
		case 'Q': 
			  printf("Thank you, Bye\n");
			  break;
		case 'I': 
		case 'i': 
			  heap_insert(heap);
			  break;
		case 'D':
		case 'd': 
			  heap_delete(heap);
			  break;
		case 'R':
		case 'r': 
			  heap_retrieve(heap);
			  break;
		case 'K':
		case 'k':
			  heap_increase(heap); 
			  break;
		case 'P':
		case 'p': 
			  heap_print(heap);
			  //heap_tree_printer(heap);
			  break;

		default : 
			  printf("Try right menu\n\n");
			  break;
	}
}

int main(){
	char a[100] ;
	ArrayHeap * heap = create_array_heap();
	assert(heap!=0x0);
	while(1){
		print_console();
		fgets(a,100,stdin);
		char key = a[0];
		console(key, heap);	
		printf("\n");
		if(key=='q'||key=='Q') return 0;
	}
	free_heap(heap);
	return 0;
}
