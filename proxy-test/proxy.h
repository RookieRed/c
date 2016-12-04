/***********************************************
**                 FICHER PROXY.H             **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/

#include "types.h"


/**
* Instancie une socket type serveur
* @param  : le port sur lequel la socket écoute
* @return : la socket crée
*/
Socket creerSocketServeur(const char *);


/**
* Instancie une socket type cliente et la connecte au serveur passé en params
* @param l'adresse du serveur
* @param le port du serveur
* @return la socket crée
*/
Socket creerSocketClient(const char *, const char *);


/**
* Procédure générale du traitement de la communication pour connexion entre le proxy le client et le serveur
* S'utilise à la suite d'un fork dans un nouveau processus
* @param La socket crée lors de la premiere connexion avec le client
* @param Le socket de communication proxy - serveur
*/
void traitementConnexionClient(const Socket, Socket *);


/**
* Attend une nouvelle connexion avec un client et gère cette nouvelle connexion
* @param La socket de rendez-vous instanciée sur lequel on attend la connexion
* @param la nouvelle socket de communication entre le proxy et le client
*/
void attenteConnexionClient(const Socket, Socket *);


/**
* Gère la communication générale entre client et serveur: transmet les messages de l'un vers l'autre
* et se termine lorsque le client quitte la session ftp
* @param: Le descripteur de socket client proxy
* @param: Le descripteur de socket proxy serveur
*/
void communication(const Socket, const Socket);

/**
* Procédure qui transmet directement un message du client au serveur
* @param Socket de la connexion client - proxy
* @param Socket de la connexion proxy - serveur
*/
void transClientServeur(const Socket, const Socket);


/**
* Procédure qui transmet directement un message du serveur au client
* @param Socket de la connexion client - proxy
* @param Socket de la connexion proxy - serveur
*/
void transServeurClient(const Socket, const Socket);
