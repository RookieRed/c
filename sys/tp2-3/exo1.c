#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int main(int argc, char const *argv[]) {

	//Vairialbes
	int i, j, val = 0, somme = 0,
		nbFois = atoi(argv[1]),
		nbProc = atoi(argv[2]);
	pid_t parent = getpid();
	pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
	
	printf("[PERE] Debut du programme\n");

	//Création des process
	for(i=0; i<nbProc; i++){
		switch(fork()){
			case -1:{
				perror("Erreur création process");
				exit(99);
			}
			case 0:{
				for(j=0; j<nbFois; j++){
					printf("Je suis le processus %d mon parent est %d mon rang est %d\n"
						, getpid(), parent, i);
				}
				exit(i);
			}
		}
	}
	//Attente du père
	for(i=0; i<nbProc; i++){
		wait(&val);
		somme += WEXITSTATUS(val);
	}
	printf("[PERE] Fin du programme, total = %d\n", somme);
	return 0;
}