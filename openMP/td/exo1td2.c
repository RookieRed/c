#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char const *argv[]) {
	
	int i, n = 100, tab[n];
	for(i=0; i<100; i++){
		tab[i] = rand()%1024;
	}

	#pragma omp parallel {
	int buffer;
	for (i = n; i>0; i--) {
		if(i%2){
			//.....
		}
	}
	}
	return 0;
}