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
	copied->parent = 0x0;//cuz: deep-copy
	copied->birthOrderStamp= pnode->birthOrderStamp;
	for(int i=0; i< copied->leafs_num ; i++){
		copied->leafs[i] = copyPNode(pnode->leafs[i]) ;
		copied->leafs[i]->parent= copied;//자식도 카피해야되는거아니야?
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


PNode* ApplyDistributeRule(PNode **root, PNode* pnode){
	//should consider all stamp and parent 
	PNode* grandMother = pnode->parent->parent;
	int mothersBirthOrderStamp = pnode->parent-> birthOrderStamp;
	//grandma null check;	
	PNode* temp = copyPNode(pnode->parent);
	for(int i=0; i< pnode->leafs_num; i++){
		PNode* originChild = copyPNode(pnode->leafs[i]);
		PNode* newChild = copyPNode(temp);
		newChild->leafs[pnode->birthOrderStamp] = originChild;
		originChild->parent = newChild;
		originChild->birthOrderStamp = pnode->birthOrderStamp;

		pnode->leafs[i] = newChild ;
		newChild->parent = pnode ;
		newChild->birthOrderStamp = i;
	}
	if(grandMother!=0x0){
		//free(pnode->parent);
		grandMother->leafs[mothersBirthOrderStamp] = pnode;
		pnode->parent = grandMother;
		pnode->birthOrderStamp = mothersBirthOrderStamp;
	}else{
		//free(pnode->parent);
		pnode->parent =0x0;
		pnode->birthOrderStamp =0;
		*root = pnode;
	}
	return pnode;
}

void ConvertToCNF(PNode** root, PNode* pnode){
	if (pnode==0x0) return;
	if(pnode->type==2) return;
	else if(pnode->type==-1){
		ConvertToCNF(root,pnode->leafs[0]);
		/*We ensure that input is NNF,
		 *so this is useless,but for using 
		 other purpose, Maybe*/
	}
	else if(pnode->type==0){
		for(int i=0 ; i < pnode->leafs_num; i++){
			
			if(pnode->leafs[i]->type ==1){
				
				PNode* start= ApplyDistributeRule(root,pnode->leafs[i]);
				//printInorder(pnode);
				printf("\n");
				if(start->parent ==0x0){
					ConvertToCNF(root,start);
				}else{
					ConvertToCNF(root,start->parent);
				}
				break;
			}
			else{
				ConvertToCNF(root,pnode->leafs[i]);
			}
		}		
	}
	else if(pnode->type==1){	
		/*we guarantee that parent is not OR*/
		for(int i=0; i< pnode->leafs_num; i++){
			ConvertToCNF(root,pnode->leafs[i]);
		}		
	}
}

void getSolution(PNode** root){
	ConvertToNNF(*root);
	ConvertToCNF(root,*root);
	printInorder(*root);
	printf("\n");
}

void case1(){

	PNode * root = createPNode(0,0);	
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
	
	//printInorder(root);
	//printf("\n\n");
	getSolution(&root);
	
}
void case2(){

	PNode * root = createPNode(0,0);	
	PNode * level_1 = insertOp(root,1);
	PNode * level_2 = insertOp(root,1);
	PNode * level_3 = insertOp(root,-1);
	insertVar(level_1,1);
	insertVar(level_1,2);
	insertVar(level_1,3);
	insertVar(level_2,4);
	insertVar(level_2,5);
	insertVar(level_3,6);
	
	//printInorder(root);
	//printf("\n\n");
	getSolution(&root);
	
}

void case3(){

	PNode * root = createPNode(0,0);	
	PNode * level_1 = insertOp(root,1);
	PNode * level_2 = insertOp(root,0);
	PNode * level_3 = insertOp(root,-1);
	insertVar(level_1,1);
	insertVar(level_1,2);
	insertVar(level_1,3);
	insertVar(level_2,4);
	insertVar(level_2,5);
	insertVar(level_3,6);
	//printInorder(root);
	//printf("\n\n");
	getSolution(&root);
}

void case4(){
	PNode * root = createPNode(0,0);	
	PNode * level_1_1 = insertOp(root,1);
	PNode * level_1_2 = insertOp(root,0);
	PNode * level_2 = insertOp(level_1_1,1);
	insertVar(level_1_2,4);
	insertVar(level_1_2,5);
	insertVar(level_1_1,3);
	insertVar(level_2,1);
	insertVar(level_2,2);

	//printInorder(root);
	//printf("\n\n");
	getSolution(&root);


}

void case5(){
	PNode * root = createPNode(0,0);	
	PNode * level_1_1 = insertOp(root,1);
	PNode * level_1_2 = insertOp(root,0);
	PNode * level_2 = insertOp(level_1_2,1);
	insertVar(level_1_2,5);
	insertVar(level_1_1,1);
	insertVar(level_1_1,2);
	insertVar(level_2,3);
	insertVar(level_2,4);

	//printInorder(root);
	//printf("\n\n");
	getSolution(&root);

}

void case6(){
	PNode * root = createPNode(1,0);	
	PNode * level_1_1 = insertOp(root,0);
	PNode * level_1_2 = insertOp(root,0);
	PNode * level_2_1 = insertOp(level_1_1,1);
	PNode * level_2_2 = insertOp(level_1_2,1);
	insertVar(level_1_2,3);
	insertVar(level_1_1,6);
	insertVar(level_2_1,1);
	insertVar(level_2_1,2);
	insertVar(level_2_2,4);
	insertVar(level_2_2,5);

	//printInorder(root);
	//printf("\n\n");
	getSolution(&root);
}

void case7(){
	PNode * root = createPNode(1,0);	
	PNode * level_1_1 = insertOp(root,0);
	PNode * level_1_2 = insertOp(root,0);
	PNode * level_2_1 = insertOp(level_1_1,1);
	PNode * level_2_2 = insertOp(level_1_1,1);
	PNode * level_2_3 = insertOp(level_1_2,1);
	insertVar(level_1_2,3);
	insertVar(level_2_1,1);
	insertVar(level_2_1,2);
	insertVar(level_2_2,6);
	insertVar(level_2_2,7);
	insertVar(level_2_3,4);
	insertVar(level_2_3,5);

	//printInorder(root);
	//printf("\n\n");
	getSolution(&root);
}

void case8(){

	PNode * root = createPNode(1,0);	
	PNode * level_1_1 = insertOp(root,0);
	PNode * level_2_1 = insertOp(level_1_1,1);
	PNode * level_2_2 = insertOp(level_1_1,0);
	PNode * level_3_1 = insertOp(level_2_2,1);
		
	insertVar(root,3);
	insertVar(level_2_1,1);
	insertVar(level_2_1,2);
	insertVar(level_2_2,4);
	insertVar(level_3_1,5);
	insertVar(level_3_1,6);

	
	//printInorder(root);
	//printf("\n\n");
	getSolution(&root);
}

void case9(){
	PNode * root = createPNode(0,0);	
	PNode * level_1_1 = insertOp(root,1);
	PNode * level_1_2 = insertOp(root,0);
	insertVar(level_1_1,3);
	PNode * level_2 = insertOp(level_1_1,1);
	insertVar(level_1_2,4);
	insertVar(level_1_2,5);
	insertVar(level_2,1);
	insertVar(level_2,2);

	//printInorder(root);
	//printf("\n\n");
	getSolution(&root);


}

void caseSimple(){
	PNode * root = createPNode(0,0);	
	PNode * level_1_1 = insertOp(root,1);
	PNode * level_1_2 = insertOp(root,1);
	insertVar(level_1_2,2);
	insertVar(level_1_1,4);
	insertVar(level_1_2,1);
	insertVar(level_1_1,3);
	//printInorder(root);
	//printf("\n\n");
	getSolution(&root);

}

void caseFinal(){
	PNode * root = createPNode(0,0);
	insertVar(root,1);
	PNode * level_1_1 = insertOp(root,0);
	insertVar(level_1_1,2);
	PNode * nota = insertOp(level_1_1,-1);
	PNode * orta  = insertOp(nota,0);
	PNode * oorta = insertOp(orta,0);
	insertVar(orta, 5);
	//printInorder(root);
	insertVar(oorta,3);
	insertVar(oorta,4);
	getSolution(&root);
	//printf("\n\n");
	
}

typedef struct t_stack PStack;

struct t_stack{
	int level;
	PNode* item[100];
};

PStack* createPStack(){
	PStack* pstack = malloc(sizeof(PStack));
	pstack->level =0;
	for(int i=0 ; i<100 ; i++){
		pstack->item[i] = 0x0;
	}
	return pstack;
}

void pushPStack(PStack* pstack, PNode* item){
	pstack->item[++pstack->level] = item;

}
PNode* popPStack(PStack* pstack){
	PNode* popped = pstack->item[pstack->level] ;
	pstack->item[pstack->level] = 0x0;
	(pstack->level==0) ? pstack->level = 0 : pstack->level--;
	return popped;	
}

PNode* input2Tree(){
	char* lines = 0x0;
	size_t buf = 0;
	getline(&lines, &buf, stdin);
	int len = strlen(lines);
	
	PNode* root = createPNode(0,0);
	PNode* nowde = 0x0;
	PStack* pstack = createPStack();

	for(int i=0 ; i<len ; i++){
		char c = lines[i];
		if(i==0 && c!='('){
			printf("Wrong Input:: First letter should be (");
			return 0x0;
		}
		if (c =='('){
			if(nowde!=0x0) 
				pushPStack(pstack, nowde);
			char d = lines[i+1];
			if(d=='n'){
				if(i==0){
				       	root->type=-1;
					nowde = root ;
				}
				else{
					nowde = insertOp(nowde,-1);
				}
			}
			else if(d=='o'){
				if(i==0){
				       	root->type=0;
					nowde = root ;
				}
				else{
					nowde = insertOp(nowde,0);
				}

			}
			else if(d=='a'&& lines[i+2]=='n'){
				if(i==0){
				       	root->type=1;
					nowde = root ;
				}
				else{
					nowde = insertOp(nowde,1);
				}
			}

			else{
				printf("Wrong Input:: Start node with operation\n");
				return 0x0;
			}	
		}
		else if(c =='a' && lines[i+1]!='n'){
			char temp[10] ;
			int j=0;
			int var = 0;
			while(1){
				char d =lines[i+j+1];
				if(d>=48 && d<=57){
					temp[j]= d ;
				}
				else{
					if(j==0){
						printf("Wrong Input:: Should enter the number");
					}
					temp[j]=0x0;
					var = atoi(temp);
					break;
				}
				j++;
			}
			insertVar(nowde,var);
		}
		else if(c==')'){
			nowde = popPStack(pstack);
				
		}
	}

	return root;	
}


int main(int argc, char * argv[]){	
	
	PNode* root = input2Tree();
	getSolution(&root);	
	/*
	case1();
	printf("\n");
	case2();
	printf("\n");
	case3();
	printf("\n");
	case4();
	printf("\n");
	case5();
	printf("\n");
	case6();
	printf("\n");
	case7();
	printf("\n");
	case8();
	printf("\n");
	case9();
	printf("\n");
	caseSimple();
	printf("\n");
	caseFinal();
	printf("\n");
	*/
	return 0;
}




