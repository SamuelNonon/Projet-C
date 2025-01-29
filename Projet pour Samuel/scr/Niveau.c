void ajouter_niveaux (Jeu* jeu){
  Niveau* next = NULL;
  for (int i = 3, i > 0, i--)
    next = creer_niveau (i, next);
  jeu->niveaux = next;
}

// Ajouter return fin creer_niveau


