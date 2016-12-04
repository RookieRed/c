#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

void * fct_threads(void * param);

int main(int argc, char const *argv[]) {
	int nbThread = atoi(argv[1]), numThread[nbThread], i;
	pthread_t ptid[nbThread];
	pthread_attr_t attr;

	printf("[PERE] Début du programme\n");

	//Initialisationd es threads
	pthread_attr_init(&attr);
	// pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	for(i=0; i<nbThread; i++){
		numThread[i] = i;
		if(pthread_create(&ptid[i], &attr, fct_threads, &numThread[i]) != 0){
			perror("Erreur lancement thread");
			exit(99);
		}
	}

	//Jointure des threads
	for (i=0; i<nbThread; i++) {
		pthread_join(ptid[i], (void**) NULL);
	}
	printf("[PERE] Fin du programme\n");
	return 0;
}


void * fct_threads(void *param){
	pthread_t pid = pthread_self();
	printf("\tThread de rang %d id = %d \n", *((int *) (param)), pid);
	pthread_exit(NULL);
}