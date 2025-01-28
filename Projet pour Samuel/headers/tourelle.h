#ifndef TOURELLE
#define TOURELLE

#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"
#include "etudiant.h"


typedef struct jeu Jeu;

typedef struct tourelle {
    int type;
    int pointsDeVie;
    int ligne;
    int position;
    int prix;
    int degats;
    struct tourelle* next;
} Tourelle;

Tourelle* creer_tourelle(int type, int ligne, int position);
Tourelle* supprimer_tourelle(Jeu* jeu, Tourelle* tourelle);
void tourelle_attack(Jeu* jeu, Tourelle* tourelle);
Tourelle* ajouter_tourelle(Jeu* jeu, int type, int ligne, int position);


#endif