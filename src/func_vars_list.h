#ifndef FUNC_VAR_L
#define FUNC_VAR_L

#include "td_array.h"

struct Func_Var_List_Node { 
    char * name;
    char * datatype;
    char * file_location;
    int number_of_arguments;
    int function_line_number;
    td_array * arguments;
    struct Func_Var_List_Node * next;
};

struct Func_Var_List {
    int list_length;
    struct Func_Var_List_Node * front, * rear;
};

typedef struct Func_Var_List Func_Var_List;
typedef struct Func_Var_List_Node Func_Var_List_Node;

void func_var_pop(Func_Var_List ** list, int position);
void func_var_push(Func_Var_List ** list, char * item_name, char * datatype, char * file_location, int function_line_number);
void func_var_argument_push(Func_Var_List ** list, char * item_name, char * argument);
int func_var_search(Func_Var_List * list, char * item_name);
void func_var_cleanup(Func_Var_List ** list);
void func_var_debugger();

#endif