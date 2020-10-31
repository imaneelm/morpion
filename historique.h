
#ifndef HISTORIQUE
#define HISTORIQUE


// Data structure for stack
//#include "plateau.h"

/**
 * Rejoue le dernier coup annulé
 */
void rejouer_coup(grille*b);


/**
 * Annule le dernier coup dans l'historique
 */
void annule_coup(grille *a, grille *b);


#endif