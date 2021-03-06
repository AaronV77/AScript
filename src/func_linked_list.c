#include "func_linked_list.h"

int func_debugger_flag = 0;

void func_ll_pop(Func_Linked_List ** list, int position) {
    if (func_debugger_flag) printf("Entering the func_var_pop function\n");

    if ((*list)) {
        if (position > ((*list)->list_length - 1) || position < 0) {
            printf("ERROR: The position given is either greater than the list size or smaller than zero.\n");
            return;
        }

        int list_iterator = 0;
        Func_Linked_List_Node * past;
        Func_Linked_List_Node * current = (*list)->front;

        while (current) {
            if (list_iterator == position) {
                if (list_iterator == 0) {
                    // Remove the leading node.
                    (*list)->front = current->next;
                    free(current->function_name);
                    free(current->function_return_type);
                    td_free(&(current->arguments));
                    free(current->arguments);
                    free(current);
                    break;
                } else if (list_iterator == ((*list)->list_length - 1)) {
                    // Removing the last node.
                    (*list)->rear = past;
                    past->next = NULL;
                    free(current->function_name);
                    free(current->function_return_type);
                    td_free(&(current->arguments));
                    free(current->arguments);
                    free(current);
                    break;
                } else {
                    // Removing a middle node.
                    past->next = current->next;
                    free(current->function_name);
                    free(current->function_return_type);
                    td_free(&(current->arguments));
                    free(current->arguments);
                    free(current);
                    break;
                }
            }

            past = current;
            current = current->next;
            list_iterator++;
        }
    } else {
        printf("WARNING: You passed in a empty functions list...\n");
    }

    if (func_debugger_flag) printf("Leaving the func_var_pop function\n");
    return;
}

void func_ll_push(Func_Linked_List ** list, char * function_name, char * function_return_type, char * file_location, int function_line_number) {
    if (func_debugger_flag) printf("Entering the func_var_push function\n");

    Func_Linked_List_Node * temp = calloc(1, sizeof(Func_Linked_List_Node));

    temp->function_name = calloc(strlen(function_name) + 1, sizeof(char));
    temp->function_return_type = calloc(strlen(function_return_type) + 1, sizeof(char));
    temp->file_location = calloc(strlen(file_location) + 1, sizeof(char));

    strcpy(temp->function_name, function_name);
    strcpy(temp->function_return_type, function_return_type);
    strcpy(temp->file_location, file_location);
    temp->function_line_number = function_line_number;
    temp->next = NULL;

    temp->arguments = calloc(1, sizeof(td_array));
    temp->arguments->array = NULL;
    temp->number_of_arguments = 0;
    td_allocation(&temp->arguments, "char", 5, 5, 0);

    if (!*list) {
        *list = calloc(1, sizeof(Func_Linked_List));
        (*list)->front = (*list)->rear = temp;
    } else {
        (*list)->rear->next = temp;
        (*list)->rear = temp;
    }
    (*list)->list_length++;

    if (func_debugger_flag) printf("Leaving the func_var_push function\n");
    return;
}

void func_ll_argument_push(Func_Linked_List ** list, char * function_name, char * argument) {
    
    if (func_debugger_flag) printf("Entering the func_var_argument_push function\n");

    if ((*list)) {
        Func_Linked_List_Node * temp = (*list)->front;
        while (temp) {
            if (!strcmp(temp->function_name, function_name)) {
                td_push(&temp->arguments, "char", &argument);
                temp->number_of_arguments++;
            }
            temp = temp->next;
        }
    } else {
        printf("WARNING: You passed in a empty functions list...\n");
    }

    if (func_debugger_flag) printf("Leaving the func_var_argument_push function\n");
    
    return;
}

Func_Linked_List_Node * func_ll_search(Func_Linked_List * list, char * function_name) {

    if (func_debugger_flag) printf("Entering the func_var_search function\n");

    if (list) {
        Func_Linked_List_Node * temp = list->front;
        while (temp) {
            if (!strcmp(temp->function_name, function_name)) {
                return temp;
            }
            temp = temp->next;
        }
    } else {
        printf("WARNING: You passed in a empty list...\n");
    }

    if (func_debugger_flag) printf("Leaving the func_var_search function\n");

    return NULL;
}

void func_ll_cleanup(Func_Linked_List ** list) {
    if (func_debugger_flag) printf("Entering the func_var_cleanup function\n");

    if ((*list)) {
        if ((*list)->list_length > 0) {
            Func_Linked_List_Node * temp;
            while ((*list)->front) {
                temp = (*list)->front->next;
                free((*list)->front->function_name);
                free((*list)->front->function_return_type);
                free((*list)->front->file_location);
                td_free(&((*list)->front->arguments));
                free((*list)->front->arguments);
                free((*list)->front);
                (*list)->front = temp;
            }
            free(*list);
        }
    } else {
        printf("WARNING: You passed in a empty functions list...\n");
    }

    if (func_debugger_flag) printf("Leaving the func_var_cleanup function\n");
    return;
}

void func_ll_debugger() {
    func_debugger_flag = 1;
    return;
}