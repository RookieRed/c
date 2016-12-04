#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>

#define TAILLE 100

void main(){
	int i, j, k, somme = 0,
	mA[TAILLE][TAILLE], mB[TAILLE][TAILLE], mProd[TAILLE][TAILLE];

	//Création des matrices
	for (i = 0; i<TAILLE; ++i) {
		for (j=0; j<TAILLE; ++j) {
			mA[i][j] = rand()%100;
			mB[i][j] = rand()%100;
		}
	}
	//Calcul du produit
	#pragma omp parallel for private(j,k)
	for (i = 0; i<TAILLE; ++i) {
		printf("%d -", omp_get_thread_num);
		for (j=0; j<TAILLE; ++j) {
			for(k=0; k<TAILLE; k++){
				somme += mA[i][k]*mB[k][j];
			}
			mProd[i][j] = somme;
			// printf(" %d ", somme);
		}
		printf("\n");
	}
}