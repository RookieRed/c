#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <sys/time.h>

#define NB_OBJ 96
#define CAPACITY 640400
#define NB_THREADS 2


double get_time() {
  struct timeval tv;
  gettimeofday(&tv, (void *)0);
  return (double) (tv.tv_sec + tv.tv_usec*1e-6)*1e4;
}

int S[NB_OBJ][CAPACITY+1];


int main(int argc, char const *argv[])
{
	int M[NB_OBJ];
	int U[NB_OBJ];
	
	int E[NB_OBJ];
	int i,j,n;

	double t,start,stop;

	start = get_time();

	static const char filename[] = "pb6";
	FILE *file = fopen ( filename, "r" );

	if ( file != NULL ){

		char line [ 128 ]; /* or other suitable maximum line size */

		n = 0;
		i = 0;

		while ( fgets ( line, sizeof line, file ) != NULL ) /* read a line */
		{
			if(n > 0){
				sscanf(line, "o %d %d\n", &M[i], &U[i]);
				i++;
			}
			n++;
		}
		fclose ( file );
	}
	else {
		perror ( filename ); /* why didn't the file open? */
	}

	stop = get_time();
	t = stop-start;
	printf("temps nécessaire à la lecture du fichier décrivant le problème : %f\n", t);

	int val1, val2;

	double t = get_time();

	#pragma omp parallel private(i, j)
	{
		//Cas de la première ligne
		i = 0;
		#pragma omp for private(j)
		for (j = 0; j < (CAPACITY+1); j++)
		{
			//Est-ce que l'oject rentre dans le sac de capacité j ??
			if(M[i] <= j){
				S[i][j] = U[i];
			}else{
				S[i][j] = 0;
			}
		}
		
		//Pour le reste de la matrice
			for (i = 1; i < NB_OBJ; i++)
			{
				#pragma omp for private(val1,val2)
				for (j = 0; j < (CAPACITY+1); j++)
				{
					//Est-ce que l'oject rentre dans le sac de capacité j ??
					val1 = S[i-1][j];
					val2 = S[i-1][j-M[i]] + U[i];
					if(M[i] <= j){
						if(val1 < val2){
							S[i][j] = val2;
						}else{
							S[i][j] = val1;
						}
					}else{
						S[i][j] = val1;
					}
				
				}
				
			}
		
		
	}

	printf("%d\n", S[NB_OBJ-1][CAPACITY]); 
	printf("Temps d'exécution : %f\n", t-get_time());
	exit(0);

	start = get_time();

	int e;
	int trouve = 0;

	e = NB_OBJ-1;
	i = NB_OBJ-1;
	j = CAPACITY;

	while(!trouve){
		if(S[i][j] != S[i-1][j]){
			j = j-M[i];
			i = i-1;
			E[e] = 1;
		}else{
			i = i-1;
			E[e] = 0;
		}
		e--;
		if(i == 0){
			trouve = 1;
		}
	}
	if(S[i][j] > 0){
		E[e] = 1;
	}else{
		E[e] = 0;
	}


	//On affiche les objets récoltés
	for (int i = 0; i < NB_OBJ; i++)
	{
		if(E[i] == 1){
			printf("%d\n", i);
		}
	}

	stop = get_time();
	t = stop-start;
	printf("temps de recherche des objets à emporter (calcul du vecteur E) : %f\n", t);
	
	exit(0);

	//On affiche la matrice
	for (i = 0; i < NB_OBJ; i++)
	{
		printf("|");
		for (j = 0; j < CAPACITY+1; j++)
		{
			printf(" %d |", S[i][j]);
		}
		printf("\n");
	}
	return 0;
}