#include <iostream>
#include <string>
#include <sstream>
#include <vector>

using std::cin;
using std::cout;
using std::endl;
using std::string;
using std::vector;
using std::stringstream;

int main(){

	string a;
	

	while(std::getline(cin,a)){
		if(cin.eof()) break;
		std::stringstream ss(a);	
		vector<int> vect;
		for(int i ; ss>> i;){
			vect.push_back(i);
			if(ss.peek()== ',') ss.ignore();
		}	

		cout << vect[0]+ vect[1] << endl;
	}
	
	return 0;
}

