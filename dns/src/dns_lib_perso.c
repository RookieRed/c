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
	socklen_t client_len; 					//
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

			//Récpération du TLD
			int i, tld_len = 0,
				qname_len = strlen(qname);
			char tld[qname_len], tldMiroir[qname_len];
			//Parcours inverse du qname jusqu'au premier point
			do {
				tldMiroir[tld_len] = qname[qname_len-tld_len-1];
				tld_len++;
			} while(qname[qname_len-tld_len-1] != '.');
			//Récupération du tld grace au miroir
			for (i = 0; i < tld_len; ++i) {
				tld[i] = tldMiroir[tld_len-i];
			}
			printf("%s - %d - %d\n", tld, tld_len, strlen(tld));
			//Si serveur TLD, récupération du reste de la question
			if(typeServ == TLD){

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

void interceptionSIGINT(int sig){
	printf("\n (i) Bye bye !\n");
	exit(0);
}
