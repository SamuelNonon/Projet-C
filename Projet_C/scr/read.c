#include "read.h"


int Charge_Level(Jeu* jeu, int difficulte) {
    char* name = malloc (28 * sizeof(char));
    sprintf (name, "Niveaux/niveau_%d/niveau.txt", difficulte);
    FILE *fichier = fopen(name, "r");

    if (fichier == NULL) {
        printf("Ouverture du fichier '%s' impossible\n", name);
        return 0;
    }
    

    fscanf(fichier, "%d\n", &(jeu->cagnotte));
    char type;
    int tour;
    int ligne;
    
    while (fscanf(fichier, "%d %d %c\n", &tour, &ligne, &type) != EOF) {
        Etudiant *etudiant = creer_etudiant(type, tour, ligne);
        ajouter_etudiant(jeu, etudiant);
    }
    
    fclose(fichier);
    free (name);
    return 1;
}


int Charge_Sauvegarde(Jeu* jeu, int difficulte) {
    char* name = malloc (32 * sizeof(char));
    sprintf (name, "Niveaux/niveau_%d/sauvegarde.txt", difficulte);
    FILE *fichier = fopen(name, "r");
    if (fichier == NULL) {
        printf("Ouverture du fichier '%s' impossible, il n'y a pas encore de sauvegarde pour ce niveau\n", name);
        return 0;
    }
    
    fscanf(fichier, "%d\n%d\n%d\n", &(jeu->cagnotte), &(jeu->tour), &(jeu->score));
    int tour;
    int ligne;
    int position;
    char type;
    int pointsDeVie;

    while (fscanf(fichier, "%d %d %d %c %d\n", &tour, &ligne, &position, &type, &pointsDeVie) != EOF) {
        if (type == 'F')       // F indique la fin de la liste des etudiants et le debut de celle des tourelles
            break;
        Etudiant* etudiant = charger_etudiant(tour, ligne, position, type, pointsDeVie);
        ajouter_etudiant(jeu, etudiant);
    }
    
    if (type == 'F'){
        while (fscanf(fichier, "%d %d %c %d\n", &ligne, &position, &type, &pointsDeVie) != EOF){
            Tourelle* tourelle = charger_tourelle (type, ligne, position, pointsDeVie);
            ajouter_tourelle (jeu, tourelle);
        }
    }

    fclose(fichier);
    free (name);
    return 1;
}


int Sauvegarder_partie (Jeu* jeu, int difficulte){
    char* name = malloc (32 * sizeof(char));
    sprintf (name, "Niveaux/niveau_%d/sauvegarde.txt", difficulte);
    FILE *fichier = fopen(name, "w");
    if (fichier == NULL) {
        printf("Erreur lors de la sauvegarde");
        return 0;
    }

    fprintf(fichier, "%d\n%d\n%d\n", jeu->cagnotte, jeu->tour, jeu->score);
    Etudiant* etudiant = jeu->etudiants;
    while (etudiant != NULL){
        fprintf(fichier, "%d %d %d %c %d\n", etudiant->tour, etudiant->ligne, etudiant->position, etudiant->type, etudiant->pointsDeVie);
        etudiant = etudiant->next;
    }
    fprintf(fichier, "%d %d %d %c %d\n", 0, 0, 0, 'F', 0);

    Tourelle* tourelle = jeu->tourelles;
    while (tourelle != NULL){
        fprintf(fichier, "%d %d %c %d\n", tourelle->ligne, tourelle->position, tourelle->type, tourelle->pointsDeVie);
        tourelle = tourelle->next;
    }

    fclose(fichier);
    free (name);
    return 1;
}

int Sauvegarder_Score (Jeu* jeu, int difficulte){
    char* name = malloc (38 * sizeof(char));
    sprintf (name, "Niveaux/niveau_%d/meilleurs_scores.txt", difficulte);
    FILE *fichier = fopen(name, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier de score\n");
        return 0;
    }
    int listeScores[10];
    char listeNoms[10][4];
    int classement = 10;
    for (int i = 0; i < 10; i ++){
        fscanf(fichier, "%3s %d\n", &listeNoms[i], &listeScores[i]);
        if (listeScores[i] < jeu->score && classement == 10)
            classement = i;
    }
    if (classement < 10){
        listeScores[classement] = jeu->score;
        printf ("Bravo vous etes %d au classement ! Entrez votre nom (trois lettres majuscules) ->", classement + 1);
        scanf ("%s", listeNoms[classement]);
        fclose (fichier);
        FILE *fichier = fopen(name, "w");
        for (int i = 0; i < 10; i ++)
            fprintf(fichier, "%s %d\n", listeNoms[i], listeScores[i]);
    }
    fclose (fichier);
    free (name);
    return 1;
}