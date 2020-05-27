#include "td_array.h"

int td_debugger_flag = 0;

int td_row_reallocation(td_array ** array, int new_row_size) {

	if (td_debugger_flag) printf("Entering the td_row_reallocation function.\n");

	int error_flag = 0;

	(*array)->array = realloc((*array)->array, new_row_size * sizeof(char *));

	if ((*array)->array) {
		if ((*array)->total_num_cols) {
			for (int i = (*array)->current_num_rows; i < new_row_size; ++i) {
				if (!strcmp((*array)->format, "char")) {
					((char**)(*array)->array)[i] = calloc((*array)->total_num_cols, sizeof(char));
					if (!((char**)(*array)->array)[i]) {
						error_flag = i;
						break;
					}					
				} else {
					printf("ERROR: The given data type is not supported...\n");
					if (td_debugger_flag) printf("Leaving the td_row_reallocation function.\n");
					return 1;
				}
			}	
			
			if (error_flag) {
				printf("ERROR: Calloc has failed...\n");
				for (int i = 0; i < error_flag; ++i)
					free(((char**)(*array)->array)[i]);
				free((*array)->array);
				if (td_debugger_flag) printf("Leaving the td_row_reallocation function.\n");
				return 1;
			}
		}
	} else {
		printf("ERROR: Realloc has failed...\n");
		if (td_debugger_flag) printf("Leaving the td_row_reallocation function.\n");
		return 1;
	}

	if (td_debugger_flag) printf("Leaving the td_row_reallocation function.\n");

	return 0;
}

int td_column_reallocation(td_array ** array, int new_col_size) {

	if (td_debugger_flag) printf("Entering the td_column_reallocation function.\n");

	int error_flag = 0;

	for (int i = 0; i < (*array)->total_num_rows; ++i) {
		if (!strcmp((*array)->format, "char")) {
			((char**)(*array)->array)[i] = realloc(((char**)(*array)->array)[i], new_col_size * sizeof(char));
			if (!((char**)(*array)->array)[i]) {
				error_flag = i;
				break;
			}	
		} else { 
			printf("ERROR: The format is not supported in the td_column_reallocation function...\n");
			return 1;
		}
	}
	
	if (error_flag) {
		printf("ERROR: Realloc has failed...\n");
		for (int i = 0; i < error_flag; ++i)
			free(((char**)(*array)->array)[i]);
		free((*array)->array);
		if (td_debugger_flag) printf("Leaving the td_column_reallocation function.\n");
		return 1;
	}

	if (td_debugger_flag) printf("Leaving the td_column_reallocation function.\n");

	return 0;
}

int td_allocation(td_array ** array, char * format, int number_of_rows, int row_incrementation, int number_of_columns) {

	if (td_debugger_flag) printf("Entering the td_allocation.\n");

	if ((*array)->array) {
		printf("ERROR: The passed in td_array structure was not empty...\n");
		return 1;
	}

	int error_flag = 0;
	(*array)->current_num_rows = 0;
	(*array)->total_num_rows = number_of_rows;
	(*array)->total_num_cols = number_of_columns;

	if (row_incrementation <= 0)
		(*array)->row_incrementation = 5;
	else
		(*array)->row_incrementation = row_incrementation;

	(*array)->format = calloc(strlen(format) + 1, sizeof(char));
	strcpy((*array)->format, format);

	if (!strcmp(format, "char")) {
		(*array)->array = calloc(number_of_rows, sizeof(char*));
		if (!(*array)->array) {
			printf("ERROR: Calloc has failed...\n");
			if (td_debugger_flag) printf("Leaving the td_allocation function.\n");
			return 0;
		}
	} else {
		printf("ERROR: The given data type is not supported...\n");
		if (td_debugger_flag) printf("Leaving the td_allocation function.\n");
		return 0;
	}

	if (number_of_columns) {
		for (int i = 0; i < number_of_rows; ++i) {
			if (!strcmp((*array)->format, "char")) {
				((char**)(*array)->array)[i] = calloc(number_of_columns, sizeof(char));
				if (!((char**)(*array)->array)[i]) {
					error_flag = 1;
					break;
				}
			}
		}
		
		if (error_flag) {
			printf("ERROR: Calloc has failed...\n");
			for (int i = 0; i < number_of_rows; ++i) 
				free(((char**)(*array)->array)[i]);	
			free((*array)->array);
			if (td_debugger_flag) printf("Leaving the td_allocation function.\n");
			return 0;		
		}
	} else {
		if (td_debugger_flag) printf("WARNING: Not allocating memory for the columns in the td_array structure...\n");
	}
	
	if (td_debugger_flag) printf("Leaving the td_allocation function.\n");
	
	return 1;
}

int td_push(td_array ** array, const char * format, ...) {

	if (td_debugger_flag) printf("Entering the td_push function.\n");

	va_list arguments;
	va_start(arguments, format);

	void * vptr;

	if (!strcmp(format, "char")) {
		vptr = va_arg(arguments, char**);
 	} else {
		printf("ERROR: The given data type is not supported...\n");
		if (td_debugger_flag) printf("Leaving the td_push funtion.\n");
		va_end(arguments);
		return 1;
	}

	if ((*array)->current_num_rows == (*array)->total_num_rows) {
		int temp = (*array)->total_num_rows + (*array)->row_incrementation;
		td_row_reallocation(array, temp);
		(*array)->total_num_rows = temp;
	}

	if ((*array)->total_num_cols) {
		if (!strcmp(format, "char")) {
			if (strlen(*(char**)vptr) > (*array)->total_num_cols) {
				int temp = strlen(*(char**)vptr) + 1;
				td_column_reallocation(array, temp);
				(*array)->total_num_cols = temp;
			}
			
			if (!strcmp(*(char**)vptr, "NULL")) {
				((char**)(*array)->array)[(*array)->current_num_rows] = NULL;
			} else {
				strcpy(((char**)(*array)->array)[(*array)->current_num_rows], *(char**)vptr);
			}
		} 
	} else {
		if (!strcmp(format, "char")) {
			((char**)(*array)->array)[(*array)->current_num_rows] = calloc(strlen(*(char**)vptr) + 1, sizeof(char));
			strcpy(((char**)(*array)->array)[(*array)->current_num_rows], *(char**)vptr);
		}
	}
	(*array)->current_num_rows++;

	va_end(arguments);

	if (td_debugger_flag) printf("Leaving the td_push funtion.\n");

	return 0;
}

int td_free(td_array ** array) {

	if (td_debugger_flag) printf("Entering the td_free function.\n");

	if ((*array)->total_num_cols) {
		for (int i = 0; i < (*array)->total_num_rows; ++i) {
			if (!strcmp((*array)->format, "char")) {
				free(((char**)(*array)->array)[i]);
			} else {
				printf("ERROR: The 2D array that you are trying to free has a type that is not supported...\n");
				return 1;
			}
		}
	} else {
		for (int i = 0; i < (*array)->current_num_rows; ++i) {
			if (!strcmp((*array)->format, "char")) {
				free(((char**)(*array)->array)[i]);
			} else {
				printf("ERROR: The 2D array that you are trying to free has a type that is not supported...\n");
				return 1;
			}
		}
	}
	free((*array)->array);
	free((*array)->format);

	if (td_debugger_flag) printf("Leaving the td_free funtion\n");

	return 0;
}

void td_debugger() {
	td_debugger_flag = 1;
	return;
}
