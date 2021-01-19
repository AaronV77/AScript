#ifndef ENV_LINKED_L
#define ENV_LINKED_L

#include <stdio.h>      // printf
#include <stdlib.h>     // calloc
#include <string.h>     // strcpy

struct Env_Linked_List_Node { 
    char * path;
    struct Env_Linked_List_Node * next;
};

struct Env_Linked_List {
    int list_length;
    struct Env_Linked_List_Node * front, * rear;
};

typedef struct Env_Linked_List Env_Linked_List;
typedef struct Env_Linked_List_Node Env_Linked_List_Node;

void env_ll_pop(Env_Linked_List ** list, int position);
void env_ll_push(Env_Linked_List ** list, char * incoming_string);
void env_ll_cleanup(Env_Linked_List ** list);
void env_ll_listall(Env_Linked_List * list);
void env_ll_debugger();

#endif