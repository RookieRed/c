/***********************************************
**              FICHIER OUTPUT.C              **
************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>


/**
* Ecrire erreur : fonction qui affiche sur la sortie standard une erreur et quitte le programme
* @param message : le message d'erreur à afficher
* @param code : le code d'erreur à la sortie, si 0 alors pas d'erreurs
*/
void ecrireErreur(const char * message, const int code){
	char intro[256];
	strcpy(intro, " [X] ");
	strcat(intro, message);
	if (code != 0) {
		perror(intro);
		exit(code);
	}
}

/**
* Ecrire Info : fonction qui affiche sur la sortie standard une information simple
* @param message : le message à afficher
*/
void ecrireInfo(const char * message){
	char intro[256];
	strcpy(intro, " [X] ");
	strcat(intro, message);
	printf(" [i] %s\n", message);
}