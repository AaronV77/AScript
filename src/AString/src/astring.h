/**
* @file
* @author Aaron Valoroso <Aaron.A.Valoroso@erdc.dren.mil>
* @date June 15th, 2020
*
* @section Description
*
* This file contains all the library functions for a string implementation in C.
*/
#ifndef ASTRING
#define ASTRING

#include <stdlib.h> // calloc(), realloc()
#include <stdarg.h> // va_list(), va_start(), va_end(), va_arg()
#include <string.h> // strcmp(), strlen()
#include <stdio.h>  // printf()

typedef struct string {
    char * array;
    char ** tokens;
    int total_num_tokens;
    int current_num_col;
    int total_num_cols;
    int col_incrementation;
}string;

// int column_reallocation(string ** array, int new_col_size); (Static)

string * salloc(int number_of_columns, int col_incrementation);

int sadd(string ** array, const char * format, ...);

int sinsert(string ** array, int position, const char * format, ...);

int soccurences(string * array, char find_char);

int stokenize(string ** array, char token_char);

void strunc(string ** array, int position, char character);

void sremove_leading_and_trailing_spaces(string ** array);

void sremove(string ** array, char * remove_characters);

void sclear(string ** array);

int sreset(string ** array, int number_of_columns, int col_incrementation);

void sfree(string ** array);

#endif
