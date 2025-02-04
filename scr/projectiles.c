#include "projectiles.h"



Projectile* creer_projectile(int type, Etudiant* cible, int degats, int x, int y, int cx, int cy) {
    Projectile *res = malloc(sizeof(Projectile));
    res->cx = cx;
    res->cy = cy;
    res->x = x;
    res->y = y;
    res->type = type;
    res->next = NULL;
    res->etat = 1;
    res->index_animation = 1;
    res->cible = cible;
    res->degats = degats;
    return res;
}


Projectile* supprimer_projectile(Jeu* jeu, Projectile* projectile) {
    Projectile* courant = jeu->projectiles;
    if (projectile == jeu->projectiles) {jeu->projectiles = projectile->next; free(projectile); return jeu->projectiles;}
    while (courant->next != projectile) {
        courant = courant->next;
    }
    courant->next = projectile->next;
    free(projectile);
    return courant->next;
}

void ajouter_projectile(Jeu* jeu, int type, Etudiant* cible, int degats, int x, int y, int cx, int cy) {
    Projectile* projectile = creer_projectile(type, cible, degats, x, y, cx, cy);

    if (jeu->projectiles == NULL) {jeu->projectiles = projectile; return;}
    Projectile* courant = jeu->projectiles;
    while (courant->next != NULL) {
        courant = courant->next;
    }
    courant->next = projectile;
    return;
}


Projectile* actualiser_projectile(Jeu* jeu, Projectile* projectile) {
    printf("Flag 0\n");
    if (projectile == NULL) {return NULL;}
    printf("Flag 1\n");
    if (projectile->x > 1590 && projectile->cible == NULL) {
        printf("Flag 1.5\n");
        return supprimer_projectile(jeu, projectile);
    }
    printf("Flag 2\n");

    char nom1[37];
    snprintf(nom1, 37, "assets/projectiles/%c/travel_%d.png", projectile->type, projectile->index_animation);
    
    if (projectile->type == 'F') afficherImage(jeu, nom1, projectile->x, projectile->y, 30, 12);
    if (projectile->type == 'L') afficherImage(jeu, nom1, projectile->x, projectile->y-20, 90, 60);
    


    if (projectile->index_animation == 4) projectile->index_animation=0;
    projectile->index_animation += 1;
    if (projectile->x>projectile->cx) projectile->x -= PORJECTILE_VITESSE;
    else if (projectile->x<projectile->cx) projectile->x += PORJECTILE_VITESSE;

    
    if (abs(projectile->x - projectile->cx) < PORJECTILE_VITESSE && projectile->cible != NULL) {
        ajouter_effet(jeu, 'E', projectile->cible->ligne, projectile->cible->position);
        if (projectile->cible != NULL ) {
            projectile->cible->pointsDeVie -= projectile->degats;
            
            
            if (projectile->cible->pointsDeVie <=0) {
                projectile->cible->pointsDeVie = 0;
                Projectile* courant = jeu->projectiles;
                while (courant != NULL) {
                    if (courant != projectile && courant->cible == projectile->cible) {
                        if (projectile->cible->next_line != NULL) {
                            courant->cible = projectile->cible->next_line; 
                            courant->cx += arrondi( X_START + X_CONSTANTE*(0.5+courant->cible->position-1) - courant->cx);
                        }
                        else {courant->cible = NULL ; courant->cx = 1590;}
                        
                    }
                    courant = courant->next;
                }
                projectile->cible->etat = 4;
            }
        }
        return supprimer_projectile(jeu, projectile);
    }

    return projectile->next;
}


void afficher_projectiles(Jeu* jeu) {
    Projectile* courant = jeu->projectiles;
    printf("\n\n\n********************\n");
    while (courant !=NULL) {
        printf("%c : x->%d, y->%d, cx->%d, cy->%d\n", courant->type, courant->x, courant->y, courant->cx, courant->cy);
        courant = courant->next;
    }
    
}

int est_vide_projectiles(Jeu* jeu) {
    if (jeu->projectiles == NULL) return 1;
    else return 0;
}