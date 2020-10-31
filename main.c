
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "plateau.h"
#include "historique.h"
#include "interface.h"

int main(int argc, char *argv[])
{
    FILE * fichierExport = NULL;
    grille Init;

    if (argc == 1) {init_jeu(&Init);}

    else if(argc==2 && strcmp (argv[1], "-h") == 0){
        afficher_aide();
        return 1;
    }

    else if (argc == 3 && strcmp (argv[1], "-r") == 0) {
        FILE* fichier;
        fichier = fopen(argv[2], "r+");
        init_jeu_file(&Init,fichier);
    }

    else {
        afficher_erreur_args();
        return 1;
    }

    afficher_intro();

    int /*NIV, */ choix;
    /*NIV = */  // on initialise le jeu de base
    rech_coup(&Init);           // on recherche les coups possibles
    afficher_grille(&Init);     // on affiche la grille

    do
    {
        
        choix = getChoix();

        switch (choix)
        {
        case 1:
            getCoup(&Init);
            rech_coup(&Init);
            break;
        case 2:
            annule_coup(&Init, &Init);
            rech_coup(&Init);
            break;
        case 3:
            rejouer_coup(&Init);
            rech_coup(&Init);
            break;
        case 4:
            afficher_coups_Possibles(&Init);
            break;
        case 5:
            jouer_coup_possible(&Init);
            rech_coup(&Init);
            break;
        //case 6: affichertab(&Init);break; // là pour le debug
        //case 6: afficher_grille(&Init);
        case 6:
            afficher_coups_possibles_grille(&Init);
            break;
        case 7:
            ecrire_grille_fichier(&Init,fichierExport);
            break;
        default:
            break;
        }

    afficher_grille(&Init);

    } while (Init.nbcoup > 0);

    fin_partie(&Init);
    
}