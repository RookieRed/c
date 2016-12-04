#include <stdio.h>
#include <omp.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int n = 2048;

int main(int argc, char const *argv[])
{
	float tab[n];
	float moy = 0;
	int nbsup = 0;
	srand(time(NULL));
	//Initialisation du tab
	for(int i=0; i<n; i++){
		tab[i] = rand()%50000/(rand()%20+1);
	}

	for (int i = 0; i < n; ++i)
	{
		moy += tab[i];
	}
	moy = moy/n;

	for (int i = 0; i < n; ++i)
	{
		if(tab[i]>moy){
			nbsup++;
		}
	}
	printf("Moy = %f NB sup = %d\n", moy, nbsup);

	return 0;
}