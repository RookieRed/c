

pid_t * fils;
int count = 0;

void handler(int sig){
	//Reception du siga
	if(sig = SIGALRM){
		for (int i = 0; i < count; ++i) {
			kill(fils[i], SIGUSR1);
		}
	}
}


int main(int argc, char const *argv[]) {
	//Variables
	int periode = atoi(argv[1]);
	count = argc-2;
	fils = (pid_t*) malloc((argc-1)*sizeof(pid_t));
	for (int i = 0; i < count; ++i)	{
		fils[i] = toi(argv[i+2]);
	}

	//Liaison fct -> sig
	signal(SIGALRM, handler);
	//On s'envoie un SIGALARM toutes les n secondes
	alarm(periode);

	//Boucle infinie de la mort qui tue
	while(1);

	return 0;
}