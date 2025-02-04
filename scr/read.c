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

        ajouter_etudiant(jeu, creer_etudiant(type, tour, ligne));
    }
    
    fclose(fichier);
    return 1;
}

int Charge_Sauvegarde(Jeu* jeu, char* name) {
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
        ajouter_etudiant(jeu, charger_etudiant(tour, ligne, position, type, pointsDeVie));
    }
    
    if (type == 'F'){
        while (fscanf(fichier, "%d %d %c %d\n", &ligne, &position, &type, &pointsDeVie) != EOF){
            ajouter_tourelle_existante(jeu, charger_tourelle(type, ligne, position, pointsDeVie));
        }
    }

    fclose(fichier);
    return 1;
}


int existe_fichier(char* name) {
    printf("On ouvre le fichier %s\n", name);
    FILE *fichier = fopen(name, "r");
    if (fichier == NULL) {
        return 0;
    }
    fclose(fichier);
    return 1;
}


int Sauvegarder_partie (Jeu* jeu){

    char sauvegarde_name[33];
    snprintf(sauvegarde_name, 33, "niveaux/niveau_%d/sauvegarde.txt", jeu->niveau);

    FILE *fichier = fopen(sauvegarde_name, "w");
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
    return 1;
}


void afficher_score(Jeu* jeu){
    char nom[39]; 
    snprintf (nom, 39, "niveaux/niveau_%d/meilleurs_scores.txt", jeu->niveau);
    FILE *fichier = fopen(nom, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier de score\n");
        return;
    }

    char nom2[22];
    int score;
    for (int i = 0; i < 10; i ++){
        fscanf(fichier, "%s %d\n", nom2, &score);
        char texte[40];
        snprintf(texte, 40, "%d / %s : %d", i+1, nom2, score);
        afficher_texte(jeu, "MEILLEURS SCORES", 1000, 250, 50, (SDL_Color){255, 255, 255, 255});
        afficher_texte(jeu, texte, 1020, 320 + 55*i, 40, (SDL_Color){255, 255, 255, 255});
    }
    fclose (fichier);
    return;
}


int Sauvegarder_Score (Jeu* jeu, int score){
    char nom[39]; 
    snprintf (nom, 39, "niveaux/niveau_%d/meilleurs_scores.txt", jeu->niveau);
    FILE *fichier = fopen(nom, "r");
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier de score\n");
        return 0;
    }
    int listeScores[10];
    char* listeNoms[10];
    for (int i = 0; i < 10; i ++) listeNoms[i] = malloc(21*sizeof(char));
    int classement = 10;
    for (int i = 0; i < 10; i ++){
        fscanf(fichier, "%s %d\n", listeNoms[i], &listeScores[i]);
        if (listeScores[i] < score && classement == 10)
            classement = i;
    }
    printf("%d\n", classement);
    if (classement < 10){
        listeScores[classement] = score;
        strcpy(listeNoms[classement], jeu->pseudo);
        fclose (fichier);
        FILE *fichier = fopen(nom, "w");
        for (int i = 0; i < 10; i ++)
            fprintf(fichier, "%s %d\n", listeNoms[i], listeScores[i]);
    }
    fclose (fichier);
    for (int i = 0; i < 10; i ++) free(listeNoms[i]);
    return 1;
}