#include "sudoku_functions.h"

int main(int argc, char* argv[])
{

    struct sudoku_exercice_solve* tab_solve[200];

    struct sudoku_list* main_list = malloc(sizeof(struct sudoku_list));
    main_list->first = NULL;
    main_list->load = NULL;
    init_list_sudoku(main_list);
    init_tab_exercice(main_list);

    int iter = 0;
    struct sudoku_element* elem = main_list->load;


    while(elem != NULL)
    {
        /**
         * @todo remplacer par la bonne fonction solve_exercice
         */
        tab_solve[iter] = solve_exercice(elem->sudoku);
        printf("\n\n");
        elem = elem->next;
        iter++;
    }
    for(int iter = 0 ; iter < 200 ; iter++)
    {
        tab_solve[iter] = NULL;
    }
    delete_tab_exercice(main_list);
    return 0;
}
