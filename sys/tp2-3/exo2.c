#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>

void interception(int sig){
}

int main(int argc, char const *argv[]) {

	//Variables
	pid_t pere, fils;
	int i = 0;
	sigset_t set;
	pere = getpid(), fils;

	//Liaison du signal SIGUSR1 à la fonction d'interception
	signal(SIGUSR1, interception);

	//Mise en place du mask
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);
	//Ne pas oublier de supprimer les signaux ajoutés pour utiliser sigsuspend
	sigdelset(&set, SIGUSR1);

	//Creation du fils
	switch(fils = fork()){
		case -1:{
			perror("Erreur création process");
			exit(99);
		}
		//Fils
		case 0:{
			fils = getpid();
			while(1){
				sigsuspend(&set);
				printf("\t[FILS] pid = %d affichage %d\n", fils, i);
				kill(pere, SIGUSR1);
				//Point d'arrêt (pour plus de visibilité)
				if(i>20) break;
				i++;
			}
			exit(0);
		}
	}

	//Traitement pere
	while(1){
		printf("[PERE] pid = %d affichage %d\n", pere, i);
		kill(fils, SIGUSR1);
		sigsuspend(&set);
		//Point d'arrêt (pour plus de visibilité)
		if(i>20) break;
		i++;
	}
	//Synchronisation
	wait();

	return 0;
}


void version1(){
	//Creation du fils
	switch(fils = fork()){
		case -1:{
			perror("Erreur création process");
			exit(99);
		}
		//Fils
		case 0:{
			fils = getpid();
			for(j=0; j<NBPING; j++){
				kill(pere, SIGUSR2);
				pause();
			}
			exit(0);
		}
	}

	//Traitement pere
	for(i=0; i<NBPING; i++){
		pause();
		printf("[PERE] pid = %d affichage %d\n", pere, i);
		kill(fils, SIGUSR1);
	}
}