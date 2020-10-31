
#ifndef PLATEAU
#define PLATEAU

#define MAXGRI 50
#define MAXPIL 7000
#define MAXLAR 75
#define MAXCOU 200

/**
 * Structure grille contenant les attributs nbcoup= nbre de coup joues, 
 * le tableau des coups, le nombre de coup dans l'historique, l'historique des coups annulés
 * et le nombre de coup annulés;
 */
typedef struct{

	//Valeur d'une cellule
	
	//                 |     /                           \        |
	//       .    +    +    +    +-   +    +    +   -+    +      -+-
	//                                 \   |   /                  |
	//       0	  1    2    4    8    16   32  64  128   256  2+8+32+128
	//		                                                    =170    
	int gri[MAXGRI][MAXGRI];

	int griInter[MAXGRI][MAXGRI];

	// Liste des coups legaux
	int nbcoup;

	int coup[MAXLAR];

	// Nombre de mouvements dans l'historique
	int nbhc;
	
	// Historique des mouvements
	int hc[MAXCOU];

	// Liste des coups annulés 
	int hcAnnu[MAXCOU];

	// Nombre de coups annulés
	int nbAnnu;	

} grille;



/**
 * Fonctions de changement de couleurs des printf
 */
void red();

void yellow();

void reset();


/**
 * Initialise grille du jeu à parti d'un fichier
 */
int init_jeu_file(grille *a, FILE *fichier);


/**
 * Initialise le tableau de jeu de base
 */
int init_jeu(grille *a);


/**
 * Conversion du mouvement en coordonnées X, Y, D, K
 */
int cpX(int a);

int cpY(int a);

int cpD(int a);

int cpK(int a);


/**
 * Conversion coordonnées X, Y, D, K => compacte
 */
int cpXYDK(int x, int y, int d, int k);


/**
 * Recherche des coups legaux (version non optimisee)
 */ 
void rech_coup(grille *a);

/**
 * Recherche des coups legaux optimisée. nécessite la grille précedente, liste des coups précédente et dernier coup joué
 */
void rech_coup_opti(grille *a, grille *b, int coup_joue);

/**
 * Joue le coup numéro 'coup_joue' de la liste des coups possibles sur la grille
 */
void joue_coup(grille *a, grille *b, int coup_joue);


/**
 * Retourne 1 si le coup est joué 0 sinon
 */
int jouer_coup_coord(grille *b, int x, int y, int d, int k);


/**
 * Fonction qui verifie si un coup est bien dans la liste des coups
 */
int coupValide(grille *b, int x);

/**
 * Fonction qui prend en argument une grille et affiche le tableau correspondant
 */
void affichertab(grille *a);

/**
 * Fonction qui prend en argument une grille 'a' 
 * et un fichier 'fichier' et écris la grille dans le fichier 'fichier'.
 * Permet notamment de sauvegarder une partie;
 */
int ecrire_grille_fichier(grille *a, FILE *fichier);


void grille_Aleatoire (grille *a);

#endif