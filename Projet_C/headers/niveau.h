#ifndef NIVEAU
#define NIVEAU

#include "jeu.h"
#include <stdio.h>


typedef struct niveau {
    int difficulte;
    char* nom;
    char* sauvegarde;
    char* meilleursScores;
}Niveau;


Niveau* creer_niveau (int difficulte);


#endif