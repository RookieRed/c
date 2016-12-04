/***********************************************
**                 FICHER PROXY.H             **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/

#include "proxy.h"
#include "output.h"



		/***************************
		**  CREER SOCKET SERVEUR  **
		***************************/

Socket creerSocketServeur(const char * port){

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
	memset(&res, 0, AIlen);
	hints.ai_flags 		= AI_PASSIVE;   // Mode serveur (passif)
	hints.ai_socktype 	= SOCK_STREAM;  // protocole tcp
	hints.ai_family 	= AF_INET;      // famille adresse : IPv4

	//Récupération des address info
	err = getaddrinfo(LOCAL_ADDRESS, port, &hints, &res);
	if (err == -1) {
		ecrireErreur("Erreur get address info", 1);
	}

	//Publication de la socket
	err = bind(socFD, res->ai_addr, res->ai_addrlen);
	if (err) {
		ecrireErreur("Erreur bind", 2);
	}
    err = listen(socFD, LISTEN_LENGTH);
    if(err){
        ecrireErreur("Erreur listen", 1);
    }

	//On affiche à l'écran
	ecrireInfo("Socket serveur instanciée");


	//Retourne le fichier descripteur de la socket
	return socFD;
}




		/**************************
		**  CREER SOCKET CLIENT  **
		**************************/

Socket creerSocketClient(const char * adresseDist, const char * portDist){

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
		ecrireErreur("Erreur socket", 0);
		return 0;
	}

	//Instanciation des addrinfo
	memset(&hints, 0, AIlen);
	hints.ai_family   = AF_INET;     // IPv4
	hints.ai_socktype = SOCK_STREAM; // TCP 

	//récupération des infos du serveur
	err = getaddrinfo(adresseDist, portDist, &hints, &res);
	if (err == -1) {
		ecrireErreur("Erreur get address info", 0);
		return 0;
	}

	//Connexion au serveur
	err = connect(socFD, res->ai_addr, res->ai_addrlen);
	if (err) {
		ecrireErreur("Erreur get address info", 0);
		return 0;
	}

	//On affiche à l'écran
	ecrireInfo("Création de la socket cliente");

	//Retourne le fichier descripteur de la socket
	return socFD;
}



		/**********************************
		**  ATTENTE DE CONNEXION CLIENT  **
		**********************************/

void attenteConnexionClient(const Socket socRDV, Socket * socCom){

//==  VARIABLES  ===============================
                                              //
	int err, addLen, mesLen=512;              //
	struct sockaddr_storage addrClient;       //
	char message[mesLen];                     //
                                              //
//==============================================

	ecrireInfo("En attente d'une connexion entrante...");
	//Fonction accept
	err = accept(socRDV, (struct sockaddr *) &addrClient, (socklen_t *) &addLen);
	if (err == -1) {
		ecrireInfo("Erreur accept");
	}
	else {
		*(socCom) = err;
		ecrireInfo("Connecté à un client");
		memset(message, 0, mesLen);
		strcpy(message, "220 - Bienvenue sur le proxy! Connectez vous log@hote\n");
		write(*socCom, message, strlen(message));
	}
}




		/**************************************
		**  TRAITEMENTS DE LA COMMUNICATION  **
		***************************************/

void traitementConnexionClient(const Socket socCom, Socket * socProxServ){

//==  VARIABLES  ===============================
                                              //
	char login[50], buffer[MBL],              //
	     message[50],                         //
	     serveur[MHL], mdp[50];               //
	int n=0;                                  //
                                              //
//==============================================

	//On attend la réponse cotennant login@hote
	memset(buffer, 0, sizeof(buffer));
	read(socCom, buffer, sizeof(buffer));
	ecrireInfo("Log reçu");

	//On parse le login et l'adresse
	sscanf(buffer, "%[^@]@%s", login, serveur);

	//Connexion au serveur, si échec on recommence l'opération
    *socProxServ = creerSocketClient(serveur, "21");
    if(socProxServ == 0){
    	//On indique au client le problème
		memset(message, 0, sizeof(message));
		strcpy(message, "431 Erreur de connexion à l'ĥote, recommencez\n");
		write(socCom, message, strlen(message));
    }
    //Si non on lance la connexion au serveur
    else{
	    //On se connecte au serveur FTP
	    ecrireInfo("Tentaive de login au serveur...");
		write(*socProxServ, "", 0);
		memset(buffer, 0, sizeof(buffer));
	    read(*socProxServ, buffer, sizeof(buffer));
	    printf("\n%s\n", buffer);

	    //On envoie le login si 220
	    if(strstr(buffer, "220") != NULL){
	    	ecrireInfo("Envoie du login");
		    strcat(login, "\n");
		    printf(login);
			write(*socProxServ, login, strlen(login));

			//On attend la réponse du serveur
			memset(buffer, 0, sizeof(buffer));
			read(*socProxServ, buffer, sizeof(buffer));
	    	printf("%s\n", buffer);

			//On demande et récupère le mot de passe avant de l'envoyer au serveur
			write(socCom, buffer, strlen(buffer));
			ecrireInfo("En attente du mot de passe");
			memset(mdp, 0, sizeof(mdp));
			read(socCom, mdp, sizeof(mdp));
			ecrireInfo("Mot de passe reçu");
			printf(mdp);

		    //Envoie du mdp
			write(*socProxServ, mdp, strlen(mdp));

			//On récupère la réponse du serveur et on l'envoie au client
	    	transServeurClient(socCom, *socProxServ);

			//Le client répond SYST on transmet au serveur
			transClientServeur(socCom, *socProxServ);

			//On récupère la réponse du serveur et on l'envoie au client
			transServeurClient(socCom, *socProxServ);

		}
	}
}



		/********************
		**  COMMUNICATION  **
		********************/

void communication(const Socket socCP, const Socket socPS){

	char buffer[MBL], portCP[MPL],
	     portPS[MPL], hote[MHL], port[MPL];
	Socket socComCP=FALSE, socComPS=FALSE;

	ecrireInfo("Début de la communication");

	//Tant que l'utilisateur ne quitte pas la session
	memset(buffer, 0, sizeof(buffer));
	while(strstr(buffer, "QUIT") == NULL){

		//On récupère les commandes client
		memset(buffer, 0, sizeof(buffer));
		read(socCP, buffer, sizeof(buffer));

		// Si on transfère en mode actif
		if (strstr(buffer, "PORT") != NULL){

			//On parse la commande PORT
			int a1, a2, a3, a4, p1, p2 = 0;
			sscanf(buffer, "PORT %d,%d,%d,%d,%d,%d", &a1, &a2, &a3, &a4, &p1, &p2);
			sprintf(hote, "%d.%d.%d.%d", a1, a2, a3, a4);
			sprintf(port, "%d", p1*256+p2);

			//On crée le socket communication client-proxy
			socComCP = creerSocketClient(port, hote);

			//On envoie le PASV au serveur
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, "PASV\n");
			write(socPS, buffer, strlen(buffer));

			//On récupère les infos de connexion pour le nouveau socket
			memset(buffer, 0, sizeof(buffer));
			read(socPS, buffer, sizeof(buffer));

			//On parse les infos de connexion
			sscanf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &a1, &a2, &a3, &a4, &p1, &p2);
			sprintf(hote, "%d.%d.%d.%d", a1, a2, a3, a4);
			sprintf(port, "%d", p1*256+p2);

			//On crée la socket communication proxy-serveur
			socComPS = creerSocketClient(port, hote);
		}	
		else if(strstr(buffer, "LIST") != NULL){

			//On envoie le LIST au serveur
			write(socPS, buffer, strlen(buffer));

			// on lis la réponse du serveur FTP
			memset(buffer, 0, sizeof(buffer));
			read(socPS, buffer, sizeof(buffer));

			// on redirige vers le client
			memset(buffer, 0, sizeof(buffer));
			strcpy(buffer, "150 Attente de données\n");
			write(socCP, buffer, strlen(buffer));

			// on effectue la transaction (données)
			transServeurClient(socComCP, socComPS);

			// on ferme le bus de données
			close(socComCP);
			close(socComPS);

			// on lit la réponse du serveur FTP
			memset(buffer, 0, sizeof(buffer));
			read(socPS, buffer, sizeof(buffer)); // renvoie 226 fin de transf

			// on rediriga au client FTP
			memset(buffer, 0, sizeof(buffer));
			write(socCP, "226 Fin du transfert\n", strlen("226 Fin du transfert\n"));


		}

		//On envoie la requête au serveur
		write(socPS, buffer, strlen(buffer));

		//On retransmet la réponse du serveur au client
		transServeurClient(socCP, socPS);
	}

	//On envoie "QUIT" au serveur
	write(socPS, "QUIT\n", strlen("QUIT\n"));

	//On referme les sockets
	close(socCP);
	close(socPS);
	if(socComCP){close(socComCP);}
	if(socComPS){close(socComPS);}
}




void transClientServeur(const Socket socClientProx, const Socket socProxServ){
	
	char buffer[MBL];

	ecrireInfo("Transfert client serveur :");
	//On récupère la réponse du serveur et on l'envoie au client
	memset(buffer, 0, sizeof(buffer));
    read(socClientProx, buffer, sizeof(buffer));

	printf("%s\n", buffer);
	ecrireInfo("Envoie de la réponse du client");

	write(socProxServ, buffer, strlen(buffer));
}



void transServeurClient(const Socket socClientProx, const Socket socProxServ){

	 char buffer[MBL];

	ecrireInfo("Transfert Serveur Client:");
	//On récupère la réponse du serveur et on l'envoie au client
	memset(buffer, 0, sizeof(buffer));
    read(socProxServ, buffer, sizeof(buffer));

	printf("\n%s\n", buffer);
	ecrireInfo("Envoie de la réponse du serveur");

	write(socClientProx, buffer, strlen(buffer));
}
