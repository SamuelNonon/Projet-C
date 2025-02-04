#ifndef TOURELLE
#define TOURELLE

#include <stdio.h>
#include <stdlib.h>
#include "jeu.h"
#include "etudiant.h"
#include "projectiles.h"


typedef struct jeu Jeu;

typedef struct tourelle {
    int type;
    int pointsDeVie;
    int vie_totale;
    int ligne;
    int position;
    int prix;
    int degats;
    int hauteur;
    int portee;
    int largeur;
    int decalage_x;
    int decalage_y;
    int nb_projectiles;
    int type_projectile;
    int etat;
    struct tourelle* next;


    int index_animation;
} Tourelle;

Tourelle* creer_tourelle(int type, int ligne, int position);
Tourelle* charger_tourelle(char type, int ligne, int position, int pointsDeVie);
Tourelle* supprimer_tourelle(Jeu* jeu, Tourelle* tourelle);
void tourelle_attack(Jeu* jeu, Tourelle* tourelle);
void ajouter_tourelle(Jeu* jeu, int type, int ligne, int position);
void ajouter_tourelle_existante(Jeu* jeu, Tourelle* tourelle);
void actualiser_image_tourelle(Jeu* jeu, Tourelle* tourelle);
int est_en_mouvement_tourelles(Jeu* jeu);


#endif