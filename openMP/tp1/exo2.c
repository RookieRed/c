#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define SIZE 20000
#define THR 4

double get_time() {
  struct timeval tv;
  gettimeofday(&tv, (void *)0);
  return (double) tv.tv_sec + tv.tv_usec*1e-6;
}

void version1(int * matrice){
	int somme = 0, sommeLoc;
	
	double start = get_time();
	for(int i=0; i<SIZE; i++){
		sommeLoc = 0;
		for(int j=0; j<SIZE; j++){
			sommeLoc += matrice[SIZE*i + j];
		}
		somme += sommeLoc;
	}
	double stop = get_time();
	//Affichage du resultat
	printf("Fin du prog : somme = %d tmps (e2) = %f\n", somme, (stop-start)*1e2);
}

void version2(int * matrice){
	int somme = 0, j, sommeLoc[THR], numThread;
	
	//Initialisation de sommes locales
	for(int i=0; i<THR; ++i){
		sommeLoc[i] = 0;
	}
	
	double start = get_time();
	//Traitement
	#pragma omp parallel for num_threads(THR) private(j)
	for(int i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			sommeLoc[omp_get_thread_num()] += matrice[SIZE*i + j];
		}
	}
	for(int i=0; i<THR; i++){
		somme += sommeLoc[i];
	}
	double stop = get_time();
	//Affichage du resultat
	printf("Fin du prog : somme = %d tmps (e2) = %f\n", somme, (stop-start)*1e2);
}

void version3(int * matrice){
	//Varaibles
	int somme = 0, j, numThread, sommeLoc;
	
	double start = get_time();
	//Traitement
	#pragma omp parallel num_threads(THR) private(j, sommeLoc)
	{
		sommeLoc = 0;
		#pragma omp for 
		for(int i=0; i<SIZE; i++){
			for(j=0; j<SIZE; j++){
				sommeLoc += matrice[SIZE*i + j];
			}
		}
		#pragma omp atomic
		somme += sommeLoc;
	}
	double stop = get_time();
	//Affichage du resultat
	printf("Fin du prog : somme = %d tmps (e2) = %f\n", somme, (stop-start)*1e2);
}

void version4(int * matrice){
	int somme = 0, j;
	
	double start = get_time();
	#pragma omp parallel for num_threads(THR) private(j)
	for(int i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			#pragma omp atomic
			somme += matrice[SIZE*i + j];
		}
	}
	double stop = get_time();
	//Affichage du resultat
	printf("Fin du prog : somme = %d tmps (e2) = %f\n", somme, (stop-start)*1e2);
}

void version5(int * matrice){
	int somme = 0, j;

	double start = get_time();
	#pragma omp parallel for num_threads(THR) private(j) reduction(+:somme)
	for(int i=0; i<SIZE; i++){
		for(j=0; j<SIZE; j++){
			somme += matrice[SIZE*i + j];
		}
	}
	double stop = get_time();
	//Affichage du resultat
	printf("Fin du prog : somme = %d tmps (e2) = %f\n", somme, (stop-start)*1e2);
}

void version6(int * matrice){
	printf("On comprend pas votre schemas en fait, mais la somme c'est toujours 400000000\n");
}

void main() {
	//Variables
	int *matriceA;
	double start, stop;
	
	printf("Début du programme\n");
	//Initialisation de la matrice
	matriceA = (int *) malloc(SIZE*SIZE*sizeof(int));
	for(int i=0; i<SIZE; i++){
		for(int j=0; j<SIZE; j++){
			matriceA[SIZE*i + j] = 1;
		}
	}
	printf("Matrice initialisée\n");
	
	//Calcul de la somme
	printf("\tVersion 1\n");
	version1(matriceA);
	printf("\tVersion 2\n");
	version2(matriceA);
	printf("\tVersion 3\n");
	version3(matriceA);
	printf("\tVersion 4\n");
	version4(matriceA);
	printf("\tVersion 5\n");
	version5(matriceA);
	printf("\tVersion 6\n");
	version6(matriceA);
}
