#include "headers/includes.h"


int main(int argc, char const *argv[]) {

//==== VARIABLES =========================
										//
	Socket sockCom;						//
	const char *fichierConf;			//
	int portEcoute;						//
										//
//========================================

	//Vérification des paramètres d'entrée
	if(argc != 3 && atoi(argv[1]) == 0){
		printf(" [!] Erreur de paramètres\n");
		exit(99);
	}
	fichierConf = argv[1];
	portEcoute = atoi(argv[2]);

	signal(SIGINT, interceptionSIGINT);

	//Création de la socket
	initialiserSocketUDP(portEcoute, NULL, &sockCom, TYPE_SERVEUR, NULL);
	while(1){
		communicationServeurUDP(sockCom, fichierConf, "univ-tlse3.fr", AUTHORITATIVE);
	}

	//Fermeture de la socket de comunication
	close(sockCom);
	printf(" (i) Fin de l'écoute\n");
	return 0;
}
