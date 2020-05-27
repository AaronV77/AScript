#include "linked_list.h"

int sm_debugger_flag = 0;

void vec_pop(Linked_List ** list, int position) {
    if (sm_debugger_flag) printf("Entering the vec_pop function\n");

    if (position > ((*list)->list_length - 1) || position < 0) {
        printf("ERROR: The position given is either greater than the list size or smaller than zero.\n");
        return;
    }

    int list_iterator = 0;
    Linked_List_Node * past;
    Linked_List_Node * current = (*list)->front;

    while (current) {
        
        if (list_iterator == position) {
            if (list_iterator == 0) {
                // Remove the leading node.
                (*list)->front = current->next;
                free(current->list_item);
                free(current);
                break;
            } else if (list_iterator == ((*list)->list_length - 1)) {
                // Removing the last node.
                (*list)->rear = past;
                past->next = NULL;
                free(current->list_item);
                free(current);
                break;
            } else {
                // Removing a middle node.
                past->next = current->next;
                free(current->list_item);
                free(current);
                break;
            }
        }

        past = current;
        current = current->next;
        list_iterator++;
    }

    if (sm_debugger_flag) printf("Leaving the vec_pop function\n");
    return;
}

void vec_push(Linked_List ** list, int array_size, const char * format, ...) {
    if (sm_debugger_flag) printf("Entering the vec_push function\n");

    if (array_size == 0) {
        printf("ERROR: The datasize that was given cannot be zero...\n");
        return;
    }

	va_list arguments;
	va_start(arguments, format);

    Linked_List_Node * temp = calloc(1, sizeof(Linked_List_Node));
    if (!strcmp(format, "string*")) {
        char ** incoming = va_arg(arguments, char**);
        temp->list_item = incoming[0];
    } else if (!strcmp(format, "string")) {
        char * string = va_arg(arguments, char*);
        char * string2 = calloc(array_size + 1, sizeof(char));
        strncpy(string2, string, array_size);
        temp->list_item = string2;
    } else if (!strcmp(format, "char")) {
        if (array_size > 1) {
            printf("ERROR: There is an issue with the array size and the variable type..\n");
            return;
        }
        char incoming = va_arg(arguments, int);
        char * incoming2 = calloc(2, sizeof(char));
        incoming2[0] = incoming;
        incoming2[1] = '\0';
        temp->list_item = incoming2;
    } else if (!strcmp(format, "int")) {
        if (array_size > 1) {
            int * numbers = va_arg(arguments, int*);
            int * numbers2 = calloc(array_size, sizeof(int));
            memcpy(numbers2, numbers, sizeof(int) * array_size);
            temp->list_item = numbers2;
        } else {
            int incoming = va_arg(arguments, int);
            int * incoming2 = calloc(1, sizeof(int));
            incoming2[0] = incoming; 
            temp->list_item = incoming2;
        }
    } else if (!strcmp(format, "int*")) {
            int ** incoming = va_arg(arguments, int**);
            temp->list_item = incoming[0];
    } else if (!strcmp(format, "double")) {
        if (array_size > 1) {
            double * numbers = va_arg(arguments, double*);
            double * numbers2 = calloc(array_size, sizeof(double));
            memcpy(numbers2, numbers, sizeof(double) * array_size);
            temp->list_item = numbers2;
        } else {
            double incoming = va_arg(arguments, double);
            double * incoming2 = calloc(1, sizeof(double));
            incoming2[0] = incoming; 
            temp->list_item = incoming2;
        }
    } else if (!strcmp(format, "double*")) {
            double ** incoming = va_arg(arguments, double**);
            temp->list_item = incoming[0];
    } else if (!strcmp(format, "float")) {
        if (array_size > 1) {
            float * numbers = va_arg(arguments, float*);
            float * numbers2 = calloc(array_size, sizeof(float));
            memcpy(numbers2, numbers, sizeof(float) * array_size);
            temp->list_item = numbers2;
        } else {
            float incoming = va_arg(arguments, double);
            float * incoming2 = calloc(1, sizeof(float));
            incoming2[0] = incoming; 
            temp->list_item = incoming2;
        }
    } else if (!strcmp(format, "float*")) {
        float ** incoming = va_arg(arguments, float**);
        temp->list_item = incoming[0];
    } else {
        printf("ERROR: That datatype is not supported...\n");
        return;
    }

    temp->format = calloc(10, sizeof(char));
    strcpy(temp->format, format);
    temp->item_size = array_size;
    temp->next = NULL;

    if (!*list) {
        *list = calloc(1, sizeof(Linked_List));
        (*list)->front = (*list)->rear = temp;
    } else {
        (*list)->rear->next = temp;
        (*list)->rear = temp;
    }
    (*list)->list_length++;

	va_end(arguments);

    if (sm_debugger_flag) printf("Leaving the vec_push function\n");
    return;
}

void vec_cleanup(Linked_List ** list) {
    if (sm_debugger_flag) printf("Entering the vec_cleanup function\n");

    if ((*list)) {
        if ((*list)->list_length > 0) {
            int iterator = 0;
            Linked_List_Node * temp;
            while ((*list)->front) {
                temp = (*list)->front->next;
                free((*list)->front->list_item);
                free((*list)->front->format);
                free((*list)->front);
                (*list)->front = temp;
                iterator++;
            }
            free(*list);
        }
    }
    
    if (sm_debugger_flag) printf("Leaving the vec_cleanup function\n");
    return;
}

void vec_listall(Linked_List * list) {
    if (sm_debugger_flag) printf("Entering the vec_listall function\n");

    if (!list->front) {
        printf("ERROR: The linked list is empty..\n");
        return;
    }

    Linked_List_Node * temp = list->front;
    while (temp) {
        if (!strcmp(temp->format, "string") || !strcmp(temp->format, "string*") || !strcmp(temp->format, "char")) {
            printf("%s\n", temp->list_item);
        } else if (!strcmp(temp->format, "int") || !strcmp(temp->format, "int*")) {
            if (temp->item_size == 1) {
                printf("%d\n", *(int*)temp->list_item);
            } else {
                int * data = temp->list_item;
                for (int i = 0; i < temp->item_size; ++i)
                    printf("%d ",data[i]);
                printf("\n");
            }
        } else if (!strcmp(temp->format, "double") || !strcmp(temp->format, "doulbe*")) {
            if (temp->item_size == 1) {
                printf("%f\n", *(double*)temp->list_item);
            } else {
                double * data = temp->list_item;
                for (int i = 0; i < temp->item_size; ++i)
                    printf("%f ",data[i]);
                printf("\n");
            }
        } else if (!strcmp(temp->format, "float") || !strcmp(temp->format, "float*")) {
            if (temp->item_size == 1) {
                printf("%f\n", *(float*)temp->list_item);
            } else {
                float * data = temp->list_item;
                for (int i = 0; i < temp->item_size; ++i)
                    printf("%f ",data[i]);
                printf("\n");
            }
        }
        temp = temp->next;
    }

    if (sm_debugger_flag) printf("Leaving the vec_listall function\n");
    return;
}

void simple_vector_debugger() {
    sm_debugger_flag = 1;
    return;
}