#include "sd_array.h"

int sd_debugger_flag = 1;

int sd_column_reallocation(sd_array ** array, int new_col_size) {

	if (sd_debugger_flag) printf("Entering the td_column_reallocation function.\n");

    int current_size = sizeof((*array)->array);
    printf("Current size: %d\n", current_size);

    if (!strcmp((*array)->format, "char")) {
        // There is a conditional jump if the next line runs. 
        // I'm curious if the linked list or td_array has this same problem.
        (*array)->array = realloc((*array)->array, new_col_size * sizeof(char));
        if (!(char*)(*array)->array) {
            printf("ERROR: There was an issue with the reallocation of your pointer.\n");
            free((char*)(*array)->array);
            return 1;
        }	
    } else { 
        printf("ERROR: The format is not supported in the td_column_reallocation function...\n");
        return 1;
    }

	if (sd_debugger_flag) printf("Leaving the td_column_reallocation function.\n");

	return 0;
}

int sd_allocation(sd_array ** array, char * format, int number_of_columns, int col_incrementation) {

	if (sd_debugger_flag) printf("Entering the sd_allocation.\n");

	if ((*array)->array) {
		printf("ERROR: The passed in sd_array structure was not empty...\n");
		return 1;
	}

	int error_flag = 0;
	(*array)->total_num_cols = number_of_columns;
    
	if (col_incrementation <= 0)
		(*array)->col_incrementation = 5;
	else
		(*array)->col_incrementation = col_incrementation;

	(*array)->format = calloc(strlen(format) + 1, sizeof(char));
	strcpy((*array)->format, format);
    
	if (!strcmp(format, "char")) {
		(*array)->array = calloc(number_of_columns, sizeof(char));
		if (!(*array)->array) {
			printf("ERROR: Calloc has failed...\n");
			if (sd_debugger_flag) printf("Leaving the td_allocation function.\n");
			return 0;
		}
	} else {
		printf("ERROR: The given data type is not supported...\n");
		if (sd_debugger_flag) printf("Leaving the td_allocation function.\n");
		return 0;
	}
	
	if (sd_debugger_flag) printf("Leaving the td_allocation function.\n");
	
	return 1;
}

int sd_add(sd_array ** array, const char * format, ...) {

	if (sd_debugger_flag) printf("Entering the td_push function.\n");

	va_list arguments;
	va_start(arguments, format);

	void * vptr;

    // Grab the characters that are to be added to the end of a string.
	if (!strcmp(format, "char")) {
        char * var = calloc(2, sizeof(char));
		var[0] = va_arg(arguments, int);
        vptr = var;
    } else if (!strcmp(format, "string*")) {
        char ** incoming = va_arg(arguments, char**);
        vptr = incoming[0];
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
		if (sd_debugger_flag) printf("Leaving the td_push funtion.\n");
		va_end(arguments);
		return 1;
    }

    if ((*array)->current_num_col == (*array)->total_num_cols) {
        int temp = (*array)->total_num_cols + (*array)->col_incrementation * 4;
        sd_column_reallocation(array, temp);
        (*array)->total_num_cols = temp;
    }

    if (!strcmp(format, "string") || !strcmp(format, "string*")) {
        int temp = (*array)->current_num_col + strlen(vptr);
        if ((*array)->total_num_cols < temp) {
            temp += (*array)->col_incrementation;
            sd_column_reallocation(array, temp);
        }
        (*array)->total_num_cols = temp;
    }
    
	if (!strcmp((*array)->format, "char")) {
        char * number = NULL;
        if (!strcmp(format, "double") || !strcmp(format, "float")) {
            number = calloc(256, sizeof(char));
            sprintf(number, "%f", ((double*)vptr)[0]);
            printf("Here: %s\n", number);          
            for (int i = 0; i < strlen(number); ++i, (*array)->current_num_col++) {
                ((char*)(*array)->array)[(*array)->current_num_col] = number[i];
            }
        } else if (!strcmp(format, "int")) {
            number = calloc(256, sizeof(char));
            sprintf(number, "%d", ((int*)vptr)[0]);
            for (int i = 0; i < strlen(number); ++i, (*array)->current_num_col++) {
                ((char*)(*array)->array)[(*array)->current_num_col] = number[i];
            }
        } else {
            for (int i = 0; i < strlen((char*)vptr); ++i, (*array)->current_num_col++)
                ((char*)(*array)->array)[(*array)->current_num_col] = ((char*)vptr)[i];
        }
        if (number)
            free(number);

 	} else {
		printf("ERROR: The given data type is not supported...\n");
		if (sd_debugger_flag) printf("Leaving the td_push funtion.\n");
		va_end(arguments);
		return 1;
     }

	va_end(arguments);

    if (strcmp(format, "string"))
        free(vptr);

	if (sd_debugger_flag) printf("Leaving the td_push funtion.\n");

	return 0;
}

int sd_free(sd_array ** array) {

	if (sd_debugger_flag) printf("Entering the td_free function.\n");

    free((*array)->array);
	free((*array)->format);

	if (sd_debugger_flag) printf("Leaving the td_free funtion\n");

	return 0;
}

void sd_debugger() {
	sd_debugger_flag = 1;
	return;
}
