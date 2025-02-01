#include "tourelle.h"


Tourelle* creer_tourelle(char type, int ligne, int position) {
    Tourelle* t = malloc(sizeof(Tourelle));
    t->ligne = ligne;
    t->position = position;
    t->next = NULL;
    t->type = type;

    if (type == 'T') {     // Tourelle Classique
        t->pointsDeVie = 3;
        t->prix = 100;
        t->degats = 1;
    }
    if (type == 'W') {     // Wall
        t->pointsDeVie = 7;
        t->prix = 100;
        t->degats = 0;
    }
    if (type == 'M') {     // Mine - tue un etudiant à sa destruction
        t->pointsDeVie = 1;
        t->prix = 50;
        t->degats = 0;
    }
    if (type == 'S') {     // Sniper
        t->pointsDeVie = 4;
        t->prix = 250;
        t->degats = 3;
    }
    if (type == 'P') {     // Perforant - touche les 2 premiers enemis d'une ligne
        t->pointsDeVie = 4;
        t->prix = 200;
        t->degats = 1;
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

    if (type == 'T') {     // Tourelle Classique
        t->prix = 100;
        t->degats = 1;
    }
    if (type == 'W') {     // Wall
        t->prix = 100;
        t->degats = 0;
    }
    if (type == 'M') {     // Mine - tue un etudiant à sa destruction
        t->prix = 50;
        t->degats = 0;
    }
    if (type == 'S') {     // Sniper
        t->prix = 250;
        t->degats = 3;
    }
    if (type == 'P') {     // Perforant - touche les 2 premiers enemis d'une ligne
        t->prix = 200;
        t->degats = 1;
    }
    return t;
}


Tourelle* supprimer_tourelle(Jeu* jeu, Tourelle* tourelle) {
    Tourelle* current = jeu->tourelles;
    if (current == tourelle) {jeu->tourelles = NULL; return NULL;}
    while (current->next != tourelle) {current = current->next;}
    current->next = tourelle->next;
    free(tourelle);
    return current->next;
}

void tourelle_attack(Jeu* jeu, Tourelle* tourelle) {
    for (int i=tourelle->position+1; i<max_tour(jeu)+14; i++) { //a revoir pour rendre le parametre dynamique
        Etudiant* etudiant = search_etudiant(jeu, tourelle->ligne, i);
        if (etudiant != NULL) {
            etudiant->pointsDeVie -=tourelle->degats;
            Etudiant* suivant = etudiant->next_line;
            if (etudiant->pointsDeVie <= 0) {
                jeu->score += etudiant->score;
                supprimer_etudiant(jeu, etudiant);
                if (jeu->etudiants == NULL)
                    jeu->launched = 3;

            }
            if (tourelle->type == 'P'){
                if (suivant != NULL){
                    suivant->pointsDeVie -=tourelle->degats;
                    if (etudiant->pointsDeVie <= 0) {
                        jeu->score += etudiant->score;
                        supprimer_etudiant(jeu, etudiant);
                        if (jeu->etudiants == NULL)
                            jeu->launched = 3;
                    }
                }
            }
            break;
        }
    }
    return;
}

Tourelle* ajouter_tourelle(Jeu* jeu, Tourelle* tourelle) {
    if (jeu->tourelles == NULL) {jeu->tourelles = tourelle; return tourelle;}
    Tourelle* current = jeu->tourelles;
    while (current->next != NULL) {
        current = current->next;
    }
    current->next = tourelle;
    return tourelle;
}
