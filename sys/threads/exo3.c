#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {
	pthread_mutex_t mutex;
	int valeur;
} somme_t;

somme_t somme = {
	.mutex = PTHREAD_MUTEX_INITIALIZER,
	.valeur = 0,
};

void * fct_threads(void * param);

int main(int argc, char const *argv[]) {
	//Variables
	int nbThread = atoi(argv[1]), i, *retThread, tab[nbThread];
	pthread_t ptid[nbThread];
	pthread_attr_t attr;

	printf("[PERE] id = %d Début du programme\n", pthread_self());

	//Initialisationd es threads
	pthread_attr_init(&attr);
	// pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	for(i=0; i<nbThread; i++){
		tab[i] = i;
		if(pthread_create(&ptid[i], &attr, fct_threads, &tab[i]) != 0){
			perror("Erreur lancement thread");
			exit(99);
		}
	}

	//Jointure des threads
	for (i=0; i<nbThread; i++) {
		pthread_join(ptid[i], (void**) &retThread);
		printf("Retour du thread numero %d = %d\n", ptid[i], *retThread);
		free(retThread);
	}
	printf("[PERE] Fin du programme, somme des valeurs = %d\n", somme.valeur);
	return 0;
}


void * fct_threads(void * param){
	//Variables
	pthread_t pid = pthread_self();
	int *i = malloc(sizeof(int));
	*i = (rand()%10)+1;

	//Début de la zone critique
	pthread_mutex_lock(&somme.mutex);
	somme.valeur += *i;
	printf("\tThread numero %d id = %d valeur renvoyée = %d \n", *((int*)param), pid, *i);

	//Fin de la zone critique
	pthread_mutex_unlock(&somme.mutex);
	pthread_exit((void*) i);
}