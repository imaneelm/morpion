#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <time.h>

#include "plateau.h"

// offset 15/30 
// x = 19
//y = 19

/**
 * Initialise le tableau de jeu de base à partir d'un fichier
 * @param a la grille a initialiser
 * @param le fichier à partir duquel l'initialiser
 * @return 1 si la création s'est bien passée 0 sinon
 * */
int init_jeu_file(grille *a, FILE *fichier){
	
	int i = 0;
  int j = 0;
 
 	for (i = 0; i<MAXGRI; i++){
		for (j = 0; j<MAXGRI; j++){
			a->gri[i][j] = 0;
		}
	}
	
	i = 0;
	j=0;
	char tmpChar;
	int tmpInt;
    
	for(i=0; i<MAXGRI;i++){
		for(j=0;j<MAXGRI;j++){
			tmpChar = fgetc(fichier);
			tmpInt = atoi(&tmpChar);
			a->gri[j][i] = tmpInt;

		}
	}
	
	a->nbhc = 0;
	a->nbAnnu = 0;
	int delta = (MAXGRI - 20) / 2;
		
	for (i = 0; i < MAXGRI; i++)
		for (j = 0; j < MAXGRI; j++)
			if ((i > delta) && (j > delta))
			{
				a->gri[i - delta][j - delta] = a->gri[i][j];
				a->gri[i][j] = 0;
			}
	memcpy(a->griInter, a->gri, sizeof(a->griInter));
	return 1;

} 


/**
 * Initialise la grille avec la croix de malte et la centre au milieu du tableau 
 * en fonction de dimension maxi de la grille
 * @param a, la grille a initialiser
 * @return 1 si la création s'est bien passée 0 sinon
 * */
int init_jeu(grille *a){

	// Centre le jeu en fonction de MAXGRI (dimension maxi de la grille de jeu)
	int i, j, delta;

	// mise à zero historique des coups
	a->nbhc = 0;
	a->nbAnnu = 0;

	// mise à zero de toutes les cases

	for (i = 0; i<MAXGRI; i++){
		for (j = 0; j<MAXGRI; j++){
			a->gri[i][j] = 0;
		}
	}

	// F:croix de malte
	a->gri[31][34] = 1;
	a->gri[31][35] = 1;
	a->gri[31][36] = 1;
	a->gri[31][37] = 1;
	a->gri[32][34] = 1;
	a->gri[32][37] = 1;
	a->gri[33][34] = 1;
	a->gri[33][37] = 1;
	a->gri[34][34] = 1;
	a->gri[34][37] = 1;
	a->gri[34][31] = 1;
	a->gri[34][32] = 1;
	a->gri[34][33] = 1;
	a->gri[34][38] = 1;
	a->gri[34][39] = 1;
	a->gri[34][40] = 1;
	a->gri[35][31] = 1;
	a->gri[35][40] = 1;
	a->gri[36][31] = 1;
	a->gri[36][40] = 1;
	a->gri[37][31] = 1;
	a->gri[37][32] = 1;
	a->gri[37][33] = 1;
	a->gri[37][34] = 1;
	a->gri[37][37] = 1;
	a->gri[37][38] = 1;
	a->gri[37][39] = 1;
	a->gri[37][40] = 1;
	a->gri[38][34] = 1;
	a->gri[38][37] = 1;
	a->gri[39][34] = 1;
	a->gri[39][37] = 1;
	a->gri[40][34] = 1;
	a->gri[40][35] = 1;
	a->gri[40][36] = 1;
	a->gri[40][37] = 1;


	// Recentrage de la croix
	delta = (MAXGRI - 30) / 2;
	for (i = 0; i<MAXGRI; i++)
		for (j = 0; j<MAXGRI; j++)
			if ((i>delta) && (j>delta))
				{
				a->gri[i - delta][j - delta] = a->gri[i][j];
				a->gri[i][j] = 0;
				}

	memcpy(a->griInter, a->gri, sizeof(a->griInter)); // on copie notre grille pour l'affichage en invite de commande dans notre grille pour l'affiche graphique

	//  retour niveau de jeu
	return(0);

}

/**
 * Extraction coordonnees X du coup 
 */
int cpX(int a){
	return (a / 10000);
}

/**
 * Extraction coordonnees Y du coup
 */
int cpY(int a){
	return ((a % 10000) / 100);
}

/**
 * Extraction de la direction du coup
 */
int cpD(int a){
	return ((a % 100) / 10);
}

/**
 * Extraction de la position du point dans l'alignement
 */
int cpK(int a){
	return (a % 10);
}

/**
 * Concatenation x,y,d,k
 */
int cpXYDK(int x, int y, int d, int k){
	return(10000 * x + 100 * y + 10 * d + k);
}

/**
 * Recherche les coups possibles sur la grille (p1) et renseigne les données dans la structure grille nbcoup et coup[]
 */
void rech_coup(grille *a){

	int i, j, k, ii, jj, kk, dir, tot;
	int tab[15], coupX, coupY, coupD, coupK;

	//  table des directions
	int dirX[4] = { 0, 1, 1, 1 };
	int dirY[4] = { 1, 1, 0, -1 };
	int dirD[4] = { 2, 4, 8, 16 };
	int dirO[4] = { 32, 64, 128, 256 };
	int dirDO[4] = { 34, 68, 136, 272 };


	//  remise a zero du nb de coups trouves
	a->nbcoup = 0;

	//  parcours de toutes les cases de la grille gri[ptr][MAXGRxMAXGR]
	for (i = 1; i<MAXGRI-1; i++){
		for (j = 1; j<MAXGRI-1; j++){
			
			// si la case pointée est vide et il y a quelque chose dans une des 8 cases autour
			if (a->gri[i][j] == 0)
			{
				if ((a->gri[i - 1][j - 1] + a->gri[i - 1][j] + a->gri[i - 1][j + 1] + a->gri[i][j - 1] + a->gri[i][j + 1] + a->gri[i + 1][j - 1] + a->gri[i + 1][j] + a->gri[i + 1][j + 1]) != 0)
				{

					// Pour chaque direction (N-S, NE-SO, E-O, SE-NO)
					for (dir = 0; dir<4; dir++)
					{
						if ((a->gri[i + dirX[dir]][j + dirY[dir]] != 0) || (a->gri[i - dirX[dir]][j - dirY[dir]] != 0))
						{								
							// extraction des 15 cases centrees sur la case courante (7 avant, 7 après)
							for (k = -7; k <= 7; k++)
								{
								tab[k + 7] = 1023;
								ii = i + k*dirX[dir];
								jj = j + k*dirY[dir];
								if ((ii >= 0) && (ii<MAXGRI) && (jj >= 0) && (jj<MAXGRI))
									tab[k + 7] = a->gri[ii][jj];
								}

							// la table tab[] est chargée, recherche d'une ligne dans ces cases
							for (k = 3; k<8; k++)
								{
									tot = 0;
									if ((tab[k] & dirD[dir]) == 0){ tot = tot + 1; }
									if ((tab[k + 1] & dirDO[dir]) == 0){ tot = tot + 1; }
									if ((tab[k + 2] & dirDO[dir]) == 0){ tot = tot + 1; }
									if ((tab[k + 3] & dirDO[dir]) == 0){ tot = tot + 1; }
									if ((tab[k + 4] & dirO[dir]) == 0){ tot = tot + 1; }
									for (kk = 0; kk<5; kk++)
										if (((k + kk) != 7) && ((tab[k + kk] & 1) == 1)){ tot = tot + 1; }

									//  si un alignement est possible
									if (tot == 9)
									{
										coupX = i;
										coupY = j;
										coupD = dir;
										coupK = k - 3;

										// chargement du coup trouve dans la tables des coups legaux
										a->coup[a->nbcoup] = cpXYDK(coupX, coupY, coupD, coupK);						
										a->nbcoup += 1;

											// verifications de coherence
										if (a->nbcoup>MAXCOU)
											{
											printf("\n******** Table des coups possibles trop petite %d %d ******** \n\n", a->nbcoup, MAXCOU);
											exit(1);
											}
										if ((i == 1) || (i == (MAXGRI - 1)) || (j == 1) || (j == (MAXGRI - 1)))
											{
											printf("\n ******** Table grille trop petite : coups trouvé en bordure %d %d ********\n\n", i, j);
											exit(1);
											}
									}
								}
						}
					}
				}
			}
		}
	}

}

/**
 *
 */
void rech_coup_opti(grille *a, grille *b, int coup_joue){

	int i, j, ix, jx, dir, k, ii, jj, kk, tot, res, xx, tab[15];
	int coupX, coupY, coupD, coupK;

	// table des directions
	int dirX[4] = { 0, 1, 1, 1 };
	int dirY[4] = { 1, 1, 0, -1 };
	int dirD[4] = { 2, 4, 8, 16 };
	int dirO[4] = { 32, 64, 128, 256 };
	int dirDO[4] = { 34, 68, 136, 272 };

	// Pour chaque coup de la table des coups précédents (table a) on regarde s'il est encore valide pour  la table b
	// Si oui, on recopie le coup vers la table des coups valides de b
	// on gagne un peu de temps au lieu de tous les rechercher et on les filtrera apres
	b->nbcoup = 0;

	// Recherche de coups poossibles autour du coup précédent
	ix = cpX(a->coup[coup_joue]);
	jx = cpY(a->coup[coup_joue]);
	for (i = ix - 4; i <= ix + 4; i++){
		for (j = jx - 4; j <= jx + 4; j++){
			if ((b->gri[i][j] == 0) && (i>0) && (j>0) && (i<MAXGRI) && (j<MAXGRI)){
				if ((b->gri[i - 1][j - 1] + b->gri[i - 1][j] + b->gri[i - 1][j + 1] + b->gri[i][j - 1] + b->gri[i][j + 1] + b->gri[i + 1][j - 1] + b->gri[i + 1][j] + b->gri[i + 1][j + 1]) != 0){
					for (dir = 0; dir<4; dir++){
						if ((b->gri[i + dirX[dir]][j + dirY[dir]] != 0) || (b->gri[i - dirX[dir]][j - dirY[dir]] != 0)){
							for (k = -7; k<=7; k++){
								tab[k + 7] = 1023;
								ii = i + k*dirX[dir];
								jj = j + k*dirY[dir];
								if ((ii >= 0) && (ii<MAXGRI) && (jj >= 0) && (jj<MAXGRI))
									tab[k + 7] = b->gri[ii][jj];
							}
							for (k = 3; k<8; k++){
								tot = 0;
								if ((tab[k] & dirD[dir]) == 0){ tot = tot + 1; }
								if ((tab[k + 1] & dirDO[dir]) == 0){ tot = tot + 1; }
								if ((tab[k + 2] & dirDO[dir]) == 0){ tot = tot + 1; }
								if ((tab[k + 3] & dirDO[dir]) == 0){ tot = tot + 1; }
								if ((tab[k + 4] & dirO[dir]) == 0){ tot = tot + 1; }

								for (kk = 0; kk<5; kk++)
									if (((k + kk) != 7) && ((tab[k + kk] & 1) == 1)){ tot = tot + 1; }
								if (tot == 9){

									// meme algo que rech_coup, sauf qu'on recherche les doublons
									res = 0;
									for (xx = 0; xx < b->nbcoup; xx++){
										coupX = cpX(b->coup[xx]);
										coupY = cpY(b->coup[xx]);
										coupD = cpD(b->coup[xx]);
										coupK = cpK(b->coup[xx]);
										if ((coupX == i) && (coupY == j) && (coupD == dir) && (coupK == (k - 3))) res = 1;
									}

									if (res == 0){
										coupX = i;
										coupY = j;
										coupD = dir;
										coupK = k - 3;

									}

								}
								
								b->coup[b->nbcoup] = cpXYDK(coupX, coupY, coupD, coupK);

								// verifications de taille
								if (b->nbcoup > MAXCOU){
									printf("\n ******** Table des coups possibles trop petite %d %d ******** \n\n", b->nbcoup, MAXCOU);
									exit(1);
								}
										
								if ((i == 1) || (i == (MAXGRI - 1)) || (j == 1) || (j == (MAXGRI - 1))){
									printf("\n ******** Table grille trop petite : coups trouvé en bordure %d %d ********\n\n", i, j);
									exit(1);
											
								}
							}
						}
					}
				}
			}
		}
	}
}


/**
 * Jouer un coup à partir de son Numéro (ID : coup_joue)
 * après avoir vu 'afficher_coup_valides()'
 */
void joue_coup(grille *a, grille *b, int coup_joue){

	// joue le coup i de la grille a sur la grille b
	int i, j, ii, jj, uu, alpha, coupX, coupY, coupD, coupK;

	// table des directions
	int dirX[4] = { 0, 1, 1, 1 };
	int dirY[4] = { 1, 1, 0, -1 };
	int dirD[4] = { 2, 4, 8, 16 };
	int dirO[4] = { 32, 64, 128, 256 };
	int dirDO[4] = { 34, 68, 136, 272 };

	// si la grille cible est differente de la grille initiale, duplication de la grille et historique des coups
	if (a != b){

		for (i = 0; i<MAXGRI; i++)
			for (j = 0; j<MAXGRI; j++)
				b->gri[i][j] = a->gri[i][j];
				
		b->nbhc = a->nbhc;
		
		for (i = 0; i<b->nbhc; i++)
			b->hc[i] = a->hc[i];

	}

	//On joue
	coupX = cpX(a->coup[coup_joue]);
	coupY = cpY(a->coup[coup_joue]);
	coupD = cpD(a->coup[coup_joue]);
	coupK = cpK(a->coup[coup_joue]);

	b->gri[coupX][coupY] = 1;
	b->griInter[coupY][coupX] = 1; // On joue le coup aussi sur la grille pour l'interface graphe

	for (uu = -4; uu <= 0; uu++){

		alpha = coupK + uu;
		ii = coupX + (alpha*dirX[coupD]);
		jj = coupY + (alpha*dirY[coupD]);

		// tests si un point ne manque pas
		if (((b->gri[ii][jj]) % 2) != 1){
			printf("\n");
			printf("\n ******** L'alignement est incomplet  : contenu:%d i=%d j=%d ********\n\n", b->gri[ii][jj], ii, jj);
			exit(1);
		}
		
		if (uu == -4) b->gri[ii][jj] = (b->gri[ii][jj] + dirD[coupD]); // on ajoute les valeurs destinées aux liasons 
		if (uu == 0)  b->gri[ii][jj] = (b->gri[ii][jj] + dirO[coupD]);
		if ((uu == -3) || (uu == -2) || (uu == -1))	 b->gri[ii][jj] = (b->gri[ii][jj] + dirDO[coupD]);
	}

	rech_coup(b);
	b->hc[b->nbhc] = a->coup[coup_joue];
	b->nbhc += 1;

}


/**
 * 
 */
int jouer_coup_coord(grille *b,int x,int y, int d, int k){ 
	
	// Joue un coup avec ses coordonées
	int ii, jj, uu, alpha;
	int coup = cpXYDK(x,y,d,k);
	int res = coupValide(b,coup);
	
	if(!res){
		printf("\n******** Votre coup n'est pas valide veuillez reesayer ********\n");
		return 0;
	}

	
	// table des directions
	int dirX[4] = { 0, 1, 1, 1 };
	int dirY[4] = { 1, 1, 0, -1 };
	int dirD[4] = { 2, 4, 8, 16 };
	int dirO[4] = { 32, 64, 128, 256 };
	int dirDO[4] = { 34, 68, 136, 272 };

	b->gri[x][y] = 1;
	b->griInter[y][x] = 1; // On joue le coup aussi sur la grille pour l'interface graphe
	
	for (uu = -4; uu <= 0; uu++)
	{
		alpha = k + uu;
		ii = x + (alpha*dirX[d]);
		jj = y + (alpha*dirY[d]);

		// tests si un point ne manque pas
		if (((b->gri[ii][jj]) % 2) != 1)
		{
			printf("\n");
			printf("\n******** L'alignement est incomplet  : contenu:%d i=%d j=%d ********\n", b->gri[ii][jj], ii, jj);
			b->gri[x][y] = 1; //
			b->griInter[y][x] = 1;// On annule le coup dans les deux grilles
			return -1; // probleme dans l'alignement
		}
		
		if (uu == -4)	b->gri[ii][jj] = (b->gri[ii][jj] + dirD[d]); // on ajoute les valeurs destinées aux liasons 
		if (uu == 0)	b->gri[ii][jj] = (b->gri[ii][jj] + dirO[d]);
		if ((uu == -3) || (uu == -2) || (uu == -1))		b->gri[ii][jj] = (b->gri[ii][jj] + dirDO[d]);
	}
		
		b->hc[b->nbhc] = cpXYDK(x,y,d,k);
		b->nbhc += 1;
		return 1;
}


/**
 * Fonction qui verifie si un coup est bien dans la liste des coups
 */
int coupValide(grille *b, int x){
	int i;
	for(i=0;i<b->nbcoup;i++){
		if(b->coup[i]==x){
			return 1;
		}

	}
	return 0;

}

/**
 * Fonction qui prend en argument une grille et affiche le tableau correspondant
 */
void affichertab(grille *a){
    
	for(int i = 0; i<50;i++){
        for(int j = 0; j<50;j++){
            printf("%d",a->gri[i][j]);
        }

        printf("\n");
    }

}


/**
 * Fonction qui prend en argument une grille 'a' 
 * et un fichier 'fichier' et écris la grille dans le fichier 'fichier'.
 * Permet notamment de sauvegarder une partie;
 */
int ecrire_grille_fichier(grille *a, FILE *fichier)
{
	fichier = fopen("Export.txt", "w+");
	int i, j;
	for (i = 0; i < MAXGRI; i++)
	{
		for (j = 0; j < MAXGRI; j++)
		{
			fprintf(fichier, "%d", a->griInter[i][j]);
		}
		fprintf(fichier, "\n");
	}
	fclose(fichier);
	return 1;
}

void grille_Aleatoire (grille *a){
	int x, y, sens;
	srand(time(NULL));
	for (int i = 0; i < 9; i++)
	{
		x = rand() % 50;
		y = rand() % 50;
		sens = rand() % 100;
		if (sens <= 25 && x < 46) // 25 % d'un alignement vertical et on évite de rentrer dans un mur
		{
			for (int j = 0; j < 4; j++)
			{
				a->gri[x + j][y] = 1;
			}
		}
		else if(sens > 25 && sens <= 50 && y>4){
			for (int j = 0; j < 4; j++)
			{
				a->gri[x][y - j] = 1;
			}
		}
		else if (sens > 50 && sens <= 75 && x > 4 && y > 4)
		{
			for (int j = 0; j < 4; j++)
			{
				a->gri[x - j][y - j] = 1;
			}
		}
		else if( x < 46 && y < 46) 
		{
				for (int j = 0; j < 4; j++)
			{
				a->gri[x + j][y + j] = 1;
			}
		}
		
	}

	// création des lignes aléatoirs de 4 points
	/*a->gri[20][24] = 1;
	a->gri[21][24] = 1;
	a->gri[22][24] = 1;
	a->gri[23][24] = 1;

	a->gri[28][21] = 1;
	a->gri[28][22] = 1;
	a->gri[28][23] = 1;
	a->gri[28][24] = 1;

	a->gri[20][21] = 1;
	a->gri[21][22] = 1;
	a->gri[22][23] = 1;
	a->gri[23][24] = 1;

	a->gri[20][21] = 1;
	a->gri[21][22] = 1;
	a->gri[22][23] = 1;
	a->gri[23][24] = 1;

	a->gri[24][29] = 1;
	a->gri[25][29] = 1;
	a->gri[26][29] = 1;
	a->gri[27][29] = 1;

	a->gri[25][26] = 1;
	a->gri[26][27] = 1;
	a->gri[27][28] = 1;
	a->gri[28][29] = 1;

	a->gri[28][21] = 1;
	a->gri[29][21] = 1;
	a->gri[30][21] = 1;
	a->gri[31][21] = 1;

	a->gri[24][27] = 1;
	a->gri[25][27] = 1;
	a->gri[26][27] = 1;
	a->gri[27][27] = 1;*/

}
