#include "headers/includes.h"


int main(int argc, char const *argv[]) {

//==== VARIABLES =====================================
													//
	struct sockaddr_in si_serv;						//
	char buffer[MAX_BUFF_LEN], servAddr[IP_LEN],	//
		qname[MAX_BUFF_LEN],						//
		qnameEncode[MAX_BUFF_LEN];					//
	unsigned char addrIP[4];						//
	Socket sockCom;									//
	int portServeur = 0, i, buff_len = 0,			//
		qname_len;									//
	socklen_t si_serv_len = sizeof(si_serv);		//
													//
//====================================================

	//Vérification des paramètres
	if(argc != 4 && argc != 6) {
		printf(" [!] Nombre d'arguments incorrect\n");
		return -1;
	}
	i=1;
	while(i < argc - 1){
		//Reconnaissance de l'adresse
		if(strcmp(argv[i], "-s") == 0){
			strcpy(servAddr, argv[i+1]);
		}
		//Reconnaissance du port
		else if(strcmp(argv[i], "-p") == 0){
			portServeur = atoi(argv[i+1]);
		}
		//Erreur
		else {
			printf(" [!] Paramètre inconnu : %s sont autorisés : -p -s\n", argv[i]);
			return -2;
		}
		i += 2;
	}

	//Si l'adresse surveur est toujour à NULL
	if(servAddr == NULL){
		printf(" [!] Aucune adresse spécifiée %s\n", argv[i]);
		return -2;
	}
	//Vérification du numéro de port
	if(portServeur == 0){
		portServeur = PORT_DEFAUT;
	}

	//Enregistrement de la question DNS
	strcpy(qname, argv[argc-1]);

	printf("%s -> %s:%d\n", qname, servAddr, portServeur);

	//Création de la socket
	initialiserSocketUDP(portServeur, servAddr, &sockCom, TYPE_CLIENT, &si_serv);

	//Contruction de la requete
	encode_domain_name_string(qname, qnameEncode, &qname_len);
	buff_len = build_dns_request(buffer, qnameEncode);
	//Envoie de la requete
	if ( (sendto(sockCom, buffer, buff_len, 0, (struct sockaddr*) &si_serv, si_serv_len)) < 0) {
	    perror(" [!] Erreur sendto");
	    exit(99);
	}
	printf(" (i) Question envoyée au serveur...\n");

	//Reception d ela reponse
	buff_len = recvfrom(sockCom, buffer, sizeof(buffer), 0, (struct sockaddr *) &si_serv, &si_serv_len);
	if(buff_len < 0){
		perror("Erreur recvfrom");
		exit(6);
	}
	
	printf(" (i) Réponse reçue : ");
	//Test si erreur name
	if(buffer[3] & 3){
		printf("Error name\n");
	}
	//Affichage de la réponse
	else {
		ip_from_answer(buffer, addrIP);
		printf("%d.%d.%d.%d\n", addrIP[0], addrIP[1], addrIP[2], addrIP[3]);
	}

    close(sockCom);
    return 0;

}