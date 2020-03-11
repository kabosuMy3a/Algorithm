#include <iostream>
#include "stdlib.h"

using std::cout;
using std::endl;

namespace lo{
	void v(void){
		cout << "lov" <<endl;
	}
}

namespace ro{
	void v(void){
		cout << "vv" <<endl;
	}
}

auto func(double m, int i) -> decltype(m/i)
{
	if( i !=0)
	return m/i;
}

//#define sq(x) (x) * (x)
inline auto sq(int x){
	return x / 3.0;
}

int main(){

	int m = 11;
	int n = 22;
	auto e = 2.71828;
	const double* doublePtr = &e;
	cout << *doublePtr << endl;

	cout << "m" << m
	       <<"\nn" << n<< endl;

	lo::v();
	ro::v();
	/*
	int arr[5] = {0,1,2,3,4};
	for(size_t i = 0 ; i < 5 ; ++){
		cout << func(4,arr[i]) << endl;
	};*/
	cout << sq(5+9) << endl;

	return 0;	

}
