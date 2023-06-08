#include "sudoku_functions.h"

void init_sudoku_exercice(struct sudoku_exercice *exerc)
{
    memset(&(exerc->identifiant), '0', 11 * sizeof(char));
    memset(&(exerc->matrice), '.', 81 * sizeof(char));
    exerc->nb_iteration = 0;
    memset(&(exerc->nb_iteration), 0, sizeof(int));
}

struct sudoku_exercice *copy_sudoku_exercice(struct sudoku_exercice *from_data)
{
    /**
     * @todo 001
     */
    struct sudoku_exercice *exer = malloc(sizeof(struct sudoku_exercice));
    memmove(exer, from_data, sizeof(struct sudoku_exercice));
    return exer;
}

struct matrice_possibility *init_matrice()
{

    struct matrice_possibility *mat = NULL;
    /**
     * @todo 002
     */
    mat = malloc(sizeof(struct matrice_possibility));
    memset(mat, 0, sizeof(struct matrice_possibility));
    return mat;
}

/**
 * @brief compute_matrice permet de remplir la matrice en fonction des différentes possibilité du sudoku exercice passé en paramètre
 * l'idee est parcourir toute la matrice et de crééer la liste des valeurs possible pour chaque emplacement
 * @param matrice la matrice que l'on va remplir
 * @param exerc le sudoku exercice contenant les informations de base
 * @return un entier qui ne sera pas utilisé
 */
int compute_matrice(struct matrice_possibility *matrice, struct sudoku_exercice *exerc)
{
    for (int iter_line = 0; iter_line < 9; iter_line++)
    {
        for (int iter_column = 0; iter_column < 9; iter_column++)
        {
            matrice->possibilities[iter_line][iter_column] = malloc(sizeof(struct mpl));
            memset(matrice->possibilities[iter_line][iter_column], 0, sizeof(struct mpl));
            char current_char = exerc->matrice[iter_line][iter_column];
            switch (current_char)
            {
            case '.':
            {
                int is_present = 0;
                for (char iter_char = '1'; iter_char < ':'; iter_char++)
                {
                    for (int iter_col_line = 0; iter_col_line < 9; iter_col_line++)
                    {
                        if ((exerc->matrice[iter_line][iter_col_line] == iter_char) || (exerc->matrice[iter_col_line][iter_column] == iter_char))
                        {
                            is_present = 1;
                            break;
                        }
                    }
                    if (is_present != 1)
                    {
                        struct mpe *new_elem = malloc(sizeof(struct mpe));
                        new_elem->element = iter_char;
                        new_elem->next = matrice->possibilities[iter_line][iter_column]->load;
                        matrice->possibilities[iter_line][iter_column]->load = new_elem;
                        matrice->possibilities[iter_line][iter_column]->nb_elem += 1;
                    }
                    is_present = 0;
                }
                break;
            }
            default:
            {
                struct mpe *new_elem = malloc(sizeof(struct mpe));
                new_elem->element = current_char;
                new_elem->next = matrice->possibilities[iter_line][iter_column]->load;
                matrice->possibilities[iter_line][iter_column]->load = new_elem;
                matrice->possibilities[iter_line][iter_column]->nb_elem += 1;
            }
            }
            if (matrice->possibilities[iter_line][iter_column]->nb_elem == 1)
            {
                matrice->possibilities[iter_line][iter_column]->current_element = matrice->possibilities[iter_line][iter_column]->load;
            }
        }
    }

    /**
     * @todo 003
     */
    print_possibility_matrice(matrice);
    return 1;
}

int compute_possibility(struct matrice_possibility *matrice, int *nb_iter)
{
    // first we init all the list
    int current_line = 0;
    int current_colonne = 0;
    for (int iter_line = 0; iter_line < 9; iter_line++)
    {
        for (int iter_col = 0; iter_col < 9; iter_col++)
        {
            if (matrice->possibilities[iter_line][iter_col]->nb_elem > 1 && matrice->possibilities[iter_line][iter_col]->current_element == NULL)
            {
                /**
                 * @todo 004
                 */
                current_line = iter_line;
                current_colonne = iter_col;
                iter_line = 9;
                iter_col = 9;
            }
        }
    }

    matrice->possibilities[current_line][current_colonne]->current_element = matrice->possibilities[current_line][current_colonne]->load;
    while (1)
    {
        /**
         * @todo 005
         */
        *nb_iter += 1;
        if (matrice->possibilities[current_line][current_colonne]->current_element == NULL)
        {
            return -1;
        }
        /**
         * @note tout se fait avec des if dasn cette partie
         */

        /**
         * @todo 006
         */
        int entier_uplet = compute_uplet(matrice, current_line, current_colonne);
        if (entier_uplet == -1)
        {
            matrice->possibilities[current_line][current_colonne]->current_element = matrice->possibilities[current_line][current_colonne]->current_element->next;
            continue;
        }

        /**
         * @todo 019
         */
        int entier_carre = compute_carre(matrice, current_line, current_colonne);
        if (entier_carre == -1)
        {
            matrice->possibilities[current_line][current_colonne]->current_element = matrice->possibilities[current_line][current_colonne]->current_element->next;
            continue;
        }

        /**
         * @todo 007
         */
        int entier_complexity = compute_complexity(matrice);
        if (entier_complexity == -1)
        {
            return -1;
        }
        else if (entier_complexity == 81)
        {
            return 81;
        }

        /**
         * @todo 008
         */
        int entier_possibility = compute_possibility(matrice, nb_iter);
        if (entier_possibility == 81)
        {
            return 81;
        }

        matrice->possibilities[current_line][current_colonne]->current_element = matrice->possibilities[current_line][current_colonne]->current_element->next;
    }
}

int compute_uplet(struct matrice_possibility *matrice, int line, int colonne)
{
    char current_char = matrice->possibilities[line][colonne]->current_element->element;
    for (int iter_line_col = 0; iter_line_col < 9; iter_line_col++)
    {
        if (matrice->possibilities[iter_line_col][colonne]->current_element != NULL && iter_line_col != line)
        {
            if (current_char == matrice->possibilities[iter_line_col][colonne]->current_element->element)
            {
                /**
                 * @todo 009
                 */
                return -1;
            }
        }
        if (matrice->possibilities[iter_line_col][colonne]->nb_elem == 1 && iter_line_col != line && current_char == matrice->possibilities[iter_line_col][colonne]->load->element)
        {
            return -1;
        }
        if (matrice->possibilities[line][iter_line_col]->current_element != NULL && iter_line_col != colonne)
        {
            if (current_char == matrice->possibilities[line][iter_line_col]->current_element->element)
            {
                return -1;
            }
        }
        if (matrice->possibilities[line][iter_line_col]->nb_elem == 1 && iter_line_col != colonne && current_char == matrice->possibilities[line][iter_line_col]->load->element)
        {
            return -1;
        }
    }
    return 1;
}

int contain_char_mpe(char c, struct mpl *list)
{
    struct mpe *iter = list->load;
    while (iter != NULL)
    {

        /**
         * @todo 010
         */
        if (iter->element == c)
        {
            return -1;
        }
        iter = iter->next;
    }
    return list->nb_elem;
}

void delete_mpl(struct mpl *mpl_to_delete)
{
    struct mpe *mpe_elem = mpl_to_delete->load;
    while (mpe_elem != NULL)
    {
        struct mpe *del = mpe_elem;
        mpe_elem = mpe_elem->next;
        /**
         * @todo 011
         */
        mpl_to_delete->nb_elem = mpl_to_delete->nb_elem - 1;
        free(del);
    }
}

void delete_matrice(struct matrice_possibility *matrice)
{
    if (matrice != NULL)
    {
        for (int iter_column = 0; iter_column < 9; iter_column++)
        {
            for (int iter_line = 0; iter_line < 9; iter_line++)
            {
                if (matrice->possibilities[iter_line][iter_column] != NULL && matrice->possibilities[iter_line][iter_column]->nb_elem != 0)
                {
                    /**
                     * @todo 012
                     */
                    delete_mpl(matrice->possibilities[iter_line][iter_column]);
                    matrice->possibilities[iter_line][iter_column] = NULL;
                }
            }
        }
        free(matrice);
    }
    matrice = NULL;
}

struct sudoku_exercice_solve *solve_exercice(struct sudoku_exercice *exerc)
{

    struct sudoku_exercice_solve *solve = malloc(sizeof(struct sudoku_exercice_solve));
    solve->exerc = exerc;
    /**
     * @todo 013
     */
    solve->matrice = init_matrice();
    /**
     * @todo 014
     */
    compute_matrice(solve->matrice, solve->exerc);
    /**
     * @todo : utiliser la fonction compute_possibility
     */
    int compute_pos = compute_possibility(solve->matrice, &(solve->nb_iteration));
    if (compute_pos == 81)
    {
        /**
         * @todo : utiliser la fonction fill_sudoku_exerc_with_matrice
         */
        fill_sudoku_exerc_with_matrice(solve->exerc, solve->matrice);
        struct sudoku_element elem;
        elem.sudoku = solve->exerc;
        printf("\nsudoku resolu avec %d iterations\n", solve->nb_iteration);
        print_sudoku_element(&elem);
        printf("\n");
    }
    /**
     * @Todo 015
     */
    return solve;
}

int compute_complexity(struct matrice_possibility *possibility)
{
    int complexity = 0;
    for (int iter_line = 0; iter_line < 9; iter_line++)
    {
        for (int iter_col = 0; iter_col < 9; iter_col++)
        {
            if (possibility->possibilities[iter_line][iter_col]->current_element != NULL)
            {
                /**
                 * @todo 016
                 */
                complexity += 1;
            }
            else
            {
                int nb_elem = possibility->possibilities[iter_line][iter_col]->nb_elem;
                if (nb_elem == 0)
                {
                    return -1;
                }
                /**
                 * @todo 017
                 */
                complexity += nb_elem;
            }
        }
    }
    return complexity;
}

void fill_sudoku_exerc_with_matrice(struct sudoku_exercice *exerc, struct matrice_possibility *possi)
{
    for (int iter_line = 0; iter_line < 9; iter_line++)
    {
        for (int iter_col = 0; iter_col < 9; iter_col++)
        {
            if (possi->possibilities[iter_line][iter_col]->current_element != NULL)
            {
                /**
                 * @todo 018
                 */
                exerc->matrice[iter_line][iter_col] = possi->possibilities[iter_line][iter_col]->current_element->element;
            }
        }
    }
}

void delete_tab_exercice(struct sudoku_list *list)
{
    list->load = list->first;
    while (list->load != NULL)
    {
        struct sudoku_element *del_elem = list->load;
        list->load = list->load->next;
        free(del_elem->sudoku);
    }
}

/**
 * @brief initialize une liste de sudoku
 * @param list_to_init un pointer sur la liste à initialiser
 * @note lors de l'initialisation, la liste doit être vide et ses variables complètement intialisées.
 * Si la liste n'est pas vide, faire un message d'erreur et ne rien faire de plus
 */
void init_list_sudoku(struct sudoku_list *list_to_init)
{
    if (list_to_init->first != NULL)
    {
        if (list_to_init->load == NULL)
        {
            list_to_init->load = list_to_init->first;
        }
        printf("un init de liste a été demandé mais la liste n'est pas vide. Abandon\n");
        return;
    }
    list_to_init->first = NULL;
    list_to_init->load = NULL;
    list_to_init->nb_element = 0;
}

void init_tab_exercice(struct sudoku_list *list)
{
    char tab_exercice[] = ".7.39.5...91.2...6.25....3....5...61..31..2.5.1..7........4.......6....76349.....\n4..2.8.1.............1.7...........8.89.62.3.236.....4.9..73.62..7........491...7\n78.93...4.19..2....4..7.1....2..8...4.....7.589.35...2...1.7....5..83.9..........\n..15..3.97..12...............5.........79.......34.19.5.943..7147...2.3..3....4.6\n8.7..2...2...4.78.......6.3...32..4......9.6...285....6..2.4...7.45.8..99........\n25.8.41....41....7.19.56..8...6.2..........4..8.....62.....9....42.71...5..46....\n5.63.8.94...1.58.6.846......2.....4..59...........31...9...1...64...9312..5.....9\n..8......1.28.....46......3.1.3..4.6.....2.7........51.7..5.......18....5.4.23.8.\n..73..2.14...2........1.64..........62...453.5.4...16...2....5.3.....4.6...73...9\n..5...72..3....6.42..4713.....72........16......5932......6549..........19..4..76\n..65.....3...98....92..........3.5.6...1.....8..95...3.......2.9..38....5436..9..\n36...7.....16.....7...........8.13.51..4..6.....56......9..4..84.....5....6.83.41\n..9.....22.1.8.....34...5.14..9......23.........74...83.5.......9....2757.....9..\n.4.9....2.6.....8.2....86359.......7....5.2....4..3.5.59.....637.8..............9\n8.1...95.59.........7..3..8.....74.1.........1..52.8.7..5...3...13.9..422..1....9\n.5.......2..6..37.78...5..2.3.....8...5.6....41...7...12.4.....5..2186.4...7.....\n.....8.398..9...2....47..6.9..1...86.1....3..32..84..7....9.17..4.....9.2........\n.794....23.8.7.9...5.1.....4..91..7.2.7...58........94...8.7..............52.47..\n2...........56..93.....8.5.....173.9..........6.2....87.2..9..6.136...7...6.829..\n7..5...6.....4...5...9..8..5.........81......92.....7..57..6..11..7.524.2...8...9\n..89....2.1.5....7.....4...85...19....6...78...9..6.25...7....1.6.192.......68..9\n....4..2..28.56.473.1...6.....72.3.4.8...15.........12.6...3..8..458.2.........3.\n.9..1..54.2...4.....4.6..1...6..........7.....5.8..64..42..7..8.1..9..62....8..7.\n13...7..5.......23.....64..........84..5...7...8....42..3...5.4...6.1....27.351..\n27...5.....124....8.............29..5.3......1.73.8....8...319...948..3..5.1...74\n9.........4...8..5....45.934....987....7..4.2.72.3....864.....93......2..5.......\n......9612............5924..9..7...65.2.417.......2.3.....976..6......1..5.1.3...\n....8.2.....25...3..97.....7...3..582...1..39.865..7..9...2...7.1.3......6...1...\n25.6...8...34..6...16...9........5......3...1.8.9.6.4.....94.6......82....92.5...\n31.......9.5.....17..1.......9..435.46..3....1....6..8....5.187....4.......7...95\n........33.58........1...8.1...........2.4715...7..34...362..78.78..5.24.1...8...\n5...31.......9.8.5.......1......7.56.8.....9..65...7.14.......719.7...4....3..6..\n52.....6.1.9..8..5...9..........2..139.....7.78........5.163..9...79...2..6..5...\n34.1..2.5.....569.9...3.1..15..8.........9....8.....6.82..619..63.........14.8...\n.7....5.14.3.....2.....2....57...1..6.2.....781..6......41..7.6...67.8.....3.4...\n...217..............8.6.1..93..8...78..4.6....17...8....6..8..3...7426......3.91.\n4....16......9........8...5..9...2.6.1...9.8..4..26.5.682.47...9...62......5.....\n..953...7.....425....9.....5.26..........1....1..9.6........3.21..76.....7.2.5..1\n.9..7.....6.3...1.1..9..587...1....9.3...5...7....3.26..6..8.3....26.........9...\n...9......62.....57.3.5..122.........1.....7.6...3..2...16.....825..73.....1..9.7\n8...42.............1.3....237....8...2.....5....5...31.8.15.4...536....8...4..72.\n....3.97..4.1...6.5..8....47.....3....6..3.89....9...71...7.49...8..1.5....4.....\n297...8.4..6..8.5..58.....6...6.........82...6...5...7....76.1...2..567....9....5\n4...............4.9.82.1....1.6....8..491.32.2..5.3..6..2...9.....1.5.3.1......65\n....8...12....436.....6..24.8.329......6..........8....32.4.1..4....5..2..693..5.\n2.5.7.....4........375.2..9.....74....8.3....42.1......9.75.......2.3.75.53.14...\n....1.....3..89..61....3.2.........58.2..6..4....3.9...9.3....2.1.96..8.......4..\n........3..3..1.97.283..1....7.4.8.9.9......44...3.....81.2.....46......3....7...\n....18...3.....7..7......6.26.....1.....8...6.4.2..37..3.5.6..8..1..3....254.....\n.718..2.9...19...7.3..7.1.5..352....2.4.3.7....7..8......2.......8..5.....5.193..\n.5......626.....3....83..7.63..5971.......6..91......5...6.2..7....9.......7...51\n...4...8.8..16..23..4.5.1...6.54...1..........9...2.6.64..3........1.3....12....8\n.8....54......4...32..7.....7..8...9..1.92...8....1.5....92......7.5....43...72..\n.9...7..3..6.5..7.78.1.9..5...2......2.......53....6.7....9....1..56........8.3.9\n..1.2..363..5......2......9..2..9..8.4..5...2..5..3......39.....9.1.42.5.8...5...\n..5.34.9...8.5.6......9.5.....7....6.64..3.71......98.....7..1.3......5....8....4\n...1....3.....71....3..6.8....6..81...8.95.3.6.......22...785.93....92...6.2.....\n.......36.93..4.5..5...98.4.1.83.......5..6.....64..2.5.......1..4......8.9.....5\n.8......2..2....7...6...3....3.5..6.8...7.....2.4..9...9...7.8.5..83...4.3.29....\n86...74....2..4.1.7..3.5.86.2.......643......5..4....3.3......8..528..7.1.7.....2\n2....3...9.76.41...4.2..7....94....3.1..68..9.831...2.6..9...4....7..3...........\n8.74..5...1...6.........87..24........6..3..1....47.......684..6.871.2..5..2.....\n....53.4.3..6.752......1.6775..........82........6..3....4......65.1.2..13..7...6\n..7.3..5...85.9.4.9...7..3...2..489.8...........3...17........14....5.....1.8...9\n28...796..15.....7..914..85...5.......6.78.52.9..6...86.2..........8.....7......9\n...3..72.28...54.....48...3.....6.....672.....57......13....8...62....457......3.\n....359.13.............2......6.......5.9..63.9..2....6.8.....727.....3..1..5.68.\n7...65.3..........351.........1.3.6.6...4.7....8..9..4.......4......857.58....3.6\n6341...8..7.......1.......691.4.7..2.8....9.3....917.......4678.9.6....1.........\n.5........4..9..5...7.2...1.1...5.6..9..1..75.....6..9.7..6.82.8....4...1..7..9..\n.8...7...2....6..4......3...498..6.18.2..1....6.3.4.7..9.62.8.......9..7..4.....2\n.7.4...2...3......6......8..1..48..776..2..4....5....384.7.1...1...6.....2....8..\n761...9...9.5.....5........4....5..9....6.72.61.8..5...541.8....7...6...2......8.\n34..6...2.17.....68....7.436.1..8.3..........23..7......692...8......9.14........\n.........1..7...4.4.95..6.3..1.8...5......73..856.....5....4.......6..9763....41.\n..3...1...8.14..9......9.38.....7..4.17...8..54....6.....7......9....36..3.6...85\n.94...13.........42..7...5....9.8.1..4..2.9.7.7.41.5..5.9........3.9........3...2\n..9.....77..91.4...3..2......27..9.4..5....3.....81...........9.18.3.......54.2..\n56...8..3..8...9.1.7.94.....93...4..8..3....665.49.1...1.85................6..5..\n.34..82..7.9......8..9...6.62......93.8...7...97..........3...1....1.82...627.9.3\n.2.9..6..54..3......6.....981.36..2..53.1.......8.........2.8...8..5.2.33.......1\n6...........79...6.9.2..81..2..5.7...879....191.......1......6.......15...56...3.\n...8..3....59748......6......4.18..7...69...8...4..5...71.3....85......3.395..1..\n9....7...186.9.....2......8.1..3.2.......5...3.5.86.17.3....8......7..9.562.1....\n........9.8.5..3....5..642.........34.7.8.5..82...96.....8...3.......9.2...245...\n48.7....5..7......53...1.8.....4...8..1....36......5..3.5.8..2....6.27...26..3..1\n7.8..32...5..........85.4...2..4...66.1.7.5.4..5.318.7.....6....6.2....1...71....\n4..2.58....81.75........6..6....1.9.79.42......5...2.6....4297....9......6...8...\n3....17....4.2.9.1.17.6...2..........9.2..4.7...9163...63..........98....4.7.2...\n....7.81.7.......258.....6.26.......8.7.3.......26......1..5.2.45...67.3..2..7...\n.7.5..........62..6.4.72.1.5....4..9...3....5...1.....4......9......362...1..5..3\n..7...1...8..5..6.........7...13..2..1..7.8.9..6895.....8.6.7.3......2...9.31..86\n1.9.....66.2....9......2..5...36.1.7....5......612.38.78.9.....9..5.38...........\n54.71..2..91..2..5..6..597....9....2...5....7.......1.6..1.....9.8.6...........4.\n...5.1...2....37.11...764..71....9.663.2....4..5....2.9.4...2.............7..4..3\n.6.7.5.........9.5.......3..7..3...63...4.5.8.......7.69.5..8..8....4.191...8.4..\n..12..6...63.9..4.8.43.6.97....2.7.3.1........3....4187..4.2......8.............6\n..71...243..4..971....5...3.2..3.....4...8.9.1..2.5.4.......8..91.........8....1.\n1...2.3.8236..4.1..........61.......5.84..9...9.152...8.....5..364.........9...4.\n...71.4....394..85..8...9....5.2.1.4..21...93.8.3.....3.1...5..8.....3.......5...\n.3...2...9........4.73..89..68...5....458.7..5...2.9.....9...6......34.....74....\n.......1.1...8...9......6..4....3..8..76.....3.6.489...9..6.7..8.5..43....3.7.19.\n....1.6..69.....5..51........5..691....8....328.9..5.41..2..89..32....4.8....4.3.\n...81..6......6..8......4..7....48...46.2.13...39.......8.9.6....17..284....4.9.1\n.8...7.2.1...9.8.59..1....78............4....3.9.........9..7.....45.1.852..713..\n..8.6.5....3.....7.4.9........89.......6..93...7..5.8.73...9..4.......1.1.5..486.\n....684..3.1.5..2..2......7....7..5....8.51..7..41.....7..3......6.......5....81.\n..46........29....12....97.....3.6.....7.14..3...6..52..5...13.6......27.....4...\n2.3..58...9...7....5....2.4....5..4.67...93...3.....989.....58.48..6...7.........\n8....2..9.19......26.......6..2.5........8...1.8..7....4.....15...7...267.6.1..98\n..1......5.8.....2..4.85.6.4.7.........1...3..5..26..46.2..431...........3...1..7\n.67...1.2..1.3....9.3...7...4.6..2........4..3..8....6.1.46.87......2......19.6.4\n.......6..8.7......41.6..3.1..3.....5..9...14..8..639.865.4....9....58....4.....1\n89..7.61........35......8..7.....12..1..2.3..........967.91..8.13...5..6..9.32...\n...2...5.29...1.3....8..4.7.5..3...1....1...4.1...5.8.13..7.8.5......7....4......\n..5.8..6.76...2..1.2....7..2.8..6.17.........5149...8.....5....4...61...6.......9\n.6.......1....264.....9..7.62.4.1..7.1..58....57............1........72..82..6.59\n8.2.....5..6......71...9....4..925.6......1..5..7..98.9..1.42..4....5....8......9\n5.8.....6.......7....8....49..7....5....3.49..539......36...5478.9.6.3.....1.....\n9.........31.6..74..2.53.......1........752.6..69.4713....3.4...7..46.....4.....2\n.2671..845.....72..845....3.....8..6....2.4...3..5.....5...136....6....5....3...8\n...43...5....15..2.....98...26....185......9.391....6....6..3......83.......21..9\n.........632.......9...2..485.....6..2.6...79.....3....6.5943.89...2...7..53.8..2\n8..5.6.7...6...4..72.4..1.99.165.....8...........712..56..2...819...8............\n.......12..........7..2.3.651...6.....7.43..12....5.9.13..7....9.6...54........2.\n.2...3.64....8......8.....741....7..9...124..7......9...6...2...7....9..3.9.6....\n.3..7....7.184......9..6..2.....43..813....276..........7.1.96...46..8.........3.\n..1.3..953....578.5..7.9..3....6......79.....8...2.61..9...2....13....7.7.6.....2\n.2....1.9.41......89..175...84..9.7....4....227.......51.29......75.8....6..4....\n7.2......4.9...765.......9.85....64.......9.......78.3.8.5.......4..6.27....43...\n...7..9..7..6....34.3...21..8...........63..21..9.2........7.8....34....2...89...\n5..6....78....3.1..96.18.........68.9.17.6...3...94....3.......65.3.7.....74.....\n....42..8.28...4.7...7.....9....536.5..4.6.8..67.9........2.......3.85...9.6.7...\n....32.81....87.........4....4..6.3..38..5....6..7....38....6..6..72..4..5.6....7\n2....8....19........74...58.52.1.8.......4.7.....6..4.6...8........2.71.7....69..\n3...2.9..89.4.....2.1..7..64...32....3.....84...84.1.....2.6..9....9..71...7.8...\n....3.5....5.9.3.886..4..17.19.....5..8..6.91.4........8.......3.7......95..87.43\n..17.....4..6...3.89....4.7..3....86......3...4....1.93..5...6..84.........84..95\n..........5.9..4...4....78259...4.1.8.2..53....31.8.25.......799......5..3.5..1..\n...63.2.925...........7..6........211.52.....9....3....6.1...38..8..7.....2.69..7\n...2......4.961.7.....741............7.18.....2.7....9..7.9.62.21....3.846.....9.\n38..27....971....31...3...682....95.95...81.........2.....7......8....915.....76.\n....5...23.46.2....6.7.1.8..4............6.1.1.5........71.5.268....7.....63.4..1\n..9....4..7...15....47.6..1..1......6.......9.28..7415.4.8.......2...9..8.6..5.2.\n827...4...13.....7..........5237....6.1..85.......4.......6.3.52...1...8.7.2...6.\n.8.....64....1....9.6.....8..7.9.8...4.67.3.1....8.6...62...7....1......4...31.9.\n...7.....3..2.8...57..41....3.....857.4..3.2..2..1....1...6....2.....61.....854..\n7....5.2..5...8..34..71......3.8...4814....3..........2..5....7..9.....218...3.9.\n........7...48.9...9.7.1....7......884913.7..12..4..........68.......492...9.2..1\n....274.341...5.6.......2.......6158...8.26......1....7...6....2.65713....3..4...\n2.31.........5.7.......8..2.4.5.9.1...92....31...4.5..3..9....847........2......7\n.9...71..2....1.9..........9...2..6.54......18...16.47....5.27..7.........59..8..\n7.5....3..81.3...99.......445.9.........52.1....3...9852.84........19...8..5.7...\n..4.2...8.....9.4.....5.16.1.7...452............2..9.74.....69.25..468..6.8.1....\n....87...8..1.64...6.43.5...169...7...974..1.3......94......3...58..4....7.......\n...2.5....48..1...9.18..........8.4.2.5....96...967......6..45..9....8.21........\n.....8..3....5.9..95.4...265...396...7..6...2..3....572....4.6.4....7.39.........\n..7...4.1.......8..9.1..6.5.7.3149.6..56.8.........3..8..95..3..4......9.......1.\n4.......3..24..9...9.73.1...8....3..3.16..42..........8...9......7.4..6.2...78...\n...2.......65..3...8.....2...79.6..8.1.3.57........6..5..7.....13.....8..29.3.1..\n.89.5....7518..9....4.....75.8..146..6.4.83....7.6...1.......3....7..........9.4.\n...93.......1.27...2....43....59.......4.1...93..6.....4.6..1...537....2.19....57\n..8.6...2.2...98...7.1..6..4..2.7.....1..849.............5.4...98..2......4.....1\n91.6...2...8..........2...9......5......35.8.76...........9.83..8.3....65.7...1..\n9.....5......1..63.58..........2.13.39..5...2.8.9...5.......8.15......9..1739..2.\n......7....1.8...........612.........3.26.17..6..7.8....98..6....31.5.8..2.3...9.\n.........2.....1.7...1..4.3....9...13..7......5....29.1.38.....72.5.3....95.7.8..\n94.......1.........8..1.3...1..69.4.2.7....8.....8.9.36..39...17..6.........4..7.\n...8............9..6.4....8.2..6..51.8...9.3.4..1..8..31....26585.........9.3....\n...5.....1..7...6.....36....3.2..47.5...6...1..7153.8.48...5.32..6..2.......4...8\n....34..73...7.6.5.5.9..........5.1.6....14.....7......8....9.493.6...78..1....6.\n...471.6..........54..93.........62.4..3..7..72...934...6.2491...........9..1.2.7\n.....8.....39....829.37.........9.......1.6..5......928.12.7..6...5..1..3.518.7..\n.9....4.7...1....3...2.9.......4.9....6........8...7.11.7.8....38....6.....36...4\n.7.......6.4...1.3.....3..6.8........19..5....4...1375...9.........16..49...3765.\n..7.5...81.8....9..4.1..3.68134..........5...5..6.9.81..1.....36.....54.........7\n.51..9.......64.........1...6.5.14.2.7...6..8..59....6......2.74.7....6..1.2..9.5\n.2.1..9..96.2...3.1.7.6.82..3....76.....74.....9...1.....7...1......9...3716.2...\n..2...7.9..3.19..2...76..5............5...2..9...5..31.5.4......6.2.5.932...96...\n.571.9...96273.........5.....5...2......1..3..2..9.56....2.1.......67..18.......3\n......7.1.8...34.66.478..5...7....12..6......5..1..83..2...5......3.4.......2.3..\n57.8..21.3......8.8.2.9.......28..9..5..39.7..2..751....7....45........9..4...3..\n.87..25..2...4.37.........4.....5......293.......8.4...52.....384...7...3..9.8..1\n3..........5..8.1..7......21......5..54......7.....13..61.87......4.32..8..2.157.\n.53...2.6....5............7.6.7........5...8.53...8.4...9....7.68..4.3..24..9.5.8\n....6.28..6..1....7......4........2.4...2.69.32.6..1.41.9..3....73..18...8.......\n5.6.1289.2.8.....1....9.....3.9..2....2.....8...6.3.5.3..2.8.1.1.7.3......5...6..\n..4..6...29.38...5.6......9.4......68.....91.5...2..48..1....2....8...3....4.1...\n.......43.....1.9..21.7.5....87.....5...8.9..39...57.11...5...2..7........3.....4\n....3....17.9...6...9.5....3.5..8.....7....18...2......2.38..96.3.6.2.577..1.....\n5...6..21..7..9..3..925.8...23.1.7..9....6......9..2..6......52...........8.7....\n2.4.51.8..854.........8.6.........23.1......6.....6....7.2.4...3..67.5..4......7.\n...14.9....3......54..6....95.....3...2......4.6...51.86..942..2.1.56.4..3.....9.\n41..9...........2...3184.....6...34.......2....7.3869..7.6439.......1....3....467\n..5...6.2...2.95.8...........9.478...6.5....74....69...52...3..89.......6.7...45.\n....2....183....6...9..3.....75..1..6...7.......8.2........12.......879..3529..1.\n8.1.3..26.4...9...5.3.....9.2..........1....5.68.9.............6.9.845.....61..8.\n.4.12...55..4....2..15...76..6......72.......958...6...7..1.2...6...89.71..9.....\n......27.9...35..1..1.6.....7.6......6528....2..3.....5.2...9..39......5....7....\n.....2....48...257.7...4..6....7...1.....5842...6..5.3.83......5......1..62.....4\n\0";
    int iter = 0;
    int number = 0;
    char current_tab[82];
    memset(current_tab, 0, 82);
    while (1)
    {
        char current_char = tab_exercice[iter];
        switch (current_char)
        {
        case '\0':
        {
            return;
        }
        case '\n':
        {
            struct sudoku_element *elem = create_end(list);
            elem->sudoku = malloc(sizeof(struct sudoku_exercice));
            init_sudoku_exercice(elem->sudoku);
            for (int i = 0; i < 9; i++)
            {
                memmove(&(elem->sudoku->matrice[i]), &(current_tab[i * 9]), 9);
            }
            generate_id(number, elem->sudoku->identifiant);
            number++;
            iter++;
            memset(current_tab, 0, 82);
            break;
        }
        default:
        {
            current_tab[iter - number * 82] = current_char;
            iter++;
        }
        }
    }
}

void print_sudoku_element(struct sudoku_element *elem)
{
    printf("sudoku element reference : %s\n", elem->sudoku->identifiant);
    printf("%s\n", elem->sudoku->identifiant);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            printf("|%c", elem->sudoku->matrice[i][j]);
        }
        printf("|");
        printf("\n");
    }
    printf("\n");
}

struct sudoku_element *create_end(struct sudoku_list *base_list)
{
    struct sudoku_element *iter_elem = base_list->load;
    struct sudoku_element *elem_ptr = malloc(sizeof(struct sudoku_element));
    if (iter_elem == NULL)
    {
        iter_elem = malloc(sizeof(struct sudoku_element));
        init_sudoku_element(iter_elem);
        base_list->load = iter_elem;
        base_list->first = iter_elem;
        base_list->nb_element += 1;
        return iter_elem;
    }
    else
    {
        while (iter_elem->next != NULL)
        {
            iter_elem = iter_elem->next;
        }
    }
    init_sudoku_element(elem_ptr);
    iter_elem->next = elem_ptr;
    elem_ptr->prev = iter_elem;
    base_list->nb_element += 1;
    return elem_ptr;
}

void generate_id(int num_sudoku, char *dest)
{
    sprintf(dest, "sudoku%04d", num_sudoku);
}

void init_sudoku_element(struct sudoku_element *elem)
{
    memset(elem, 0, sizeof(struct sudoku_element));
}

int print_possibility_matrice(struct matrice_possibility *possibility)
{
    printf("\tmatrice des possibilites :\n");
    printf("######################################################################################################################################################################################\n");
    for (int iter_line = 0; iter_line < 9; iter_line++)
    {
        char tab[9][21];
        // constantes
        printf("#");
        for (int iter_col = 0; iter_col < 9; iter_col++)
        {
            memset(tab[iter_col], 0, 21);
            memset(tab[iter_col], 32, 20);
            tab[iter_col][0] = '|';
            tab[iter_col][19] = '|';
            struct mpl *elem = possibility->possibilities[iter_line][iter_col];
            if (elem->nb_elem == 1)
            {
                tab[iter_col][1] = elem->load->element;
            }
            else
            {
                struct mpe *elem_iter = elem->load;
                int iter_tab = 1;
                if (elem_iter == NULL)
                {
                    return -1;
                }
                tab[iter_col][iter_tab] = elem_iter->element;
                elem_iter = elem_iter->next;
                while (elem_iter != NULL)
                {
                    tab[iter_col][iter_tab + 1] = ' ';
                    tab[iter_col][iter_tab + 2] = elem_iter->element;
                    iter_tab += 2;
                    elem_iter = elem_iter->next;
                }
            }
            if (elem->current_element != NULL)
            {
                tab[iter_col][17] = '|';
                tab[iter_col][18] = elem->current_element->element;
            }
            printf("%s", tab[iter_col]);
        }
        printf("#\n");
        if (iter_line != 8)
        {
            printf("#************************************************************************************************************************************************************************************#\n");
        }
    }
    printf("######################################################################################################################################################################################\n");
    return 1;
}

int compute_carre(struct matrice_possibility *possibility, int line, int col)
{
    if (possibility->possibilities[line][col]->current_element == NULL)
    {
        /**
         * @todo 0020
         */
        return 1;
    }

    /**
     * @todo 0021
     */
    char current_char = possibility->possibilities[line][col]->current_element->element;
    int start_line = line - (line % 3);
    int start_col = col - (col % 3);
    for (int iter_line = start_line; iter_line < (start_line + 3); iter_line++)
    {
        for (int iter_col = start_col; iter_col < (start_col + 3); iter_col++)
        {
            if (iter_line != line && iter_col != col && possibility->possibilities[iter_line][iter_col]->current_element != NULL && possibility->possibilities[iter_line][iter_col]->current_element->element == current_char)
            {
                return -1;
            }
        }
    }
    /**
     * @todo 0022
     */
    return 1;
}
