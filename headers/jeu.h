#ifndef JEU
#define JEU

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "tourelle.h"
#include "etudiant.h"
#include "graphics.h"
#include "projectiles.h"

#define DELAI_IMAGE (1000/FPS)
#define NUMERO_POSITION_1ER_APPARITION 15
#define MAX_CASE 16
#define X_CONSTANTE 96
#define Y_CONSTANTE 127
#define X_START 98
#define Y_START 12
#define ETUDIANT_LARGEUR 47
#define ETUDIANT_HAUTEUR 70
#define ETUDIANT_IMAGE_VITESSE 10
#define PORJECTILE_VITESSE 30
#define FENETRE_LARGEUR 1536
#define FENETRE_HAUTEUR 984
#define MAX_LONGUEUR_PSEUDO 15
#define MAP_IMAGE "assets/mapnew.png"


typedef struct etudiant Etudiant;
typedef struct tourelle Tourelle;
typedef struct projectile Projectile;
typedef struct effet Effet;
typedef struct texture Texture;

typedef struct jeu {
    Tourelle* tourelles;
    Etudiant* etudiants;
    Projectile* projectiles;
    Effet* effets;
    Texture* textures;
    int cagnotte;
    int tour;
    int statut;
    SDL_Window *window;
    SDL_Event event;
    SDL_Renderer* renderer;
    int pause;
    int niveau;
    int score;
    int sauvegarde;
    char *pseudo;
    int etat; //différentes etapes du deroulement d'un tour
    int choix_joueur;
} Jeu;

Jeu* creer_jeu();
int max_tour(Jeu* jeu);
Etudiant* search_etudiant(Jeu* jeu, int ligne, int position);
Tourelle* search_tourelle(Jeu* jeu, int ligne, int position);
void supprimer_etudiants(Jeu* jeu);
void supprimer_tourelles(Jeu* jeu);
void supprimer_projectiles(Jeu* jeu);
void supprimer_effets(Jeu* jeu);
void supprimer_textures(Jeu* jeu);
void demander_tourelle(Jeu* jeu);
void actualiser_attaques_etudiants(Jeu* jeu);
void actualiser_attaques_tourelles(Jeu* jeu);
void actualiser_positions(Jeu* jeu);
void gestion_event(Jeu* jeu);
void Nouvelles_appartitions(Jeu* jeu);
void actualier_prev_next_line_etudiant(Jeu* jeu);

#endif