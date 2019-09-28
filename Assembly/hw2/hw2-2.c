#include <stdio.h>

int data[10] = {10, -2, 5, 22 ,99, 0 ,-5, 9, 30, 7};
void bubble_sort(int a[], int size);
void main(){
	bubble_sort(data,10);
	for(int i=0 ; i<10; i++) printf("%d: %d\n",i, data[i]);
}
void bubble_sort(int a[], int size){
	int j, temp;
	if(size>0){
		for(j=0; j<size-1; j++){
			if(a[j] > a[j+1]){
				temp=a[j];
				a[j]=a[j+1];
				a[j+1]=temp;
			}
		}
		bubble_sort(a,size-1);
	}
}
