/***********************************************
**               FICHIER TYPES.H              **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/

#ifndef PROXY_PREPROC

	#define PROXY_PREPROC

	#include <stdio.h>
	#include <stdlib.h>
	#include <sys/socket.h>
	#include <netdb.h>
	#include <string.h>
	#include <sys/types.h>
	#include <unistd.h>

	//Définition du type booléen
	#define TRUE -1
	#define FALSE 0
	//typedef enum booleen {FALSE=0, TRUE=-1};

	//Définition du type Socket
	typedef int Socket;

	//Constantes du programme
	#define LOCAL_ADDRESS    "127.0.0.1"   // Définition de l'adresse IP d'écoute
	#define LOCAL_PORT       "5678"        // Définition du port d'écoute, si 0 port choisi dynamiquement
	#define LISTEN_LENGTH     1            // Taille du tampon de demande de connexion
	#define MBL               1024         // Taille maximale du buffer (echange sur réseau)
	#define MHL               64           // Taille maximale des adresses IP
	#define MPL               6            // Taille maximale des ports

#endif
