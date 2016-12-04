#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>

#define TAILLETAB 2048

int main(int argc, char const *argv[]) {
	
	//Varirables
	int i, nbMaxLocaux = 0;
	float tab[TAILLETAB], max[TAILLETAB];

	//Initialisation du tab
	for(i=0; i<TAILLETAB; i++){
		tab[i] = (rand()%50000)/(rand()%100 + 1);
		max[i] = 0;
	}

	//Execution du traitement
	#pragma omp parallel for
	for (i = 1; i < TAILLETAB-1; ++i) {
		if(tab[i-1] < tab[i] && tab[i] > tab[i+1]){
		#pragma omp critical 
			max[i] = tab[i];
			nbMaxLocaux++;
		}
	}

	return 0;
}