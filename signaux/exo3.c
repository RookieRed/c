#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int procFils = 0;
pid_t f1, f2;

void interception(int sig){
	printf("Reception du signal\n");
	if(procFils == 1){
		kill(f2, SIGINT);
		printf("Meutre sans etats d'ames du proc 2\n");
	}
	else if(procFils == 2){
		kill(f1, SIGINT);
		printf("Meutre sans etats d'ames du proc 1\n");
	}
	else {
		printf("Erreur signal : %d\n", procFils);
	}
}

int calculComplexe(int max){
	int i, j, k;
	for (i=0; i < max; ++i) {
		for (j=0; j < max; ++j) {
			for (k=0; k < max; ++k) {
				1+1;
			}
		}
	}
	return i;
}

int main(int argc, char const *argv[]) {
	
	//Variables
	int r1, r2;
	pid_t pid_pere = getpid();
	signal(SIGUSR1, interception);

	//Lancement du 1er fils
	switch(f1 = fork()){
		case -1: {
			perror("Erreur lancement proc");
			exit(99);
		}
		case 0: {
			printf("Début du fils f1\n");
			r1 = calculComplexe(100);
			procFils = 1;
			kill(pid_pere, SIGUSR1);
			printf(" -> Resultat du calcul 1 : %d\n", r1);
			exit(0);
		}
	}

	//Lancement du 2e fils
	switch(f2 = fork()){
		case -1: {
			perror("Erreur lancement proc");
			exit(99);
		}
		case 0: {
			printf("Début du fils f2\n");
			r2 = calculComplexe(100);
			procFils = 2;
			kill(pid_pere, SIGUSR1);
			printf(" -> Resultat du calcul 2 : %d\n", r2);
			exit(0);
		}
	}

	pause();
	wait(NULL);
	printf("Sortie du programme\n");

	return 0;
}