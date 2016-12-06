#include "headers/includes.h"

#define ADDR_ROOT "127.0.0.1"
#define PORT_ROOT 1234


int main(int argc, char const *argv[]) {

//==== VARIABLES =====================================
													//
	Socket sockCom;									//
	int portEcoute, buff_len;						//
	char qname[MAX_BUFF_LEN], buffer[MAX_BUFF_LEN];	//
													//
//====================================================

	//Vérification des paramètres
	if(argc != 2){
		printf(" [!] Erreur de paramètres\n");
		return -1;
	}
	portEcoute = atoi(argv[1]);

	//Création de la socket
	initialiserSocketUDP(portEcoute, NULL, &sockCom, TYPE_SERVEUR, NULL);
	printf(" (i) Serveur mis en écoute\n");

	while(true){
		//Récéption de la question DNS
		buff_len = recvfrom(sockCom, buffer, sizeof(buffer), 0, (struct sockaddr *) &client, &client_len);
		if(buff_len < 0){
			perror("Erreur recvfrom");
			exit(6);
		}
		//On parse la requete pour en récupérer la question
		qname_from_question(buffer, buff_len, qname);

		//Intérogation du serveur racine

		//Intérogation du serveur TLD récupéré

		//Intérogation du serveur d'authorité (si besoin)
	}


	//Fermeture de la socket de comunication
	close(sockCom);
	printf(" (i) Fin de l'écoute\n");
	return 0;
	}
}
