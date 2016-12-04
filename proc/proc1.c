/************************
*     TD MACHINE 1      *
*************************
* GESTION DES PROC FILS *
************************/

#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void main(){

	//Déclaration des variables
	time_t t = time(NULL);
	pid_t pid;
	FILE * f;

	//Ouverture du fichier de sortie
	f = fopen("proc1.trace", "w+");
	if (f == NULL) {
		perror(" [x] Erreur ouverture fichier proc1.trace");
		exit(1);
	}

	//Accueil du programme père
	fprintf(f, "\t -> Programme proc1\n");
	fprintf(f, " [P] PID\t\t  = %d\n", getpid());
	fprintf(f, " [P] IUD\t\t  = %d\n",getuid());
	fprintf(f, " [P] Username\t\t  = %s\n", getlogin());
	fprintf(f, " [P] Heure & date système = %s", ctime(&t));

	//Lancement des programmes fils
	pid = fork();
	switch(pid) {

		case -1:
			perror(" [x] Erreur fork");
			exit(1);

		case 0:
			fprintf(f, "\t==> Processus fils 1\n");
			fprintf(f, "  [f1] PID fils = %d\n", getpid());
			exit(0);
		default:
			fprintf(f, " [P] PID Fils 1\t\t  = %d\n", pid);
	}

	fprintf(f, "fin\n");
	fclose(f);
	printf("OK\n");
}