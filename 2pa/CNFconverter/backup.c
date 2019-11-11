#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct type_first FirstNode;
typedef struct type_second SecondNode;
typedef struct type_child ChildNode;

struct type_first{
	int or0and1;
	int children_num;

	ChildNode * children[100] ;	
};

struct type_second{
	//not
	ChildNode * child ;
};

/*struct type_third{	
} thirdNode;*/

struct type_child{
	//only for children or child
	FirstNode * child_first ; 
	SecondNode * child_second ;
	int variable ;
	int v0f1s2 ;
};
ChildNode * createChildNode(int type, FirstNode* f, SecondNode* s,int v);

FirstNode* createFirstNode(int andor){
	FirstNode * firstnode = malloc(sizeof(FirstNode));
	for(int i=0; i<100 ;i++){
		firstnode->children[i] = createChildNode(0,0x0,0x0,0);
	}

	firstnode->or0and1 = andor;
	return firstnode ;
}

ChildNode * createChildNode(int type, FirstNode* f, SecondNode* s,int v){
	ChildNode * child = malloc(sizeof(ChildNode));
	child->child_first = f;
	child->child_second = s;
	child->variable = v;
	child->v0f1s2 = type;

	return child;
}	

void pushChildNode(ChildNode* child, int type, FirstNode* f, SecondNode* s, int v){

	child->child_first = f;
	child->child_second= s;
	child->variable = v;
	child->v0f1s2 = type;
}






int main(int argc, char * argv[]){

	FirstNode * root = createFirstNode(1);
	printf("%d\n", root->or0and1);
	
	FirstNode * leaf = createFirstNode(0);
	pushChildNode(root->children[0], 1, leaf, 0x0, 0);	
	printf("%d\n", root->children[0]->child_first->or0and1);	

	return 0;
}




