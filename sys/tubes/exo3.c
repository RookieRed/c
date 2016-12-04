#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <string.h>

void codeAlpha(int tube[]){
	//Variables
	int tab[26], i;
	char c;
	//Initialisation du tableau
	for(i=0; i<26; i++){
		tab[i] = 0;
	}
	close(tube[1]);
	//Récupération des connées
	while(read(tube[0], &c, 1)){
		c = tolower(&c);
		tab[(int) (c - 'a')]++;
	} 
	//Affichage des résultats
	for(i=0; i<26;i++){
		printf("%s => %d\n", (char) (c+'a'), tab[i]);
	}
	close(tube[0]);
}

void codeNum(int tube[]){
	//Variables
	int tab[10], i, somme=0;
	char c;
	//Initialisation du tableau
	for(i=0; i<10; i++){
		tab[i] = 0;
	}
	
	close(tube[1]);
	//Récupération des connées
	while(read(tube[0], &c, 1)){
		somme += atoi(&c);
	}
	printf("Somme des chiffres : %d\n", somme);
	close(tube[0]);
}


int main(int argc, char const *argv[]) {

	//Variables
	int tubeAlpha[2], tubeNum[2];
	char c = '0';
	int i = 0;

	if(pipe(tubeAlpha)){
		perror("Erreur pipe");
		exit(99);
	}
	//Création du fils Alpha
	switch(fork()){
		case -1: {
			perror("Erreur fork");
			exit(99);
		}
		case 0: {
			codeAlpha(tubeAlpha);
			exit(0);
		}	
	}
	close(tubeAlpha[0]);

	//Création du second tube
	if(pipe(tubeNum)){
		perror("Erreur pipe");
		exit(99);
	}
	//Processus num
	switch(fork()){
		case -1: {
			perror("Erreur fork");
			exit(99);
		}
		case 0: {
			codeNum(tubeNum);
			exit(0);
		}
	}
	close(tubeNum[0]);

	//Processus père
	while(read(0, &c, 1)){
		c = argv[1][i];
		if(isalpha(c)){
			write(tubeAlpha[1], &c, 1);
		}
		else if(isdigit(c)){
			write(tubeNum[1], &c, 1);
		}
	}
	//Fermeture des tubes
	close(tubeAlpha[1]);
	close(tubeNum[1]);

	//Jointure
	wait(NULL);
	wait(NULL);
	printf("Fin du programme\n");
	return 0;
}


