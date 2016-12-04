#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void * fct_threads(void * param);

int main(int argc, char const *argv[]) {
	//Variables
	int nbThread = atoi(argv[1]), i, *retThread, somme = 0;
	pthread_t ptid[nbThread];
	pthread_attr_t attr;

	printf("[PERE] Début du programme\n");

	//Initialisationd es threads
	pthread_attr_init(&attr);
	// pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	for(i=0; i<nbThread; i++){
		if(pthread_create(&ptid[i], &attr, fct_threads, (void*) NULL) != 0){
			perror("Erreur lancement thread");
			exit(99);
		}
	}

	//Jointure des threads
	for (i=0; i<nbThread; i++) {
		pthread_join(ptid[i], (void**) &retThread);
		printf("Retour du thread numero %d = %d\n", ptid[i], *retThread);
		somme += *retThread;
	}
	printf("[PERE] Fin du programme, somme des valeurs = %d\n", somme);
	return 0;
}


void * fct_threads(void * param){
	pthread_t pid = pthread_self();
	int *i = malloc(sizeof(int));
	*i = (rand()%10)+1;
	printf("\tThread id = %d valeur renvoyée = %d \n", pid, *i);
	pthread_exit((void*) i);
}