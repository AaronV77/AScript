#include "var_linked_list.h"

int var_debugger_flag = 0;

void var_ll_pop(Var_Linked_List ** list, int position) {
    if (var_debugger_flag) printf("Entering the vec_pop function\n");

    if ((*list)) {
        if (position > ((*list)->list_length - 1) || position < 0) {
            printf("ERROR: The position given is either greater than the list size or smaller than zero.\n");
            return;
        }

        int list_iterator = 0;
        Var_Linked_List_Node * past;
        Var_Linked_List_Node * current = (*list)->front;

        while (current) {
            if (list_iterator == position) {
                if (list_iterator == 0) {
                    // Remove the leading node.
                    (*list)->front = current->next;
                    free(current->data);
                    free(current->variable_name);
                    free(current->variable_data_type);
                    free(current);
                    break;
                } else if (list_iterator == ((*list)->list_length - 1)) {
                    // Removing the last node.
                    (*list)->rear = past;
                    past->next = NULL;
                    free(current->data);
                    free(current->variable_name);
                    free(current->variable_data_type);
                    free(current);
                    break;
                } else {
                    // Removing a middle node.
                    past->next = current->next;
                    free(current->data);
                    free(current->variable_name);
                    free(current->variable_data_type);
                    free(current);
                    break;
                }
            }
            past = current;
            current = current->next;
            list_iterator++;
        }
    } else {
        printf("WARNING: You passed in a empty variable list...\n");
    }

    if (var_debugger_flag) printf("Leaving the vec_pop function\n");
    return;
}

void var_ll_push(Var_Linked_List ** list, char * variable_name, char * data_type, char * data) {
    if (var_debugger_flag) printf("Entering the vec_push function\n");

    Var_Linked_List_Node * temp = calloc(1, sizeof(Var_Linked_List_Node));
    
    if (data_type)
        temp->data = strdup(data);
    
    temp->variable_name = calloc(strlen(variable_name) + 1, sizeof(char));
    if (data_type) {
        temp->variable_data_type = calloc(strlen(data_type) + 1, sizeof(char));
        strcpy(temp->variable_data_type, data_type);
    } else {
        temp->variable_data_type = NULL;
        temp->data = NULL;
    }
    
    strcpy(temp->variable_name, variable_name);
    temp->next = NULL;

    if (!*list) {
        *list = calloc(1, sizeof(Var_Linked_List));
        (*list)->front = (*list)->rear = temp;
    } else {
        (*list)->rear->next = temp;
        (*list)->rear = temp;
    }
    (*list)->list_length++;

    if (var_debugger_flag) printf("Leaving the vec_push function\n");
    return;
}

void var_ll_update_value(Var_Linked_List ** list, char * variable_name, char * data_type, char * data) {

    if (var_debugger_flag) printf("Entering the func_var_search function\n");
    
    if ((*list)) {
        Var_Linked_List_Node * temp = (*list)->front;
        while (temp) {
            if (!strcmp(temp->variable_name, variable_name)) {
                temp->variable_data_type = calloc(strlen(data_type) + 1, sizeof(char));
                strcpy(temp->variable_data_type, data_type);

                if (!strcmp(data_type, "char") || !strcmp(data_type, "string")) {
                    if (!strcmp(data_type, "char") || !strcmp(data_type, "string")) {
                        temp->data = strdup(data);
                    } else
                        printf("ERROR: The variable you are updating does not have the same data type.\n");
                } else if (!strcmp(data_type, "int")) {
                    if (!strcmp(temp->variable_data_type, "int")) {
                        temp->data = strdup(data);
                    } else
                        printf("ERROR: The variable you are updating does not have the same data type.\n");                        
                } else if (!strcmp(data_type, "double") || !strcmp(data_type, "float")) {
                    if (!strcmp(temp->variable_data_type, "double") || !strcmp(temp->variable_data_type, "float")) {
                        temp->data = strdup(data);
                    } else
                        printf("ERROR: The variable you are updating does not have the same data type.\n");                                           
                } else if (!strcmp(data_type, "NULL")) {
                    temp->data = NULL;
                } else {
                    printf("ERROR: That datatype is not supported...\n");
                    return;
                }
            }
            temp = temp->next;
        }
    } else {
        printf("WARNING: You passed in a empty variable list...\n");
    }

    if (var_debugger_flag) printf("Leaving the func_var_search function\n");

    return;
}

Var_Linked_List_Node * var_ll_search(Var_Linked_List * list, char * variable_name) {

    if (var_debugger_flag) printf("Entering the func_var_search function\n");

    if (list) {
        Var_Linked_List_Node * temp = list->front;
        while (temp) {
            if (!strcmp(temp->variable_name, variable_name))
                return temp;
            temp = temp->next;
        }
    } else {
        printf("WARNING: You passed in a empty variable list...\n");
    }

    if (var_debugger_flag) printf("Leaving the func_var_search function\n");

    return NULL;
}

void var_ll_cleanup(Var_Linked_List ** list) {
    if (var_debugger_flag) printf("Entering the vec_cleanup function\n");

    if ((*list)) {
        if ((*list)->list_length > 0) {
            int iterator = 0;
            Var_Linked_List_Node * temp;
            while ((*list)->front) {
                temp = (*list)->front->next;
                free((*list)->front->data);
                free((*list)->front->variable_name);
                free((*list)->front->variable_data_type);
                free((*list)->front);
                (*list)->front = temp;
                iterator++;
            }
            free(*list);
        }
    } else {
        printf("WARNING: You passed in a empty variable list...\n");;
    }
    
    if (var_debugger_flag) printf("Leaving the vec_cleanup function\n");
    return;
}

void var_ll_listall(Var_Linked_List * list) {
    if (var_debugger_flag) printf("Entering the vec_listall function\n");

    if (list) {
        if (!list->front) {
            printf("ERROR: The linked list is empty..\n");
            return;
        }

        Var_Linked_List_Node * temp = list->front;
        while (temp) {
            printf("Name: %s and value: %s\n", temp->variable_name, temp->data);
            temp = temp->next;
        }
    } else {
        printf("WARNING: You passed in a empty variable list...\n");
    }
    
    if (var_debugger_flag) printf("Leaving the vec_listall function\n");
    return;
}

void var_ll_debugger() {
    var_debugger_flag = 1;
    return;
}