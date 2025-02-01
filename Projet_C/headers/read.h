#ifndef READ
#define READ

#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"


int Charge_Level(Jeu* jeu, int difficulte);
int Charge_Sauvegarde(Jeu* jeu, int difficulte);
int Sauvegarder_partie (Jeu* jeu, int difficulte);
int Sauvegarder_Score (Jeu* jeu, int difficulte);

#endif