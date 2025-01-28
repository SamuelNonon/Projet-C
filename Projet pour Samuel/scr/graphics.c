#include "graphics.h"

#define NB_LIGNES_AFFICHEES 15


void afficher_jeu(Jeu* jeu) {
    int max = max_tour(jeu);
    printf("Tour %d\n", jeu->tour);


    for (int i=1; i<8; i++) {
        printf("%d|", i);

        for (int j=1; j<NB_LIGNES_AFFICHEES+1; j++) {
            printf(" ");
            Etudiant* etudiant = search_etudiant(jeu, i, j);
            if (etudiant != NULL) {printf(" %d%c", etudiant->pointsDeVie, etudiant->type); continue;}
            Tourelle* tourelle = search_tourelle(jeu, i, j);
            if (tourelle != NULL) {printf(" %c ", tourelle->type); continue;}
            printf(" . ");
        }
        

        printf("\n");
    }


    return;
}

void clear_lines(int n) {
    for (int i = 0; i < n; i++) {
        printf("\033[F");  // Déplacer le curseur une ligne vers le haut
        printf("\033[K");  // Effacer la ligne actuelle
    }
    fflush(stdout);
}


int Ask_in_Menu(char **menu, int size) {
    printf("Menu :\n");
    for (int i=0; i<size; i++) {
        printf("%d/ %s\n", i+1, menu[i]);
    }
    int valide =0;
    int r;
    while (valide == 0) {

        printf("->");
        scanf("%d", &r);
        
        if (r>0 && r<=size) {
            valide = 1;
        }
    }


    return r;
}