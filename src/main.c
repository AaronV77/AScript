#include "dir_utility.h"
#include "linked_list.h"
#include "string.h"         // My string library for C

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main_debugger_var = 0;

void print_system_options() {
    printf("There are no options currently, please come again.\n");
    return;
}

void setup_environment(Linked_List ** env_path, char ** envp) {
	
	if (main_debugger_var) printf("Entering the setup_environment function.\n");

    char * token;
    char * var_temp = calloc(5, sizeof(char));
	int env_iterator = 0;
	while(envp[env_iterator] != NULL) {
		if (!strstr(envp[env_iterator], "BASH")) {
			strncpy(var_temp, envp[env_iterator], 4);
			if (!strcmp(var_temp, "PATH")) {
                // Subtracting 4 from the string length due to the PATH= in the front that gets chopped off in the next line.
                char * new_path  = calloc((strlen(envp[env_iterator]) - 4) + 1, sizeof(char));
                strncpy(new_path, &envp[env_iterator][5], (strlen(envp[env_iterator]) - 4));
                token = strtok(new_path, ":");
                while (token) {
                    vec_push(env_path, strlen(token), "string", token);
                    token = strtok(NULL, ":");
                }
                free(new_path);
            }
		}
		env_iterator++;	
	}
    free(var_temp);

	if (main_debugger_var) printf("Leaving the setup_environment function.\n");

	return;
}

int main(int argc, char * argv[], char * envp[]) {
    FILE * source_fp;
    char source_file_line[256];
    char * source_file_location;

    // Read the incoming arguments from the source
    if (argc > 1) {
        for (int i = 1; i < argc; ++i) {
            if (!strcmp(argv[i], "-h") || !strcmp(argv[i], "--help")) {
                print_system_options();
                return 0;
            } else if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")) {
                main_debugger_var = 1;
            } else {
                // This will be the file name of the given file that we are going to be parsing.
                // Need to check to make sure that the file is not a directory and available.
                if (!is_dir(argv[i]) && is_file(argv[i])) {
                    char * last_dot = strrchr(argv[i], '.');
                    // Check to make sure that the file has an extension and the correct one if it does.
                    if (!last_dot || strcmp(last_dot, ".a")) {
                        printf("ERROR: The file given did not have either an extension or the correct extension '.a'.\n");
                        return 0;
                    }
                    if (main_debugger_var) printf("Success: We have found the source file for parsing: %s\n", argv[i]);
                    source_file_location = strdup(argv[i]);
                } 
            }
        }
    } else {
        printf("ERROR: There were no arguments given to the system.\n");
        return 0;
    }

    // Grab the PATH environment variable and save the sections to a linked list data structure.
    Linked_List * env_path = NULL;
    setup_environment(&env_path, envp); 

    // Open the file for reading.
    source_fp  = fopen(source_file_location, "r");
    if (!source_fp) {
        printf("ERROR: The file '%s' could not be opened.\n", source_file_location);
        return 0;
    }

    char c;
    char datatypes[5][7] = {"char", "int", "double", "float", "void"};    
    char * comment_section;
    char * kind_of_datatype;
    int comment_flag = 0;
    int parenth_flag = 0;
    int bracket_flag = 0;
    int file_iterator = 0;
    int found_datatype = 0;
    string * line_parts = calloc(1, sizeof(string));
    salloc(&line_parts, 50, 25);

    while (fgets(source_file_line, sizeof(source_file_line), source_fp)) {
        // printf("Line: %s\n", source_file_line);
        // printf("Size: %d\n", strlen(source_file_line));
        for (int i = 0; i < strlen(source_file_line); ++i) {
            c = source_file_line[i];
            // Looking for comments
            // printf("Looking at: %c and %d\n", c, c);
            if (c == 34) {
                if (comment_flag) {
                    if (main_debugger_var) printf("DEBUG: Found the ending comment character.\n");
                    if (line_parts->number_of_spaces == 0) {
                        if (line_parts->array[0] == 'c' || line_parts->array[0] == 'd' || line_parts->array[0] == 'f' || line_parts->array[0] == 'i' || line_parts->array[0] == 'v') {
                            for (int i = 0; i < 5; ++i) {
                                if (!strcmp(line_parts->array, datatypes[i])) {
                                    printf("Found a datatype: %s\n", line_parts->array);
                                    found_datatype = 1;
                                    // Save the Current Datatype.
                                    break;
                                }
                            }
                        }
                    }
                    // If the commented section is not a datatype then ignore and move on. 
                    // Also if we have found the datatype reset the string. 
                    sclear(&line_parts, 0, 0);
                    comment_flag = 0;
                } else {
                    if (main_debugger_var) printf("DEBUG: Found the starting comment character.\n");
                    comment_flag = 1;
                    sclear(&line_parts, 0, 0);
                }
                continue;
            }
            // Looking for parentheses
            if (c == 40 || c == 41) {
                if (parenth_flag) {
                    if (main_debugger_var) printf("DEBUG: Found the ending paraentheses character.\n");
                    printf("This is between a pair of paraentheses: %s and %d\n", line_parts->array, strlen(line_parts->array));
                    sclear(&line_parts, 0, 0);
                    parenth_flag = 0;
                } else {
                    if (main_debugger_var) printf("DEBUG: Found the starting paraentheses character.\n");
                    srspaces(&line_parts);
                    printf("We have this before the first paraenthese: %s and %d\n", line_parts->array, strlen(line_parts->array));

                    parenth_flag = 1;
                    sclear(&line_parts, 0, 0);
                }
                continue;
            }
            // Looking for brackets
            if (c == 123 || c == 125) {
                if (bracket_flag) {
                    if (main_debugger_var) printf("DEBUG: Found the ending bracket character.\n");
                    bracket_flag = 0;
                } else {
                    if (main_debugger_var) printf("DEBUG: Found the starting bracket character.\n");
                    bracket_flag = 1;
                }
                continue;
            }
            sadd(&line_parts, "char", c);
        }
        
        if (parenth_flag == 0 && comment_flag == 0)
            sclear(&line_parts, 0, 0);

        file_iterator++;
        found_datatype = 0;
    }

    if (main_debugger_var) printf("DEBUG: The number of lines within the file is: %d\n", file_iterator);

    // Clean up the system.
    fclose(source_fp);
    sfree(&line_parts);
    free(line_parts);
    vec_cleanup(&env_path);
    free(source_file_location);

    return 0;
}