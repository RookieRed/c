/***********************
** TP MACHINE 2 EXO 2 **
************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

#include "output.h"

//On défini PtrFct comme un pointeur de fonction de type void(int)
typedef void(* PtrFct )(int);

void traitementFils(int[2]);
void traitementSig(int);

void main(){

	//Déclaration des variables
	int   tube[2], err, taille;
	pid_t fils;
	char  buffer[256], message[128];

	ecrireInfo("[père] Début du prgm");
	//Instanciation du pipe
	err = pipe(tube);
	if(err){ecrireErreur("Erreur pipe", 1);}

	//On récupère le PID du processus fils
	fils = fork();
	if(fils == 0){
		traitementFils(tube);
	}
	else if(fils == 1){
		ecrireErreur("fork erreur", 2);
	}

	//On récupère le PID
	read(tube[0], buffer, taille);
	sprintf(message, "[père] PID fils récupéré : %s", buffer);
	ecrireInfo(message);
	kill(SIGUSR1, atoi(buffer));
}


void traitementFils(int tube[2]) {
	pid_t pid = getpid();
	char message[50];
	int err;

	sprintf(message, "[fils] PID %d", pid);
	ecrireInfo(message);
	err = write(tube[1], (void *) &pid, sizeof(pid));
	if(!err){ecrireInfo("[fils] PID envoyé, en attente");}
	else {perror("");}
	
	//Traitement du signal
	signal(SIGUSR1, traitementSig);
	while(1){
		sleep(10);
		ecrireInfo("[fils] toujours en attente");
	}

}

void traitementSig(int sig){
	ecrireInfo("Signal reçu");
	exit(0);
}