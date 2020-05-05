#ifndef POINTER_MANAGEMENT
#define POINTER_MANAGEMENT

#include <stdlib.h> // calloc(), realloc()
#include <stdarg.h> // va_list(), va_start(), va_end(), va_arg()
#include <string.h> // strcmp(), strlen()
#include <stdio.h>  // printf()

typedef struct string {
    char * array;
    int number_of_spaces;
    int current_num_col;
    int total_num_cols;
    int col_incrementation;
}string;

// int column_reallocation(string ** array, int new_col_size); 

int salloc(string ** array, int number_of_columns, int col_incrementation);

int sadd(string ** array, const char * format, ...);

int sclear(string ** array, int number_of_columns, int col_incrementation);

int srspaces(string ** array);

int sfree(string ** array);

void string_debugger();

#endif
