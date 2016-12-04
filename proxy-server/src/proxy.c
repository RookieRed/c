/***********************************************
**                 FICHER PROXY.H             **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/

#include "header/proxy.h"

Socket creerSocketServeur(const * char port){

//==== VARIABLES =================================
                                                //
	struct addrinfo hints, *res;                //
	int AIlen = sizeof(struct addrinfo);        //
	Socket socFD;                               //
	int err;                                    //
                                                //
//================================================

	//Création de la socket
	socFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socFD == 0) {
		ecrireErreur("Erreur socket", 1);
	}

	//Instanciation des addrinfo
	memset(&res, 0, AIlen);
	hints.ai_flags 		= AI_PASSIVE;   // Mode serveur (passif)
	hints.ai_socktype 	= SOCK_STREAM;  // protocole tcp
	hints.ai_family 	= AF_INET;      // famille adresse : IPv4

	//Récupération des address info
	err = getaddrinfo(SERVER_ADDRESS, port, &hints, &res);
	if (err) {
		ecrireErreur("Erreur get address info", 1);
	}

	//Publication de la socket
	err = bind(sockFD, res->ai_addr, res->ai_addrlen);
	if (err) {
		ecrireErreur("Erreur bind", 2);
	}

	//On affiche à l'écran
	ecrireMessage("Création de la socket serveur");

	//Retourne le fichier descripteur de la socket
	return sockFD;
}


/*
* Instancie une socket type cliente et la connecte au serveur passé en params
*/
Socket creerSocketClient(const char * adresseDist, const * char portDist){

//==  VARIABLES  ===============================
                                              //
	struct addrinfo hints, *res;              //
	int AIlen = sizeof(struct addrinfo);      //
	Socket socFD;                             //
	int err;                                  //
                                              //
//==============================================

	//Création de la socket
	socFD = socket(AF_INET, SOCK_STREAM, 0);
	if (socFD == 0) {
		ecrireErreur("Erreur socket", 1);
	}

	//Instanciation des addrinfo
	memset(&hints, 0, sizeof(struct addrinfo));
	hints.ai_family   = AF_INET;   // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP 

	//récupération des infos du serveur
	err = getaddrinfo(adresseDist, portDist, &hints, &res);
	if (err) {
		ecrireErreur("Erreur get address info", 1);
	}

	//Connexion au serveur
	err = connect(descSock, res->ai_addr, res->ai_addrlen);
	if (err) {
		ecrireErreur("Erreur get address info", 1);
	}

	//On affiche à l'écran
	ecrireMessage("Création de la socket cliente");

	//Retourne le fichier descripteur de la socket
	return sockFD;
}

void traitementCom(const Socket socCom){
	//Variables locales pour processus
	char login[50], serveur[50], buffer[110], message[50];

	//On demande les identifiants de connexion à l'utilisateur
	strcpy(message, "220 Login@Adresse : ")
	write(socCom, "220 Login@Adresse : ", sizeof(message));
	read(socCom, buffer, sizeof(buffer));

	//On parse le login et l'adresse
	scanf(buffer "%[^@]@%S", login, serveur);

	//On crée la socket qui communiquera avec le serveur FTP
	creerSocketClient(serveur, "21");
	communication();
}



void attenteConnexionClient(const Socket socRDV, Socket * socCom){
	int err, addLen;
	sockaddr addrClient;
	char message[256];

	err = listen(socRDV, LISTEN_LENGTH);
	if(err){
		ecrireErreur("Erreur listen", 2);
	}
	err = accept(socRDV, &addrClient, &addLen);
	if (err == -1) {
		ecrireErreur("Erreur accept");
	}
	else {
		*(socCom) = err;
		ecrireInfo("Connecté à un client");
		strcpy(message, "Bienvenue sur le proxy!\n");
		write(*(socCom), message, sizeof(message));
	}
}


void communication(){

}


void transClientServeur(Socket socFD, char * buffer, int * bufLen){

}

void transServeurClient(Socket socFC, char * buffer, int * bufLen){

}