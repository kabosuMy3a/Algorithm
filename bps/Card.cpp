#include <iostream>

using std::cout;
using std::endl;

enum class Suit { clubs, diamonds, hearts, spades};
struct Card
{
	Suit s;
	int rank;
};

Card deck[52];

void init_deck(Card d[]){

	for(int i =0; i< 52 ; i++){
		d[i].s= Suit::hearts;
		d[i].rank = 1 + i%13;
	}
}

int main(int argc, char** argv){
	
	init_deck(deck);

	if(deck[3].s == Suit::clubs){
		cout<<"clubs"<<endl;
	}
	 
	cout << argv[1] << endl;
	return 1;
}
