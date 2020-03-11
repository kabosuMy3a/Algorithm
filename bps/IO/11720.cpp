#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;


int main(){

	auto a=0;
	auto c=0;
	string b;

	cin >> a;
	getline(cin,b);
	getline(cin,b);
	for(int i=0; i<a; i++){
		char* temp = &b[i];
		c+= temp[0]- '0';
	}

	cout<< c << endl;
	return 0;

}
