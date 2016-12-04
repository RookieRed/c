#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

//Variables globales
pid_t * pid_ouvriers;
int nbOuvrier = 0;
int * tubes;

//Fonctions
void interception(int sig){}
void setMask(sigset_t *, int);
void ouvrier(int);
void client1(int);
void client2(int);

//Types personnels
typedef struct {
	int num;
	int idClient;
	int info[200];
} Commande;

Commande nouvelleCommande(int numCommande, int idClient){
	Commande commande = {
		.num = numCommande,
		.idClient = idClient
	};
	memset(commande.info, 0, 200);
	return commande;
}

void afficherCommande(Commande * c){
	int i;
	printf("<< %d %d", (*c).num, (*c).idClient);
	for(i=0; i<nbOuvrier; i++){
		printf(" info[%d] = %d", i, (*c).info[i]);
	}
	printf(" >>\n");
}


			/***********
			**  MAIN  **
			***********/

int main(int argc, char const *argv[]) {
	//Paramètres
	if(argc != 4){
		printf("Erreur arguments");
		exit(99);
	}
	nbOuvrier = atoi(argv[1]);
	int nbClient1 = atoi(argv[2]);
	int nbClient2 = atoi(argv[3]);

	//Variables
	int i;
	sigset_t set;
	pid_t pid;
	pid_ouvriers = (pid_t*) malloc(nbOuvrier*sizeof(pid_t));

	//Mise en pplace du masque
	signal(SIGUSR1, interception);
	sigemptyset(&set);
	sigaddset(&set, SIGUSR1);
	sigprocmask(SIG_BLOCK, &set, NULL);
	//Ne pas oublier de supprimer les signaux ajoutés pour utiliser sigsuspend
	sigdelset(&set, SIGUSR1);
	

	//Création des tubes
	tubes = (int*) malloc((2*nbOuvrier+1)*sizeof(int));
	for(i=0; i<nbOuvrier; i++){
		if(pipe(tubes+(i*2)) == -1){
			perror("Erreur pipe");
			exit(99);
		}
	}

	//Création des ouvriers
	for (i = 0; i < nbOuvrier; ++i) {
		if((pid = fork()) == -1){
			perror("erreur fork");
			exit(99);
		}
		else if(pid == 0){
			// while(1){
				sigsuspend(&set);
				Commande com = nouvelleCommande(0, 0);

				//En attente d'une commande
				printf("Ouvrier %d %d lecture de la commande : ", getpid(), i);

				//Lecture de la commande & modification
				read(tubes[i*2], &com, sizeof(com));
				com.info[i] = i+1;
				afficherCommande(&com);


				//Renvoie de la commande au suivant
				write(tubes[(i+1)*2+1], &com, sizeof(com));
				if(i + 1 == nbOuvrier){
					kill((pid_t) com.idClient, SIGUSR1);
				}
				else {
					kill(pid_ouvriers[i+1], SIGUSR1);
				}
			// }
			exit(0);
		}
		else {
			pid_ouvriers[i] = pid;
		}
	}
	//Création des clients type 1
	printf(" *** Début du programme : attente du 1er client\n");
	for (i = 0; i < nbClient1; ++i) {
		if((pid = fork()) == -1){
			perror("erreur fork");
			exit(99);
		}
		else if(pid == 0){
			//Variables
			pid_t pid = getpid();
			Commande com;

			printf("Client %d (%d) de type 1, envoie d'une commande et attente...\n", i, pid);
			//Création de la commande
			com = nouvelleCommande(i, (int) pid);

			//Envoie sur le tube & signal
			write(tubes[1], &com, sizeof(com));
			kill(pid_ouvriers[0], SIGUSR1);

			//Attente de la commande
			sigsuspend(&set);
			read(tubes[2*nbOuvrier], &com, sizeof(com));
			printf("Client %d (%d), comamnde reçue : ", i, pid);
			afficherCommande(&com);
			exit(0);
		}
	}

	//Création des clients type 2
	for (i = 0; i < nbClient2; ++i) {
		if((pid = fork()) == -1){
			perror("Erreur fork");
			exit(99);
		}
		else if(pid == 0){
			client2(i);
			exit(0);
		}
	}

	for(i=0; i< nbOuvrier+nbClient1+nbClient2; i++){
		wait(NULL);
	}
	printf(" *** Fin du programme\n");
	return 0;
}

void ouvrier(int numero){
	//Variables
}

void client1(int numero){
}

void client2(int numero){
	//Variables
	pid_t pid = getpid();
	// Commande com = nouvelleCommande(numero, (int) pid);

	printf("Client %d (%d) de type 2, envoie d'une commande\n", numero, pid);
}


void setMask(sigset_t * set, int sig){

}