/***********************************************
**              FICHIER OUTPUT.C              **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/

#include "output.h"

//fonction ecrire message d'erreur
void ecrireErreur(const char * message, const int code){
	char intro[100];
	strcpy(intro, " [X] ");
	strcat(intro, message);
	perror(intro);
	exit(code);
}

//Fonction ecrire message
void ecrireInfo(const char * message){
	char intro[100];
	strcpy(intro, " [X] ");
	strcat(intro, message);
	printf(" [i] %s\n", message);
}