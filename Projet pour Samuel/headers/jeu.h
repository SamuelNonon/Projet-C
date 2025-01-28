#ifndef JEU
#define JEU

#include <stdio.h>
#include <stdlib.h>
#include "tourelle.h"
#include "etudiant.h"
#include "graphics.h"


#define MAX_CASE 30

typedef struct etudiant Etudiant;
typedef struct tourelle Tourelle;

typedef struct jeu {
    Tourelle* tourelles;
    Etudiant* etudiants;
    int cagnotte;
    int tour;
    int launched;
    int pause;
    int score;
} Jeu;

Jeu* creer_jeu();
int max_tour(Jeu* jeu);
Etudiant* search_etudiant(Jeu* jeu, int ligne, int position);
Tourelle* search_tourelle(Jeu* jeu, int ligne, int position);
void supprimer_etudiants(Jeu* jeu);
void supprimer_tourelles(Jeu* jeu);
void ask_for_tourelles(Jeu* jeu);
void Update_attack(Jeu* jeu);
void Update_positions(Jeu* jeu);

#endif