#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

int i=0;

void sigint(int signal){
	i++;
	printf("\nEnvoie n° %d de ctrl+C\n", i);
	if(i==3) {
		printf("Sortie du prog\n");
		exit(0);
	}
}

int main(){
	//Variables
	struct sigaction act;
	act.sa_handler = sigint;
	act.sa_flags = SA_SIGINFO;
	sigemptyset(&act.sa_mask);

	printf("Début du programme\n");
	//Interception du signal
	sigaction(SIGINT, &act, NULL);
	while(1);
	return 0;
}