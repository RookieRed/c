#include "headers/includes.h"


int main(int argc, char const *argv[]) {

//==== VARIABLES =========================
										//
	Socket sockCom;						//
	const char *fichierConf;			//
	int portEcoute, typeServeur;		//
	char domaine[500];					//
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
	
	//On détermine le type de serveur : ROOT AUTHORITATIVE TLD
	if((typeServeur = read_server_type_from_conf(fichierConf, domaine)) != 0){
		// printf(" (i) Nom de domaine : %s %d \n", domaine, typeServeur);
		while(true)
			communicationServeurUDP(sockCom, fichierConf, domaine, typeServeur);
	}
	else {
		printf(" [!] Type de serveur non reconnu\n");
	}

	//Fermeture de la socket de comunication
	close(sockCom);
	printf(" (i) Fin de l'écoute\n");
	return 0;
}
