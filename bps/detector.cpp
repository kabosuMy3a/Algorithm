#include <cstdio>
#include <cstdlib>
#include <cctype>



struct LinkChar{

	char cha;
	LinkChar* next;
};

LinkChar* createLinkChar(char a){
	LinkChar* linkChar = (LinkChar*) malloc(sizeof(LinkChar)) ;
	linkChar->cha = a;
	linkChar->next = 0x0;
	return linkChar;
}

LinkChar* insertLinkChar(LinkChar* now, char a){

	now->next = createLinkChar(a); 
	return now->next;
}

void toString(LinkChar** to, char* string){
	int i=0;
	LinkChar* tail = 0x0;
	while(string[i] !=0x0){
		if(*to == 0x0){
			*to = createLinkChar(tolower(string[i]));
			tail = *to;
		} else {
			tail = insertLinkChar(tail, tolower(string[i]));
		}
		i++;
	}
}

void printString(LinkChar* root){
	LinkChar* tail = root;
	while(tail != 0x0){
		printf("%c", tail->cha);
		tail = tail->next;

	}
	printf("\n");
}
	
bool compare(LinkChar* one, LinkChar* two){
	if (one==0x0) return true;
	if (two==0x0) return false;

	if(one->cha < two->cha) return true;
	else if(one->cha == two->cha){
		compare(one->next, two->next);
	} else return false;
	
}


int main(int argc, char** argv){

	LinkChar* one = 0x0;
	LinkChar* tail = 0x0;
	LinkChar* two = 0x0;
	LinkChar* tail2 = 0x0;

	toString(&one, argv[1]);
	toString(&two, argv[2]);
	
	printf("%s\n", compare(one,two) ? "true" : "false");

	//printString(one);
	//printString(two);


	return 0;
}
