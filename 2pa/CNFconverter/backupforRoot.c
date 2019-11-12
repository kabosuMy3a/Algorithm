#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct t_pnode PNode;

struct t_pnode/*propostion node*/{
	
	int type;/* -1= not, 0=or, 1=and, 2=var */
	int var ;/*if type=2, var,,negative means not
		   else fix to 0*/ 
	int leafs_num ;
	/*
	 *if type is 0 or -1, leafs_num can be infinity
	 *if type is -2, leafs_num should be 1;
	 *if type is positive, leafs_num should be 0; 
	 */
	int birthOrderStamp ;
	PNode* parent;
	
	PNode* leafs[100] ;	
};


PNode* createPNode(int type,int var)/*only for making root*/{
	PNode* pnode = malloc(sizeof(PNode));
	pnode->type = type;
	pnode->var = var;  
	pnode->leafs_num =0;
	pnode->parent =0x0;//if parent==0x0 root;
	pnode->birthOrderStamp=0;

	for(int i=0; i< 100 ; i++){
		pnode->leafs[i]=0x0;
	}		
	return pnode ;
}

void freePNode(PNode* pnode ){
	if (pnode == 0x0) return;
	else{
		for(int i=0; i< pnode->leafs_num ; i++){
			freePNode(pnode->leafs[i]);
		}
		free(pnode);
	}
}

PNode* copyPNode(PNode* pnode){
	PNode* copied = createPNode(pnode->type,pnode->var);
	copied->leafs_num = pnode->leafs_num ;
	copied->parent = pnode->parent;
	copied->birthOrderStamp= pnode->birthOrderStamp;
	for(int i=0; i< copied->leafs_num ; i++){
		copied->leafs[i] = pnode->leafs[i] ;
	}
	return copied;
}

PNode* insertLeaf(PNode* pnode, int type, int var){
	PNode* leaf = createPNode(type,var);
	pnode->leafs[pnode->leafs_num] = leaf;	
	leaf->birthOrderStamp = pnode->leafs_num;
	pnode->leafs_num++;
	leaf->parent = pnode;
	return leaf;
}
PNode* insertVar(PNode* pnode, int var){
	PNode* leaf = createPNode(2,var);
	pnode->leafs[pnode->leafs_num] = leaf;
	leaf->birthOrderStamp = pnode->leafs_num;
	pnode->leafs_num++;
	leaf->parent = pnode;
	return leaf;
}
PNode* insertOp(PNode* pnode, int type){
	PNode* leaf = createPNode(type,0);
	pnode->leafs[pnode->leafs_num] = leaf;	
	leaf->birthOrderStamp = pnode->leafs_num;
	pnode->leafs_num++;
	leaf->parent = pnode;
	return leaf;
}

void printInorder(PNode* pnode){
	if (pnode==0x0) return;
	if (pnode->type==2) printf("%d ",pnode->var);
	else if(pnode->type== -1){
		//assert that we already converted to NNF Form.
		printf("not ");
		printInorder(pnode->leafs[0]);
	}
	else if(pnode->type==0||pnode->type==1){
		for(int i=0; i< pnode-> leafs_num; i++){
			printInorder(pnode->leafs[i]);
			if(i!=pnode->leafs_num-1){
				(pnode->type == 1) 
					? printf("\n")
					: printf(" ");
			//assert that we already converted to CNF Form.
			}	
		}
	}
}

void NegationALL(PNode* pnode){
	if (pnode==0x0) return;
	if (pnode->type==2){
		pnode->var = pnode->var *-1;
		return;
	}
	else if (pnode->type==0){
		pnode->type = 1 ;
		for(int i=0; i< pnode->leafs_num; i++){
			NegationALL(pnode->leafs[i]);
		}	
	}
	else if (pnode->type==1){
		pnode->type = 0 ;
		for(int i=0; i< pnode->leafs_num; i++){
			NegationALL(pnode->leafs[i]);
		}	
	}
	else if (pnode->type==-1){
		if(pnode->parent != 0x0){
			pnode->parent->leafs[pnode->birthOrderStamp] = pnode->leafs[0];
			pnode->leafs[0]->parent = pnode->parent;
			pnode->leafs[0]->birthOrderStamp = pnode->birthOrderStamp;
		}
	}
}

void ConvertToNNF(PNode* pnode){
	if (pnode==0x0) return ;

	if (pnode->type==2) return ;
	else if (pnode->type==-1){
		NegationALL(pnode->leafs[0]);
		if(pnode->parent != 0x0){
			pnode->parent->leafs[pnode->birthOrderStamp] = pnode->leafs[0];
			pnode->leafs[0]->parent = pnode->parent;
			pnode->leafs[0]->birthOrderStamp = pnode->birthOrderStamp;
		
		}
		ConvertToNNF(pnode->leafs[0]);
	}
	else if(pnode->type==0 || pnode->type ==1){
		for(int i=0; i< pnode->leafs_num; i++){
			ConvertToNNF(pnode->leafs[i]);
		}	
	}
}


void ApplyDistributeRule(PNode* pnode){
	//should consider all stamp and parent 
	PNode* grandMother = pnode->parent->parent;
	int mothersBirthOrderStamp = pnode->parent-> birthOrderStamp;
	//grandma null check;
	for(int i=0; i< pnode->leafs_num; i++){
		PNode* originChild = pnode->leafs[i];
		PNode* newChild = copyPNode(pnode->parent);
		
		newChild->leafs[pnode->birthOrderStamp] = originChild;
		originChild->parent = newChild;
		originChild->birthOrderStamp = pnode->birthOrderStamp;

		pnode->leafs[i] = newChild ;
		newChild->parent = pnode ;
		newChild->birthOrderStamp = i;
	}
	if(grandMother!=0x0){
		grandMother->leafs[mothersBirthOrderStamp] = pnode;
		pnode->parent = grandMother;
		pnode->birthOrderStamp = mothersBirthOrderStamp;
	}else{
		pnode->parent = 0x0;
		pnode->birthOrderStamp =0;
		/*how to make this node as root*/
	}
}

void ConvertToCNF(PNode* pnode){
	if (pnode==0x0) return;
	
	if(pnode->type==2) return;
	else if(pnode->type==-1){
		ConvertToCNF(pnode->leafs[0]);
		/*We ensure that input is NNF,
		 *so this is useless,but for using 
		 other purpose, Maybe*/
	}
	else if(pnode->type==0){
		for(int i=0; i< pnode->leafs_num; i++){
			ConvertToCNF(pnode->leafs[i]);
		}	
	}
	else if(pnode->type==1){
		
		if(pnode->parent!=0x0){
			if(pnode->parent->type==0){
				ApplyDistributeRule(pnode);
				ConvertToCNF(pnode);//Check New Family Relation.
			}
			else{
				for(int i=0; i< pnode->leafs_num; i++){
					ConvertToCNF(pnode->leafs[i]);
				}
			}
		}	
		else{
			for(int i=0; i< pnode->leafs_num; i++){
				ConvertToCNF(pnode->leafs[i]);
			}
		}	
	}
	
}

void getSolution(PNode* root){
	ConvertToNNF(root);
	ConvertToCNF(root);
	printInorder(root);
	printf("\n");
}


int main(int argc, char * argv[]){	
	
	PNode * realRoot = createPNode(1,0);	
	PNode * root = insertOp(realRoot,0);
	insertVar(realRoot,10);
	//TODO: Root 처리 
	//PNode * root = createPNode(0,0);	
	PNode* semi = insertOp(root, -1);	
	insertVar(root,4);
	semi = insertOp(semi,0);
	PNode* jaymy = insertOp(semi,-1);
	insertVar(semi,3);
	PNode* ricky = insertOp(semi, 1);
	jaymy= insertOp(jaymy,0);
	insertVar(jaymy,1);
	insertVar(jaymy,2);
	jaymy= insertOp(ricky,-1);
	insertVar(jaymy,6);
	insertVar(ricky,5);

	getSolution(realRoot);
	

	return 0;
}




