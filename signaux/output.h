/***********************************************
**              FICHIER OUTPUT.H              **
************************************************/


/**
* Ecrire erreur : fonction qui affiche sur la sortie standard une erreur et quitte le programme
* @param message : le message d'erreur à afficher
* @param code : le code d'erreur à la sortie, si 0 alors pas d'erreurs
*/
void ecrireErreur(const char * message, const int code);

/**
* Ecrire Info : fonction qui affiche sur la sortie standard une information simple
* @param message : le message à afficher
*/
void ecrireInfo(const char * message);