#include <stdio.h>
#include <stdlib.h>
#include <string.h>



long long recursive_combination(long long  n, long long k){
	if(k == 0){
		return 1 ;
	}
	if(n == 0){
		return 0 ;
	} else if (k==n) {
		return 1 ;
	} else {
		return recursive_combination(n-1,k-1) + recursive_combination(n-1, k) ;
	}
}

void DP_combinatnion(long long n, long long k){

}


int main(int argc, char** argv){


	
	long long n ; 
	long long k ;
	scanf("%lld %lld", &n, &k);

	
	long long b[k+1][n+1] ;
//	b = (long long **) malloc(sizeof(long*) * k+1);
//	for(long long i = 0 ; i<=k ; i++){
//		b[i] = (long long *) malloc(sizeof(long) * n+1);
//	}

	if (n < k) {
		printf("K should be less than N");
		return -1; 
	}

	for(long long i = 0 ; i<= n ; i++){
		b[0][i] = 1;
		if(i <= k) b[i][i] = 1;
	}
	
	for(long long j = 1 ; j <= k ; j++){
		for(long long m=j+1 ; m<= n ; m++){
		   b[j][m] = b[j-1][m-1] + b[j][m-1] ;
		}
	}
	
	DP_combinatnion(n,k);
	printf("DP = %lld\n", b[k][n]);
//	printf("recursive = %lld\n", recursive_combination(n,k));

	return 0;
}
