#include "tourelle.h"

/*
type de tourelle :
1/ z : 3pv, 1 degat, 100 prix, premier ennemu entre
*/


Tourelle* creer_tourelle(int type, int ligne, int position) {
    Tourelle* t = malloc(sizeof(Tourelle));
    t->ligne = ligne;
    t->position = position;
    t->next = NULL;
    t->type = type;
    t->index_animation = 1;
    t->etat = 1;

    switch (type){
    case 'T':
        t->pointsDeVie = 3;
        t->vie_totale = 3;
        t->prix = 100;
        t->degats = 1;
        t->hauteur = 70;
        t->largeur = 50;
        t->degats = 1;
        t->nb_projectiles = 1;
        t->decalage_x = 5;
        t->decalage_y = 9;
        t->type_projectile = (int)'F';
        t->portee = 10;
        break;
    
    case 'O':
        t->pointsDeVie = 5;
        t->vie_totale = 5;
        t->prix = 200;
        t->degats = 1;
        t->hauteur = 70;
        t->largeur = 30;
        t->decalage_x = 13;
        t->decalage_y = 6;
        t->type_projectile = (int)'F';
        t->portee = 15;
        t->nb_projectiles = 2;
        break;
    
    case 'M':
        t->pointsDeVie = 5;
        t->vie_totale = 5;
        t->degats = 0;
        t->hauteur = 60;
        t->largeur = 20;
        t->prix = 50;
        t->decalage_x = 20;
        t->decalage_y = 9;
        t->type_projectile = 0;
        t->portee = 1;
        break;
    case 'L':
        t->pointsDeVie = 10;
        t->vie_totale = 10;
        t->degats = 2;
        t->nb_projectiles = 3;
        t->hauteur = 70;
        t->largeur = 45;
        t->prix = 50;
        t->decalage_x = 12;
        t->decalage_y = 0;
        t->type_projectile = (int)'L';
        t->portee = 15;
        break;
    case 'B':
        t->pointsDeVie = 1;
        t->vie_totale = 1;
        t->degats = 10;
        t->hauteur = 25;
        t->largeur = 25;
        t->prix = 50;
        t->decalage_x = 16;
        t->decalage_y = 45;
        t->portee = 2;
        break;
    case 'F':
        t->pointsDeVie = 8;
        t->vie_totale = 8;
        t->degats = 3;
        t->hauteur = 80;
        t->largeur = 40;
        t->prix = 200;
        t->decalage_x = 10;
        t->decalage_y = -5;
        t->portee = 2;
        break;
    }

    return t;
}


Tourelle* charger_tourelle(char type, int ligne, int position, int pointsDeVie) {   //depuis une sauvegarde
    Tourelle* t = malloc(sizeof(Tourelle));
    t->ligne = ligne;
    t->position = position;
    t->next = NULL;
    t->type = type;
    t->pointsDeVie = pointsDeVie;
    t->index_animation = 1;
    t->etat = 1;

    switch (type){
    case 'T':
        t->vie_totale = 3;
        t->prix = 100;
        t->degats = 1;
        t->hauteur = 70;
        t->largeur = 50;
        t->degats = 1;
        t->decalage_x = 5;
        t->nb_projectiles = 1;
        t->decalage_y = 9;
        t->type_projectile = (int)'F';
        t->portee = 10;
        break;
    
    case 'O':
        t->vie_totale = 3;
        t->prix = 200;
        t->nb_projectiles = 2;
        t->degats = 1;
        t->hauteur = 70;
        t->largeur = 30;
        t->decalage_x = 13;
        t->decalage_y = 6;
        t->type_projectile = (int)'F';
        t->portee = 15;
        break;
    
    case 'M':
        t->vie_totale = 4;
        t->degats = 0;
        t->hauteur = 60;
        t->largeur = 20;
        t->prix = 50;
        t->decalage_x = 20;
        t->decalage_y = 9;
        t->type_projectile = 0;
        t->portee = 1;
        break;

    case 'L':
        t->vie_totale = 10;
        t->degats = 2;
        t->hauteur = 80;
        t->largeur = 40;
        t->nb_projectiles = 3;
        t->prix = 50;
        t->decalage_x = 10;
        t->decalage_y = 0;
        t->type_projectile = (int)'L';
        t->portee = 15;
        break;

    case 'B':
        t->pointsDeVie = 1;
        t->vie_totale = 1;
        t->degats = 10;
        t->hauteur = 25;
        t->largeur = 25;
        t->prix = 50;
        t->decalage_x = 16;
        t->decalage_y = 45;
        t->portee = 2;
        break;
    case 'F':
        t->vie_totale = 8;
        t->degats = 3;
        t->hauteur = 80;
        t->largeur = 40;
        t->prix = 200;
        t->decalage_x = 10;
        t->decalage_y = -5;
        t->portee = 2;
        break;
    }
    return t;

}

Tourelle* supprimer_tourelle(Jeu* jeu, Tourelle* tourelle) {
    Tourelle* courant = jeu->tourelles;
    if (courant == tourelle) {jeu->tourelles = courant->next;free(tourelle); return jeu->tourelles;}
    while (courant->next != tourelle) {courant = courant->next;}
    courant->next = tourelle->next;
    free(tourelle);
    return courant->next;
}

void tourelle_attack(Jeu* jeu, Tourelle* tourelle) {
    if (tourelle->type == 'M' || tourelle->type == 'B') {return;} //explosion de la bombe dans les deplacements des etudiant.


    if (tourelle->type == 'T' || tourelle->type == 'O' || tourelle->type == 'L'){
        printf("On va attaquer\n");
        printf(" de %d a %d ou %d\n", tourelle->position+1, MAX_CASE, tourelle->position + tourelle->portee +1);
        for (int i=tourelle->position+1; i<minimum(MAX_CASE, tourelle->position + tourelle->portee +1); i++) { 
            Etudiant* etudiant = search_etudiant(jeu, tourelle->ligne, i);
            
            if (etudiant != NULL) {
                for (int j=0; j<tourelle->nb_projectiles; j++) {
                    printf("On tire\n");
                ajouter_projectile(jeu, tourelle->type_projectile, etudiant, tourelle->degats, arrondi( j*40 + X_START + X_CONSTANTE*(0.5+tourelle->position-1)), arrondi( Y_START + Y_CONSTANTE*(0.2+tourelle->ligne-1)), arrondi( X_START + X_CONSTANTE*(0.2+etudiant->position-1)), arrondi(Y_CONSTANTE*(0.2+etudiant->ligne-1)));
                }
                break;
            }
        }
    }


    if (tourelle->type == 'F') {
        for (int i=tourelle->position+1; i<minimum(MAX_CASE, tourelle->position + tourelle->portee +1); i++) {
            Etudiant* etudiant = search_etudiant(jeu, tourelle->ligne, i);
            if (etudiant != NULL) {
                ajouter_effet(jeu, 'F', etudiant->ligne, etudiant->position);
                etudiant->pointsDeVie -= tourelle->degats;
                if (etudiant->pointsDeVie <= 0) {etudiant->pointsDeVie = 0; etudiant->etat = 4; etudiant->index_animation = 0;}
            }
        }

    }
    
    return;
}


void ajouter_tourelle(Jeu* jeu, int type, int ligne, int position) {
    
    Tourelle* tourelle = creer_tourelle(type, ligne, position);

    
    if (jeu->tourelles == NULL) {jeu->tourelles = tourelle; return;}
    Tourelle* courant = jeu->tourelles;
    while (courant->next != NULL) {
        courant = courant->next;
    }
    courant->next = tourelle;

    
    
    return;

}

void ajouter_tourelle_existante(Jeu* jeu, Tourelle* tourelle) {
    if (jeu->tourelles == NULL) {jeu->tourelles = tourelle; return;}
    Tourelle* courant = jeu->tourelles;
    while (courant->next != NULL) {
        courant = courant->next;
    }
    courant->next = tourelle;
    return;
}

int est_en_mouvement_tourelles(Jeu* jeu) {
    Tourelle* courant = jeu->tourelles;
    while (courant != NULL) {
        if (courant->etat == 2) {return 1;}
        courant = courant->next;
    }
    return 0;
}

void actualiser_image_tourelle(Jeu* jeu, Tourelle* tourelle) {
    if (tourelle->index_animation == 10 && tourelle->etat == 1) {tourelle->index_animation = 0;}
    if (tourelle->type == 'M' && tourelle->etat == 1) tourelle->index_animation = 0;
    switch (tourelle->etat){
    case 1:
        tourelle->index_animation += 1;
        char nom[32];
        snprintf(nom, 32, "assets/tourelles/%c/wait_%d.png", tourelle->type, tourelle->index_animation);
        afficherImage(jeu, nom, X_START +(tourelle->position-1)*X_CONSTANTE + tourelle->decalage_x, Y_START + (tourelle->ligne-1)*Y_CONSTANTE + tourelle->decalage_y, tourelle->largeur, tourelle->hauteur);
        break;
    
    case 2:
        tourelle->index_animation += 1;
        char nom2[31];
        snprintf(nom2, 31, "assets/effets/explosion_%d.png", tourelle->index_animation);
        afficherImage(jeu, nom2, X_START +(tourelle->position-1)*X_CONSTANTE, Y_START + (tourelle->ligne-1)*Y_CONSTANTE, 75, 75);
        if (tourelle->index_animation == 7) {supprimer_tourelle(jeu, tourelle);}
        break;
    }
    if (tourelle->etat != 2 && tourelle->type != 'B') {
        dessiner_rectangle(jeu, (SDL_Color){100, 100, 100, 255}, X_START + (tourelle->position-1)*X_CONSTANTE + 5, Y_START + (tourelle->ligne-1)*Y_CONSTANTE - 7, 50, 3, 0);
        dessiner_rectangle(jeu, (SDL_Color){0, 200, 0, 255}, X_START + (tourelle->position-1)*X_CONSTANTE + 5, Y_START + (tourelle->ligne-1)*Y_CONSTANTE - 7, arrondi(((float)tourelle->pointsDeVie/(float)tourelle->vie_totale)*50), 3, 0);
    }
    return;
}
