#ifndef SUDOKU_FUNCTIONS_H
#define SUDOKU_FUNCTIONS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

//matrice possility element
struct mpe {
    char element;
    struct mpe* next;
};


//matrice possility list
struct mpl {
    struct mpe* load;
    int nb_elem;
    struct mpe* current_element;
};

struct matrice_possibility {
    struct mpl* possibilities[9][9];
};

/**
 * @brief structure contenant un sudoku
 * @property identifiant chaine de caractère de 10 (attention il y a un piège)
 * @property matrice de caractères contenant les donnees du sudoku (9 par 9 case)
 * @property nb_iteration un entier non signé permettant de voir combien d'iteration on été réalisée
 */
struct sudoku_exercice {
    char identifiant[11];
    char matrice[9][9];
    unsigned int nb_iteration;
};

/**
 * @brief init_sudoku_exercice permet d'initialiser un sodoku exercice
 * @param exerc un pointer sur la structure à initialiser
 * @note memset ? tous les champs ne seront pas intialisés avec la même valeur
 */
void init_sudoku_exercice(struct sudoku_exercice* exerc);

/**
 * @brief structure de liste contenant un element sudoku exercice
 * @property prev un pointer sur l'element precedent
 * @property next un pointer sur l'element suivant
 * @property sudoku un pointeur sur l'exercice
 */
struct sudoku_element {
    struct sudoku_element* prev;
    struct sudoku_element* next;
    struct sudoku_exercice* sudoku;
};


/**
 * @brief structure encapsulant une liste de sudoku element. Permet de realiser toutes les opérations sur la liste
 * @property load un pointer sur la liste d'éléments (en fait sur le premier qui donne acces aux suivants).
 * @property first un pointer sur le premier element mais qui ne sera jamais modifié (permet de toujours conserver l'information, sécurité).
 * @property nb_element un entier non signé contenant le nombre d'élément présentement dans la liste.
 * @note il aurait été tout à fait possible de se passer de cette structure mais elle est pratique
 */
struct sudoku_list {
    struct sudoku_element* load;
    struct sudoku_element* first;
    unsigned int nb_element;
};

/**
 * @brief structure permettant de résoudre un sudoku
 * @property exerc un pointeur sur le sudoku exercice
 * @property matrice un pointeur sur la matrice des possibilités
 * @property nb_iteration le nombre d'itérations qui seront nécessaires pour résoudre le sudoku
*/
struct sudoku_exercice_solve {
    struct sudoku_exercice* exerc;
    struct matrice_possibility* matrice;
    int nb_iteration;
};

/**
 * @brief initialize une liste de sudoku
 * @param list_to_init un pointer sur la liste à initialiser
 * @note lors de l'initialisation, la liste doit être vide et ses variables complètement intialisées.
 * Si la liste n'est pas vide, faire un message d'erreur et ne rien faire de plus
 */
void init_list_sudoku(struct sudoku_list* list);

/**
 * @brief copy_sudoku_exercice renvoie une copie du sudoku exercice passé en paramètre
 * @param from_data un pointer sur la structure qui sera copiée
 * @return un pointer sur la copie du sudoku exercice. allocation dynamique de memoire
 */
struct sudoku_exercice* copy_sudoku_exercice(struct sudoku_exercice* from_data);

/**
 * @brief init_matrice initialise une matrice en allouant dynamiquement de la mémoire, met toute la structure à 0
 * @return un pointer sur la matrice alouée dynamiquement
 */
struct matrice_possibility* init_matrice();

/**
 * @brief compute_matrice permet de remplir la matrice en fonction des différentes possibilité du sudoku exercice passé en paramètre
 * l'idee est parcourir toute la matrice et de crééer la liste des valeurs possible pour chaque emplacement
 * @param matrice la matrice que l'on va remplir
 * @param exerc le sudoku exercice contenant les informations de base
 * @return un entier qui ne sera pas utilisé
 */
int compute_matrice(struct matrice_possibility* matrice, struct sudoku_exercice *exerc);

/**
 * @brief compute_possibility coeur de resolution des sudokus. Cette fonction prend en paramètre la matrice des possibilités et recherche la première case
 * contenant plus d'un possibilités de valeurs. Une fois qu'elle l'a trouvée, et choisi une valeur qu'elle fixe à l'aide du current element de la structure mpl.
 * Une foix ce current element fixé, et vérifie si la valeur est possible considérant les regles s'appliquant aux ligne et aux colonnes dans un sudoku.
 * Si la valeur est possible, alors elle passe à la prochaine case ou il y a plusieurs valeurs possibles en s'appelant elle même.
 * Si la valeur n'est pas possible, alors elle passe a sa propre valeur possible suivante.
 * Si il n'y a plus de valeur suivante possible pour cette case, alors on remonte d'une case et on recommence avec une autre valeur.
 * @param matrice base sur laquelle l'on efefctue les calculs
 * @param nb_possibility un pointeur sur un entier dans lequel l'on va stocker le nombre d'iteration
 * @return un entier representant l'etat du calcul dans la matrice. les valeurs possibles sont :
 *      > -1 : le calcul n'abouti pas (la valeur fixée ne peut pas etre la bonne)
 *      > 81 : toutes les valeurs on été fixées et une seule valeur valide a été choisie par case. On a donc résolu le sudoku et on peut passer au suivant.
 */
int compute_possibility(struct matrice_possibility* matrice, int* nb_possibility);

/**
 * @brief compute_uplet permet de savoir si la valeur de la matrice à la case line:colonne est valide dans la ligne line et dans la colonne colonne
 * @param matrice la matrice dans laquelle l'on vérifie les informations
 * @param line la ligne que l'on vérifie
 * @param colonne la colonne que l'on vérifie
 * @return -1 si la valeur n'est pas bonne (elle est deja presente de façon unique dans la ligne et dans la colonne), 1 sinon
 */
int compute_uplet(struct matrice_possibility* matrice, int line, int colonne);

/**
 * @brief contain_char_mpe permet de savoir si une liste de valeurs possibles (une case de la matrice de possibilités ayant plus d'une valeur)
 * contient le caractère que l'on test
 * @param c le caractère à tester
 * @param e la liste de valeur possibles dans laquelle l'on va fare la recherche
 * @return -1 si la valeur est présente dans la liste, le nombre d'element de la liste sinon
 */
int contain_char_mpe(char c, struct mpl* e);

/**
 * @brief delete_mpl supprime l'ensemble des elements alloués dynamiquement de la liste passée en paramètre un par un
 * @param mpl_to_delete la liste que l'on souhaite supprimer
 * @note ne pas oublier de supprimer la structure list en desalouant la mémoire à l'aide d'un free et de mettre le pointer à NULL
 */
void delete_mpl(struct mpl* mpl_to_delete);

/**
 * @brief delete_matrice supprime l'ensemble de la matrice de possibilité, notament toutes les listes de valeurs possibles qui la compose
 * @param matrice la matrice que l'on souhaite supprimer
 * @note ne pas oublier de supprimer la structure matrice en desalouant la mémoire à l'aide d'un free et de mettre le pointer à NULL
 */
void delete_matrice(struct matrice_possibility* matrice);

/**
 * @brief solve_exercice resout completement un exercice sudoku
 * alloue dynamiquement l'exercice solve, initialise la matrice, la rempli avec les valeurs possibles, la résoud, et retourne un pointer sur la structure de resultat
 * @param exerc l'exercice de sudoku qui sert de base de travail
 * @return un pointer sur l'exercice solve alloué dynamiquement et constitué dans la fonction
 */
struct sudoku_exercice_solve* solve_exercice(struct sudoku_exercice* exerc);

/**
 * @brief compute_complexity_correct renvoie la complexité de la matrice, c'est à dire le nombre de valeurs possibles dans cette matrice
 * Pour chaque case de la matrice, cette fonction regarde si un élément à été fixé (possibility->possibilities[iter_line][iter_col]->current_element != NULL)
 * dans ce cas précis, la fonction considère que la complexité de la matrice est égale à 1 (même si il reste des valeurs dans la liste des possibilités qui n'ont pas été testées)
 * si possibility->possibilities[iter_line][iter_col]->current_element == NULL alors la complexité de la case est égale au nombre d'éléments de la liste de possiblité (cette case
 * n'a pas encore de valeur fixée)
 * Si la complexité est de 81, alors l'exercice est résolu (toutes les cases ont une valeur fixée et toutes ces valeurs sont valides selon les règles du sudoku)
 * @param possibility la matrice dont on cherche la complexité
 * @return la complexité de la matrice ou -1 si au moins l'une des listes de possibilités a un nombre d'éléments possible de 0
 * @note a chaque fois qu'une valeur est choisie, la complexité diminiue ou alors est égale à -1 (la valeur fixée n'est pas une valeur possible selon les
 * règles du sudoku)
 */
int compute_complexity(struct matrice_possibility* possibility);

/**
 * @brief fill_sudoku_exerc_with_matrice_correct prend toutes les valeurs de la matrice de possibilités et les met dans la matrice du sudoku
 * @param exerc le sudoku que l'on va remplir
 * @param possi la matrice qui sert d'informations
 * @note a n'appeler qu'une fois que la complexité de la matrice est de 81. Apres cette fonction, si la matrice de possibilité est valide, le sudoku est resolu et peut etre affiché
 */
void fill_sudoku_exerc_with_matrice(struct sudoku_exercice* exerc, struct matrice_possibility* possi);

/**
 * @brief delete_tab_exercice supprime la liste de sudoku
 * @param list un pointer sur la liste ou supprimer les element
 */
void delete_tab_exercice(struct sudoku_list* list);

/**
 * @brief print_sudoku_element affiche un sudoku element dans la console
 * @param elem un pointer sur l'element a afficher
 */
void print_sudoku_element(struct sudoku_element* elem);

/**
 * @brief initialise le tableau de sudoku
 * @param list un pointer sur la liste ou l'on va stocker les resultats de la génération
 */
void init_tab_exercice(struct sudoku_list* list);

/**
 * @brief creer un element en fin de liste
 * @param base_list un pointer sur la liste ou l'on va insérer le sudoku
 * @return un pointer sur le sudoku_element créé.
 * @note ne pas oublier d'initialiser correctement le sudoku element
 */
struct sudoku_element* create_end(struct sudoku_list* base_list);

/**
 * @brief generate_id genere l'id d'un sudoku a partir du numero passe en parametre
 * @param num_sudoku le numero du sudoku.
 * @param dest le tableau de char ou l'id sera enregistre
 * @note l'id aura pour forme "sudokuXXXX" ou XXXX est un nombre fill 0
 */
void generate_id(int num_sudoku, char* dest);

/**
 * @brief init_sudoku_element initialise un sudoku element
 * @param elem un pointer sur la structure à initialiser
 */
void init_sudoku_element(struct sudoku_element* elem);

/**
 * @brief print_possibility_matrice affiche la matrice des possibilités
 * @param possibility la matrice à afficher
 * @return -1 si une des cases de la matrice ne contient pas d'éléments
 */
int print_possibility_matrice(struct matrice_possibility* possibility);

/**
 * @brief compute_carre permet de savoir si la valeur de la matrice à la case line:colonne est valide dans le carré
 * @param matrice la matrice dans laquelle l'on vérifie les informations
 * @param line la ligne que l'on vérifie
 * @param colonne la colonne que l'on vérifie
 * @return -1 si la valeur n'est pas bonne (elle est deja presente de façon unique dans le carre), 1 sinon
 */
int compute_carre(struct matrice_possibility* possibility, int line, int col);



#endif // SUDOKU_FUNCTIONS_H
