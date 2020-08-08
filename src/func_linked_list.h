#ifndef FUNC_LINKED_L
#define FUNC_LINKED_L

#include "td_array.h"

struct Func_Linked_List_Node { 
    char * name;
    char * datatype;
    char * file_location;
    int number_of_arguments;
    int function_line_number;
    td_array * arguments;
    struct Func_Linked_List_Node * next;
};

struct Func_Linked_List {
    int list_length;
    struct Func_Linked_List_Node * front, * rear;
};

typedef struct Func_Linked_List Func_Linked_List;
typedef struct Func_Linked_List_Node Func_Linked_List_Node;

void func_ll_pop(Func_Linked_List ** list, int position);
void func_ll_push(Func_Linked_List ** list, char * item_name, char * datatype, char * file_location, int function_line_number);
void func_ll_argument_push(Func_Linked_List ** list, char * item_name, char * argument);
Func_Linked_List_Node * func_ll_search(Func_Linked_List * list, char * item_name);
void func_ll_cleanup(Func_Linked_List ** list);
void func_ll_debugger();

#endif