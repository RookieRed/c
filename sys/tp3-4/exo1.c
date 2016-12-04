#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

pthread_mutex_t mutex1, mutex2;

void * executionThread(void * param) {
	//Variables
	int val1 = 0, val2 = 0, num = *((int*) param);
	pthread_t ptid = pthread_self();
	pthread_mutex_t *mutexSelf, *mutexAlter;
	
	//Gestion des mutexs
	if(num == 0){
		mutexSelf = &mutex1;
		mutexAlter = &mutex2;
	}
	else {
		mutexSelf = &mutex2;
		mutexAlter = &mutex1;
	}
	
	while(val1 < 5){
		pthread_mutex_lock(mutexSelf);
		printf("Thread %d affichage %d-%d\n", ptid, val1, val2);
		val2 = (val2 + 1) % 2;
		if(val2 == 0){
			val1++;
		}
		printf("Thread %d affichage %d-%d\n", ptid, val1, val2);
		pthread_mutex_unlock(mutexAlter);
	}
	
}

int main(int argc, char const *argv[]) {
	//Variables
	int nbThread = 2, i;
	pthread_t ptid[nbThread];
	pthread_attr_t attr;
	
	pthread_mutex_init(&mutex1, NULL);
	pthread_mutex_init(&mutex2, NULL);
	pthread_mutex_lock(&mutex1);
	
	//Initialisation des threads
	pthread_attr_init(&attr);
	int num[2];
	for(i=0; i<nbThread; i++){
		num[i] = i;
		if(pthread_create(&ptid[i], &attr, executionThread, num+i) != 0){
				perror("Erreur lancement thread 1");
				exit(99);
		}
	}
	
	//Jointure des threads
	for (i=0; i<nbThread; i++) {
		pthread_join(ptid[i], (void**) NULL);
	}
	
	return 0;
}