#include "sudoku_functions.h"

void evaluation_programm();
void affichage_exercice();

int main(int argc, char* argv[])
{

    //affichage_exercice();
    evaluation_programm();
    return 0;
}

void affichage_exercice(){
    struct sudoku_list* main_list = malloc(sizeof(struct sudoku_list));
    main_list->first = NULL;
    main_list->load = NULL;
    init_list_sudoku(main_list);
    init_tab_exercice(main_list);
    while(main_list->load){
        print_sudoku_element(main_list->load);
        main_list->load = main_list->load->next;
    }
}

void evaluation_programm(){
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
}
