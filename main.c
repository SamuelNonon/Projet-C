
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tourelle.h"
#include "etudiant.h"
#include "jeu.h"
#include "read.h"
#include "graphics.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

#define FPS 12
#define DELAI_IMAGE (1000/FPS)


/*
jeu->statut ->
-2/ choix du niveau
-1/ sauvegarde
0/ Pause 
1/ en cours de jeu
2/ un ennemi a atteint l'universite
3/tous les ennemis sont morts




jeu->etat : definit ou on en est dans le tour.
0/ appartition des nouveau
1/ attente fin des nouveau
2/ Attaque tourelle puis attaque ennemi. (Une seul fonction mais les deux se font bien dans cet ordre)
2/ Deplacement des ennenmie
3/ placememnt des nouveaux elements par le joureur

*/




int main(void) {
    Jeu* jeu = creer_jeu();

    

    init_SDL(jeu);
    if (!jeu->window) {
        printf("Erreur de création de la fenêtre: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    
    int debut_image;
    int temps_image;



    while (jeu->statut == -3) {
        SDL_RenderClear(jeu->renderer);
        afficherImage(jeu, "assets/ecran_titre.png", 0, 0, FENETRE_LARGEUR, FENETRE_HAUTEUR);
        afficher_texte(jeu, "Tappez votre pseudo, puis appuyez sur entrer", 300, 300, 50, (SDL_Color){255, 255, 255, 255});
        afficher_texte(jeu, "pseuso -> ", 500, 500, 50, (SDL_Color){255, 255, 255, 255});
        afficher_texte(jeu, jeu->pseudo, 700, 501, 50, (SDL_Color){255, 255, 255, 255});
        dessiner_rectangle(jeu, (SDL_Color){255, 255, 255, 255}, 668, 700, 200,75, 0);
        afficher_texte(jeu, "Valider", 700, 720, 50, (SDL_Color){59, 56, 70, 255});


        while (SDL_PollEvent(&(jeu->event))) {
            switch (jeu->event.type) {
                case SDL_QUIT : 
                    jeu->statut = 100;
                    break;
                case SDL_KEYDOWN :
                    switch (jeu->event.key.keysym.sym){
                        
                        case SDLK_RETURN : 
                            if (strlen(jeu->pseudo) > 0) jeu->statut = -2;
                        case SDLK_BACKSPACE : jeu->pseudo[strlen(jeu->pseudo)-1] = '\0'; break;
                        default:
                          if (strlen(jeu->pseudo) < MAX_LONGUEUR_PSEUDO && strlen(SDL_GetKeyName(jeu->event.key.keysym.sym)) == 1) {
                            if ((SDL_GetKeyName(jeu->event.key.keysym.sym)[0] >= 'a' && SDL_GetKeyName(jeu->event.key.keysym.sym)[0] <= 'z') || (SDL_GetKeyName(jeu->event.key.keysym.sym)[0] >= 'A' && SDL_GetKeyName(jeu->event.key.keysym.sym)[0] <= 'Z')) {
                                strcat(jeu->pseudo, &SDL_GetKeyName(jeu->event.key.keysym.sym)[0]);
                            }
                            
                            }
                    }
                case SDL_MOUSEBUTTONDOWN :
                    switch (jeu->event.button.button) {
                        case SDL_BUTTON_LEFT : 
                            if (jeu->event.button.x >= 668 && jeu->event.button.x <= 868 && jeu->event.button.y >= 700 && jeu->event.button.y <= 775) {
                                printf("Flag 1\n");
                                if (strlen(jeu->pseudo) > 0) jeu->statut = -2;
                            }
                    }
            }
        }

        SDL_RenderPresent(jeu->renderer);
    }

    

    while (jeu->statut == -2) {
        debut_image = SDL_GetTicks();
        SDL_RenderClear(jeu->renderer);
        afficherImage(jeu, "assets/ecran_titre.png", 0, 0, FENETRE_LARGEUR, FENETRE_HAUTEUR);
        afficherImage(jeu, "assets/facile.png", 100, 400, 300, 100);
        afficherImage(jeu, "assets/moyen.png", 600, 400, 300, 100);
        afficherImage(jeu, "assets/difficile.png", 1100, 400, 300, 100);
        afficherImage(jeu, "assets/hardcore.png", 350, 550, 300, 100);
        afficherImage(jeu, "assets/infini.png", 850, 550, 300, 100);
        gestion_event(jeu);



        temps_image = SDL_GetTicks() - debut_image;
        if (temps_image < DELAI_IMAGE) {
            SDL_Delay(DELAI_IMAGE - temps_image);
        }
        SDL_RenderPresent(jeu->renderer);
    }

    char sauvegarde_name[33];
    snprintf(sauvegarde_name, 32, "niveaux/niveau_%d/sauvegarde.txt", jeu->niveau);
    printf("On cherche : '%s'\n", sauvegarde_name);


    if (existe_fichier(sauvegarde_name) == 1) jeu->sauvegarde = 1;
    else jeu->sauvegarde = 0;
    

    while (jeu->statut == -1) {
        debut_image = SDL_GetTicks();
        SDL_RenderClear(jeu->renderer);
        afficherImage(jeu, "assets/ecran_titre_no_name.png", 0, 0, FENETRE_LARGEUR, FENETRE_HAUTEUR);
        afficherImage(jeu, "assets/nouvelle_partie.png", 100, 400, 300, 100);
        if (jeu->sauvegarde == 1) afficherImage(jeu, "assets/charger_sauvegarde.png", 600, 400, 300, 100);
        else afficher_texte(jeu, "< Pas de sauvegarde >", 600, 400, 50, (SDL_Color){255, 255, 255, 255});
        afficher_score(jeu);
        
        gestion_event(jeu);
        temps_image = SDL_GetTicks() - debut_image;
        if (temps_image < DELAI_IMAGE) {
            SDL_Delay(DELAI_IMAGE - temps_image);
        }
        SDL_RenderPresent(jeu->renderer);

    }
    


    if (jeu->sauvegarde == 1) {
        if (Charge_Sauvegarde(jeu, sauvegarde_name) == 0) {
            printf("Erreur lors du chargement de la sauvegarde\n");
            return 1;
        }
    }
    else {
        snprintf(sauvegarde_name, 29, "niveaux/niveau_%d/niveau.txt", jeu->niveau);
        if (Charge_Level(jeu, sauvegarde_name) == 0) {
            printf("Erreur lors du chargement du niveau\n");
            return 1;
        }
    }
    
    if (jeu->sauvegarde == 1 || jeu->niveau == 5) {
        printf("Sauvergade, on passe durecte au jeu\n");
        goto debut;
    }
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@ prévusualitsation des vagues
    
    Etudiant* courant = jeu->etudiants;
    while (courant != NULL) {
        courant->dx = FENETRE_LARGEUR + courant->tour*X_CONSTANTE + X_START;
        courant->index_animation = 0;
        courant = courant->next;

    }

    
    while (jeu->etudiants != NULL) {

        debut_image = SDL_GetTicks();
        SDL_RenderClear(jeu->renderer);
        afficherImage(jeu, MAP_IMAGE, 0, 0, 1536, 864);
        Etudiant* courant = jeu->etudiants;
        

        
        while (courant != NULL) {
            if (courant->index_animation == 10) {courant->etat = 2; courant->index_animation = 0;}
            courant->dx -= ETUDIANT_IMAGE_VITESSE;
            courant->index_animation += 1;
            char name2[31];
            snprintf(name2, 31, "assets/etudiants/%c/run_%d.png", courant->type, courant->index_animation);
            afficherImage(jeu, name2,courant->dx + courant->decalage_x, Y_START + (courant->ligne-1)*Y_CONSTANTE + courant->decalage_y, ETUDIANT_LARGEUR, courant->hauteur);
            
            courant = courant->next;
        }
        afficher_texte(jeu, "Visualisation de la vague...", 300, 890, 50, (SDL_Color){255, 255, 255, 255});
        temps_image = SDL_GetTicks() - debut_image;
        gestion_event(jeu);
        if (temps_image < DELAI_IMAGE/3) {
            SDL_Delay(DELAI_IMAGE/3 - temps_image);
        }
        SDL_RenderPresent(jeu->renderer);

        courant = jeu->etudiants;
        while (courant != NULL) {
            if (courant->dx <= 0) {courant = supprimer_etudiant(jeu, courant);}
            else courant = courant->next;
        }
        

    }
    supprimer_etudiants(jeu);
    snprintf(sauvegarde_name, 29, "niveaux/niveau_%d/niveau.txt", jeu->niveau);
    if (Charge_Level(jeu, sauvegarde_name) == 0) {
        printf("Erreur lors du chargement du niveau\n");
        return 1;
    }
    // @@@@@@@@@@@@@@@@@@@@@@@@@@@@

    
debut:

    jeu->pause = 0;

    int compteur = 0;
    Nouvelles_appartitions(jeu);


    while (jeu->statut == 1) {
        debut_image = SDL_GetTicks();
        SDL_RenderClear(jeu->renderer);
        afficherImage(jeu, MAP_IMAGE, 0, 0, 1536, 864);


        if (jeu->pause == 1) {
            afficherImage(jeu, "assets/ecran_plein.png", 300, 200, 900, 600);
            dessiner_rectangle(jeu, (SDL_Color){255, 255, 255, 255}, 300, 200, 900, 600, 4);
            afficher_texte(jeu, "PAUSE", 650, 220, 40, (SDL_Color){255, 255, 255, 255});
            afficherImage(jeu, "assets/reprendre.png", 600, 400, 300, 100);
            afficherImage(jeu, "assets/quitter_sauvegarder.png", 600, 550, 300, 100);


            gestion_event(jeu);
            SDL_RenderPresent(jeu->renderer);

            continue;
        }




        
        //je le fais la et pas dans update image pour permettre l'ajout d'élément tel que des projectiles qui ne serait qu'occasionnel.
        
        char texte[22];
        snprintf(texte, 22, "etat : %d", jeu->etat);
        afficher_texte(jeu, texte, 0, 0, 50, (SDL_Color){255, 255, 255, 255});
        //******** Gestion des etapes du tour ********

        if (compteur == 2) {
            switch (jeu->etat){
            case 0:
                actualier_prev_next_line_etudiant(jeu);
                Nouvelles_appartitions(jeu);
                jeu->etat = 1;
                break;

            case 1:
                if (est_en_mouvement_etudiants(jeu) == 0) {jeu->etat = 101;}
                break;
            case 101:
                if (est_en_mouvement_etudiants(jeu) == 0) {detection_mines(jeu); jeu->etat = 102;}
                break;
            case 102:
                if (est_en_mouvement_etudiants(jeu) == 0 && est_en_mouvement_tourelles(jeu) == 0 && effet_en_cours(jeu) == 0) {jeu->etat = 2;}
                break;
            case 2:
                actualier_prev_next_line_etudiant(jeu);
                actualiser_attaques_tourelles(jeu);
                jeu->etat = 3;
                break;
            case 3:
                if (est_en_mouvement_etudiants(jeu) == 0 && est_vide_projectiles(jeu) == 1 && effet_en_cours(jeu) == 0) {jeu->etat = 4;}
                break;

            case 4:
                actualiser_attaques_etudiants(jeu);
                jeu->etat = 5;
                break;
            case 5:
                if (est_en_mouvement_tourelles(jeu) == 0 && est_vide_projectiles(jeu) == 1 && effet_en_cours(jeu) == 0) {
                    jeu->etat = 6;
                }

            case 6:
                actualier_prev_next_line_etudiant(jeu);
                actualiser_positions(jeu);
                jeu->etat = 100;
                break;
            case 100:
                if (est_en_mouvement_etudiants(jeu) == 0 && est_en_mouvement_tourelles(jeu) == 0 && est_vide_projectiles(jeu) == 1 && effet_en_cours(jeu) == 0) {
                    detection_mines(jeu);
                    jeu->etat = 7;
                }
                break;
            case 7:
                if (est_en_mouvement_etudiants(jeu) == 0 && est_en_mouvement_tourelles(jeu) == 0 && effet_en_cours(jeu) == 0) {jeu->etat = 8; Sauvegarder_partie(jeu);} // le tour est finit on sauvegarde la partie.
                break;
            case 8:
                if (jeu->etudiants == NULL) {jeu->statut = 3;}
                break;
            case 9:
                jeu->tour += 1;
                jeu->etat = 0;
                break;

            
            }

            

            compteur =1;
            
        }



        
        
        gestion_event(jeu);

        Update_Image(jeu);

        temps_image = SDL_GetTicks() - debut_image;
        if (temps_image < DELAI_IMAGE) {
            SDL_Delay(DELAI_IMAGE - temps_image);
        }

        compteur += 1;
        
    }

    while (jeu->statut == 2 || jeu->statut == 3) {
        SDL_RenderClear(jeu->renderer);
        afficherImage(jeu, MAP_IMAGE, 0, 0, 1536, 864);
        afficherImage(jeu, "assets/ecran_plein.png", 300, 200, 900, 600);
        dessiner_rectangle(jeu, (SDL_Color){255, 255, 255, 255}, 300, 200, 900, 600, 4);
        
        if (jeu->statut == 2) afficher_texte(jeu, "Vous avez perdu !", 650, 220, 40, (SDL_Color){255, 255, 255, 255});
        else afficher_texte(jeu, "Vous avez gagne !", 650, 220, 40, (SDL_Color){255, 255, 255, 255});


        gestion_event(jeu);
        SDL_RenderPresent(jeu->renderer);
    }

    



    


    printf("Liberation de la memoire\n");
    SDL_DestroyWindow(jeu->window);
    clean_SDL(jeu);
    supprimer_effets(jeu);
    supprimer_etudiants(jeu);
    supprimer_tourelles(jeu);
    supprimer_projectiles(jeu);
    supprimer_textures(jeu);
    free(jeu->pseudo);
    free(jeu);



    return 0;
}