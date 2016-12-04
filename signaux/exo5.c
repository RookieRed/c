#include "exo5.h"

pid_t pilote;
pid_t * eleves;
int seuil;
int nbSigEleve = 0;
int nbEleves;
bool 

int main(int argc, char const *argv[]) {
	//Parrametres
	int uniteTmps = atoi(argv[1]);
	nbEleves = atoi(argv[2]);
	seuil = atoi(argv[3]);
	eleves = (pid_t*) malloc(nbEleves*sizeof(pid_t));
	pilote = getpid();

	//Sigaction
	struct sigaction sa;
	sa.sa_handler = pilote;
	sa.sa_flags = 0;
	sigemptyset(&sa.sa_mask);
	if(sigaction(SIGUSR1, &sa, NULL) == -1){
		perror("Erreur sigaction");
		exit(99);
	}
	if(sigaction(SIGALRM, &sa, NULL) == -1){
		perror("Erreur sigaction");
		exit(99);
	}

	//Création des fils
	for (int i = 0; i < nbEleves; ++i) {
		if((eleves[i] = fork()) == 0){
			perror("Erreur fork");
			exit(99);
		}
		else if(eleves[i] == 1){
			eleve();
			exit(0);
		}
	}

	alarm()

	//Traitement du pere
	return 0;
}

void pilote(int sig){
	if(sig == SIGUSR1){
		nbSigEleve++;
	}
	else if(sig == SIGALRM){
		if((nbSigEleve*100/nbEleves) > seuil){
			printf("Seuil dépassée -> envoie des signaux de la défaite\n");
			for (int i = 0; i < nbEleves; ++i) {
				kill(SIGUSR2, eleves[i]);
			}
		}
	}
}

void victoireEleve(int sig){
	if(sig == SIGUSR2){
		printf("On a gagné! :D\n");
		exit(0);
	}
}

void eleve(){

}