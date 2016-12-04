#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include <string.h>

#define BUFFSIZE 50

int tube[2];
int num = 0;

//Fonction executé à la reception de SIGALRM dans le père
void handler(int sig){
	//Variables
	char buff[BUFFSIZE];

	//Si on reçoit un SIGALRM alors il faut ecrire dans le tube
	if(sig == SIGALRM){
		sprintf(buff, "Message du père n°%d", num);
		num++;
		write(tube[1], buff, strlen(buff));
	}
	//Si le signal est sigusr1 alors il faut lire le tube
	else if(sig == SIGUSR1) {
		read(tube[0], buff, BUFFSIZE);
		printf("%s\n", buff);
	}
}

int main(int argc, char const *argv[]) {
	
	//Variables
	int i, intval = atoi(argv[1]);
	pid_t pid;

	//Création du pipe
	pipe(tube);

	signal(SIGUSR1, handler);
	//Lancement du fils
	pid = fork();
	if(pid == -1){
		perror("Erreur fork");
		exit(99);
	}
	else if(pid == 0){
		close(tube[1]);
		//Varaibles
		pid = getpid();
		sigset_t set;
		
		//Mise en place du mask
		sigemptyset(&set);
		sigaddset(&set, SIGUSR1);
		sigprocmask(SIG_BLOCK, &set, NULL);
		//Ne pas oublier de supprimer les signaux ajoutés pour utiliser sigsuspend
		sigdelset(&set, SIGUSR1);
		while(1){
			sigsuspend(&set);
		}
		exit(0);
	}
	else {
		//Liaison du signal SIGALRM à la fonction d'interception
		signal(SIGALRM, handler);

		//Execution du père
		close(tube[0]);
		while(1){
			alarm(intval);
			pause();
			//On envoie le SIGUSR1 au fils
			kill(pid, SIGUSR1);
		}
	}

	return 0;
}