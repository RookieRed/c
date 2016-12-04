#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <semaphore.h> 
#include <string.h>

#define MAX_LEN 50

//Enumération booléenne
typedef enum bool bool;
enum bool{
	false = 0,
	true = 1
};

//Structure buffer
typedef struct {
	sem_t casesVides;
	sem_t casesPleines;
	char* messages[10];
	int iDepot;
	int iRetrait;
	pthread_mutex_t mutexDepot;
	pthread_mutex_t mutexRetrait;
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
void deposerMessage(char *m);

/**
* Permet de lire le dernier message du buffer global
*/
char * retirerMessage();

int main(int argc, char const *argv[]) {
	
	//Vérification des arguments
	if(argc != 6) {
		printf("Erreur arguments !\n");
		return -1;
	}
	
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
	buffer_t b;

	//Création du buffer
	for(i=0; i<nbMessages; i++){
		b.messages[i] = malloc(MAX_LEN*sizeof(char));
	}
	sem_init(&(b.casesVides), 0, nbMessages);
	sem_init(&(b.casesPleines), 0, 0);
	pthread_mutex_init(&(b.mutexRetrait), NULL);
	pthread_mutex_init(&(b.mutexDepot), NULL);
	b.iRetrait = 0;
	b.iDepot = 0;
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
	char m[MAX_LEN];
	//Dépots des messages
	for(i=0; i<p.max; i++){
		sprintf(m, "Bonjour %d de %d", i, p.rang);
		printf(" [P %d] Demande de depot : %d - %s\n", p.rang, i, m);
		deposerMessage(m);
		printf("\tMessage depose\n");
	}
	pthread_exit(NULL);
}

void * fctThreadRetrait(void * param){
	//Variables
	int i;
	param_t p = (*((param_t*)param));
	char *m;
	//Retrait & lecture des messages
	for(i=0; i<p.max; i++){
		printf(" [C %d] Demande de retrait : %d\n", p.rang, i);
		m = retirerMessage();
		printf("\tMessage lu = %s\n", m);
		free(m);
	}
	pthread_exit(NULL);
}

void deposerMessage(char *m){
	//Entrée dans la zone critique
	pthread_mutex_lock(&(*buffer).mutexDepot);
	sem_wait(&(*buffer).casesVides);
	//Dépot du message
	strcpy((*buffer).messages[(*buffer).iDepot], m);
	(*buffer).iDepot = ((*buffer).iDepot+1) % nbMessages;

	//Sortie de la zone critique
	sem_post(&(*buffer).casesPleines);
	pthread_mutex_unlock(&(*buffer).mutexDepot);

}

char * retirerMessage(){
	//Variables
	char *m = malloc(MAX_LEN*sizeof(char));

	//Entrée dans la zone critique
	pthread_mutex_lock(&(*buffer).mutexRetrait);
	sem_wait(&(*buffer).casesPleines);
	strcpy(m, (*buffer).messages[(*buffer).iRetrait]);
	(*buffer).iRetrait = ((*buffer).iRetrait+1) % nbMessages;

	//Sortie de la zone critique
	sem_post(&(*buffer).casesVides);
	pthread_mutex_unlock(&(*buffer).mutexRetrait);

	return m;
}
