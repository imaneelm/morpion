
#ifndef INTERFACE
#define INTERFACE


/**
 * Fonction qui recupere le choix de l'utilisateur parmi ceux pssibles (jouer un coup,
 * effectuer un coup, annuler un coup et lister les coups possibles, etc);
 */
int getChoix();


/**
 * Fonction qui prend en argument une grille a et affiche l'ensemble d es coup possible 
 * directement sur la grille;
 */
void afficher_coups_possibles_grille(grille *a);


/**
 * Fonction qui prend en argument une grille a  qui recupere le coup que veux jouer 
 * l'utilisateur et le joue sur la  grille;
 */
void getCoup(grille *a);


/**
 * Fonction qui prend en argumemnt une grille 'a' et l'affiche sur la console 
 */
void afficher_grille(grille *a);


/**
 * Fonction qui prend en argument une grille 'a' 
 * et affiche la liste des coups qu'il est possiblle de jouer sur la grille
 */
void afficher_coups_Possibles(grille *a);


/** 
 * Fonction qui prend en argument une grille 
 * et affiche l'historique des coups qui ont deja été joué
 */
void afficher_historique(grille *a);


/** 
 * Fonction qui prend en argument une 
 * grille et affiche l'historique des coups qui ont deja été annulés
 */
void afficher_coups_annules(grille *a);


/** 
 * Fonction qui prend en argument une grille 
 * et recupere le numero correspondant a un des coup possible et le joue sur la grille
 */
void jouer_coup_possible(grille *a);


/** 
 * Fonction qui permet d'afficher l'aide du jeu
 */
void afficher_aide();


/** 
 * Fonction qui permet d'afficher l'introduction du jeu
 */
 void afficher_intro();


/** 
 * Fonction qui met fin à la partie en cours
 */
void fin_partie(grille *a);


/** 
 * Fonction qui affiche une erreur lorsqu'il y a  un problème avec les arguments entrés en paramétres
 */
void afficher_erreur_args();


/** 
 * Fonction qui affiche l'aide concernat les argument à entrer en paramétre
 */
void afficher_aide_args();


#endif