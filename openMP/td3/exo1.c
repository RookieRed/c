#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>

#define TAILLETAB 1024

int main(int argc, char const *argv[]) {
	//Variables
	int i, j, estPrem, nbPrems = 0;
	//Traitement
	#pragma omp parallel private(j, estPrem)
	for(i=1; i<=TAILLETAB; i++){
		estPrem = 1;
		for(j=2; j<i; j++){
			if(i%j == 0){
				estPrem = 0;
			}
		}
		if(estPrem){
			printf("%d\n", i);
			//Section critique atomique
			#pragma omp atomic 
			nbPrems++; 
		}
	}
	printf("Il y a %d nb premiers\n", nbPrems);
	return 0;
}