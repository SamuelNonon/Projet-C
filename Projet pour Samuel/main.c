
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourelle.h"
#include "etudiant.h"
#include "jeu.h"
#include "read.h"
#include "graphics.h"
#include <unistd.h>

#define FPS 10
#define FRAME_DELAY (1000/FPS)



/*
jeu->launched ->

0/ en chargement
1/ en cours de jeu
2/ un ennemi a atteint l'universite
3/tous les ennemis sont morts

*/




int main(void) {
    char level_name[] = "level.txt";
    Jeu* jeu = creer_jeu();
    if (Charge_Level(jeu, level_name) == 0) {
        printf("Il y a eu un probleme lors du chargement\n");
    }

    
    


    
    ask_for_tourelles(jeu);

    system("CLS");

    sleep(1);
    afficher_jeu(jeu);
    sleep(1);

    while (jeu->launched == 1) {
        jeu->tour += 1;
        Update_attack(jeu);
        system("CLS");
        afficher_jeu(jeu);
        printf ("\n Score : %d\n", jeu->score);
        Update_positions(jeu);
        sleep(1);
        system("CLS");
        afficher_jeu(jeu);
        printf ("\n Score : %d\n", jeu->score);
        sleep(1);
    }

    if (jeu->launched == 2) {
        system("CLS");
        printf("Les ennemis ont gagné !\n");
    }else {
        system("CLS");
        printf("Félicitarion ! Vous avez sauvé l'université !\n");
    }
    

    supprimer_etudiants(jeu);
    supprimer_tourelles(jeu);
    free(jeu);

    return 0;
}