#ifndef GRAPHICS
#define GRAPHICS


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "tourelle.h"
#include "etudiant.h"
#include "projectiles.h"
#include "jeu.h"


typedef struct effet {
    int type;
    int position;
    int ligne;
    int index_animation;
    struct effet* next;
} Effet;

typedef struct texture {
    SDL_Texture* texture;
    char *nom;
    struct texture* next;
} Texture;


typedef struct jeu Jeu;

Texture* creer_texture(Jeu* jeu, char* nom);
void ajouter_texture(Jeu* jeu, Texture* texture);
Texture* supprimer_texture(Jeu* jeu, Texture* texture);
Texture* existe_texture(Jeu* jeu, char* nom);



void ajouter_effet(Jeu* jeu, int type, int ligne, int position);
Effet* creer_effet(int type, int ligne, int position);
Effet* supprimer_effet(Jeu* jeu, Effet* effet);
int effet_en_cours(Jeu* jeu);
void actualiser_effets(Jeu* jeu);

int arrondi(float x);
int maximum(int x, int y);
int minimum(int x, int y);
void afficher_jeu(Jeu* jeu);
int Ask_in_Menu(char **menu, int size);
void clear_lines(int n);

int init_SDL(Jeu* jeu);
void clean_SDL(Jeu* jeu);
void afficherImage(Jeu* jeu, char* cheminImage, int x, int y, int largeur, int hauteur);
void dessiner_rectangle(Jeu* jeu, SDL_Color couleur, int x, int y, int largeur, int hauteur, int bordure);
void Update_Image(Jeu* jeu);
void afficher_texte(Jeu* jeu, char* texte, int x, int y, int taille, SDL_Color couleur);

#endif