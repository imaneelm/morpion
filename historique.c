#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "plateau.h"
#include "historique.h"

/**
 * Rejoue le dernier coup annulé
 */
void rejouer_coup(grille *b){
	if(b->nbAnnu == 0){
		printf("\n******** Il n'y a pas de coup a rejouer!!!. Veuillez annuler un coup ********\n\n");
		return;
	}
	int coupRejoue = b->hcAnnu[b->nbAnnu-1];
	int coupX = cpX(coupRejoue);
	int coupY = cpY(coupRejoue);
	int coupD = cpD(coupRejoue);
	int coupK = cpK(coupRejoue);
    rech_coup(b);
	if(jouer_coup_coord(b,coupX,coupY,coupD,coupK)){ // on essaie de jouer le coup, s'il est joué on retire le coup de l'historique
        b->hcAnnu[b->nbAnnu-1] = 0; // On retire le coup joué de la liste
	    b->nbAnnu-=1;
    } 
    else {
        printf("\n******** Impossible de rejouer le coup merci de vous referer au message d'erreur au dessus ********\n\n");

    }
	

}


/**
 * Annule le dernier coup dans l'historique
 */
void annule_coup(grille *a, grille *b) {

	// Joue le coup i de la grille a sur la grille b

	if(a->nbhc == 0) {
		printf("\n******** Il n'y a pas de coups dans l'historique!!!. Veuillez jouer un coup *******\n\n");
		return;
	}

	int ii, jj, uu, alpha, coupX, coupY, coupD, coupK;

	// table des directions
	int dirX[4] = { 0, 1, 1, 1 };
	int dirY[4] = { 1, 1, 0, -1 };
	int dirD[4] = { 2, 4, 8, 16 };
	int dirO[4] = { 32, 64, 128, 256 };
	int dirDO[4] = { 34, 68, 136, 272 };

	//On joue
	coupX = cpX(a->hc[a->nbhc-1]);
	coupY = cpY(a->hc[a->nbhc-1]);
	coupD = cpD(a->hc[a->nbhc-1]);
	coupK = cpK(a->hc[a->nbhc-1]);
	b->gri[coupX][coupY] = 0;
	b->griInter[coupY][coupX] = 0; // On annule le coup aussi sur la grille pour l'interface graphique

	for (uu = -4; uu <= 0; uu++)
	{
		alpha = coupK + uu;
		ii = coupX + (alpha*dirX[coupD]);
		jj = coupY + (alpha*dirY[coupD]);

		// on retire les valeurs destinées aux liasons uniquement sur les cases ou le point n'a pas été placé
		if (uu == -4 && coupK != 4)	b->gri[ii][jj] = (b->gri[ii][jj] - dirD[coupD]); 
		if (uu == 0 && coupK != 0)	b->gri[ii][jj] = (b->gri[ii][jj] - dirO[coupD]);
		if ((uu == -3 && coupK != 3) || (uu == -2 && coupK !=2) || (uu == -1 && coupK != 1))	b->gri[ii][jj] = (b->gri[ii][jj] - dirDO[coupD]);
	}

	b->hc[b->nbhc] = 0; // on supprime le coup annulé de l'historique des coups joués
	b->nbhc -= 1; // On décrémente le nombre d'elements dans l'historique
	b->hcAnnu[b->nbAnnu] = cpXYDK(coupX,coupY,coupD,coupK); // On ajoute le coup dans l'historique des coups annulés
	b->nbAnnu+=1; // on incrémente le compteur de coups annulés 
}
