#ifndef READ
#define READ

#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"


int Charge_Level(Jeu* jeu, char* name);
int Charge_Sauvegarde(Jeu* jeu, char* name);
int Sauvegarder_partie (Jeu* jeu);
int Sauvegarder_Score (Jeu* jeu);

#endif