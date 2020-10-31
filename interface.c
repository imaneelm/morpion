#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <unistd.h>

#include "plateau.h"
#include "interface.h"

/**
 * Fonction qui recupere le choix de l'utilisateur parmi ceux pssibles (jouer un coup,
 * effectuer un coup, annuler un coup et lister les coups possibles, etc);
 */
int getChoix()
{

	int nb;

	do
	{
		printf("\n\n\n\tOPTIONS A JOUER \n \n");
		printf("1) Effectuer un Coup (Coordonnées).\n");
		printf("2) Annuler le dernier Coup.\n");
		printf("3) Rejouer le dernier coup annulé\n");
		printf("4) Afficher la liste des coups possibles.\n");
		printf("5) Selectioner un coup.\n");
		printf("6) Afficher les coups possibles sur la grille\n");
		printf("7) Ecrire la grille actuelle sur un fichier nommé export.txt\n");
		printf("\n\n  - Veuillez choisir une action a effectuer : ");
		scanf("%d", &nb);

		sleep(2);

	} while (nb > 7 || nb < 1);

	return nb;
}


/**
 * Fonction qui prend en argument une grille a et affiche l'ensemble d es coup possible 
 * directement sur la grille;
 */
void afficher_coups_possibles_grille(grille *a)
{
	int i, x, y;
	grille*indices = malloc(sizeof(grille)); // on malloc notre grille temporaire pour eviter les fuites mémoires si cette fonction est appelée plusieurs fois
	memcpy(indices, a, sizeof(grille));		  // on copie notre grille dans la grille temporaire
	afficher_coups_Possibles(indices);

	for (i = 0; i < a->nbcoup; i++)
	{
		x = cpX(a->coup[i]);
		y = cpY(a->coup[i]); // on ne fait que placer les "coups" sur la grille sans ajouter les liaisons
		indices->gri[x][y] = 1;
		indices->hc[i] = a->coup[i]; // on ajoute les coups à l'historique de notre grille
		indices->nbhc++;
	}

	afficher_grille(indices); // on affiche la grille d'indices
	free(indices);			  // et on la free
}


/**
 * Fonction qui prend en argument une grille a  qui recupere le coup que veux jouer 
 * l'utilisateur et le joue sur la  grille;
 */
void getCoup(grille*a)
{
	int x, y, d, k;
	printf(" - Merci de rentrer les coordonées X, Y, la Direction et la Position du point dans la ligne\n");
	printf("x : ");
	scanf("%d", &x);
	printf("\n");
	printf("y : ");
	scanf("%d", &y);
	printf("\n");
	printf("d : ");
	scanf("%d", &d);
	printf("\n");
	printf("k : ");
	scanf("%d", &k);
	printf("\n");
	jouer_coup_coord(a, x, y, d, k);
}


/**
 * Fonction qui prend en argumemnt une grille 'a' et l'affiche sur la console 
 */
void afficher_grille(grille *a)
{
		// Affiche la grille de jeu et les parametres principaux

		int i, ii, jj, j, tot, minGRx, maxGRx, minGRy, maxGRy;
		char lig1[5 * MAXGRI + 20], lig2[5 * MAXGRI + 20], lig3[5 * MAXGRI + 20], lig4[5 * MAXGRI + 20], lig5[5 * MAXGRI + 20], vlf[100];
		
		// F: recentrage du dessin
		tot = 0;
		minGRx = MAXGRI;
		maxGRx = 0;
		minGRy = MAXGRI;
		maxGRy = 0;
		for (i = 1; i < MAXGRI; i++)
			for (j = 1; j < MAXGRI; j++)
				if ((a->gri[i][j] != 0) && (a->gri[i][j] != 512))
				{
					tot = tot + 1;
					if (i > maxGRx) maxGRx = i;
					if (i < minGRx) minGRx = i;
					if (j > maxGRy) maxGRy = j;
					if (j < minGRy) minGRy = j;
				}

		sleep (2);		
		printf("\n\nGrille Score total =%d\n", tot);
		printf("xMin=%d xMax=%d yMin=%d yMax=%d\n\n\n", minGRx, maxGRx, minGRy, maxGRy);
		
		sleep(3);

		// Affichage de la grille
		for (jj = MAXGRI - maxGRy - 1; jj <= MAXGRI - minGRy - 1; jj++)
		{
			j = MAXGRI - jj - 1;
			strcpy(lig1, "    | ");
			strcpy(lig2, "    | ");
			sprintf(vlf, "%03d | ", j);
			strcpy(lig3, vlf);
			strcpy(lig4, "    | ");
			strcpy(lig5, "    | ");
			for (i = minGRx; i <= maxGRx; i++)
			{
				if ((a->gri[i][j] & 256) == 256)
				{
					strcat(lig1, "\\ ");
					strcat(lig2, " \\");
				}
				else
				{
					strcat(lig1, "  ");
					strcat(lig2, "  ");
				}
				if ((a->gri[i][j] & 2) == 2)
				{
					strcat(lig1, "|");
					strcat(lig2, "|");
				}
				else
				{
					strcat(lig1, " ");
					strcat(lig2, " ");
				}

				if ((a->gri[i][j] & 4) == 4)
				{
					strcat(lig1, " /");
					strcat(lig2, "/ ");
				}
				else
				{
					strcat(lig1, "  ");
					strcat(lig2, "  ");
				}

				if ((a->gri[i][j] & 128) == 128)
					strcat(lig3, "-");
				else
					strcat(lig3, " ");

				if (((a->gri[i][j]) % 2) == 0)
					if (a->gri[i][j] > 0)
						strcpy(vlf, " ? ");
					else
						strcpy(vlf, "   ");
				else
				{
					strcpy(vlf, "xxx"); // on remplace les points par des xxx
					for (ii = 0; ii < a->nbhc; ii++)
						if ((i == cpX(a->hc[ii])) && (j == cpY(a->hc[ii])))
							sprintf(vlf, "\033[1;31m%03d\033[0m", ii + 1); // On remplace les points placés (xxx) par leur numéro dans l'historique
					if (strcmp(vlf, "xxx") == 0)
						strcpy(vlf, " + "); //sinon c'est un +
				}
				strcat(lig3, vlf);

				if ((a->gri[i][j] & 8) == 8)
					strcat(lig3, "-");
				else
					strcat(lig3, " ");

				if ((a->gri[i][j] & 64) == 64)
				{
					strcat(lig4, " /");
					strcat(lig5, "/ ");
				}
				else
				{
					strcat(lig4, "  ");
					strcat(lig5, "  ");
				}

				if ((a->gri[i][j] & 32) == 32)
				{
					strcat(lig4, "|");
					strcat(lig5, "|");
				}
				else
				{
					strcat(lig4, " ");
					strcat(lig5, " ");
				}

				if ((a->gri[i][j] & 16) == 16)
				{
					strcat(lig4, "\\ ");
					strcat(lig5, " \\");
				}
				else
				{
					strcat(lig4, "  ");
					strcat(lig5, "  ");
				}
			}
			printf("%s\n", lig1);
			printf("%s\n", lig2);
			printf("%s\n", lig3);
			printf("%s\n", lig4);
			printf("%s\n", lig5);
		}
		strcpy(lig1, "----+-");
		for (i = minGRx; i <= maxGRx; i++)
		{
			sprintf(vlf, "-%03d-", i);
			strcat(lig1, vlf);
		}
		printf("%s\n", lig1);

}


/**
 * Fonction qui prend en argument une grille 'a' 
 * et affiche la liste des coups qu'il est possiblle de jouer sur la grille
 */
void afficher_coups_Possibles(grille*a)
{
	int i;
	printf("\n\nCoups possibles à partir de cette grille :\n");
	for (i = 0; i < a->nbcoup; i++)
	{
		printf("- coup %02d (%d) x=%d y=%d d=%d k=%d \n", i + 1, a->coup[i], cpX(a->coup[i]), cpY(a->coup[i]), cpD(a->coup[i]), cpK(a->coup[i]));
	}
}


/** 
 * Fonction qui prend en argument une grille 
 * et affiche l'historique des coups qui ont deja été joué
 */
void afficher_historique(grille *a)
{
	int i;
	printf("\n\nHistorique des coups :\n");
	for (i = 0; i < a->nbhc; i++)
	{
		printf("- coup %02d %d x=%d y=%d d=%d k=%d \n", i + 1, a->hc[i], cpX(a->hc[i]), cpY(a->hc[i]), cpD(a->hc[i]), cpK(a->hc[i]));
	}
}


/** 
 * Fonction qui prend en argument une 
 * grille et affiche l'historique des coups qui ont deja été annulés
 */
void afficher_coups_annules(grille *a)
{
	int i;

	printf("\n\nCoups annulés jusqu'à maintenant : ");

	for (i = 0; i < a->nbAnnu; i++)
	{
		printf("- coup %02d %d x=%d y=%d d=%d k=%d \n", i + 1, a->hcAnnu[i], cpX(a->hcAnnu[i]), cpY(a->hcAnnu[i]), cpD(a->hcAnnu[i]), cpK(a->hcAnnu[i]));
	}
}


/** 
 * Fonction qui prend en argument une grille 
 * et recupere le numero correspondant a un des coup possible et le joue sur la grille
 */
void jouer_coup_possible(grille *a)
{
	int choix = 0;
	printf("- Merci de rentrer le numéro du coup que vous voulez jouer (après avoir vu la liste de 'coups_possibles'): ");
	scanf("%d", &choix);
	printf("\n");
	joue_coup(a, a, choix);
}


/** 
 * Fonction qui permet d'afficher l'aide du jeu
 */
void afficher_aide()
{
	printf("\nBIENVENUE DANS LE JEU MORPION SOLITAIRE.\n\n Le but de ce jeu est d'ajouter des points de telle sorte à ce qu'il forme un alignement de 5 points.\n");
	printf("Deux alignements ne peuvent avoir qu'un seul point en commun c'est à dire ils peuvent se croiser ou se superposer en 1 et 1 seul point.\n");
	printf("Pour jouer, vous pouvez jouer un coup manuellement en rentrant les coordonées du point, la direction et la position du point dans la ligne concernée par l'alignement.\n");
	printf("La DIRECTION est codée comme suit : \n - 0 = EST et OUEST \n - 1 = NORD EST et SUD OUEST \n - 2 = NORD ET SUD \n - 3 = NORD-OUEST et SUD-EST\n");
	printf("La POSITION du point vaudra 0 s'il est le premier de l'alignement et s'incrementera jusqu'à 4 pour le dernier point de l'alignement.\n\n");
	printf("Vous pouvez aussi afficher la liste des coups possibles à partir de votre grille et jouer un coup parmi cette liste.\n\n");
}


/** 
 * Fonction qui permet d'afficher l'introduction du jeu
 */
void afficher_intro(){
	printf("\n\n\t\t========================================================\n\n");
    printf("\t\t\tBIENVENUE DANS NOTRE MORPION SOLITAIRE \n\n ");
    printf("\t\t========================================================\n\n");
}


/** 
 * Fonction qui met fin à la partie en cours
 */
void fin_partie(grille *a){
	printf("\n\n\t\t========================================================\n\n");
    printf("\t\t\tPARTIE TERMINEE VOICI LA GRILLE FINALE \n\n ");
    printf("\t\t========================================================\n\n");
    afficher_grille(a);

}


/** 
 * Fonction qui affiche une erreur lorsqu'il y a  un problème avec les arguments entrés en paramétres
 */
void afficher_erreur_args(){
	printf("Erreur d'arguments\n");
}


/** 
 * Fonction qui affiche l'aide concernat les argument à entrer en paramétre
 */
void afficher_aide_args(){
	printf("\nNotre executable permets de lancer le jeu du morpion solitaire, les regles détaillées sont disponibles dans l'executable \n");
	printf("Notre executable prends 2 options -h et -r, la premiere permets d'afficher l'aide que vous êtes en train de lire.\n");
	printf("La seconde permets de passer un fichier en parametre qui servira de grille de base pour le jeu. ");
	printf("La grille de base est de taille 50x50, votre fichier doit donc contenir 50 lignes et 50 colonnes de 0 et de 1 pour un point placé\n\n");

}