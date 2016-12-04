#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h> 

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
	sem_t mutex;
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

int main(int argc, char const *argv[]) {
	//Arguments
	int i,
		nbProd  = atoi(argv[1]),
		nbCons  = atoi(argv[2]),
		maxProd = atoi(argv[3]),
		maxCons = atoi(argv[4]);
	nbMessages  = atoi(argv[5]);

	//Variables
	pthread_t ptCons[nbCons],
			  ptProd[nbProd];
	pthread_attr_t attr; 
	param_t p[nbCons+nbProd];
	buffer_t b = {
		.messages = malloc(nbMessages*sizeof(message_t)),
		.iDepot = 0,
		.iRetrait = 0
	};
	sem_init(&(b.mutex), 0, 1);
	buffer = &b;

	printf("[PERE] id = %d Debut du programme\n", pthread_self());

	//Initialisationd des threads
	pthread_attr_init(&attr);
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
	for(i=0; i<nbCons; i++){
		p[nbProd+i].rang = i;
		p[nbProd+i].max = maxCons;
		if(pthread_create(&ptCons[i], &attr, fctThreadRetrait, &p[nbProd+i]) != 0){
			perror("Erreur lancement thread");
			exit(99);
		}
	}
	for(i=0; i<nbProd; i++){
		p[i].rang = i;
		p[i].max = maxProd;
		if(pthread_create(&ptProd[i], &attr, fctThreadDepot, &p[i]) != 0){
			perror("Erreur lancement thread");
			exit(99);
		}
	}

	//Jointure des threads
	for (i=0; i<nbProd; i++) {
		pthread_join(ptProd[i], (void**) NULL);
		printf("Retour du thread prod numero %d\n", i);
	}
	for (i=0; i<nbCons; i++) {
		pthread_join(ptCons[i], (void**) NULL);
		printf("Retour du thread cons numero %d\n", i);
	}
	return 0;
}


void * fctThreadDepot(void * param){
	//Variables
	int i;
	param_t p = *((param_t*)param);
	message_t m;
	m.type = VRAI;
	printf("Producteur %d \n", p.rang);
	//Dépots des messages
	for(i=0; i<p.max; i++){
		printf("\tDemande de depot : %d\n", i);
		m.message = rand() % 256;
		deposerMessage(m);
		printf("\tMessage depose = %d, %d\n", m.message, m.type);
	}
	pthread_exit(NULL);
}

void * fctThreadRetrait(void * param){
	//Variables
	int i;
	param_t p = (*((param_t*)param));
	message_t m;
	printf("Consommateur %d \n", p.rang);
	//Retrait & lecture des messages
	for(i=0; i<p.max; i++){
		printf("\tDemande de retrait : %d\n", i);
		m = retirerMessage();
		printf("\tMessage lu = %d, %d\n", m.message, m.type);
	}
	pthread_exit(NULL);
}

void deposerMessage(message_t m){
	//Entrée dans la zone critique
	sem_wait(&(*buffer).mutex);
	(*buffer).messages[(*buffer).iDepot] = m;
	(*buffer).iDepot = ((*buffer).iDepot+1) % nbMessages;
	//Sortie de la zone critique
	sem_post(&(*buffer).mutex);

}

message_t retirerMessage(){
	//Variables
	message_t m;
	//Entrée dans la zone critique
	sem_wait(&(*buffer).mutex);
	m = (*buffer).messages[(*buffer).iRetrait];
	(*buffer).iRetrait = ((*buffer).iRetrait+1) % nbMessages;
	//Sortie de la zone critique
	sem_post(&(*buffer).mutex);
	return m;
}
