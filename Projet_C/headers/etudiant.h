#ifndef ETUDIANT
#define ETUDIANT

#include "jeu.h"
#include <stdlib.h>

typedef struct jeu Jeu;

typedef struct etudiant {
    char type;
    int pointsDeVie;
    int ligne;
    int position;
    int vitesse;
    int tour;
    struct etudiant* next;
    struct etudiant* next_line;
    struct etudiant* prev_line;
    


    int index_animation;
    int etat;
    int degats;
    int score;
} Etudiant;


Etudiant* creer_etudiant(char type, int tour, int ligne);
Etudiant* charger_etudiant(int tour, int ligne, int position, char type, int pointsDeVie);
void ajouter_etudiant(Jeu* jeu, Etudiant* etudiant);
void afficher_etudiants(Jeu* jeu);
Etudiant* supprimer_etudiant(Jeu* jeu, Etudiant* etudiant);
Etudiant* update_prev_line(Jeu* jeu, Etudiant* etudiant); //met a jour et renvoie la valeur du pointeur prev_line
Etudiant* update_next_line(Jeu* jeu, Etudiant* etudiant); //met a jour et renvoie la valeur du pointeur next_line
#endif