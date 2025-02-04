#include "jeu.h"


int max_tour(Jeu* jeu) { //retourne le numéro de tour le plus haut où un etudiant doit apparaitre.
    if (jeu->etudiants == NULL) return 0;
    Etudiant* current = jeu->etudiants;
    int max = 0;
    while (current != NULL) {
        if (current->position>max) {
            max = current->position;
        }
        current = current->next;
    }
    return max;
}

Jeu* creer_jeu(void) {
    Jeu* res = malloc(sizeof(Jeu));
    res->etudiants = NULL;
    res->tourelles = NULL;
    res->effets = NULL;
    res->textures = NULL;
    res->projectiles = NULL;
    res->tour = 0;
    res->cagnotte = 0;
    res->statut = -3;
    res->pause = 0;
    res->etat = 8;
    res->window = NULL;
    res->renderer = NULL;
    res->score = 0;
    res->sauvegarde = 0;
    res->choix_joueur = (int)'T';
    res->niveau = 1;
    res->pseudo = malloc((MAX_LONGUEUR_PSEUDO + 1)*sizeof(char));
    res->pseudo[0] = '\0';
    return res;
}

Etudiant* search_etudiant(Jeu* jeu, int ligne, int position) { //a une ligne/position donnee, return NULL s'il n y a pas d'etudiant, si oui il retourne le pointeur sur lui.
    Etudiant* current = jeu->etudiants;
    while (current != NULL) {
        if (current->ligne == ligne && current->position == position && current->position != -10 ) {
            return current;
        }
        current = current->next;
    }
        
    return NULL;
}

Tourelle* search_tourelle(Jeu* jeu, int ligne, int position) {
    Tourelle* current = jeu->tourelles;
    while (current != NULL) {
        if (current->ligne == ligne && current->position == position) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void supprimer_effets(Jeu* jeu) {
    while (jeu->effets != NULL) {supprimer_effet(jeu, jeu->effets);}
    return;
}

void supprimer_etudiants(Jeu* jeu) {
    if (jeu->etudiants == NULL) {return;}
    while (jeu->etudiants != NULL) {jeu->etudiants = supprimer_etudiant(jeu, jeu->etudiants);}
    return;
}

void supprimer_tourelles(Jeu* jeu) {
    if (jeu->tourelles == NULL) {return;}
    while (jeu->tourelles != NULL) jeu->tourelles = supprimer_tourelle(jeu, jeu->tourelles);
    return;
}

void supprimer_projectiles(Jeu* jeu) {
    if (jeu->projectiles == NULL) return;
    while (jeu->projectiles != NULL) {jeu->projectiles = supprimer_projectile(jeu, jeu->projectiles);}
    return;
}

void supprimer_textures(Jeu* jeu) {
    if (jeu->textures == NULL) return;
    while (jeu->textures != NULL) {
        supprimer_texture(jeu, jeu->textures);
    }
    return;

}

void Nouvelles_appartitions(Jeu* jeu) {
    Etudiant* etudiant = jeu->etudiants;
    while (etudiant != NULL) {
        if (etudiant->position == -10 && etudiant->tour == jeu->tour) {
            if (search_etudiant(jeu, etudiant->ligne, NUMERO_POSITION_1ER_APPARITION) == NULL && (search_tourelle(jeu, etudiant->ligne, NUMERO_POSITION_1ER_APPARITION) == NULL || search_tourelle(jeu, etudiant->ligne, NUMERO_POSITION_1ER_APPARITION)->type == 'B')) {
                etudiant->position = NUMERO_POSITION_1ER_APPARITION;
                etudiant->etat = 2;
                etudiant->dx = 100;
            }
            else etudiant->tour += 1; // la place est deja prise
        }
        etudiant = etudiant->next;
    }

    return;
}


void actualier_prev_next_line_etudiant(Jeu* jeu) {
    Etudiant* etudiant = jeu->etudiants;
    while (etudiant != NULL) {
        update_prev_line(jeu, etudiant);
        update_next_line(jeu, etudiant);
        etudiant = etudiant->next;
    }
    return;
}




void demander_tourelle(Jeu* jeu) {
    
    int r = 1;
    while ( r== 1) {
        system("clear");
        afficher_jeu(jeu);
        printf("Cagnotte : %d\n", jeu->cagnotte);
        char *menu[] = {"Placer une nouvelle tourelle", "Lancer la partie"};
        r = Ask_in_Menu(menu, 2);
        clear_lines(4);
        int valide = 0;
        
        while (valide == 0 && r==1) {
            if (jeu->cagnotte < 100) {
            printf("Vous n'avez pas assez d'argent pour une autre tourelle\n");
            return;
            }
            jeu->cagnotte -= 100;
            int l, p;
            printf("Ligne ->");
            scanf("%d", &l);
            printf("Position ->");
            scanf("%d", &p);
            if (l>=1 && l<=7 && p>=1 && p<= MAX_CASE) {
                if (search_etudiant(jeu, l, p) != NULL) {
                    clear_lines(4);
                    printf("Il y a deja un ennemi a cette position\n");
                    continue;
                }
                if (search_tourelle(jeu, l, p) != NULL) {
                    clear_lines(4);
                    printf("Il y a deja une tourelle a cette position\n");
                    continue;
                }
                ajouter_tourelle(jeu, (int)'T', l, p);
                valide =1;
            }
        }
    }

    return;
}

void actualiser_attaques_etudiants(Jeu* jeu) {
    Etudiant* etudiant = jeu->etudiants;
    while (etudiant != NULL && etudiant->position >-5) {
        printf("On attaque entre : %d et %d : \n",etudiant->position -1, etudiant->position - etudiant->portee);
        for (int i = etudiant->position -1; i>= etudiant->position - etudiant->portee; i--) {
            Tourelle* t = search_tourelle(jeu, etudiant->ligne, i);
            if (etudiant->type == 'M') {ajouter_effet(jeu, 'F', etudiant->ligne, i);}
            if (t!= NULL && t->type != 'B' && etudiant->prev_line == NULL) {
                t->pointsDeVie -= etudiant->degats;
                etudiant->etat = 3;
                if (etudiant->type == 'T') {ajouter_effet(jeu, 'E', etudiant->ligne, i);}
                if (t->pointsDeVie <= 0) {ajouter_effet(jeu, (int)'E', t->ligne, t->position) ; supprimer_tourelle(jeu, t);}
            }
        }
        etudiant = etudiant->next;
    }
    return;
}

void actualiser_attaques_tourelles(Jeu* jeu) {
    Tourelle* tourelle = jeu->tourelles;
    while (tourelle != NULL) {
        tourelle_attack(jeu, tourelle);
        tourelle = tourelle->next;
        
    }
    return;
}

void actualiser_positions(Jeu* jeu) {

    Etudiant* courant = jeu->etudiants;
    
    while(courant != NULL && courant->position >-5) {
        update_prev_line(jeu, courant);
        
        int j = 0;
        for(int i=1; i<courant->vitesse+1; i++) {
            
            if(courant->type == 'D' && (courant->prev_line == NULL || courant->prev_line->position < courant->position-1) && (search_tourelle(jeu, courant->ligne, courant->position-1) == NULL || search_tourelle(jeu, courant->ligne, courant->position-1)->type == 'B' || search_tourelle(jeu, courant->ligne, courant->position-1)->type == 'M')) {courant->position -= 1; j++; continue;}


            if (search_tourelle(jeu, courant->ligne, courant->position) != NULL && search_tourelle(jeu, courant->ligne, courant->position)->type == 'B') break; //si sur la place actuelle il y a une bombe
            if (search_tourelle(jeu, courant->ligne, courant->position-1) != NULL && search_tourelle(jeu, courant->ligne, courant->position-1)->type != 'B') break; //si il y a une tourelle juste devant qui n'est pas une bombe



            if (courant->prev_line == NULL || courant->prev_line->position < courant->position-1) {courant->position -= 1; j++; continue;}
            break;
        }
        
        if (j > 0) {courant->etat = 2; courant->dx = j*X_CONSTANTE;}   
        if (courant->position <= 0) {jeu->statut = 2; return;}
        courant = courant->next;
        
    }

    return;
}



void gestion_event(Jeu* jeu) {


    int x;
    int y;
    SDL_GetMouseState(&x, &y);
    if (x>= 270 && x<= 270+100 && y>= 870 && y<= 870+100) {afficherImage(jeu, "assets/infos/O.png", 270, 670, 100, 200);}



    while (SDL_PollEvent(&(jeu->event))) {
        switch (jeu->event.type) {

                case SDL_QUIT : 
                    jeu->statut = 100;
                    break;
                
                case SDL_WINDOWEVENT : 
                    if (jeu->event.window.event == SDL_WINDOWEVENT_MINIMIZED) {
                        jeu->pause = 1;
                    }
                    break;
                

                case SDL_KEYDOWN :
                    switch (jeu->event.key.keysym.sym){
                        case  SDLK_a : printf("touche aaaaaaa"); break;
                        case SDLK_ESCAPE : 
                            if (jeu->pause == 0) {jeu->pause = 1; break;}
                            else if (jeu->pause == 1) {jeu->pause = 0; break;}
                        case SDLK_RETURN : if (jeu->etat == 8 && jeu->statut ==1) {jeu->etat = 9;} break;
                    }
                    break;

                
                case SDL_MOUSEBUTTONDOWN :
                    switch (jeu->event.button.button) {
                        case SDL_BUTTON_LEFT : 

//Jeu statut = -2
                            if (jeu->statut == -2) {
                                if (jeu->event.button.x >= 100 && jeu->event.button.x <= 100+300 && jeu->event.button.y >= 400 && jeu->event.button.y <= 400+100) {
                                    jeu->niveau = 1;
                                    jeu->statut = -1;
                                    break;
                                }
                                if (jeu->event.button.x >= 600 && jeu->event.button.x <= 600+300 && jeu->event.button.y >= 400 && jeu->event.button.y <= 400+100) {
                                    jeu->niveau = 2;
                                    jeu->statut = -1;
                                    break;
                                }
                                if (jeu->event.button.x >= 1100 && jeu->event.button.x <= 1100+300 && jeu->event.button.y >= 400 && jeu->event.button.y <= 400+100) {
                                    jeu->niveau = 3;
                                    jeu->statut = -1;
                                    break;
                                }
                                if (jeu->event.button.x >= 600 && jeu->event.button.x <= 600+300 && jeu->event.button.y >= 550 && jeu->event.button.y <= 550+100) {
                                    jeu->niveau = 4;
                                    jeu->statut = -1;
                                    break;
                                }
                            }
//Jeu statut = -1
                            if (jeu->statut == -1) {
                                if (jeu->event.button.x >= 100 && jeu->event.button.x <= 100+300 && jeu->event.button.y >= 400 && jeu->event.button.y <= 400+100) {
                                    jeu->sauvegarde = 0;
                                    jeu->statut = 1;
                                    break;
                                }
                                if (jeu->event.button.x >= 600 && jeu->event.button.x <= 600+300 && jeu->event.button.y >= 400 && jeu->event.button.y <= 400+100) {
                                    jeu->statut = 1;
                                    break;
                                }
                                break;
                            }
//Jeu partie en cours
                            if (jeu->statut == 1 && jeu->pause == 0) {


                                if (jeu->event.button.x >= 270 && jeu->event.button.x <= 270+100 && jeu->event.button.y >= 870 && jeu->event.button.y <= 870+100) {jeu->choix_joueur = (int)'T';}
                                if (jeu->event.button.x >= 380 && jeu->event.button.x <= 380+100 && jeu->event.button.y >= 870 && jeu->event.button.y <= 870+100) {jeu->choix_joueur = (int)'O';}
                                if (jeu->event.button.x >= 490 && jeu->event.button.x <= 490+100 && jeu->event.button.y >= 870 && jeu->event.button.y <= 870+100) {jeu->choix_joueur = (int)'M';}
                                if (jeu->event.button.x >= 600 && jeu->event.button.x <= 600+100 && jeu->event.button.y >= 870 && jeu->event.button.y <= 870+100) {jeu->choix_joueur = (int)'L';}
                                if (jeu->event.button.x >= 710 && jeu->event.button.x <= 710+100 && jeu->event.button.y >= 870 && jeu->event.button.y <= 870+100) {jeu->choix_joueur = (int)'B';}
                                if (jeu->event.button.x >= 820 && jeu->event.button.x <= 820+100 && jeu->event.button.y >= 870 && jeu->event.button.y <= 870+100) {jeu->choix_joueur = (int)'F';}




                                //placement des tourelles quand c'est le bon moment
                                if (jeu->etat == 8) {
                                    for (int i=1; i<8; i++) {
                                        for (int j=1; j<MAX_CASE; j++) {
                                            if (jeu->event.button.x >= X_START + (j-1)*X_CONSTANTE && jeu->event.button.x <= X_START + (j-1)*X_CONSTANTE + X_CONSTANTE/2 && jeu->event.button.y >= Y_START + (i-1)*Y_CONSTANTE && jeu->event.button.y <= Y_START + (i-1)*Y_CONSTANTE + Y_CONSTANTE/2) {
                                                if (search_etudiant(jeu, i, j) == NULL && search_tourelle(jeu, i, j) == NULL) {
                                                    switch (jeu->choix_joueur){
                                                    case 'T':
                                                        if (jeu->cagnotte >= 100) {ajouter_tourelle(jeu, jeu->choix_joueur, i, j); jeu->cagnotte-= 100;}
                                                        break;
                                                    
                                                    case 'O':
                                                        if (jeu->cagnotte >= 200) {ajouter_tourelle(jeu, jeu->choix_joueur, i, j); jeu->cagnotte-= 200;}
                                                        break;
                                                    
                                                    case 'M':
                                                        if (jeu->cagnotte >= 50) {ajouter_tourelle(jeu, jeu->choix_joueur, i, j); jeu->cagnotte-= 50;}
                                                        break;
                                                    case 'L':
                                                        if (jeu->cagnotte >= 400) {ajouter_tourelle(jeu, jeu->choix_joueur, i, j); jeu->cagnotte-= 400;}
                                                        break;
                                                    case 'B':
                                                        if (jeu->cagnotte >= 100) {ajouter_tourelle(jeu, jeu->choix_joueur, i, j); jeu->cagnotte-= 100;}
                                                        break;
                                                    case 'F':
                                                        if (jeu->cagnotte >= 200) {ajouter_tourelle(jeu, jeu->choix_joueur, i, j); jeu->cagnotte-= 200;}
                                                        break;
                                                    }
                                                }
                                            }
                                        }
                                    }

                                    if (jeu->event.button.x >= 928 && jeu->event.button.x <= 928+280 && jeu->event.button.y >= 866 && jeu->event.button.y <= 866+108) {
                                        jeu->etat = 9;
                                    }

                                }
                            }
// en pause
                            if (jeu->pause == 1) {
                                if (jeu->event.button.x >= 600 && jeu->event.button.x <= 600+300 && jeu->event.button.y >= 400 && jeu->event.button.y <= 400+100) {
                                    jeu->pause = 0;
                                }
                                if (jeu->event.button.x >= 600 && jeu->event.button.x <= 600+300 && jeu->event.button.y >= 550 && jeu->event.button.y <= 550+100) {
                                    jeu->statut = 100;
                                }
                            }
                            
                            break;
                    }
        }
    }
}

