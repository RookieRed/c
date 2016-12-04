/***********************
** TP MACHINE 3 EXO 3 **
************************/

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <setjmp.h>

#include "output.h"

#define TAILLE_MSG 250
#define CODE_REP   1
//On défini PtrFct comme un pointeur de fonction de type void(int)
typedef void(* PtrFct )(int);

//On déclare en global le point de reprise
jmp_buf jump;

//Déclarationd e la fonction exécuté a la reception du signal
void timeout(int);

int main(){

	//Déclarationd es variables
	int      ret, tentatives = 0;
	unsigned duree = 3;
	char     message[TAILLE_MSG];
	PtrFct   retsig;

	//On ancre le point de reprise ici
	ret = setjmp(jump);
	if (ret == CODE_REP) {
		printf(" [i] Vous avez raté %d essai(s)\n", tentatives);
		if (tentatives > 2)	{
			ecrireErreur("Nombre de tentatives max atteint", 0);
			exit(0);
		}
		else {
			ecrireInfo("Nouvelle tentative");
		}
	}
	
	//Si on capte le signal alarme on appele la fonction timeout
	retsig = signal(SIGALRM, timeout);
	if (retsig == SIG_ERR) {
		ecrireErreur("Erreur fonction signal", 1);
	}
	alarm(duree);
	tentatives++;

	//Traitement du main: lecture du clavier
	printf("Entrez quelque chose...\n");
	scanf("%s", message);
	ecrireInfo(message);
	
}

void timeout(int sig){

	switch (sig){
		case SIGALRM:
			//On démasque le signal pour opérer un traitement différent
			sigrelse(SIGALRM);
			ecrireInfo("Temps écoulé");
			longjmp(jump, CODE_REP);
			break;

		default:
			ecrireErreur("Erreur signaux systèmes", 2);
			break;
	}
}