#ifndef PROJECTILES
#define PROJECTILES

#include <stdio.h>
#include <stdlib.h>
#include "etudiant.h"
#include "graphics.h"
#include "projectiles.h"
#include "jeu.h"

typedef struct jeu Jeu;
typedef struct etudiant Etudiant;


typedef struct projectile {
    int x;
    int y;
    int type;
    int cx;
    int cy;
    int etat;
    int index_animation;
    struct projectile* next;
    Etudiant* cible;
    int degats;
} Projectile;

Projectile* creer_projectile(int type, Etudiant* cible, int degats, int x, int y, int cx, int cy);
void ajouter_projectile(Jeu* jeu, int type, Etudiant* cible, int degats, int x, int y, int cx, int cy);
Projectile* supprimer_projectile(Jeu* jeu, Projectile* projectile);
Projectile* actualiser_projectile(Jeu* jeu, Projectile* projectile);
void afficher_projectiles(Jeu* jeu);
int est_vide_projectiles(Jeu* jeu);


#endif