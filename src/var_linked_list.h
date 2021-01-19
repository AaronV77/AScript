#ifndef VAR_LINKED_L
#define VAR_LINKED_L

#include <stdio.h>      // printf
#include <stdlib.h>     // calloc
#include <string.h>     // strcpy

struct Var_Linked_List_Node { 
    char * variable_name;
    char * variable_data_type;
    char * data;
    struct Var_Linked_List_Node * next;
};

struct Var_Linked_List {
    int list_length;
    struct Var_Linked_List_Node * front, * rear;
};

typedef struct Var_Linked_List Var_Linked_List;
typedef struct Var_Linked_List_Node Var_Linked_List_Node;

void var_ll_pop(Var_Linked_List ** list, int position);
void var_ll_push(Var_Linked_List ** list, char * variable_name, char * data_type, char * data);
void var_ll_update_value(Var_Linked_List ** list, char * variable_name, char * data_type, char * data);
Var_Linked_List_Node * var_ll_search(Var_Linked_List * list, char * item_name);
void var_ll_cleanup(Var_Linked_List ** list);
void var_ll_listall(Var_Linked_List * list);
void var_ll_debugger();

#endif