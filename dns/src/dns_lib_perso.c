#include "headers/includes.h"


	/*****************************
	**  INITIALISER SOCKET UDP  **
	******************************/

void initialiserSocketUDP(const int port, const char *adresseServeur, Socket *sockCom,
	type_sock type, struct sockaddr_in *si_serv){

//==== VARIABLES =================================
                                                //
	struct sockaddr_in si_hote;					//
	int si_len = sizeof(struct sockaddr_in),	//
		addLen, err;							//
                                                //
//================================================

	//Vérification des paramètres
	if(type == TYPE_CLIENT && (adresseServeur == NULL || si_serv == NULL)){
		printf("[!] Erreur arguments initialiserSocket\n");
		return ;
	}

	//Création de la socket
	*sockCom = socket(AF_INET, SOCK_DGRAM, 0);
	if (*sockCom < 0) {
		perror(" [!] Erreur socket");
		exit(1);
	}

	//Instanciation du sockaddr_in
	memset(&si_hote, 0, si_len);
	si_hote.sin_port 		= htons(port);
	si_hote.sin_family 		= AF_INET;
	si_hote.sin_addr.s_addr = ( (type == TYPE_CLIENT)
		? inet_addr(adresseServeur)
		: htonl(INADDR_ANY) );

	//Enregistrement des si_info (client)
	if(type == TYPE_CLIENT){
		memcpy(si_serv, &si_hote, sizeof(si_hote));
	}

	//Publication de la socket (serveur)
	if(type == TYPE_SERVEUR){
		err = bind(*sockCom, (struct sockaddr*) &si_hote, sizeof(si_hote));
		if (err) {
			perror(" [!] Erreur bind");
			exit(3);
		}
		//Informations sur l'état de connexion
		printf(" (i) Serveur en écoute...\n");
	}
	else {
		printf(" (i) Socket instanciée\n");
	}

}


		/*************************************
		**  COMMUNICATION CLIENT - SERVEUR  **
		*************************************/
void communicationServeurUDP(Socket sockCom, const char * fichierConf, char * domaine, int typeServ){

//==== VARIABLES =============================
                                    	    //
	char buffer[MAX_BUFF_LEN],				//
		qname[MAX_BUFF_LEN], *index,		//
		adresseIP[IP_LEN];                  //
	int buff_len = 0, reponse_len;          //
	struct sockaddr_in client = {0};		//
	socklen_t client_len = sizeof(client);	//
											//
//============================================

	//Initialisation du buffer
	memset(buffer, 0, sizeof(buffer));
	//Lecture de la requete du client

	buff_len = recvfrom(sockCom, buffer, sizeof(buffer), 0, (struct sockaddr *) &client, &client_len);
	if(buff_len < 0){
		perror("Erreur recvfrom");
		exit(6);
	}
	//On parse la requete pour en récupérer la question
	qname_from_question(buffer, buff_len, qname);
	
	bool err = true;
	//Vérification du domaine
	index = strstr(qname, domaine);
	if(index != NULL) {

		//Serveur racine ou TLD
		if((typeServ == ROOT) || (typeServ == TLD)){
			char *tabQname[10];
			int nbTokens = 0;
			
			//On parse le qname dans un tableau
			tabQname[nbTokens] = strtok(qname, ".");
			while(tabQname[nbTokens] != NULL) {
				nbTokens++;
				tabQname[nbTokens] = strtok(NULL, ".");
			}

			//On récupère l'objet de la recherche...
			char recherche[MAX_BUFF_LEN];
			if(typeServ == ROOT){ // ...le tld pour le serveur de type ROOT suivi d'un point ...
				sprintf(recherche, "%s.", tabQname[nbTokens-1]);
			}
			else { // ... le nom de domaine + le TLD pour le serveur TLD
				sprintf(recherche, "%s.%s.", tabQname[nbTokens-2], tabQname[nbTokens-1]);
			}

			char nomDomaine[MAX_BUFF_LEN];
			//Récupération du nom de domaine du serveur TLD
			if(get_rr_data_from_name(fichierConf, recherche, nomDomaine)){

				//Récupération de l'adresse ip correspondant au nom de domaine	
				if(get_rr_data_from_name(fichierConf, nomDomaine, adresseIP)){
					char unsigned ip[4];
					sscanf(adresseIP, "%d.%d.%d.%d", ip, ip+1, ip+2, ip+3);

					//Construction de la réponse DNS
					reponse_len = build_dns_additional(buffer, recherche, nomDomaine, ip);
					err = false;
				}
			}
		}

		//Serveur d'authorité
		else if(typeServ == AUTHORITATIVE){ 
			int sub_dom_len = index - qname;
			char subDomain[sub_dom_len];
			//Récupération du sous domaine
			memcpy(subDomain, qname, sub_dom_len);
			subDomain[sub_dom_len] = '\0';

			//Récupération de l'adresse ip correspondante
			if(get_rr_data_from_name(fichierConf, subDomain, adresseIP)){
				char unsigned ip[4];
				sscanf(adresseIP, "%d.%d.%d.%d", ip, ip+1, ip+2, ip+3);
				reponse_len = build_dns_answer(buffer, ip);
				err = false;
			}
		}
		//Cas non reconnu
		else { 
			printf(" [!] Type de serveur non reconnu\n");
		}
	}
	if(err) { //Error name
    	build_dns_reply_name_error(buffer);
    	reponse_len = buff_len;
	}
	//Envoie de la réponse au client
	sendto(sockCom, buffer, reponse_len, 0, (struct sockaddr *) &client, client_len);
}


bool comminucationServeurLocal(Socket sockCom, char * buffer, int buff_len,
	const char* adresseServeur, const int portServeur, int typeServ){
	
}

void interceptionSIGINT(int sig){
	printf("\n (i) Bye bye !\n");
	exit(0);
}
