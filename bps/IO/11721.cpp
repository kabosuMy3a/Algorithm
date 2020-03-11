#include <iostream>
#include <cstdlib>
#include <string>

using std::cin;
using std::getline;
using std::string;
using std::cout;
using std::endl;


int main(){
	string a;
	getline(cin,a);

	int num = a.length();
	auto index =0;
	while(num >0){
		if(num > 10) cout << a.substr(index, 10)<<endl;
		else cout << a.substr(index, num) << endl;
		index +=10 ;
		num = num -10;
	}

		
	
	return 0;
}
