#ifndef DNS_LIB_PERSO

#define DNS_LIB_PERSO

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <arpa/inet.h>	// inet_addr, htons
#include "dns_server_rr_files.h"
#include "dns_packet_parsing.h"

//Définition du type booléen
typedef enum {
	false,
	true
} bool;
#define false 0
#define true 1

//Enumération type client/serveur
typedef enum {
	TYPE_SERVEUR,
	TYPE_CLIENT
} type_sock;

//Definition du type socket
typedef int Socket;

//Constantes
#define LOCAL_ADDRESS	"127.0.0.1"	//Définition de l'adresse IP d'écoute
#define MAX_ATTENTE		5			//Longueur max de a file d'attente du listen
#define MAX_BUFF_LEN	1024		//Taille maximale des buffer pour la communication
#define IP_LEN          16          //Taille max d'une adresse IP
#define PORT_DEFAUT     53          //Port par défaut du protocole dns

/**
* Fonciton d'interception de SIGINT pour arrêter le démon
* @param : le signal intercepté
*/
void interceptionSIGINT(int);

/**
* Fonction générique instanciant une socket du type précisé en paramètre (client / serveur) en mode UDP
* 	-> TYPE_SERVEUR : met la socket sur écoute au port spécifié
* 	-> TYPE_CLIENT : créée la socket et la connecte à l'adresse ip et au port passé en paramètres
*
* @param  : le port sur lequel la socket écoute / port sur lequel la socket doit se connnecter
* @param  : l'adresse serveur à laquelle on souhaite se connecter (NULL si type serveur)
* @param  : un pointeur sur la socket de communication créée par la fonction
* @param  : le type de socket spécifié : SERVEUR ou CLIENT
* @param  : un pointeur sur la structure sockaddr_in qui recevra les infos serveur (type client, NULL si serveur)
*/
void initialiserSocketUDP(const int, const char *, Socket *, type_sock, struct sockaddr_in *);

/**
* Gère la communicaiton coté serveur entre un client et le serveur DNS comme il suit:
* 	-> écoute et récupération de la question du client
*	-> consultation du fichier de configuration
*	-> envoie de la réponse au client (l'adresse ip correspondante ou sinon un Error Name)
*
* @param : la socket de communication entre le client et le serveur
* @param : un string contenant le chemin vers le fichier RR
* @param : un string contenant le nom de mdomaine associé au serveur
* @param : Le type de serveur parmi AUTH, ROOT ou TLD
*/
void communicationServeurUDP(Socket, const char *, char *, int);

/**
* 
*/
bool comminucationServeurLocal(Socket sockCom, char* , int*, const char*, const int, int);


#endif
