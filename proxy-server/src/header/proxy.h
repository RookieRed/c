/***********************************************
**                 FICHER PROXY.H             **
************************************************
**     Eloundou Cédric - Fauvet Guillaume     **
**                                            **
**              PROJET PROXY SERVER           **
************************************************/

#include "types.h"

Socket creerSocketServeur(const char * port);

Socket creerSocketClient(const char * adresse, const char port);