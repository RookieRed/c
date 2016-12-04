#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>

#define MAXTHREADS 4
#define TAILLETAB 2048

void methode1(int*);
void methode2(int*);

int main(int argc, char const *argv[]) {
	int i;
	//Initialisation du tab
	int tab[TAILLETAB];
	for(i=0; i<TAILLETAB; i++){
		tab[i] = rand()%5000;
	}
	methode2(tab);
	return 0;
}

void methode1(int* tab){
	//Variables
	int max[MAXTHREADS];
	int i = 0, maximum = 0;

	//Lancement des 4 threads
	#pragma omp parallel num_threads(MAXTHREADS)
	{	
		int id = omp_get_thread_num();
		max[id] = 0;
		printf("Thread numero %d\n", id);
		//Exécution parallèle
		#pragma omp for
		for(i=0; i<TAILLETAB; i++){
			if(tab[i] > max[id]){
				max[id] = tab[i];
			}
		}
		//On revient à un seul thread
		#pragma omp single
		{
			printf("Les maximums retournés par les threads sont :\n");
			for(i=0; i<MAXTHREADS; i++){
				printf("\t %d : %d\n", i, max[i]);
				if(max[i] > maximum){
					maximum = max[i];
				}
			}
		}
	}
	printf("Maximum du tableau : %d\n", maximum);
}

void methode2(int* tab){
	//Variables
	int i, max = 0;
	//Execution parallele
	#pragma omp parallel num_threads(MAXTHREADS)
	{
		for(i=0; i<TAILLETAB; i++){
			if(tab[i] > max){
				//Débt de la section critique
				#pragma omp critical
				{if(tab[i] > max){
					max = tab[i];
					printf("Thread n°%d, val max = %d\n", omp_get_thread_num(), max);
				}}
			}
		}
	}
	printf("Maximum : %d\n", max);
}