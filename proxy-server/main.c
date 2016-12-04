/***********************************************
** FICHIER MAIN.C POINT D'ENTRÉE DU PROGRAMME **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/


#include "src/header/types.h"

#define LOCAL_ADDRESS    "127.0.0.1"   // Définition de l'adresse IP d'écoute
#define LOCAL_PORT       "5678"        // Définition du port d'écoute, si 0 port choisi dynamiquement
#define LISTEN_LENGTH     1            // Taille du tampon de demande de connexion
#define MAX_BUFFER_LENGTH 1024         // Taille maximale du buffer (echange sur réseau)
#define MAX_HOST_LENGTH   64           // Taille maximale des adresses IP
#define MAX_PORT_LENGTH   6            // Taille maximale des ports

void getLoginEtAdresseDuServeur(buffer, char *login, char *adresse);

int main(){
	// VARIABLES
    Socket socRDV, socCom;
    pid_t pid;

    //On crée la socket de RDV
    socRDV = creerSocketServeur(LOCAL_PORT);


    while(TRUE){
    	//On récupère les connexions entrantes et on créer le processus fils
    	attenteConnexionClient(socRDV, &socCom);

    	//Gestion de la connexion client - proxy
    	pid = fork();
    	if(pid == 0){
    		traitementCom(socCom);
    	}
    	else if(pid == -1){
    		ecrireErreur("Erreur fork, communication impossible", 3);
    	}
    	//on referme la socket de RDV
    	close(socRDV);
    }

}
