#include <iostream>

using namespace std;

int main(){

	auto num=0, a=0, b=0;

	cin>> num;
	for(int i=0; i<num ; i++){
		cin >> a>>b;
		cout << "Case #"<< i+1 << " " << a+b << endl;
	}

	return 0;
}
