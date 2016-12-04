#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>
#include <sys/time.h>

#define BUF_MAX 50
#define MAX_OBJ 96
#define CAP_MAX 640400
#define NB_FIC 3

//Variables globales
int s[MAX_OBJ][CAP_MAX];

//Fonctions
double get_time();
//Les fonctions suivantes retournent leur temps d'execution
double versionNonParallele(int*, int*, int, int) ;
double versionParallele1(int*, int*, int, int, int);

int main(int argc, char * argv[]){
    
    //Variables
    int c, n, E[MAX_OBJ], u[MAX_OBJ], m[MAX_OBJ];
    char buf[BUF_MAX];

    //Noms des fichiers de données
    char tabFic[NB_FIC][5];
    strcpy(tabFic[0], "pb1");
    strcpy(tabFic[1], "pb2");
    strcpy(tabFic[2], "pb6");
    
    for (int k=0; k<NB_FIC; k++){
        printf("\n\n\t\t-=== FICHIER %s ===-\n", tabFic[k]);

        //RaZ des variables
        c = 0;
        n = 0;
        memset(E, 0, MAX_OBJ);
        memset(m, 0, MAX_OBJ);
        memset(u, 0, MAX_OBJ);
        memset(s, 0, MAX_OBJ*CAP_MAX);

        double t1 = get_time();
        //Lecture du fichier
        FILE *fic = fopen(tabFic[k], "r");
        if(fic == NULL){
            perror("Erreur fopen");
            exit(99);
        }
        
        //Parseur
        while(fgets(buf, BUF_MAX, fic) != NULL && n <= MAX_OBJ){
            //Premiere ligne
            if(n==0){
                sscanf(buf, "c %d", &c);
            }
            else {
                sscanf(buf, "o %d %d", &m[n-1], &u[n-1]);
            }
            n++;
        }
        fclose(fic);
        n--;

        printf("Temps de lecture du fichier : %d\n", t1 - get_time());

        //Affichage des rsultats
        double t = versionNonParallele(m, u, c, n);
        printf("Fin version non parallele : temps exec = %f, utilité max : %d\n", t, s[n-1][c]);
        printf("Fin version parallele 1 (2 threads) : temps exec = %f utilité max : %d\n",
            versionParallele1(m, u, c, n, 2), s[n-1][c]);
        printf("Fin version parallele 1 (3 threads) : temps exec = %f utilité max : %d\n",
            versionParallele1(m, u, c, n, 3), s[n-1][c]);
        printf("Fin version parallele 1 (4 threads) : temps exec = %f utilité max : %d\n",
            versionParallele1(m, u, c, n, 4), s[n-1][c]);
        printf("Fin version parallele 1 (5 threads) : temps exec = %f utilité max : %d\n",
            versionParallele1(m, u, c, n, 5), s[n-1][c]);
        
        //Recherche des objets selectionnés
        int i = n-1, j = c, e = 0, trouve = 0;
        while(!trouve){
            if(s[i][j] != s[i-1][j]){
                j = j-m[i];
                i--;
                E[e] = 1;
            }
            else {
                i = i-1;
                E[e] = 0;
            }
            e--;
            if(i == 0){
                trouve = 1;
            }
        }
        if(s[i][j] > 0){
            E[e] = 1;
        }
        else {
            E[e] = 0;
        }

        //Afichage des éléments trouvés
        printf("Ensemble des objets selectionnés : \n");
        for (i = 0; i < n; ++i) {
            if(E[i])
                printf("%d ", i);
        }
        printf("\n");
    }

    return 0; 
}


double get_time() {
  struct timeval tv;
  gettimeofday(&tv, (void *)0);
  return (double) (tv.tv_sec + tv.tv_usec*1e-6)*1e4;
}

double versionNonParallele(int *m, int *u, int c, int n){
    double t1 = get_time();

    //Création de la grosse matrice sa mere
    for(int i=0; i<n; i++){
        for(int j=0; j<=c; j++){
            //Premiere ligne du tableau
            if(i==0){
                if(m[i] <= j){
                    s[i][j] = u[i];
                }
                else {
                    s[i][j] = 0;
                }
            }
            //Pas premiere ligne
            else {
                //on détermine si l'objet loge dans le sac
                if(m[i] > j){//Si trop grand : on prend le poids optiimal précédent
                    s[i][j] = s[i-1][j];
                }
                else { //Sinon on determine la meilleure utilité entre le sac avec l'objet, et sans l'objet
                    if(u[i] + s[i-1][j - m[i]] > s[i-1][j]){
                        s[i][j] = u[i] + s[i-1][j - m[i]];
                    }
                    else {
                        s[i][j] = s[i-1][j];
                    }
                }
            }
        }
    }
    return get_time() - t1;
}

double versionParallele1(int *m, int *u, int c, int n, int nbThread) {
     double t1 = get_time();

    //Création de la grosse matrice sa mere
    for(int i=0; i<n; i++){
        #pragma omp parallel for num_threads(nbThread)
        for(int j=0; j<=c; j++){
            //Premiere ligne du tableau
            if(i==0){
                if(m[i] <= j){
                    s[i][j] = u[i];
                }
                else {
                    s[i][j] = 0;
                }
            }
            //Pas premiere ligne
            else {
                //on détermine si l'objet oge dans le sac
                if(m[i] > j){//Si trop grand : on prend le poids optiimal précédent
                    s[i][j] = s[i-1][j];
                }
                else { //Sinon on determine la meilleure utilité entre le sac avec l'objet, et sans l'objet
                    if(u[i] + s[i-1][j - m[i]] > s[i-1][j]){
                        s[i][j] = u[i] + s[i-1][j - m[i]];
                    }
                    else {
                        s[i][j] = s[i-1][j];
                    }
                }
            }
        }
    }

    return get_time() - t1;
}