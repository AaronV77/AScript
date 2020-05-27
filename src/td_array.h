#ifndef TD_ARRAY
#define TD_ARRAY

#include <stdlib.h> // calloc(), realloc()
#include <stdarg.h> // va_list(), va_start(), va_end(), va_arg()
#include <string.h> // strcmp(), strlen()
#include <stdio.h>  // printf()

typedef struct td_array {
    void * array;
    char * format;
    int current_num_rows;
    int total_num_rows;
    int total_num_cols;
    int row_incrementation;
}td_array;

int td_row_reallocation(td_array ** array, int new_row_size);

int td_column_reallocation(td_array ** array, int new_col_size);

int td_allocation(td_array ** array, char * format, int number_of_rows, int row_incrementation, int number_of_columns);

int td_push(td_array ** array, const char * format, ...);

int td_free(td_array ** array);

void td_debugger();

#endif
