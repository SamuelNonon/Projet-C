#include "etudiant.h"


/*
type étdiant :
Z :
W : 
*/


/*
etats:
1/ statique
2/ course
3/tire
4/mort

(4) la mort sera une animation a part car il faudra gérer la suppression apres la fin de l'animation qui ne peut etre faites apres.

*/

Etudiant* supprimer_etudiant(Jeu* jeu, Etudiant* etudiant) { //supprime celui en parametre et renvoie son next
    Etudiant* courant = jeu->etudiants;
    if (etudiant == jeu->etudiants) {jeu->etudiants = etudiant->next; free(etudiant); return jeu->etudiants;}
    while (courant->next != etudiant) {courant = courant->next;}
    courant->next = etudiant->next;
    free(etudiant);
    return courant->next;
}

Etudiant* creer_etudiant(int type, int tour, int ligne) {
    Etudiant* res = malloc(sizeof(Etudiant));
    res->next = NULL;
    res->type = type;
    res->tour = tour;
    res->position = -10; 
    res->ligne = ligne;
    res->next_line = NULL;
    res->prev_line = NULL;
    res->dx = 0;
    res->index_animation = 1;
    res->etat = 1;


    switch (type){

    case 'M':
        res->pointsDeVie = 8;
        res->vie_totale = 8;
        res->vitesse = 1;
        res->degats = 3;
        res->value = 30;
        res->portee = 10;
        res->largeur = 150;
        res->hauteur = 150;
        res->decalage_x = -50;
        res->decalage_y = -50;
        break;
    
    case 'T':
        res->pointsDeVie = 7;
        res->vie_totale = 7;
        res->vitesse = 1;
        res->degats = 4;
        res->value = 30;
        res->portee = 1;
        res->hauteur = 70;
        res->largeur = 40;
        res->decalage_x = 5;
        res->decalage_y = 0;
        break;
    
    case 'H':
        res->pointsDeVie = 3;
        res->vie_totale = 3;
        res->vitesse = 2;
        res->degats = 1;
        res->value = 30;
        res->portee = 1;
        res->hauteur = 60;
        res->largeur = ETUDIANT_LARGEUR;
        res->decalage_x = 5;
        res->decalage_y = 10;
        break;
    case 'Z':
        res->pointsDeVie = 5;
        res->vie_totale = 5;
        res->vitesse = 2;
        res->degats = 2;
        res->value = 100;
        res->portee = 3;
        res->hauteur = 70;
        res->largeur = ETUDIANT_LARGEUR;
        res->decalage_x = 0;
        res->decalage_y = 0;
        break;
    
    case 'D':
        res->pointsDeVie = 2;
        res->vie_totale = 2;
        res->vitesse = 3;
        res->degats = 1;
        res->portee = 2;
        res->value = 100;
        res->hauteur = 40;
        res->largeur = ETUDIANT_LARGEUR;
        res->decalage_x = 10;
        res->decalage_y = 5;
        break;
    }
    
    return res;
}

Etudiant* charger_etudiant(int tour, int ligne, int position, char type, int pointsDeVie) { //depuis une sauvegarde
    Etudiant* res = malloc(sizeof(Etudiant));
    res->next = NULL;
    res->type = type;
    res->tour = tour;
    res->position = position;
    res->ligne = ligne;
    res->pointsDeVie = pointsDeVie;
    res->next_line = NULL;
    res->prev_line = NULL;
    res->index_animation = 0;
    res->etat = 1;

    switch (type){
        
        case 'M':
            res->vie_totale = 8;
            res->vitesse = 1;
            res->degats = 3;
            res->value = 30;
            res->portee = 10;
            res->hauteur = 150;
            res->largeur = 185;
            res->decalage_x = -70;
            res->decalage_y = -40;
        break;

        case 'T':
            res->vie_totale = 7;
            res->vitesse = 1;
            res->degats = 4;
            res->value = 30;
            res->portee = 1;
            res->hauteur = 70;
            res->largeur = 40;
            res->decalage_x = 5;
            res->decalage_y = 0;
        break;

        case 'H':
            res->vie_totale = 3;
            res->vitesse = 2;
            res->degats = 1;
            res->value = 30;
            res->portee = 1;
            res->hauteur = 70;
            res->largeur = ETUDIANT_LARGEUR;
            res->decalage_x = 5;
            res->decalage_y = 10;
        break;

        case 'Z':
            res->vie_totale = 5;
            res->vitesse = 1;
            res->degats = 1;
            res->value = 100;
            res->hauteur = 70;
            res->largeur = ETUDIANT_LARGEUR;
            res->decalage_x = 0;
            res->decalage_y = 0;
            break;
        
        case 'D':
            res->vie_totale = 2;
            res->vitesse = 3;
            res->degats = 1;
            res->portee = 2;
            res->value = 100;
            res->hauteur = 40;
            res->largeur = ETUDIANT_LARGEUR;
            res->decalage_x = 10;
            res->decalage_y = 5;
            break;
        }
    return res;
}

void ajouter_etudiant(Jeu* jeu, Etudiant* etudiant) {

    
    if (jeu->etudiants == NULL) { //la liste est vide
        jeu->etudiants = etudiant;
        return;
    }
    
    Etudiant* prev = NULL;
    Etudiant* current = jeu->etudiants;
    
    while (current->tour < etudiant->tour){
        prev = current;
        current = current->next;
        if (current == NULL) break;
    }
    if (prev==NULL) {
        etudiant->next = jeu->etudiants;
        jeu->etudiants = etudiant; 
    }
    else{
        etudiant->next = prev->next;
        prev->next = etudiant;
    }
    
    return;

}

void afficher_etuiants(Jeu* jeu) {
    if (jeu->etudiants == NULL) {
        printf("Il ny a aucun étudiants chargés\n");
        return;
    }
    Etudiant* e = jeu->etudiants;
    printf("Il y a :\n");
    while (e != NULL) {
        printf("-  type : %c, ligne : %d, position : %d ,tour : %d\n", e->type, e->ligne, e->position, e->tour);
        e=e->next;
    }
    return;

}

Etudiant* update_prev_line(Jeu* jeu, Etudiant* etudiant){ //met a jour et renvoie la valeur du pointeur prev_line
    for (int i=etudiant->position-1; i>0; i--) {
        Etudiant* current = search_etudiant(jeu, etudiant->ligne, i);
        if (current != NULL) {
            etudiant->prev_line = current;
            return current;
        }
    }
    etudiant->prev_line = NULL;
    return NULL;
} 

Etudiant* update_next_line(Jeu* jeu, Etudiant* etudiant){ //met a jour et renvoie la valeur du pointeur prev_line
int max = max_tour(jeu) + NUMERO_POSITION_1ER_APPARITION + 1;
    for (int i=etudiant->position+1; i<max; i++) {
        Etudiant* current = search_etudiant(jeu, etudiant->ligne, i);
        if (current != NULL) {
            etudiant->next_line = current;
            return current;
        }
    }
    etudiant->next_line = NULL;
    return NULL;
} 

void detection_mines(Jeu* jeu) {
    Etudiant* etudiant = jeu->etudiants;
    Tourelle* t;
    while (etudiant != NULL) {
        t = search_tourelle(jeu, etudiant->ligne, etudiant->position);
        if (t != NULL && t->type == 'B' && etudiant->type != 'D') {
            etudiant->pointsDeVie -= t->degats;
            printf("On ajoute un effet\n");
            ajouter_effet(jeu, 'E', t->ligne, t->position);
            supprimer_tourelle(jeu, t);
            if (etudiant->pointsDeVie <= 0) {
                etudiant->pointsDeVie = 0;
                etudiant->etat = 4;
                etudiant->index_animation = 0;
            }
        }
        etudiant = etudiant->next;
    }
}

void actualiser_image_etudiant(Jeu* jeu, Etudiant* etudiant) {
    if (etudiant->dx > 0) {etudiant->etat = 2;}
    if (etudiant->position < -1) {return;}
    switch (etudiant->etat) {
        

        case 1:

            if (etudiant->index_animation >= 6) {etudiant->etat = 1; etudiant->index_animation = 0; actualiser_image_etudiant(jeu, etudiant); break;}
            etudiant->index_animation += 1;
            char name1[35];
            snprintf(name1, 35, "assets/etudiants/%c/wait_%d.png", etudiant->type, etudiant->index_animation);
            afficherImage(jeu, name1, X_START + (etudiant->position-1)*X_CONSTANTE + etudiant->decalage_x, Y_START + (etudiant->ligne-1)*Y_CONSTANTE + etudiant->decalage_y, etudiant->largeur, etudiant->hauteur);
            break;
        case 2:
            if (etudiant->index_animation == 10) {etudiant->etat = 2; etudiant->index_animation = 0; actualiser_image_etudiant(jeu, etudiant); break;}
            etudiant->dx -= ETUDIANT_IMAGE_VITESSE;
            etudiant->index_animation += 1;
            char name2[32];
            snprintf(name2, 32, "assets/etudiants/%c/run_%d.png", etudiant->type, etudiant->index_animation);
            afficherImage(jeu, name2, X_START + (etudiant->position-1)*X_CONSTANTE + etudiant->dx + etudiant->decalage_x, Y_START + (etudiant->ligne-1)*Y_CONSTANTE + etudiant->decalage_y, etudiant->largeur, etudiant->hauteur);


            if (abs(etudiant->position*X_CONSTANTE - (etudiant->position)*X_CONSTANTE + etudiant->dx) < ETUDIANT_IMAGE_VITESSE) {
                etudiant->dx = 0;
                etudiant->etat = 1;
                etudiant->index_animation = 0;
            }

            
            break;

        case 3:
            if (etudiant->index_animation == 10) {etudiant->etat = 1; etudiant->index_animation = 0; actualiser_image_etudiant(jeu, etudiant); break;}
            etudiant->index_animation += 1;
            char name3[34];
            snprintf(name3, 34, "assets/etudiants/%c/attack_%d.png", etudiant->type, etudiant->index_animation);
            afficherImage(jeu, name3, X_START + (etudiant->position-1)*X_CONSTANTE + etudiant->decalage_x, Y_START + (etudiant->ligne-1)*Y_CONSTANTE + etudiant->decalage_y, etudiant->largeur, etudiant->hauteur);
            break;
        
        case 4:
            etudiant->index_animation += 1;
            char name4[33];
            snprintf(name4, 33, "assets/etudiants/%c/death_%d.png", etudiant->type, etudiant->index_animation);
            afficherImage(jeu, name4, X_START + (etudiant->position-1)*X_CONSTANTE + etudiant->decalage_x, Y_START + (etudiant->ligne-1)*Y_CONSTANTE + etudiant->decalage_y, etudiant->largeur, etudiant->hauteur);
            break;
    }

    //affichage de la barre de vie au dessus du joueur
    dessiner_rectangle(jeu, (SDL_Color){100, 100, 100, 255}, X_START + (etudiant->position-1)*X_CONSTANTE + etudiant->dx, Y_START + (etudiant->ligne-1)*Y_CONSTANTE - 7, ETUDIANT_LARGEUR, 3, 0);
    dessiner_rectangle(jeu, (SDL_Color){0, 200, 0, 255}, X_START + (etudiant->position-1)*X_CONSTANTE + etudiant->dx, Y_START + (etudiant->ligne-1)*Y_CONSTANTE - 7, arrondi(((float)etudiant->pointsDeVie/(float)etudiant->vie_totale)*ETUDIANT_LARGEUR), 3, 0);

    return;
    

}


int est_en_mouvement_etudiants(Jeu* jeu) {
    Etudiant* courant = jeu->etudiants;
    while (courant != NULL) {
        if (courant->etat != 1) {
            return 1;
        }
        courant = courant->next;
    }
    return 0;
}
