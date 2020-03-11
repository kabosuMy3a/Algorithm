#include <iostream>

using std::cin;
using std::cout;
using std::endl;

int main(){


	auto a=0, b=0;

	while(true){
		
		cin >> a  >>b;
		if(cin.eof()) break;
		cout << a+b << endl;
		
	}
	
	return 0;
}

