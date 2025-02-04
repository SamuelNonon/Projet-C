#include "graphics.h"

#define NB_LIGNES_AFFICHEES 15


int arrondi(float x) {
    int y=0;
    while (fabs(x-y)>1) y++;
    return y;
}

int maximum(int x, int y) {
    if (x>y) return x;
    return y;
}

int minimum(int x, int y) {
    if (x<y) return x;
    return y;
}

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

Effet* creer_effet(int type, int ligne, int position) {
    Effet* res = malloc(sizeof(Effet));
    res->index_animation = 0;
    res->type = type;
    res->ligne = ligne;
    res->position = position;
    res->next = NULL;
    return res;
}

void ajouter_effet(Jeu* jeu, int type, int ligne, int position) {
    if (ligne < 1 || ligne > 7 || position < 1 || position > 15) {return;}
    Effet* effet = creer_effet(type, ligne, position);
    effet->next = jeu->effets;
    jeu->effets = effet;
    return;   
}

Effet* supprimer_effet(Jeu* jeu, Effet* effet) {
    Effet* courant = jeu->effets;
    if (courant == effet) {jeu->effets = effet->next; free(effet); return jeu->effets;}
    while (courant->next != effet) {courant = courant->next;}
    courant->next = effet->next;
    free(effet);
    return courant->next;
}

int effet_en_cours(Jeu* jeu) {
    if (jeu->effets != NULL) {return 1;}
    else return 0;
}

void actualiser_effets(Jeu* jeu) {
    Effet* effet = jeu->effets;
    char nom[34]; 
    while (effet != NULL) {
        effet->index_animation += 1;
        if (effet->type == 'F') {
            snprintf(nom, 34, "assets/effets/fire_%d.png", effet->index_animation);
            afficherImage(jeu, nom, X_START +(effet->position-1)*X_CONSTANTE + 15, Y_START +(effet->ligne-1)*Y_CONSTANTE, 30, 70);
        }
        if (effet->type == 'E') {
            snprintf(nom, 31, "assets/effets/explosion_%d.png", effet->index_animation);
            afficherImage(jeu, nom, X_START +(effet->position-1)*X_CONSTANTE, Y_START +(effet->ligne-1)*Y_CONSTANTE, 75, 75);
        }
        


        if (effet->index_animation == 7) {effet = supprimer_effet(jeu, effet);}
        else effet = effet->next;
    }
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

int init_SDL(Jeu* jeu) {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        printf("Erreur SDL_Init: %s\n", SDL_GetError());
        return 0;
    }
    if (TTF_Init() < 0) {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        return EXIT_FAILURE;
    }
    jeu->window = SDL_CreateWindow("Tower Defense",
                              SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                              FENETRE_LARGEUR, FENETRE_HAUTEUR, SDL_WINDOW_SHOWN);
    if (!jeu->window) {
        printf("Erreur SDL_CreateWindow: %s\n", SDL_GetError());
        SDL_Quit();
        return 0;
    }

    jeu->renderer = SDL_CreateRenderer(jeu->window, -1, SDL_RENDERER_ACCELERATED);
    if (!jeu->renderer) {
        printf("Erreur SDL_CreateRenderer: %s\n", SDL_GetError());
        SDL_DestroyWindow(jeu->window);
        SDL_Quit();
        return 0;
    }

    // Initialiser SDL_image pour les formats PNG et JPG
    if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG))) {
        printf("Erreur SDL_image Init: %s\n", IMG_GetError());
        SDL_DestroyRenderer(jeu->renderer);
        SDL_DestroyWindow(jeu->window);
        SDL_Quit();
        return 0;
    }

    return 1;
}

void clean_SDL(Jeu* jeu) {
    SDL_DestroyRenderer(jeu->renderer);
    SDL_DestroyWindow(jeu->window);
    IMG_Quit();
    TTF_Quit();
    SDL_Quit();
    return;
}



Texture* creer_texture(Jeu* jeu, char* nom) {
    Texture* res = malloc(sizeof(Texture));
    res->nom = malloc(sizeof(char)*(strlen(nom)+1));
    strcpy(res->nom, nom);

    SDL_Surface* imageSurface = IMG_Load(nom);
    if (!imageSurface) {
        printf("Erreur chargement image: %s\n", IMG_GetError());
        return NULL;
    }

    res->texture = SDL_CreateTextureFromSurface(jeu->renderer, imageSurface);
    SDL_FreeSurface(imageSurface);
    if (!res->texture) {
        printf("Erreur création texture: %s\n", SDL_GetError());
        return NULL;
    }
    ajouter_texture(jeu, res);
    return res;
}

Texture* supprimer_texture(Jeu* jeu, Texture* texture) {
    Texture* courant = jeu->textures;
    if (texture == jeu->textures) {jeu->textures = texture->next;free(texture->nom); SDL_DestroyTexture(texture->texture) ;free(texture); return jeu->textures;}
    while (courant->next != texture) {courant = courant->next;}
    courant->next = texture->next;
    free(texture->nom);
    SDL_DestroyTexture(texture->texture);
    free(texture);
    return courant->next;
}

void ajouter_texture(Jeu* jeu, Texture* texture) {
    texture->next = jeu->textures;
    jeu->textures = texture;
    return;
}

Texture* existe_texture(Jeu* jeu, char* nom) {
    Texture* courant = jeu->textures;
    while (courant != NULL) {
        if (strcmp(courant->nom, nom) == 0) {return courant;}
        courant= courant->next;
    }
    return NULL;
}

void afficherImage(Jeu* jeu, char* cheminImage, int x, int y, int largeur, int hauteur) {

    Texture* texture = existe_texture(jeu, cheminImage);
    if (texture == NULL) {
        texture = creer_texture(jeu, cheminImage);
    }

    // Positionner et dimensionner l'image sur l'écran
    SDL_Rect destRect = { x, y, largeur, hauteur };
    SDL_RenderCopy(jeu->renderer, texture->texture, NULL, &destRect);

    return;
}


void afficher_texte(Jeu* jeu, char* texte, int x, int y, int taille, SDL_Color couleur) {
    if (strlen(texte) == 0) return;

    TTF_Font* font = TTF_OpenFont("fonts/2.ttf", taille);
    if (!font)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        return;
    }

    

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, texte, couleur); 
    if (!surfaceMessage){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        return;
    }

    SDL_Texture* textureMessage = SDL_CreateTextureFromSurface(jeu->renderer, surfaceMessage);
    SDL_FreeSurface(surfaceMessage);
    if (!textureMessage){
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "[DEBUG] > %s", TTF_GetError());
        return;
    }

    int texture_largeur, texture_hauteur;
    SDL_QueryTexture(textureMessage, NULL, NULL, &texture_largeur, &texture_hauteur);

    SDL_Rect destRect = { x, y, texture_largeur, texture_hauteur };
    SDL_RenderCopy(jeu->renderer, textureMessage, NULL, &destRect);




    
    SDL_DestroyTexture(textureMessage);

    TTF_CloseFont(font);


    return;
}
void dessiner_rectangle(Jeu* jeu, SDL_Color couleur, int x, int y, int largeur, int hauteur, int bordure) {
    if (!jeu->renderer) return;

    // Définir la couleur de rendu
    SDL_SetRenderDrawColor(jeu->renderer, couleur.r, couleur.g, couleur.b, couleur.a);

    if (bordure <= 0) {
        // Dessiner un rectangle plein
        SDL_Rect rect = {x, y, largeur, hauteur};
        SDL_RenderFillRect(jeu->renderer, &rect);
    } 
    else {
        // Dessiner uniquement la bordure avec SDL_RenderDrawLine()
        for (int i = 0; i < bordure; i++) {
            // Haut
            SDL_RenderDrawLine(jeu->renderer, x + i, y + i, x + largeur - i - 1, y + i);
            // Bas
            SDL_RenderDrawLine(jeu->renderer, x + i, y + hauteur - i - 1, x + largeur - i - 1, y + hauteur - i - 1);
            // Gauche
            SDL_RenderDrawLine(jeu->renderer, x + i, y + i, x + i, y + hauteur - i - 1);
            // Droite
            SDL_RenderDrawLine(jeu->renderer, x + largeur - i - 1, y + i, x + largeur - i - 1, y + hauteur - i - 1);
        }
    }
    SDL_SetRenderDrawColor(jeu->renderer, 0, 0, 0, 0);
}


void Update_Image(Jeu* jeu) {
    char texte1[20];
    snprintf(texte1, 20,  "Cagnotte : %d", jeu->cagnotte);
    afficher_texte(jeu,texte1, 1235, 870, 45, (SDL_Color){255, 255, 255, 255});

    char texte3[16];
    snprintf(texte3, 16,  "Score : %d", jeu->score);
    afficher_texte(jeu,texte3, 1235, 930, 45, (SDL_Color){255, 255, 255, 255});
    
    char texte2[15];
    snprintf(texte2, 15,  "Tour : %d", jeu->tour);
    afficher_texte(jeu,texte2, 30, 900, 50, (SDL_Color){255, 255, 255, 255});
    
    
    Etudiant* etudiant = jeu->etudiants;
    while (etudiant!=NULL && etudiant->position != -10) {
        actualiser_image_etudiant(jeu, etudiant);
        if (etudiant->etat == 4 && etudiant->index_animation == 10) {
            Etudiant* tmp = etudiant;
            jeu->cagnotte += etudiant->value;
            jeu->score += etudiant->value;
            etudiant = supprimer_etudiant(jeu, tmp);
        }
        else etudiant = etudiant->next;
        
    }
    Tourelle* tourelle = jeu->tourelles;
    while (tourelle != NULL) {
        actualiser_image_tourelle(jeu, tourelle);
        tourelle = tourelle->next;
    }
    Projectile* projectile = jeu->projectiles;
    while (projectile != NULL) {
        projectile = actualiser_projectile(jeu, projectile);
    }

    actualiser_effets(jeu);


    if (jeu->etat == 8) {
        
        for (int i=1; i<8; i++) {
            for (int j=1; j<MAX_CASE; j++) {
                if (search_etudiant(jeu, i, j) == NULL && search_tourelle(jeu, i, j) == NULL) {
                    dessiner_rectangle(jeu, (SDL_Color) {255, 255, 0, 255}, X_START + (j-1)*X_CONSTANTE + 30, Y_START + Y_CONSTANTE*(i-1) +60 , 2, 2, 0);
                    continue;
                }
            
            }
        }
        
    }
    if (jeu->cagnotte >= 100) {
        afficherImage(jeu, "assets/tourelles/T/static_1.png", 270, 870, 100, 100);
    }
    else afficherImage(jeu, "assets/tourelles/T/static_2.png", 270, 870, 100, 100);
    if (jeu->cagnotte >= 200) {
        afficherImage(jeu, "assets/tourelles/O/static_1.png", 380, 870, 100, 100);
    }
    else afficherImage(jeu, "assets/tourelles/O/static_2.png", 380, 870, 100, 100);     
    if (jeu->cagnotte >= 50) {
        afficherImage(jeu, "assets/tourelles/M/static_1.png", 490, 870, 100, 100);
    }
    else afficherImage(jeu, "assets/tourelles/M/static_2.png", 490, 870, 100, 100);
    if (jeu->cagnotte >= 400) {
        afficherImage(jeu, "assets/tourelles/L/static_1.png", 600, 870, 100, 100);
    }
    else afficherImage(jeu, "assets/tourelles/L/static_2.png", 600, 870, 100, 100);
    if (jeu->cagnotte >= 100) {
        afficherImage(jeu, "assets/tourelles/B/static_1.png", 710, 870, 100, 100);
    }
    else afficherImage(jeu, "assets/tourelles/B/static_2.png", 710, 870, 100, 100);
    if (jeu->cagnotte >= 200) {
        afficherImage(jeu, "assets/tourelles/F/static_1.png", 820, 870, 100, 100);
    }
    else afficherImage(jeu, "assets/tourelles/F/static_2.png", 820, 870, 100, 100);


    if (jeu->etat == 8) {
        afficherImage(jeu, "assets/tour_suivant_1.png", 930, 870, 280, 100);
    }
    else afficherImage(jeu, "assets/tour_suivant_0.png", 930, 870, 280, 100);


    switch (jeu->choix_joueur){
    case 'T':
        dessiner_rectangle(jeu, (SDL_Color) {255, 255, 255, 255}, 270 - 2, 870 - 2, 104, 104, 4);
        break;

    case 'O':
        dessiner_rectangle(jeu, (SDL_Color) {255, 255, 255, 255}, 380 - 2, 870 - 2, 104, 104, 4);
        break;
    
    case 'M':
        dessiner_rectangle(jeu, (SDL_Color) {255, 255, 255, 255}, 490 - 2, 870 - 2, 104, 104, 4);
        break;
    case 'L':
        dessiner_rectangle(jeu, (SDL_Color) {255, 255, 255, 255}, 600 - 2, 870 - 2, 104, 104, 4);
        break;
    case 'B':
        dessiner_rectangle(jeu, (SDL_Color) {255, 255, 255, 255}, 710 - 2, 870 - 2, 104, 104, 4);
        break;
    case 'F':
        dessiner_rectangle(jeu, (SDL_Color) {255, 255, 255, 255}, 820 - 2, 870 - 2, 104, 104, 4);
        break;
    
    }
    
    

    SDL_RenderPresent(jeu->renderer);
}


