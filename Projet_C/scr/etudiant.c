#include "etudiant.h"
#define NUMERO_POSITION_1ER_APPARITION 15

/*
type étdiant :
z:  
*/


/*
etats:
1/ statique
2/ course
3/tire
4/mort

(4) la mort sera une animation a part car il faudra gérer la suppression apres la fin de l'animation qui ne peut etre faites apres.

*/


Etudiant* creer_etudiant(char type, int tour, int ligne) {   
    Etudiant* res = malloc(sizeof(Etudiant));
    res->next = NULL;
    res->type = type;
    res->tour = tour;
    res->position = tour + NUMERO_POSITION_1ER_APPARITION-1; 
    res->ligne = ligne;
    res->next_line = NULL;
    res->prev_line = NULL;
    if (res->type == 'Z') {     // Classique
        res->pointsDeVie = 4;
        res->vitesse = 2;
        res->degats = 1;
        res->score = 10;
    }
    if (res->type == 'M') {     // Mage
        res->pointsDeVie = 4;
        res->vitesse = 3;
        res->degats = 4;
        res->score = 20;
    }
    if (res->type == 'A') {     // Assassin
        res->pointsDeVie = 2;
        res->vitesse = 5;
        res->degats = 5;
        res->score = 20;
    }
    if (res->type == 'C') {     // Costaud
        res->pointsDeVie = 10;
        res->vitesse = 1;
        res->degats = 3;
        res->score = 40;
    }
    if (res->type == 'H') {     // Healer
        res->pointsDeVie = 5;
        res->vitesse = 2;
        res->degats = 1;
        res->score = 20;
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
    if (res->type == 'Z') {     // Classique
        res->vitesse = 2;
        res->degats = 1;
        res->score = 10;
    }
    if (res->type == 'M') {     // Mage
        res->vitesse = 3;
        res->degats = 4;
        res->score = 20;
    }
    if (res->type == 'A') {     // Assassin
        res->vitesse = 5;
        res->degats = 5;
        res->score = 20;
    }
    if (res->type == 'C') {     // Costaud
        res->vitesse = 1;
        res->degats = 3;
        res->score = 40;
    }
    if (res->type == 'H') {     // Healer
        res->vitesse = 2;
        res->degats = 1;
        res->score = 20;
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


Etudiant* supprimer_etudiant(Jeu* jeu, Etudiant* etudiant) { //supprime celui en parametre et renvoie son next
    Etudiant* current = jeu->etudiants;
    if (current == etudiant) {jeu->etudiants = etudiant->next;}
    else {
        while (current->next != etudiant) {current = current->next;}
        current->next = etudiant->next;
    }
    free(etudiant);
    return current->next;
}


void afficher_etudiants(Jeu* jeu) {
    if (jeu->etudiants == NULL) {
        printf("Il ny a aucun étudiants chargées\n");
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

