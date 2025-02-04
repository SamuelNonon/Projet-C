#ifndef ETUDIANT
#define ETUDIANT

#include "jeu.h"
#include <stdlib.h>

typedef struct jeu Jeu;

typedef struct etudiant {
    int type;
    int pointsDeVie;
    int ligne;
    int position;
    int vitesse;
    int tour;


    int value;
    int dx;
    int vie_totale;
    struct etudiant* next;
    struct etudiant* next_line;
    struct etudiant* prev_line;
    int hauteur;
    int largeur;
    int index_animation;
    int etat;
    int portee;
    int degats;
    int decalage_x;
    int decalage_y;
} Etudiant;


Etudiant* creer_etudiant(int type, int tour, int ligne);
Etudiant* charger_etudiant(int tour, int ligne, int position, char type, int pointsDeVie);
void ajouter_etudiant(Jeu* jeu, Etudiant* etudiant);
void detection_mines(Jeu* jeu);
void afficher_etuiants(Jeu* jeu);
Etudiant* supprimer_etudiant(Jeu* jeu, Etudiant* etudiant);
Etudiant* update_prev_line(Jeu* jeu, Etudiant* etudiant); //met a jour et renvoie la valeur du pointeur prev_line
Etudiant* update_next_line(Jeu* jeu, Etudiant* etudiant); //met a jour et renvoie la valeur du pointeur next_line
void actualiser_image_etudiant(Jeu* jeu, Etudiant* etudiant);
int est_en_mouvement_etudiants(Jeu* jeu);

#endif