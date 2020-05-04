#ifndef POINTER_MANAGEMENT
#define POINTER_MANAGEMENT

#include <stdlib.h> // calloc(), realloc()
#include <stdarg.h> // va_list(), va_start(), va_end(), va_arg()
#include <string.h> // strcmp(), strlen()
#include <stdio.h>  // printf()

typedef struct sd_array {
    void * array;
    char * format;
    int current_num_col;
    int total_num_cols;
    int col_incrementation;
}sd_array;

int sd_column_reallocation(sd_array ** array, int new_col_size);

int sd_allocation(sd_array ** array, char * format, int number_of_columns, int col_incrementation);

int sd_add(sd_array ** array, const char * format, ...);

int sd_free(sd_array ** array);

void sd_debugger();

#endif
