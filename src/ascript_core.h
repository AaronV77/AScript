#ifndef CO_P
#define CO_P

#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "astring.h"
#include "calc_and_if.h"
#include "dir_utility.h"
#include "env_linked_list.h"
#include "func_linked_list.h"
#include "var_linked_list.h"

void setup_core_environment(Env_Linked_List * env_path);
int skip_ahead_algorithm(FILE * source_fp, int curly_brace_index);
int parsing_algorithm(FILE * source_fp, Var_Linked_List * variables, Func_Linked_List * functions, int curly_brace_index, char * function_purpose);

#endif