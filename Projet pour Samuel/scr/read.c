#include "read.h"


int Charge_Level(Jeu* jeu, char* name) {
    FILE *fichier = fopen(name, "r");
    

    if (fichier == NULL) {
        printf("Ouverture du fichier de '%s' impossible\n", name);
        return 0;
    }
    

    fscanf(fichier, "%d\n", &(jeu->cagnotte));
    char type;
    int tour;
    int ligne;
    
    while (fscanf(fichier, "%d %d %c\n", &tour, &ligne, &type) != EOF) {
        ajouter_etudiant(jeu, type, tour, ligne);
    }
    
    fclose(fichier);
    return 1;
}