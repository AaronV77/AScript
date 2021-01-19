#include "env_linked_list.h"

int env_debugger_flag = 0;

void env_ll_pop(Env_Linked_List ** list, int position) {
    if (env_debugger_flag) printf("Entering the vec_pop function\n");

    if ((*list)) {
        if (position > ((*list)->list_length - 1) || position < 0) {
            printf("ERROR: The position given is either greater than the list size or smaller than zero.\n");
            return;
        }

        int list_iterator = 0;
        Env_Linked_List_Node * past;
        Env_Linked_List_Node * current = (*list)->front;

        while (current) {
            if (list_iterator == position) {
                if (list_iterator == 0) {
                    // Remove the leading node.
                    (*list)->front = current->next;
                    free(current->path);
                    free(current);
                    break;
                } else if (list_iterator == ((*list)->list_length - 1)) {
                    // Removing the last node.
                    (*list)->rear = past;
                    past->next = NULL;
                    free(current->path);
                    free(current);
                    break;
                } else {
                    // Removing a middle node.
                    past->next = current->next;
                    free(current->path);
                    free(current);
                    break;
                }
            }
            past = current;
            current = current->next;
            list_iterator++;
        }
    } else {
        printf("WARNING: You passed in a empty environment list...\n");
    }

    if (env_debugger_flag) printf("Leaving the vec_pop function\n");
    return;
}

void env_ll_push(Env_Linked_List ** list, char * incoming_string) {
    if (env_debugger_flag) printf("Entering the vec_push function\n");

    Env_Linked_List_Node * temp = calloc(1, sizeof(Env_Linked_List_Node));
    temp->path = calloc(strlen(incoming_string) + 1, sizeof(char));
    strcpy(temp->path, incoming_string);
    temp->next = NULL;

    if (!*list) {
        *list = calloc(1, sizeof(Env_Linked_List));
        (*list)->front = (*list)->rear = temp;
    } else {
        (*list)->rear->next = temp;
        (*list)->rear = temp;
    }
    (*list)->list_length++;

    if (env_debugger_flag) printf("Leaving the vec_push function\n");
    return;
}

void env_ll_cleanup(Env_Linked_List ** list) {
    if (env_debugger_flag) printf("Entering the vec_cleanup function\n");

    if ((*list)) {
        if ((*list)->list_length > 0) {
            int iterator = 0;
            Env_Linked_List_Node * temp;
            while ((*list)->front) {
                temp = (*list)->front->next;
                free((*list)->front->path);
                free((*list)->front);
                (*list)->front = temp;
                iterator++;
            }
            free(*list);
        }
    } else {
        printf("WARNING: You passed in a empty environment list...\n");
    }
    
    if (env_debugger_flag) printf("Leaving the vec_cleanup function\n");
    return;
}

void env_ll_listall(Env_Linked_List * list) {
    if (env_debugger_flag) printf("Entering the vec_listall function\n");

    if (list) {
        if (!list->front) {
            printf("ERROR: The linked list is empty..\n");
            return;
        }

        Env_Linked_List_Node * temp = list->front;
        while (temp) {
            printf("%s\n", temp->path);
            temp = temp->next;
        }
    } else {
        printf("WARNING: You passed in a empty list...\n");
    }
    
    if (env_debugger_flag) printf("Leaving the vec_listall function\n");
    return;
}

void env_ll_debugger() {
    env_debugger_flag = 1;
    return;
}