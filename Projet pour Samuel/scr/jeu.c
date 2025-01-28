#include "jeu.h"


int max_tour(Jeu* jeu) { //retourne le numéro de tour le plus haut où un etudiant doit apparaitre.
    if (jeu->etudiants == NULL) return 0;
    Etudiant* current = jeu->etudiants;
    int max = 0;
    while (current != NULL) {
        if (current->position>max) {
            max = current->position;
        }
        current = current->next;
    }
    return max;
}

Jeu* creer_jeu(void) {
    Jeu* res = malloc(sizeof(Jeu));
    res->etudiants = NULL;
    res->tourelles = NULL;
    res->tour = 1;
    res->cagnotte = 0;
    res->launched = 1;
    res->pause = 0;
    res->score = 0;
    return res;
}

Etudiant* search_etudiant(Jeu* jeu, int ligne, int position) { //a une ligne/position donnee, return NULL s'il n y a pas d'etudiant, si oui il retourne le pointeur sur lui.
    Etudiant* current = jeu->etudiants;
    while (current != NULL) {
        if (current->ligne == ligne && current->position == position) {
            return current;
        }
        current = current->next;
    }
        
    return NULL;
}

Tourelle* search_tourelle(Jeu* jeu, int ligne, int position) {
    Tourelle* current = jeu->tourelles;
    while (current != NULL) {
        if (current->ligne == ligne && current->position == position) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}


void supprimer_etudiants(Jeu* jeu) {
    if (jeu->etudiants == NULL) {return;}
    while (jeu->etudiants != NULL) {jeu->etudiants = supprimer_etudiant(jeu, jeu->etudiants);}
    return;
}

void supprimer_tourelles(Jeu* jeu) {
    if (jeu->tourelles == NULL) {return;}
    while (jeu->tourelles != NULL) jeu->tourelles = supprimer_tourelle(jeu, jeu->tourelles);
    return;
}





void ask_for_tourelles(Jeu* jeu) {
    
    int r = 1;
    while ( r== 1) {
        system("CLS");
        afficher_jeu(jeu);
        printf("Cagnotte : %d\n", jeu->cagnotte);
        char *menu[] = {"Placer une nouvelle tourelle", "Lancer la partie"};
        r = Ask_in_Menu(menu, 2);
        clear_lines(4);
        int valide = 0;
        
        while (valide == 0 && r==1) {
            if (jeu->cagnotte < 100) {
            printf("Vous n'avez pas assez d'argent pour une autre tourelle\n");
            return;
            }
            jeu->cagnotte -= 100;
            int l, p;
            printf("Ligne ->");
            scanf("%d", &l);
            printf("Position ->");
            scanf("%d", &p);
            if (l>=1 && l<=7 && p>=1 && p<= MAX_CASE) {
                if (search_etudiant(jeu, l, p) != NULL) {
                    clear_lines(4);
                    printf("Il y a deja un ennemi a cette position\n");
                    continue;
                }
                if (search_tourelle(jeu, l, p) != NULL) {
                    clear_lines(4);
                    printf("Il y a deja une tourelle a cette position\n");
                    continue;
                }
                ajouter_tourelle(jeu, (int)'T', l, p);
                valide =1;
            }
        }
    }

    return;
}



void Update_attack(Jeu* jeu) {

    Tourelle* tourelle = jeu->tourelles;
    while (tourelle != NULL) {
        if (tourelle->type == "T" || tourelle->type == "S" || tourelle->type == "P" )
            tourelle_attack(jeu, tourelle);
        tourelle = tourelle->next;
    }

    Etudiant* etudiant = jeu->etudiants;
    while (etudiant != NULL) {
        Tourelle* t = search_tourelle(jeu, etudiant->ligne, etudiant->position-1);
        if (t!= NULL) {
            t->pointsDeVie -= etudiant->degats;
            if (t->type == "M"){
                jeu->score += etudiant->score;
                supprimer_etudiant(jeu, etudiant);
            }
            if (t->pointsDeVie <= 0) {supprimer_tourelle(jeu, t);}
        }

        etudiant = etudiant->next;
    }

    return;
}

void Update_positions(Jeu* jeu) {

    Etudiant* current = jeu->etudiants;
    while(current != NULL) {
        for (int i=1; i<current->vitesse+1; i++) {
            if (search_tourelle(jeu, current->ligne, current->position-1) != NULL) break; //si il y a une tourelle juste devant
            if (current->prev_line == NULL) {current->position -= 1; continue;}
            if (current->prev_line->position == current->position-1) break; //si l'ennemi prev_line est juste devant
            current->position -= 1;
            
        }
        if (current->position <= 0) {jeu->launched = 2; return;}
        current = current->next;
    }

    return;
}
