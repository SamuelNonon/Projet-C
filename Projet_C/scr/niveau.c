#include "niveau.h"

Niveau* creer_niveau (int difficulte){
    Niveau* res = malloc(sizeof(Niveau));
    res->difficulte = difficulte;
    char* nom = malloc (13 * sizeof(char));
    sprintf (nom, "niveau_%d.txt", difficulte);
    res->nom = nom;
    char* sauvegarde = malloc (24 * sizeof(char));
    sprintf (sauvegarde, "niveau_%d_sauvegarde.txt", difficulte);
    res->sauvegarde = sauvegarde;
    char* scores = malloc (30 * sizeof(char));
    sprintf (scores, "niveau_%d_meilleurs_scores.txt", difficulte);
    res->meilleursScores = scores;
    return res;
}
