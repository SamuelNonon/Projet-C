#ifndef GRAPHICS
#define GRAPHICS


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourelle.h"
#include "etudiant.h"
#include "jeu.h"


void afficher_jeu(Jeu* jeu);

int Ask_in_Menu(char **menu, int size);
void clear_lines(int n);

#endif