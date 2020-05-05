#include "string.h"

int string_debugger_flag = 0;

void trim_number(char ** string) {

    for (int i = strlen(*string); i > 0; --i) {
        if ((*string)[i] > 32 && (*string)[i] != 127) {
            if ((*string)[i - 1] == 46 || ((*string)[i] < 47 && (*string)[i] > 49))
                break;
            else
                (*string)[i] = '\0';
        }
    }

    return;
}

int column_reallocation(string ** array, int new_col_size) {

	if (string_debugger_flag) printf("Entering the column_reallocation function.\n");

    char * copy = calloc(strlen((*array)->array) + 1, sizeof(char));
    strcpy(copy, (*array)->array);

    (*array)->array = realloc((*array)->array, new_col_size * sizeof(char));
    if (!(char*)(*array)->array) {
        printf("ERROR: There was an issue with the reallocation of your pointer.\n");
        free((char*)(*array)->array);
        return 1;
    }	

    memset((*array)->array, 0, new_col_size * sizeof(char));
    strcpy((*array)->array, copy);
    free(copy);

	if (string_debugger_flag) printf("Leaving the column_reallocation function.\n");

	return 0;
}

int salloc(string ** array, int number_of_columns, int col_incrementation) {

	if (string_debugger_flag) printf("Entering the salloc.\n");

    // Rather than checking to see if the memory pointer that is getting passed in, I will just allocate to it no matter what.
    // Your application will be leaking if you don't send in a fresh or clean array pointer. 

	int error_flag = 0;
	(*array)->total_num_cols = number_of_columns;
    
	if (col_incrementation <= 0)
		(*array)->col_incrementation = 5;
	else
		(*array)->col_incrementation = col_incrementation;
    
    (*array)->array = calloc(number_of_columns, sizeof(char));
    if (!(*array)->array) {
        printf("ERROR: Calloc has failed...\n");
        if (string_debugger_flag) printf("Leaving the salloc function.\n");
        return 0;
    }
	
	if (string_debugger_flag) printf("Leaving the salloc function.\n");
	
	return 1;
}

int sadd(string ** array, const char * format, ...) {

	if (string_debugger_flag) printf("Entering the sadd function.\n");

	va_list arguments;
	va_start(arguments, format);

	void * vptr;

    // Grab the characters that are to be added to the end of a string.
	if (!strcmp(format, "char")) {
        char * var = calloc(2, sizeof(char));
		var[0] = va_arg(arguments, int);
        if (var[0] == 32) (*array)->number_of_spaces++;
        vptr = var;
    } else if (!strcmp(format, "string")) {
        char * string = va_arg(arguments, char*);
        char * string2 = calloc(strlen(string) + 1, sizeof(char));
        strncpy(string2, string, strlen(string));
        vptr = string2;
    } else if (!strcmp(format, "double") || !strcmp(format, "float")) {
        double var2 = va_arg(arguments, double);
        double * var = calloc(1, sizeof(double));
        var[0] = var2; 
        vptr = var;
    } else if (!strcmp(format, "int")) {
        int var2 = va_arg(arguments, int);
        int * var = calloc(1, sizeof(int));
        var[0] = var2; 
        vptr = var;
 	} else {
		printf("ERROR: The given data type is not supported...\n");
		if (string_debugger_flag) printf("Leaving the sadd funtion.\n");
		va_end(arguments);
		return 1;
    }

    if (!strcmp(format, "double") || !strcmp(format, "float") || !strcmp(format, "int")) {
        char * number = calloc(256, sizeof(char));
        if (!strcmp(format, "double") || !strcmp(format, "float")) {
            sprintf(number, "%f", ((double*)vptr)[0]);   
            trim_number(&number);
        } else { sprintf(number, "%d", ((int*)vptr)[0]); }
        free(vptr);
        vptr = strdup(number);
        free(number);
    }

    // At this point vptr should all be a char pointer
    int temp = (*array)->current_num_col + strlen((char*)vptr);
    if ((*array)->total_num_cols < temp) {
        temp += (*array)->col_incrementation;
        column_reallocation(array, temp);
    }
    (*array)->total_num_cols = temp;

    for (int i = 0; i < strlen((char*)vptr); ++i, (*array)->current_num_col++)
        ((char*)(*array)->array)[(*array)->current_num_col] = ((char*)vptr)[i];

	va_end(arguments);
    free(vptr);

	if (string_debugger_flag) printf("Leaving the sadd funtion.\n");

	return 0;
}

int sclear(string ** array, int number_of_columns, int col_incrementation) {
    
    if (string_debugger_flag) printf("Entering the sclear function.\n");
    
    if (number_of_columns) {
        (*array)->col_incrementation = col_incrementation;
        (*array)->array = realloc((*array)->array, number_of_columns * sizeof(char));
        if (!(*array)->array) {
            printf("ERROR: There was an issue with reallocating the given pointer.\n");
            if (string_debugger_flag) printf("Leaving the sclear function.\n");
            return 1;
        }
    }
    memset((*array)->array, 0, sizeof((*array)->array));
    
    (*array)->current_num_col = 0;
    (*array)->number_of_spaces = 0;
    
    if (string_debugger_flag) printf("Leaving the sclear function.\n");

    return 0;
}

int srspaces(string ** array) {
    
    int temp_iterator = 0;
    int size = strlen((*array)->array) - (*array)->number_of_spaces + 1;
    char * copy = calloc(size, sizeof(char));

    for (int i = 0; i < strlen((*array)->array); ++i) {
        if ((*array)->array[i] != 32) {
            copy[temp_iterator] = (*array)->array[i];
            temp_iterator++;
        }
    }
    memset((*array)->array, 0, sizeof((*array)->array));
    strcpy((*array)->array, copy);
    free(copy);

    return 0;
}

int sfree(string ** array) {

	if (string_debugger_flag) printf("Entering the sfree function.\n");

    free((*array)->array);

	if (string_debugger_flag) printf("Leaving the sfree funtion\n");

	return 0;
}

void string_debugger() {
	string_debugger_flag = 1;
	return;
}
