#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

//Enumération booléenne
typedef enum boolean boolean;
enum boolean{
	FAUX = 0,
	VRAI = -1
};

//Structure de message : type de message (0,1) + message (valeur entière)
typedef struct {
	boolean type;
	int message;
} message_t;

//Structure buffer
typedef struct {
	pthread_mutex_t mutex;
	message_t* messages;
	int iDepot;
	int iRetrait;
	boolean estVide;
} buffer_t;

//Structure param : correspond aux paramètres envoyés aux fonctions threads
typedef struct {
	int rang;
	int max;
} param_t;

//Variables globales
static int nbMessages;
static buffer_t* buffer;

//Fonction exécutées par les threads créés dans le programme
void * fctThreadDepot(void *);
void * fctThreadRetrait(void *);


/**
* Permet de déposer un message dans le buffer global
*/
void deposerMessage(message_t m);

/**
* Permet de lire le dernier message du buffer global
*/
message_t retirerMessage();