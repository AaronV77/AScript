#ifndef LINKED_L
#define LINKED_L

#include <stdio.h>      // printf
#include <stdlib.h>     // calloc
#include <string.h>     // strcpy
#include <stdarg.h>

struct Linked_List_Node { 
    void * list_item;
    char * format;
    int item_size;
    struct Linked_List_Node * next;
};

struct Linked_List {
    int list_length;
    struct Linked_List_Node * front, * rear;
};

typedef struct Linked_List Linked_List;
typedef struct Linked_List_Node Linked_List_Node;

void vec_pop(Linked_List ** list, int position);
void vec_push(Linked_List ** list, int data_size, const char * format, ...);
void vec_cleanup(Linked_List ** list);
void vec_listall(Linked_List * list);
void simple_vector_debugger();

#endif