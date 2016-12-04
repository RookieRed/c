/***********************************************
** FICHIER MAIN.C POINT D'ENTRÉE DU PROGRAMME **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/


#include "types.h"
#include "proxy.h"
#include "output.h"

int main(){

//==  VARIABLES  ===============================
                                              //
    Socket socRDV, socCom, socPS;             //
    pid_t pid;                                //
    int i, rapport;                           //
    char serveur[50];                         //
                                              //
//===============================================

    printf("\n=========================================\n");
    printf("\n=            PROXY SERVER 1.0           =\n");
    printf("\n=       Bienvenue dans le programme     =\n");
    printf("\n=========================================\n");

    i = TRUE;

    while(i){
        
        //On crée la socket de RDV
        socRDV = creerSocketServeur(LOCAL_PORT);
        
        //On initialise la socket de communication
        socCom = 0;
        attenteConnexionClient(socRDV, &socCom);


        //On récupère les connexions entrantes et on créer le processus fils
    	//Gestion de la connexion client - proxy
    	pid = fork();
    	switch(pid){
            case -1:
               ecrireErreur("Erreur fork, communication impossible", 3);
               break;
            //Traitement du nouveau processus associé à la connexion
            case 0:
                //Traitement de connexion : récupération du login et du nom de serveur correspondant
                traitementConnexionClient(socCom, &socPS);
                //On laisse le client serveur se parler entre eux
                communication(socCom, socPS);
                //Et quand tout fini bien on arrête le processus
                ecrireInfo("Fin de la communication");
                exit(0);
        }

    }
    ecrireInfo("Fin de l'écoute");

}

