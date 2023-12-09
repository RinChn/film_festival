#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAXLEN 256
#if (defined(linux) || defined(MACH))
#define CLS system("clear")
#else
#define CLS system("cls")
#endif

struct film_festival {
        int numb;
        char *name;
        struct filmmakers *director;
        int length;
        float rating;
        int reviews;
        float fees;
        int premiere[3];
        struct film_festival *next;
        struct film_festival *prev;
};

struct filmmakers {
    int id;
    char *fm;
    int films;
    struct filmmakers *next;
    struct filmmakers *prev;
};

struct h_films {
    int count;
    struct film_festival *first;
    struct film_festival *last;
};

struct h_prods {
    int count;
    struct filmmakers *first;
    struct filmmakers *last;
};

typedef struct h_films head_films;
typedef struct h_prods head_producers;
typedef struct film_festival list;
typedef struct filmmakers producer;

/*Confirmation of clearing the console*/
void check_rerun();

/*memory cleanup*/
void free_string(char **str, int n);
void free_films(head_films *h);
void free_dirs(head_producers *h);

/*creating "heads" of lists*/
head_films *make_head_films();
head_producers *make_head_producers();

/*adding the first items to the lists*/
void add_first(head_films *my_head, list *new_node);
void add_first_dir(head_producers *head, producer *new_node);
void add_first_console(head_films *h_f, head_producers *h_p);

/*splitting a string into parts*/
int split_string(char **new_string, char *string, int slen, int mode, char sep);

/*reading a list from a file*/
void create_list(head_films *h_f, head_producers *h_p);

/*filling in a list item*/
list *struct_fill(char **str, int i, head_producers *h);
list *struct_fill_console(head_producers *h_p);
producer *prod_add(char *str, int i);

/*inserting items into a list*/
void insert_after(head_films *h, list *new_node, list *current_node);
producer *insert_dir(char *str, head_producers *h);

/*general menu output*/
void menu_print(head_films *h_f, head_producers *h_p);

/*help about the program*/
void information(head_films *h_f, head_producers *h_p);

/*adding a new item to the list*/
void add_new(head_films *h_f, head_producers *h_p);

/*editing list items*/
void edit(head_films *h_f, head_producers *h_p);
void edit_field_int(list *node, int mode);
void edit_field_float(list *node, int mode);
void edit_name(list *node);
void input_date(list *node);
void edit_dir(list *node, head_films *h_f, head_producers *h_p);
void edit_full(list *node, head_films *h_f, head_producers *h_p);

/*checking the date for realism*/
int check_date(char **new_str);

/*deleting list items*/
void menu_deleting(head_films *h_f, head_producers *h_p);
void del_node(head_films *h_f, head_producers *h_p);
void del_all_films(head_films *h, head_producers *h_p);
void deleting_producer(producer *del_dir, head_producers *h_p);

/*search for list items*/
void search(head_films *h_f, head_producers *h_p);
void search_name(head_films *h_f, int mode);
void search_int(head_films *h_f, int mode);
void search_float(head_films *h_f, int mode);
void search_date(head_films *h_f);

/*sorting the list*/
void sorting_menu(head_films *h_f, head_producers *h_p);
void sort(head_films *h, int mode, int order);
void sort_dirs(head_producers *h, int order);
int swap_name(list *first, list *second, int order);
int swap_dir(list *first, list *second, int order);
int swap_d1(list *first, list *second);
int swap_d2(list *first, list *second);
int swap_global(list *first, list *second, int mode, int order);
int comparison(int *node_date, char **date);

/*replacing the director in the list*/
void replace_dir(head_films *h_f, head_producers *h_p);

/*renumbering of list items*/
void renumbering_all(head_films *h);
void renumbering_position(list *node, int count);

/*checking the relevance of the director in the list*/
void check_dirs(list *del_node, head_producers *h_p);
int check_string(char *str);

/*output lists to the console*/
void struct_out(head_films *h);
void print_dirs(head_producers *dirs);

/*saving the list to a file*/
void save_list(head_films *h_f);

int main() {
    head_films *h_f;
    head_producers *h_p;
    char rerun;
    rerun = 'k';
    while (rerun == 'k') {
        CLS;
        h_f = make_head_films();
        h_p = make_head_producers();
        create_list(h_f, h_p);
        if (h_f->count != 0) {
            menu_print(h_f, h_p);
            if (h_f->count != 0) {
                free_films(h_f);
                free_dirs(h_p);
            }
            if (h_f->count != 0) getchar();
        } else getchar();
        printf("\nPlease write 'k' if you want to start over: ");                /*Ability to run the program again*/
        if ((rerun = getchar()) == 'k') getchar();
    }
    printf("\n\nThe work of the program is completed! Thanks!\n");
    return 0;
}

/*Function: check_rerun
Verification of the user's confirmation to continue the program and confirmation of clearing the console.*/

void check_rerun() {
    char rerun;
    rerun = 'o';
    while (rerun != 'k') {
        printf("\n\nEnter 'k' to continue: ");
        rerun = getchar();
        if (rerun != 'k') {
            printf("\nIncorrect value entered, try again.\n");
            fflush(stdin);
        }
    }
    CLS;
}

/*Function: free_string
Clearing a line.
str - line to clear;
n - number of line parts*/

void free_string(char **str, int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (str[i] != NULL) {
            free(str[i]);
            str[i] = NULL;
        }
    }
    free(str);
    str = NULL;
}

/*Function: free_films
Clearing the list of movies
h - "head" of the list*/

void free_films(head_films *h) {
    list *node, *dop;
    int i;
    node = h->first;
    for (i = 0; i < h->count; i++) {
        dop = node->next;
        free(node);
        node = NULL;
        node = dop;
    }
    free(h);
}

/*Function: free_dirs
Clearing the list of producers
h - "head" of the list*/

void free_dirs(head_producers *h) {
    producer *node, *dop;
    int i;
    node = h->first;
    for (i = 0; i < h->count; i++) {
        dop = node->next;
        free(node);
        node = NULL;
        node = dop;
    }
    free(h);
}

/*Function: make_head_films
Creating the "head" of the movie list.
returns: the "head" of the list*/

head_films *make_head_films() {
    head_films *cap = NULL;
    if((cap = (head_films*) malloc(sizeof(head_films))) != NULL) {
        cap->count = 0;
        cap->first=NULL;
        cap->last=NULL;
    } else printf("Error at memory allocation!");
    return cap;
}

/*Function: make_head_producers
Creating the "head" of the producer list.
returns: the "head" of the list*/

head_producers *make_head_producers() {
    head_producers *cap = NULL;
    if ((cap = (head_producers*) malloc(sizeof(head_producers))) != NULL) {
        cap->count = 0;
        cap->first=NULL;
        cap->last=NULL;
    } else printf("Error at memory allocation!");
    return cap;
}

/*Function: add_first
Adding the first item to the movie list.
my_head - The "head" of the movie list
new_node - is the first item*/

void add_first(head_films *my_head, list *new_node) {
    if (my_head != NULL && new_node != NULL) {
        my_head->first = new_node;
        my_head->last = new_node;
        new_node->next = new_node;
        new_node->prev = new_node;
    } else printf("Error at memory allocation!");
}

/*Function: add_first_dir
Adding the first item to the producer list.
head - The "head" of the producer list
new_node - is the first item*/

void add_first_dir(head_producers *head, producer *new_node) {
    if (head != NULL && new_node != NULL) {
        head->first = new_node;
        head->last = new_node;
        new_node->prev = new_node;
        new_node->next = new_node;
        head->count = 1;
    } else printf("Error at memory allocation!");
}

/*Function: add_first_console
Adding the first item from the console
h_f - the "head" of movie lists
h_p - the "head" of the list of directors*/

void add_first_console(head_films *h_f, head_producers *h_p) {
    list *node;
    printf("\nPlease enter the first item in the list:\n\n");
    node = struct_fill_console(h_p);
    add_first(h_f, node);
    node->numb = 1;
    h_f->count = 1;
}

/*Function: split_string
Splitting a string into elements.
new_string - line to fill in;
string - the original line;
slen - the length of the original string;
mode - the preset number of parts for new_string.
returns: the number of new_string parts.*/

int split_string(char **new_string, char *string, int slen, int mode, char sep) {
    int flag, i, count, parts, start;
    parts = 0;
    for (i = 0; i < slen; i++) {
        if (string[i] == sep) parts++;
    }
    for (i = 0, count = 0; i <= parts; i++, count++) {
        if((new_string[i] = (char*) malloc(slen * sizeof(char)))!= NULL) flag = 1;
        else {
            printf("Error at memory allocation!");
            flag = 0;
            i = parts;
        }
    }
    if (flag == 1 && mode == parts) {
        parts = 0;
        start = 0;
        for (i = 0; i < slen; i++) {                /*Element-by-element transfer from the old line to the new one*/
            if (string[i] != sep) new_string[parts][i - start] = string[i];
            else {
                new_string[parts][i - start] = '\0';
                start = i + 1;
                parts++;
            }
        }
    } else {                /*Checking for incorrect input*/
        free_string(new_string, count);
        printf("\nIncorrect data entered! Make sure you have filled in all the fields.\n");
    }
    return count;
}

/*Function: create_list
Creating a list of movies from a file.
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void create_list(head_films *h_f, head_producers *h_p) {
    FILE *archive;
    char **new_str;
    char rerun;
    char str_now[MAXLEN], file_name[MAXLEN];
    int n, parts, i, flag;
    list *last_node, *new_node;
    n = 0;
    parts = 0;
    flag = 1;
    printf("Enter the name of the file from where you want to read the data: ");
    fgets(file_name, MAXLEN, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    if ((archive = fopen(file_name, "r")) != NULL && h_f != NULL && h_p != NULL) {
        printf("\nThe file for reading has been opened successfully.\n");
        while ((fgets(str_now, MAXLEN, archive)) != NULL) n++;                /*Reading the number of lines in the file*/
        rewind(archive);
        if (n == 1 && strlen(str_now) == 1) n = 0;
        h_f->count = n;
        if (n != 0) {
            fgets(str_now, MAXLEN, archive);
            str_now[strlen(str_now) - 1] = '\0';
            if ((new_str = (char**) malloc(9 * sizeof(char*))) != NULL && (parts = split_string(new_str, str_now, strlen(str_now), 8, ';')) == 9) {
                add_first_dir(h_p, prod_add(new_str[1], 0));
                last_node = struct_fill(new_str, 0, h_p);
                h_p->first->films--;
                add_first(h_f, last_node);
            } else {
                printf("Error filling in the list!\n");
                flag = 0;
            }
            for (i = 1; i < n && flag == 1; i++) {                /*If the first element was entered successfully, we read the remaining lines*/
                fgets(str_now, MAXLEN, archive);
                str_now[strlen(str_now) - 1] = '\0';
                if ((new_str = (char**) malloc(9 * sizeof(char*))) != NULL && (parts = split_string(new_str, str_now, strlen(str_now), 8, ';')) == 9) {
                    new_node = struct_fill(new_str, i, h_p);
                    insert_after(h_f, new_node, last_node);
                    last_node = new_node;
                } else {
                    i = n;
                    puts("Error filling in the list!\n");
                    flag = 0;
                }
            }
            printf("\nThe list has been successfully filled with elements from the file!\n");
            if (fclose(archive) != EOF) printf("\nClosing OK\n");
            else printf("\nError closing the file!\n");
        } else {
            printf("...But your list is empty!");
            printf("\n\nType 'k' to enter the first line yourself, or any other character to start over and select a new file: ");
            rerun = getchar();
            if (rerun == 'k') {
                fflush(stdin);
                add_first_console(h_f, h_p);
            }
        }
    } else printf("\nError opening the file!\n");
    check_rerun();
}

/*Function: struct_fill
Inserts a string into the structure.
str - a string divided into parts;
i - the number of the last item added to the list;
h - the "head" of the list of directors.
returns: a pointer to the created structure.*/

list *struct_fill(char **str, int i, head_producers *h) {
    list *new_str;
    producer *dir;
    if ((new_str = (list*) malloc(sizeof(list))) != NULL) {
        new_str->numb = i + 1;
        new_str->name = str[0];
        dir = insert_dir(str[1], h);                /*Working with a list of directors' names*/
        new_str->director = dir;
        new_str->length = atoi(str[2]);
        new_str->rating = atof(str[3]);
        new_str->reviews = atoi(str[4]);
        new_str->fees = atof(str[5]);
        new_str->premiere[0] = atoi(str[6]);
        new_str->premiere[1] = atoi(str[7]);
        new_str->premiere[2] = atoi(str[8]);
        new_str->next = NULL;
        new_str->prev = NULL;
    } else printf("Error at memory allocation!");
    return new_str;
}

/*Function: struct_fill_console
Entering structure elements by fields via the console.
h_p - the "head" of the list of directors' names.*/

list *struct_fill_console(head_producers *h_p) {
    char *name, *director;
    int field_int, error_check_int, flag_name;
    float field_float, error_check_float;
    list *new_node;
    producer *dir;
    if ((new_node = (list*) malloc(sizeof(list))) != NULL && (name = (char*) malloc(MAXLEN * sizeof(char))) != NULL && (director = (char*) malloc(MAXLEN * sizeof(char))) != NULL) {
        flag_name = 0;
        while (flag_name == 0) {
            printf("\nEnter the name of the movie: ");
            fgets(name, MAXLEN, stdin);
            name[strlen(name) - 1] = '\0';
            if ((flag_name = check_string(name)) == 1) new_node->name = name;
        }
        if (h_p->count != 0) print_dirs(h_p);
        flag_name = 0;
        while (flag_name == 0) {
            printf("\nEnter the name of the director of your movie: ");
            fgets(director, MAXLEN, stdin);
            director[strlen(director) - 1] = '\0';
            if ((flag_name = check_string(director)) == 1) {
                if (h_p->count == 0) {
                    add_first_dir(h_p, prod_add(director, 0));
                    dir = h_p->first;
                } else dir = insert_dir(director, h_p);
                new_node->director = dir;
            }
        }
        error_check_int = 0;
        while (error_check_int == 0 || field_int <= 0) {
            printf("\nPlease enter the duration of the movie (a number greater than zero): ");
            error_check_int = scanf("%d", &field_int);
            if (error_check_int == 0 || field_int <= 0) {
                printf("\nIncorrect value entered, try again.\n");
                fflush(stdin);
            }
        }
        new_node->length = field_int;
        error_check_float = 0;
        while (error_check_float == 0 || field_float < 0) {
            printf("\nPlease enter the rating of the movie (a number greater than or equal to zero): ");
            error_check_float = scanf("%f", &field_float);
            if (error_check_float == 0 || field_float < 0) {
                printf("\nIncorrect value entered, try again.\n");
                fflush(stdin);
            }
        }
        new_node->rating = field_float;
        error_check_int = 0;
        while (error_check_int == 0 || field_int < 0) {
            printf("\nPlease enter the number of movie reviews (a number greater than or equal to zero): ");
            error_check_int = scanf("%d", &field_int);
            if (error_check_int == 0 || field_int < 0) {
                printf("\nIncorrect value entered, try again.\n");
                fflush(stdin);
            }
        }
        new_node->reviews = field_int;
        error_check_float = 0;
        while (error_check_float == 0 || field_float < 0) {
            printf("\nPlease enter the amount of the film collection worldwide (in millions of $, a number greater than or equal to zero): ");
            error_check_float = scanf("%f", &field_float);
            if (error_check_float == 0 || field_float < 0) {
                printf("\nIncorrect value entered, try again.\n");
                fflush(stdin);
            }
        }
        new_node->fees = field_float;
        getchar();
        input_date(new_node);                /*Correct entry of the premiere date*/
    }
    return new_node;
}

/*Function: check_string
Checking the line for filling
str - the string to check
returns: the result of the check (1 - if not empty, 0 - if empty)*/

int check_string(char *str) {
    int flag, i;
    flag = 0;
    if (strlen(str) != 0) {
        for (i = 0; i < strlen(str); i++) {
            if (str[i] != ' ' && str[i] != '\t') {
                flag = 1;
                i = strlen(str);
            }
        }
    }
    if (flag == 0) printf("\nMake sure that you have not entered an empty string! Try again.\n");
    return flag;
}

/*Function: insert_after
Inserts an item into the list.
h - the "head" of the movie list;
new_node - the structure to be inserted into the list;
current_node - the last structure added to the list.*/

void insert_after(head_films *h, list *new_node, list *current_node) {
    if(h != NULL && new_node != NULL && current_node != NULL) {
        current_node->next = new_node;
        new_node->prev = current_node;
        h->last = new_node;
        h->last->next = h->first;
        h->first->prev = h->last;
    }
}

/*Function: insert_dir
Adds a new director to the specialized list, if there hasn't been one yet.
str - the name of the director;
h - the "head" of the list of directors.
returns: an item in the list of directors.*/

producer *insert_dir(char *str, head_producers *h) {
    producer *dir, *dop;
    int j;
    dir = h->first;
    for (j = 0; j < h->count; j++) {                /*Checking whether such a director's name was already in the list*/
        if (strcmp(dir->fm, str) == 0) {
            j = h->count + 1;
        } else dir = dir->next;
    }
    if (j == h->count) {                /*Adding a name to the list, if there has not been one yet*/
        dir = prod_add(str, h->count);
        dop = h->last;
        dir->prev = dop;
        dir->next = h->first;
        dop->next = dir;
        h->last = dir;
        if (h->count == 1) h->first->next = dir;
        h->count = dir->id;
        h->first->prev = dir;
    } else dir->films++;
    return dir;
}

/*Function: prod_add
Adding the director's name to the list of directors.
str - director's name;
i - the number of the last element added to the list.
returns: is an element of the pointer type to structures with the name of the director.*/

producer *prod_add(char *str, int i) {
    producer *new_str;
    if ((new_str = (producer*) malloc(sizeof(producer))) != NULL) {
        new_str->id = i + 1;
        new_str->films = 1;
        new_str->fm = str;
        new_str->next = NULL;
        new_str->prev = NULL;
    } else printf("Error at memory allocation!");
    return new_str;
}

/*Function: menu_print
Output of the menu with the modes of operation of the program.
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void menu_print(head_films *h_f, head_producers *h_p) {
    int mode, error_check;
    mode = 1;
    while (h_f->count != 0 && mode != 10) {
        struct_out(h_f);                /*output of all authors, and then a menu with all the functions of the program*/
        printf("\nSelect the operating mode:\n");
        printf("|----------------------------------|\n");
        printf("|%5s|%28s|\n", "1", "Output information");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "2", "Output a list");
        printf("|%5s|%28s|\n", "", "of all directors");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "3", "Add a new movie");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "4", "Edit movie information");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "5", "Delete movie(-s)");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "6", "Movie search");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "7", "Sorting lists");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "8", "Replace the director");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "9", "Save the list to a file");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "10", "Finish the program");
        printf("|----------------------------------|\n\nYour choice: ");
        error_check = scanf("%d", &mode);
        printf("\n");
        if (mode < 1 || mode > 10 || error_check == 0) {
            printf("\nIncorrect value entered, try again.\n\n");
            getchar();
            if (error_check == 0) fflush(stdin);;
        } else {
            if (mode == 1) information(h_f, h_p);
            if (mode == 2) {
                print_dirs(h_p);
                getchar();
            }
            if (mode == 3) add_new(h_f, h_p);
            if (mode == 4) edit(h_f, h_p);
            if (mode == 5) menu_deleting(h_f, h_p);
            if (mode == 6) search(h_f, h_p);
            if (mode == 7) sorting_menu(h_f, h_p);
            if (mode == 8) replace_dir(h_f, h_p);
            if (mode == 9) save_list(h_f);
        }
        if (h_f->count != 0 && mode != 10) {
            check_rerun();
        }
    }
    if (h_f->count == 0) printf("\nSorry, but the list is empty! Further work is impossible.\n\n");
}

/*Function: information
Output of information about the list of films.
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void information(head_films *h_f, head_producers *h_p) {
    printf("====================================================\n");
    printf("||General information about the list of movies now||\n");
    printf("====================================================\n");
    printf("||Number of movies in the list: %d                ||\n", h_f->count);
    printf("||Number of directors in the list: %d             ||\n", h_p->count);
    printf("====================================================");
    printf("\n\n====================================================\n");
    printf("||          Information about menu items          ||\n");
    printf("====================================================\n");
    printf("||'Output a list of all directors' - output a list||\n");
    printf("||                    of unique names of directors||\n");
    printf("||------------------------------------------------||\n");
    printf("||'Add a new movie' - add a new item to the       ||\n");
    printf("||                    general list of movies      ||\n");
    printf("||------------------------------------------------||\n");
    printf("||'Edit movie information' - change information   ||\n");
    printf("||                           about any item       ||\n");
    printf("||                           in the list          ||\n");
    printf("||------------------------------------------------||\n");
    printf("||'Delete movie(-s)' - delete a movie by id or    ||\n");
    printf("||                     by director                ||\n");
    printf("||------------------------------------------------||\n");
    printf("||'Movie search' - find movies by field value     ||\n");
    printf("||------------------------------------------------||\n");
    printf("||'Sorting lists' - sort the any list by any field||\n");
    printf("||                   in reverse or forward        ||\n");
    printf("||------------------------------------------------||\n");
    printf("||'Replace the director' - replacing the director ||\n");
    printf("||                         in all his films       ||\n");
    printf("||                         by director            ||\n");
    printf("||------------------------------------------------||\n");
    printf("||'Save the list to a file' - save the current    ||\n");
    printf("||                            list to a file with ||\n");
    printf("||                            the specified name  ||\n");
    printf("====================================================\n");
    printf("\n\n====================================================\n");
    printf("||                     Hints                      ||\n");
    printf("====================================================\n");
    printf("||- When entering decimal fractional numbers, use ||\n");
    printf("||  a dot as a separator character.               ||\n");
    printf("||------------------------------------------------||\n");
    printf("||- If one of the fields after adding the list    ||\n");
    printf("||  from the file remains empty or equal to zero, ||\n");
    printf("||  it means that some data is not specified in   ||\n");
    printf("||  your file. You can fix this using the 4th menu||\n");
    printf("||  item 'edit'.                                  ||\n");
    printf("====================================================\n");
    getchar();
}

/*Function: add_new
Adding a new item to the list.
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void add_new(head_films *h_f, head_producers *h_p) {
    int position, error_check;
    list *last_node, *new_node, *dop_node;
    position = -1;
    last_node = h_f->first;
    while (position < 0 || position > h_f->count) {                /*user's choice of movie id*/
        printf("Enter the number of the movie after which you want to insert the element\nEnter 0 if you want to insert an item at the top of the list.\nYour choice: ");
        error_check = scanf("%d", &position);
        getchar();
        if (position < 0 || position > h_f->count || error_check == 0) {
            printf("\nIncorrect value entered, try again.\n\n");
            if (error_check == 0) fflush(stdin);
        }
    }
    printf("\nOk!\n");
    if (position != 0) {
        while (last_node->numb != position) last_node = last_node->next;
    }
    if ((new_node = struct_fill_console(h_p)) != NULL) {                /*inserting a new item into the list*/
        if (position != 0) {
            dop_node = last_node->next;
            new_node->next = last_node->next;
            dop_node->prev = new_node;
            new_node->prev = last_node;
            last_node->next = new_node;
            if (last_node->numb == h_f->count) h_f->last = new_node;
        } else {
            new_node->next = h_f->first;
            h_f->first->prev = new_node;
            h_f->last->next = new_node;
            h_f->first = new_node;
            new_node->prev = h_f->last;
        }
        h_f->count++;
        new_node->numb = position + 1;
        renumbering_position(new_node, h_f->count);
        printf("\nThe movie '%s' was successfully added to the list at position %d.", new_node->name, new_node->numb);
    }
}

/*Function: edit
Displays a menu with list change modes.
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void edit(head_films *h_f, head_producers *h_p) {
    int mode, position, error_check;
    list *node;
    position = -1;
    node = h_f->first;
    while (position <= 0 || position > h_f->count) {                /*user's choice of movie id*/
        printf("Enter the number of the movie to replace the information about: ");
        error_check = scanf("%d", &position);
        getchar();
        if (position <= 0 || position > h_f->count || error_check == 0) {
            printf("\nIncorrect value entered, try again.\n\n");
            if (error_check == 0) fflush(stdin);
        }
    }
    while (node->numb != position) node = node->next;
    printf("\nYour movie:\n");
    printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    printf("|%3s|%26s     |%16s       |%15s |%19s  |%19s  |%21s  |%19s      |\n", "id", "The name of the movie","Director","Duration (min)","Critics ' ratings", "Number of reviews", "Fees in the world ", "Premiere date");
    printf("|%3s|%30s |%22s |%15s |%20s |%20s |%22s |-------------------------|\n", "", "", "", "", "", "", "");
    printf("|%3s|%30s |%22s |%15s |%19s  |%19s  |%19s    |%7s | %6s |%6s |\n", "", "", "", "", "", "", "(millions of $)", "day", "month", "year");
    printf("|---|-------------------------------+-----------------------+----------------+---------------------+---------------------+-----------------------+--------+--------+-------|\n");
    printf("|%3d|%30s |%22s |%15d |%20.1f |%20d |%22.3f |%7d | %6d |%6d |\n", node->numb, node->name, node->director->fm, node->length, node->rating, node->reviews, node->fees,
            node->premiere[0], node->premiere[1], node->premiere[2]);
    printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n\n");
    while (mode < 1 || mode > 8 || error_check == 0) {                /*displays a menu with available field options to change*/
        printf("Select the field to change:\n");
        printf("|----------------------------------|\n");
        printf("|%5s|%28s|\n", "1", "The name of the movie");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "2", "Director's name");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "3", "Duration (min)");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "4", "Critics ' ratings");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "5", "Number of reviews");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "6", "Fees in the world");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "7", "Premiere date");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "8", "Change all information");
        printf("|%5s|%28s|\n", "", "about the movie");
        printf("|----------------------------------|\n\nYour choice: ");
        error_check = scanf("%d", &mode);
        printf("\n");
        if (mode < 0 || mode > 8 || error_check == 0) {
            printf("\nIncorrect value entered, try again.\n\n");
            if (error_check == 0) fflush(stdin);
        } else {
            if (mode == 3 || mode == 5) edit_field_int(node, mode);
            if (mode == 4 || mode == 6) edit_field_float(node, mode);
            getchar();
            if (mode == 7) input_date(node);
            if (mode == 1) edit_name(node);
            if (mode == 2) edit_dir(node, h_f, h_p);
            if (mode == 8) edit_full(node, h_f, h_p);
            printf("\n\nInformation about the film '%s' has been updated!\n", node->name);
        }
    }
}

/*Function: edit_field_int
Changing a field with an int value.
node - the list item in which the field will be changed;
mode - an indication of which field the work will be carried out with.*/

void edit_field_int(list *node, int mode) {
    int new_value, error_check;
    printf("\nOk!\n");
    error_check = 0;
    while (error_check == 0) {                /*entering and inserting a new value into an element*/
        printf("\nEnter a new field value: ");
        error_check = scanf("%d", &new_value);
        if (mode == 3) node->length = new_value;
        else node->reviews = new_value;
        if (error_check == 0) {
            printf("\nIncorrect value entered, try again.\n");
            fflush(stdin);
        }
    }
}

/*Function: edit_field_float
Changing a field with an float value.
node - the list item in which the field will be changed;
mode - an indication of which field the work will be carried out with.*/

void edit_field_float(list *node, int mode) {
    float new_value, error_check;
    printf("\nOk!\n");
    error_check = 0;
    while (error_check == 0) {                /*entering and inserting a new value into an element*/
        printf("\nEnter a new field value: ");
        error_check = scanf("%f", &new_value);
        if (mode == 4) node->rating = new_value;
        else node->fees = new_value;
        if (error_check == 0) {
            printf("\nIncorrect value entered, try again.\n");
            fflush(stdin);
        }
    }
}

/*Function: edit_name
Changing the name of the movie in the list item.
node - the list item in which the field will be changed.*/

void edit_name(list *node) {
    int flag_name;
    char *new_name;
    printf("\nOk!\n");
    if ((new_name = (char*) malloc(MAXLEN * sizeof(char))) != NULL) {                /*entering and inserting a new value into an element*/
        flag_name = 0;
        while (flag_name == 0) {
            printf("\nEnter the changed movie name: ");
            fgets(new_name, MAXLEN, stdin);
            new_name[strlen(new_name) - 1] = '\0';
            flag_name = check_string(new_name);
        }
        node->name = new_name;
    } else printf("\nError at memory allocation!\n");

}

/*Function: input_date
Changing the field in the list item with the premiere date.
node - the list item in which the field will be changed.*/

void input_date(list *node) {
    char **new_str;
    char str_now[MAXLEN];
    int i, parts;
    parts = 0;
    while (parts != 3) {                /*entering and inserting a new value into an element*/
        printf("\nEnter the premiere date in the format DD.MM.YYYY : ");
        fgets(str_now, MAXLEN, stdin);
        str_now[strlen(str_now) - 1] = '\0';
        if ((new_str = (char**) malloc(3 * sizeof(char*))) != NULL && (parts = split_string(new_str, str_now, strlen(str_now), 2, '.')) == 3
            && check_date(new_str) == 1) {
            for (i = 0; i < 3; i++) node->premiere[i] = atoi(new_str[i]);
        } else parts = 0;
    }
}
/*Function: check_date
Checking the date for correctness.
new_str - the date.
returns: 1 if the date is entered correctly, 0 if otherwise.*/
int check_date(char **new_str) {
    int flag, new_date[3], i;
    flag = 0;
    for (i = 0; i < 3; i++) {
        new_date[i] = atoi(new_str[i]);
    }
    if (new_date[1] > 0 && new_date[1] <= 12 && new_date[0] > 0 && new_date[0] <= 31 && new_date[2] >= 1888) {
        if (new_date[1] == 2 && (new_date[0] < 29 || ((((new_date[2] % 4) == 0 && (new_date[2] % 100) != 0) || (new_date[2] % 400) == 0) && new_date[0] < 30))) flag = 1;
        else if ((new_date[1] == 4 || new_date[1] == 6 || new_date[1] == 9 || new_date[1] == 11) && new_date[0] < 31) flag = 1;
        else if (new_date[1] == 1 || new_date[1] == 3 || new_date[1] == 5 || new_date[1] == 7 || new_date[1] == 8 || new_date[1] == 10 || new_date[1] == 12) flag = 1;
    }
    if (flag == 0) printf("\nCheck that you have entered a real date (the first film was shot in 1888)!\n");
    return flag;
}

/*Function: edit_dir
Changing the name of the film director in the list item.
node - the list item in which the field will be changed;
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void edit_dir(list *node, head_films *h_f, head_producers *h_p) {
    int flag_name;
    char *new_name;
    producer *dir;
    printf("\nOk!\n");
    if ((new_name = (char*) malloc(MAXLEN * sizeof(char))) != NULL) {                /*entering and inserting a new value into an element*/
        flag_name = 0;
        while (flag_name == 0) {
            printf("\nEnter the changed name of the director: ");
            fgets(new_name, MAXLEN, stdin);
            new_name[strlen(new_name) - 1] = '\0';
            flag_name = check_string(new_name);
        }
        check_dirs(node, h_p);
        if (h_p->count == 0) add_first_dir(h_p, prod_add(new_name, 0));
        dir = insert_dir(new_name, h_p);
        node->director = dir;
    } else printf("\nError at memory allocation!\n");
}

/*Function: edit_full
Complete change of the list item.
node - the list item in which the field will be changed;
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void edit_full(list *node, head_films *h_f, head_producers *h_p) {
    list *new_node, *p_node, *n_node;
    if ((new_node = struct_fill_console(h_p)) != NULL) {                /*replacing the old one with a new element*/
        new_node->numb = node->numb;
        check_dirs(node, h_p);
        if (node->numb == h_f->count) h_f->last = new_node;
        if (node->numb == 1) h_f->first = new_node;
        p_node = node->prev;
        n_node = node->next;
        new_node->prev = p_node;
        new_node->next = n_node;
        p_node->next = new_node;
        n_node->prev = new_node;
        free(node);
        node = NULL;
    }
}

/*Function: menu_deleting
Menu for selecting the mode for deleting list items.
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void menu_deleting(head_films *h_f, head_producers *h_p) {
    int mode, error_check;
    mode = 0;
    while (mode < 1 || mode > 2 || error_check == 0) {                /*displaying a menu with removal methods*/
        printf("Select the operating mode:\n");
        printf("|----------------------------------|\n");
        printf("|%5s|%28s|\n", "1", "Deleting a movie by id");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "2", "Deleting films");
        printf("|%5s|%28s|\n", "", "of a given director");
        printf("|----------------------------------|\n\nYour choice: ");
        error_check = scanf("%d", &mode);
        printf("\n");
        if (mode < 1 || mode > 2 || error_check == 0) {
            printf("Incorrect value entered, try again.\n\n");
            if (error_check == 0) fflush(stdin);
        } else {
            if (mode == 1) del_node(h_f, h_p);
            else del_all_films(h_f, h_p);
            if (h_p->count == 0) h_f->count = 0;
        }
    }
}

/*Function: del_node
Deleting an element by id.
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void del_node(head_films *h_f, head_producers *h_p) {
    list *p_node, *del_node, *n_node;
    int position, error_check;
    position = -1;
    del_node = h_f->first;
    while (position <= 0 || position > h_f->count ||  error_check == 0) {
        printf("Enter the number of the movie you want to delete: ");
        error_check = scanf("%d", &position);
        getchar();
        if (position <= 0 || position > h_f->count || error_check == 0) {
            printf("\nIncorrect value entered, try again.\n\n");
            if (error_check == 0) fflush(stdin);
        }
    }
    while (del_node->numb != position) del_node = del_node->next;
    h_f->count--;
    check_dirs(del_node, h_p);
    p_node = del_node->prev;                /*deleting an element using links*/
    n_node = del_node->next;
    p_node->next = n_node;
    n_node->prev = p_node;
    if (position == 1) h_f->first = n_node;
    if (position == h_f->count + 1) h_f->last = p_node;
    free(del_node);
    if (position != h_f->last->numb + 1) {
        n_node->numb = position;
        renumbering_position(n_node, h_f->count);
    }
    printf("\nThe film number %d was deleted. The order in the list has been updated.", position);
}

/*Function: del_all_films
Deleting all elements of the director specified by the user.
h - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void del_all_films(head_films *h, head_producers *h_p) {
    list *del_node, *p_node, *n_node;
    producer *del_dir;
    char name[MAXLEN];
    int count, i;
    del_node = h->first;
    getchar();
    printf("Enter the name of the director whose films you want to delete: ");
    fgets(name, MAXLEN, stdin);
    name[strlen(name) - 1] = '\0';
    for (i = 0, count = 0; i <= h->count; i++) {                /*deleting all the films of this director using links*/
        if (strcmp(del_node->director->fm, name) == 0) {
            if (h->count != 1) {
                p_node = del_node->prev;
                n_node = del_node->next;
                n_node->prev = p_node;
                p_node->next = n_node;
                if (del_node == h->first) h->first = n_node;
                if (del_node == h->last) h->last = p_node;
            }
            free(del_node);
            del_node = NULL;
            h->count--;
            if (h->count != 0) del_node = n_node;
            count++;
        } else del_node = del_node->next;
    }
    if (count == 0) printf("\nNo films of such a director have been found!");
    else {
        printf("\nAll of %s's films have been deleted!\n", name);
        del_dir = h_p->first;
        for (i = 0; i < h_p->count; i++) {                /*search for the desired director in the general list and remove him from there*/
            if (strcmp(del_dir->fm, name) == 0) i = h_p->count;
            else del_dir = del_dir->next;
        }
        deleting_producer(del_dir, h_p);
        if (h_p != 0) renumbering_all(h);
    }
}

/*Function: deleting_producer
Removing a director from the list of directors' names.
del_dir - director's name;
h_p - the "head" of the list of directors' names.*/

void deleting_producer(producer *del_dir, head_producers *h_p) {
    int i;
    producer *p_dir, *n_dir;
    if (h_p->count != 1) {
        n_dir = del_dir->next;
        p_dir = del_dir->prev;
        n_dir->prev = p_dir;
        p_dir->next = n_dir;
        if (del_dir == h_p->first) h_p->first = n_dir;
        if (del_dir == h_p->last) h_p->last = p_dir;
    }
    free(del_dir);
    del_dir = NULL;
    h_p->count--;
    if (h_p->count != 1) n_dir = h_p->first;
    for (i = 1; i <= h_p->count; i++) {
        n_dir->id = i;
        n_dir = n_dir->next;
    }
}

/*Function: search
Menu for selecting the list search mode.
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void search(head_films *h_f, head_producers *h_p) {
    int mode, error_check;
    mode = 0;
    while (mode < 1 || mode > 7) {                /*display a menu with search modes*/
        printf("Select the field to search:\n");
        printf("|----------------------------------|\n");
        printf("|%5s|%28s|\n", "1", "The name of the movie");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "2", "Director's name");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "3", "Duration (min)");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "4", "Critics ' ratings");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "5", "Number of reviews");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "6", "Fees in the world");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "7", "Premiere date");
        printf("|----------------------------------|\n\nYour choice: ");
        error_check = scanf("%d", &mode);
        printf("\n");
        if (mode < 1 || mode > 7 || error_check == 0) {
            printf("Incorrect value entered, try again.\n\n");
            if (error_check == 0) fflush(stdin);
        } else {
            if (mode == 3 || mode == 5) search_int(h_f, mode);
            if (mode == 4 || mode == 6) search_float(h_f, mode);
            getchar();
            if (mode == 1 || mode == 2) search_name(h_f, mode);
            if (mode == 7) search_date(h_f);
        }
    }
}

/*Function: search_name
Search in the list by the name of the movie or by the name of the director.
h_f - the "head" of the movie list;
mode - the selected search mode.*/

void search_name(head_films *h_f, int mode) {
    char s_name[MAXLEN];
    list *node;
    int i, count, flag_name;
    count = 0;
    node = h_f->first;
    flag_name = 0;
    while (flag_name == 0) {
        if (mode == 1) printf("Enter the name of the movie you are looking for: ");
        else printf("Enter the name of the director whose films you want to find: ");
        fgets(s_name, MAXLEN, stdin);
        s_name[strlen(s_name) - 1] = '\0';
        flag_name = check_string(s_name);
    }
    printf("\n\nFound Movies:\n");
    printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    printf("|%3s|%26s     |%16s       |%15s |%19s  |%19s  |%21s  |%19s      |\n", "id", "The name of the movie","Director","Duration (min)","Critics ' ratings", "Number of reviews", "Fees in the world ", "Premiere date");
    printf("|%3s|%30s |%22s |%15s |%20s |%20s |%22s |-------------------------|\n", "", "", "", "", "", "", "");
    printf("|%3s|%30s |%22s |%15s |%19s  |%19s  |%19s    |%7s | %6s |%6s |\n", "", "", "", "", "", "", "(millions of $)", "day", "month", "year");
    for (i = 0; i < h_f->count; i++) {
        if ((strcmp(node->name, s_name) == 0 && mode == 1) || (strcmp(node->director->fm, s_name) == 0 && mode == 2)) {
            printf("|---|-------------------------------+-----------------------+----------------+---------------------+---------------------+-----------------------+--------+--------+-------|\n");
            printf("|%3d|%30s |%22s |%15d |%20.1f |%20d |%22.3f |%7d | %6d |%6d |\n", node->numb, node->name, node->director->fm, node->length, node->rating, node->reviews, node->fees,
                node->premiere[0], node->premiere[1], node->premiere[2]);
            count++;
        }
        node = node->next;
    }
    if (count == 0) {
        printf("|---|-------------------------------+-----------------------+----------------+---------------------+---------------------+-----------------------+--------+--------+-------|\n");
        printf("|                                                                      %s                                                                      |\n", "No such films have been found!");
    }
    printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n\n");
}

/*Function: search_int
Search by the selected int field.
h_f - the "head" of the movie list;
mode - the selected search mode.*/

void search_int(head_films *h_f, int mode) {
    int s_value_min, s_value_max;
    list *node;
    int i, count, error_check;
    s_value_min = -1;
    s_value_max = -1;
    count = 0;
    error_check = 0;
    node = h_f->first;
    while (error_check == 0 || s_value_min < 0) {
        if (mode == 3) printf("Enter the minimum duration of the movie (in minutes): ");
        else printf("Enter the minimum number of movie reviews: ");
        error_check = scanf("%d", &s_value_min);
        if (error_check == 0 || s_value_min  < 0) {
            printf("\nIncorrect value entered, try again.\n\n");
            if (error_check == 0) fflush(stdin);
        }
    }
    while (error_check == 0 || s_value_max < 0) {
        if (mode == 3) printf("\nEnter the maximum duration of the movie (in minutes): ");
        else printf("\nEnter the maximum number of movie reviews: ");
        error_check = scanf("%d", &s_value_max);
        if (error_check == 0 || s_value_max  < 0) {
            printf("\nIncorrect value entered, try again.\n\n");
            if (error_check == 0) fflush(stdin);
        }
    }
    printf("\n\nFound Movies:\n");
    printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    printf("|%3s|%26s     |%16s       |%15s |%19s  |%19s  |%21s  |%19s      |\n", "id", "The name of the movie","Director","Duration (min)","Critics ' ratings", "Number of reviews", "Fees in the world ", "Premiere date");
    printf("|%3s|%30s |%22s |%15s |%20s |%20s |%22s |-------------------------|\n", "", "", "", "", "", "", "");
    printf("|%3s|%30s |%22s |%15s |%19s  |%19s  |%19s    |%7s | %6s |%6s |\n", "", "", "", "", "", "", "(millions of $)", "day", "month", "year");
    for (i = 0; i < h_f->count; i++) {                /*search for movies within the specified values*/
        if ((node->length >= s_value_min && node->length <= s_value_max && mode == 3) || (node->reviews >= s_value_min && node->reviews <= s_value_max && mode == 5)) {
            printf("|---|-------------------------------+-----------------------+----------------+---------------------+---------------------+-----------------------+--------+--------+-------|\n");
            printf("|%3d|%30s |%22s |%15d |%20.1f |%20d |%22.3f |%7d | %6d |%6d |\n", node->numb, node->name, node->director->fm, node->length, node->rating, node->reviews, node->fees,
                node->premiere[0], node->premiere[1], node->premiere[2]);
            count++;
        }
        node = node->next;
    }
    if (count == 0) {
        printf("|---|-------------------------------+-----------------------+----------------+---------------------+---------------------+-----------------------+--------+--------+-------|\n");
        printf("|%93s|\n", "No such films have been found!");
    }
    printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n\n");
}

/*Function: search_float
Search by the selected float field.
h_f - the "head" of the movie list;
mode - the selected search mode.*/

void search_float(head_films *h_f, int mode) {
    float s_value_min, s_value_max, error_check;
    list *node;
    int i, count;
    s_value_min = -1;
    s_value_max = -1;
    error_check = 0;
    node = h_f->first;
    count = 0;
    while (error_check == 0 || s_value_min < 0) {
        if (mode == 4) printf("Enter the minimum critical rating: ");
        else printf("Enter the minimum amount of fees (in millions of $): ");
        error_check = scanf("%f", &s_value_min);
        if (error_check == 0 || s_value_min  < 0) {
            printf("\nIncorrect value entered, try again.\n\n");
            if (error_check == 0) fflush(stdin);
        }
    }
    while (error_check == 0 || s_value_max < 0) {
        if (mode == 4) printf("\nEnter the maximum critics rating: ");
        else printf("\nEnter the maximum amount of fees (in millions of $): ");
        error_check = scanf("%f", &s_value_max);
        if (error_check == 0 || s_value_max  < 0) {
            printf("\nIncorrect value entered, try again.\n");
            if (error_check == 0) fflush(stdin);
        }
    }
    printf("\n\nFound Movies:\n");
    printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    printf("|%3s|%26s     |%16s       |%15s |%19s  |%19s  |%21s  |%19s      |\n", "id", "The name of the movie","Director","Duration (min)","Critics ' ratings", "Number of reviews", "Fees in the world ", "Premiere date");
    printf("|%3s|%30s |%22s |%15s |%20s |%20s |%22s |-------------------------|\n", "", "", "", "", "", "", "");
    printf("|%3s|%30s |%22s |%15s |%19s  |%19s  |%19s    |%7s | %6s |%6s |\n", "", "", "", "", "", "", "(millions of $)", "day", "month", "year");
    for (i = 0; i < h_f->count; i++) {                /*search for movies within the specified values*/
        if ((node->rating >= s_value_min && node->rating <= s_value_max && mode == 4) || (node->fees >= s_value_min && node->fees <= s_value_max && mode == 6)) {
            printf("|---|-------------------------------+-----------------------+----------------+---------------------+---------------------+-----------------------+--------+--------+-------|\n");
            printf("|%3d|%30s |%22s |%15d |%20.1f |%20d |%22.3f |%7d | %6d |%6d |\n", node->numb, node->name, node->director->fm, node->length, node->rating, node->reviews, node->fees,
                node->premiere[0], node->premiere[1], node->premiere[2]);
            count++;
        }
        node = node->next;
    }
    if (count == 0) {
        printf("|---|-------------------------------+-----------------------+----------------+---------------------+---------------------+-----------------------+--------+--------+-------|\n");
        printf("|                                                                      %s                                                                      |\n", "No such films have been found!");
    }
    printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n\n");
}

/*Function: search_date
Search by movie premiere date.
h_f - the "head" of the movie list.*/

void search_date(head_films *h_f) {
    list *node;
    char **new_date_min, **new_date_max;
    char date_min[MAXLEN], date_max[MAXLEN];
    int i, parts1, parts2, count;
    node = h_f->first;
    count = 0;
    if ((new_date_min = (char**) malloc(3 * sizeof(char*))) != NULL && (new_date_max = (char**) malloc(3 * sizeof(char*))) != NULL) {
        printf("\nNote: please enter dates in the format DD.MM.YYYY.");
        parts1 = 0;
        while (parts1 != 3) {
            printf("\nEnter the earliest movie premiere date: ");
            fgets(date_min, MAXLEN, stdin);
            date_min[strlen(date_min) - 1] = '\0';
            parts1 = split_string(new_date_min, date_min, strlen(date_min), 2, '.');
            if (parts1 == 3 && check_date(new_date_min) == 0) parts1 = 0;
        }
        parts2 = 0;
        while (parts2 != 3) {
            printf("\nEnter the latest movie premiere date: ");
            fgets(date_max, MAXLEN, stdin);
            date_max[strlen(date_max) - 1] = '\0';
            parts2 = split_string(new_date_max, date_max, strlen(date_max), 2, '.');
            if (parts2 == 3 && check_date(new_date_max) == 0) parts2 = 0;
        }
        if (atoi(new_date_max[2]) > atoi(new_date_min[2]) || (atoi(new_date_max[2]) == atoi(new_date_min[2]) && (atoi(new_date_max[1]) > atoi(new_date_min[1]) ||
            (atoi(new_date_max[1]) == atoi(new_date_min[1]) && atoi(new_date_max[0]) >= atoi(new_date_min[0]))))) {
            printf("\n\nFound Movies:\n");
            printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
            printf("|%3s|%26s     |%16s       |%15s |%19s  |%19s  |%21s  |%19s      |\n", "id", "The name of the movie","Director","Duration (min)","Critics ' ratings", "Number of reviews", "Fees in the world ", "Premiere date");
            printf("|%3s|%30s |%22s |%15s |%20s |%20s |%22s |-------------------------|\n", "", "", "", "", "", "", "");
            printf("|%3s|%30s |%22s |%15s |%19s  |%19s  |%19s    |%7s | %6s |%6s |\n", "", "", "", "", "", "", "(millions of $)", "day", "month", "year");
            for (i = 0; i < h_f->count; i++) {                /*search for movies within the specified values*/
                if (comparison(node->premiere, new_date_min) == 1 && comparison(node->premiere, new_date_max) == 0) {
                    printf("|---|-------------------------------+-----------------------+----------------+---------------------+---------------------+-----------------------+--------+--------+-------|\n");
                    printf("|%3d|%30s |%22s |%15d |%20.1f |%20d |%22.3f |%7d | %6d |%6d |\n", node->numb, node->name, node->director->fm, node->length, node->rating, node->reviews, node->fees,
                        node->premiere[0], node->premiere[1], node->premiere[2]);
                    count++;
                }
                node = node->next;
            }
            if (count == 0) {
                printf("|---|-------------------------------+-----------------------+----------------+---------------------+---------------------+-----------------------+--------+--------+-------|\n");
                printf("|%93s%77s|\n", "No such films have been found!", "");
            }
            printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n\n");
        } else printf("\nThe second date should be later than the first. Try again.\n");
    } else printf("\nError at memory allocation!\n");
}

/*Function: sorting_menu
Displays the field selection menu and the mode for sorting the list.
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors' names.*/

void sorting_menu(head_films *h_f, head_producers *h_p) {
    int mode, order, check_error_mode, check_error_order;
    mode = 0;
    while (mode < 1 || mode > 8 || check_error_mode == 0 || check_error_order == 0) {
        printf("Select a field to sort the list:\n");
        printf("|----------------------------------|\n");
        printf("|%5s|%28s|\n", "1", "The name of the movie");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "2", "Director's name");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "3", "Duration (min)");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "4", "Critics ' ratings");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "5", "Number of reviews");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "6", "Fees in the world");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "7", "Premiere date");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "8", "List of directors");
        printf("|----------------------------------|\n\nYour choice: ");
        check_error_mode = scanf("%d", &mode);
        if (check_error_mode == 0) fflush(stdin);
        printf("\nSelect the sort order:\n");
        printf("|----------------------------------|\n");
        printf("|%5s|%28s|\n", "1", "Direct order");
        printf("|-----+----------------------------|\n");
        printf("|%5s|%28s|\n", "2", "Reverse order");
        printf("|----------------------------------|\n\nYour choice: ");
        check_error_order = scanf("%d", &order);
        printf("\n");
        if (mode < 1 || mode > 8 || order < 1 || order > 2 || check_error_mode == 0 || check_error_order == 0) {
            printf("\nIncorrect value entered, try again.\n\n");
            if (check_error_order == 0) fflush(stdin);
        } else {
            if (mode != 8) sort(h_f, mode, order);
            else sort_dirs(h_p, order);
            getchar();
            renumbering_all(h_f);
        }
    }
}

/*Function: sort
Sorting the list.
h - the "head" of the movie list;
mode - the field by which the sorting takes place;
order - the direction of sorting.*/

void sort_dirs(head_producers *h, int order) {
    int f, i, count;
    producer *one_node, *two_node, *p_node, *n_node;
    f = 1;
    while (f == 1) {
        count = 0;
        one_node = h->first;
        two_node = one_node->next;
        for (i = 0; i < h->count - 1; i++) {
            if ((order == 1 && strcmp(one_node->fm, two_node->fm) > 0)
                ||(order == 2 && strcmp(one_node->fm, two_node->fm) < 0)) {                /*swapping elements using links if necessary*/
                p_node = one_node->prev;
                n_node = two_node->next;
                p_node->next = two_node;
                two_node->prev = p_node;
                two_node->next = one_node;
                one_node->prev = two_node;
                one_node->next = n_node;
                n_node->prev = one_node;
                if (one_node == h->first) h->first = two_node;
                if (two_node == h->last) h->last = one_node;
            } else count++;
            one_node = two_node;
            two_node = one_node->next;
        }
        if (count == h->count - 1) f = 0;
    }
    one_node = h->first;
    for (i = 1; i <= h->count; i++) {
        one_node->id = i;
        one_node = one_node->next;
    }
    printf("\nThe list has been sorted!\n");
}

/*Function: sort
Sorting the list.
h - the "head" of the movie list;
mode - the field by which the sorting takes place;
order - the direction of sorting.*/

void sort(head_films *h, int mode, int order) {
    int f, i, count;
    list *one_node, *two_node, *p_node, *n_node;
    f = 1;
    while (f == 1) {
        count = 0;
        one_node = h->first;
        two_node = one_node->next;
        for (i = 0; i < h->count - 1; i++) {
            if (swap_global(one_node, two_node, mode, order) == 1) {                /*swapping elements using links if necessary*/
                p_node = one_node->prev;
                n_node = two_node->next;
                p_node->next = two_node;
                two_node->prev = p_node;
                two_node->next = one_node;
                one_node->prev = two_node;
                one_node->next = n_node;
                n_node->prev = one_node;
                if (one_node == h->first) h->first = two_node;
                if (two_node == h->last) h->last = one_node;
            } else count++;
            one_node = two_node;
            two_node = one_node->next;
        }
        if (count == h->count - 1) f = 0;
    }
    printf("\nThe list has been sorted!\n");
}

/*Function: swap_name
Function for sorting by movie title.
first - the first structure for comparison;
second - the second structure for comparison;
order - sort order.
returns: the comparison mode is 1 if the elements need to be swapped, or 0 if otherwise.*/

int swap_name(list *first, list *second, int order) {
    int flag;
    flag = 0;
    if ((order == 1 && strcmp(first->name, second->name) > 0) ||(order == 2 && (strcmp(first->name, second->name) < 0))) flag = 1;
    return flag;
}

/*Function: swap_dir
A function for sorting by the names of the film directors.
first - the first structure for comparison;
second - the second structure for comparison;
order - sort order;
returns: the comparison mode is 1 if the elements need to be swapped, or 0 if otherwise.*/

int swap_dir(list *first, list *second, int order) {
    int flag;
    flag = 0;
    if ((order == 1 && strcmp(first->director->fm, second->director->fm) > 0) ||(order == 2 && (strcmp(first->director->fm, second->director->fm) < 0))) flag = 1;
    return flag;
}

/*Function: swap_d1
A function for sorting by premiere date in direct order.
first - the first structure for comparison;
second - the second structure for comparison.
returns: the comparison mode is 1 if the elements need to be swapped, or 0 if otherwise.*/

int swap_d1(list *first, list *second) {
    int flag;
    flag = 0;
    if ((first->premiere[2] > second->premiere[2]) || (first->premiere[2] == second->premiere[2] && first->premiere[1] > second->premiere[1]) ||
        (first->premiere[2] == second->premiere[2] && first->premiere[1] == second->premiere[1] && first->premiere[0] > second->premiere[0])){
        flag = 1;
    }
    return flag;
}

/*Function: swap_d2
A function for sorting by premiere date in reverse order.
first - the first structure for comparison;
second - the second structure for comparison.
returns: the comparison mode is 1 if the elements need to be swapped, or 0 if otherwise.*/

int swap_d2(list *first, list *second) {
    int flag;
    flag = 0;
    if ((first->premiere[2] < second->premiere[2]) || (first->premiere[2] == second->premiere[2] && first->premiere[1] < second->premiere[1]) ||
        (first->premiere[2] == second->premiere[2] && first->premiere[1] == second->premiere[1] && first->premiere[0] < second->premiere[0])){
        flag = 1;
    }
    return flag;
}

/*Function: swap_global
A function for sorting in a user-defined mode.
first - the first structure for comparison;
second - the second structure for comparison;
mode - the field by which the sorting takes place;
order - sort order.
returns: the comparison mode is 1 if the elements need to be swapped, or 0 if otherwise.*/

int swap_global(list *first, list *second, int mode, int order) {
    int flag;
    flag = 0;
    if (mode == 1 && swap_name(first, second, order) == 1) flag = 1;
    if (mode == 2 && swap_dir(first, second, order) == 1) flag = 1;
    if (mode == 3 && ((second->length < first->length && order == 1) || (second->length > first->length && order == 2))) flag = 1;
    if (mode == 4 && ((second->rating < first->rating && order == 1) || (second->rating > first->rating && order == 2))) flag = 1;
    if (mode == 5 && ((second->reviews < first->reviews && order == 1) || (second->reviews > first->reviews && order == 2))) flag = 1;
    if (mode == 6 && ((second->fees < first->fees && order == 1) || (second->fees > first->fees && order == 2))) flag = 1;
    if (mode == 7 && ((order == 1 && swap_d1(first, second) == 1) || (order == 2 && swap_d2(first, second) == 1))) flag = 1;
    return flag;
}

/*Function: replace_dir
Replace the name of the director in all his films with another one
h_f - the "head" of the movie list;
h_p - the "head" of the list of directors*/
void replace_dir(head_films *h_f, head_producers *h_p) {
    char *new_name, *old_name;
    int id, err, i, j;
    producer *repl_p, *new_p;
    list *node;
    if ((new_name = (char*) malloc(MAXLEN * sizeof(char))) != NULL && (old_name = (char*) malloc(MAXLEN * sizeof(char))) != NULL) {
        print_dirs(h_p);
        err = 0;
        while (id < 1 || id > h_p->count || err == 0) {
            printf("\nEnter the ID of the director you want to replace (from 1 to %d): ", h_p->count);
            err = scanf("%d", &id);
            if (id < 1 || id > h_p->count || err == 0) {
                printf("\nIncorrect value entered, try again.\n\n");
                if (err == 0) fflush(stdin);
            }
        }
        repl_p = h_p->first;
        while (repl_p->id != id) repl_p = repl_p->next;
        old_name = repl_p->fm;
        getchar();
        err = 0;
        while (err == 0) {
            printf("\nEnter the director's replacement name: ");
            fgets(new_name, MAXLEN, stdin);
            new_name[strlen(new_name) - 1] = '\0';
            err = check_string(new_name);
        }
        new_p = h_p->first;
        for (i = 0; i < h_p->count; i++) {
            if (strcmp(new_p->fm, new_name) == 0) {
                i = h_p->count + 2;
                new_p->films = new_p->films + repl_p->films;
            } else new_p = new_p->next;
        }
        if (i == h_p->count) {
            new_p = repl_p;
            new_p->fm = new_name;
        }
        if (strcmp(new_p->fm, old_name) != 0) {
            node = h_f->first;
            for (j = 0; j < h_f->count; j++) {                /*finding all the films of the replaced director*/
                if (strcmp(node->director->fm, old_name) == 0) {
                    node->director = new_p;
                }
                node = node->next;
            }
            if (i != h_p->count) deleting_producer(repl_p, h_p);
            printf("\nThe names of the directors of some films have been updated!\n");
        } else {
            printf("\nYou entered the same name! The operation is impossible.\n");
            free(new_name);
        }
        free(old_name);
    } else printf("\nError at memory allocation!");
}


/*Function: renumbering_all
Renumbering the entire list.
h - the "head" of the movie list.*/

void renumbering_all(head_films *h) {
    list *node;
    int i;
    node = h->first;
    for (i = 1; i <= h->count; i++) {
        node->numb = i;
        node = node->next;
    }
}

/*Function: renumbering_position
Renumbering the list from the specified element.
node - the element after which the renumbering takes place;
count - the length of the movie list.*/


void renumbering_position(list *node, int count) {
    int i;
    for (i = node->numb; i <= count; i++) {
        node->numb = i;
        node = node->next;
    }
}

/*Function: comparison
Comparison of premiere dates.
node_date - the date contained in the list item;
date - the date specified by the user.
returns: the result of the comparison is 0 if the item's date is earlier than the user's date, or 1 if otherwise.*/

int comparison(int *node_date, char **date) {
    int flag;
    flag = 0;
    if (node_date[2] > atoi(date[2]) || (node_date[2] == atoi(date[2]) && (node_date[1] > atoi(date[1]) ||
        (node_date[1] == atoi(date[1]) && node_date[0] >= atoi(date[0]))))) {
        flag = 1;
    }
    return flag;
}

/*Function: check_dirs
A function to check whether the director's name should be removed from the list of unique names of directors.
del_node - a list item containing the name of the director to be checked;
h_p - the "head" of the list of directors' names*/


void check_dirs(list *del_node, head_producers *h_p) {
    if (del_node->director->films == 1) deleting_producer(del_node->director, h_p);                /*the director is removed if he has only 1 movie in the list*/
    else del_node->director->films--;
}

/*Function: struct_out
Output of the entire list of movies.
h - the "head" of the movie list.*/

void struct_out(head_films *h){
    list *node;
    int i;
    node = h->first;
    printf("%95s\n", "Full list of movies:");
    printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n");
    printf("|%3s|%26s     |%16s       |%15s |%19s  |%19s  |%21s  |%19s      |\n", "id", "The name of the movie","Director","Duration (min)","Critics ' ratings", "Number of reviews", "Fees in the world ", "Premiere date");
    printf("|%3s|%30s |%22s |%15s |%20s |%20s |%22s |-------------------------|\n", "", "", "", "", "", "", "");
    printf("|%3s|%30s |%22s |%15s |%19s  |%19s  |%19s    |%7s | %6s |%6s |\n", "", "", "", "", "", "", "(millions of $)", "day", "month", "year");
    for (i = 0; i < h->count; i++) {
        printf("|---|-------------------------------+-----------------------+----------------+---------------------+---------------------+-----------------------+--------+--------+-------|\n");
        printf("|%3d|%30s |%22s |%15d |%20.1f |%20d |%22.3f |%7d | %6d |%6d |\n", node->numb, node->name, node->director->fm, node->length, node->rating, node->reviews, node->fees,
               node->premiere[0], node->premiere[1], node->premiere[2]);
        node = node->next;
    }
    printf("|--------------------------------------------------------------------------------------------------------------------------------------------------------------------------|\n\n");
}

/*Function: print_dirs
Output of the entire list of producers.
dirs - the "head" of the producer list.*/

void print_dirs(head_producers *dirs) {
    producer *node;
    int i;
    node = dirs->first;
    printf("\n%35s", "List of all directors:");
    printf("\n|------------------------------------------|\n");
    printf("|%3s|%22s     |%8s  |\n", "id", "Director's name", "movies");
    for (i = 0; i < dirs->count; i++) {
        printf("|---+---------------------------+----------|\n");
        printf("|%3d|%27s|%10d|\n", node->id, node->fm, node->films);
        node = node->next;
    }
    printf("|------------------------------------------|\n\n");
}

/*Function: save_list
Saving a list of movies to a file.
h_f - the "head" of the movie list.*/

void save_list(head_films *h_f) {
    FILE *f;
    list *node;
    int i;
    char file_name[MAXLEN];
    node = h_f->first;
    getchar();
    printf("\nOk!\n\nEnter the name of the file to which you want to write: ");
    fgets(file_name, MAXLEN, stdin);
    file_name[strlen(file_name) - 1] = '\0';
    if ((f = fopen(file_name, "w")) != NULL) {
        printf("\nThe file for reading has been opened successfully.\n");
        for (i = 0; i < h_f->count; i++) {                /*line-by-line entry of the list to the specified file*/
            fprintf(f, "%s;%s;%d;%.1f;%d;%.3f;%d;%d;%d\n", node->name, node->director->fm, node->length, node->rating, node->reviews, node->fees,
                node->premiere[0], node->premiere[1], node->premiere[2]);
            node = node->next;
        }
        printf("\nThe list was successfully written to a file '%s'!\n", file_name);
        if (fclose(f) == EOF) printf("Error closing the file!\n");
        else printf("\nClosing OK\n");
    } else puts("\nError opening the file!\n");
}
