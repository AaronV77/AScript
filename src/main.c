#include "dir_utility.h"
#include "linked_list.h"
#include "astring.h"         // My string library for C
#include "func_vars_list.h"

#include <sys/types.h>
#include <sys/wait.h>

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

string * check_available_command(Linked_List * env_path, char * command) {
    
    string * ptr = calloc(1, sizeof(string));
    salloc(&ptr, 50, 10);

    Linked_List_Node * current = env_path->front;

    while (current) {
        sadd(&ptr, "string", (char*)current->list_item);
        sadd(&ptr, "char", '/');
        sadd(&ptr, "string", command);
        if (is_file(ptr->array)) {
            return ptr;
        }
        sclear(&ptr, 0, 0);
        current = current->next;
    }
    if (main_debugger_var) printf("DEBUG: Did not find the command on the system.\n");
    return ptr;
}

int execute_command(char * command_file_location, char ** arguments, char ** envp) {
    
    pid_t  pid;
    
    pid = fork();
    if (pid == 0) {
        execve(command_file_location, arguments, envp);
    } else {
        waitpid(pid, NULL, 0);
    }

    return 0;
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

    int exit_flag = 0;
    int comment_flag = 0;
    int parenth_flag = 0;
    int bracket_flag = 0;
    int file_iterator = 0;
    int found_command = 0;
    int found_datatype = 0;
    int found_variable = 0;
    int found_old_function = 0;
    int number_of_datatypes = 6;
    int found_function_declaration = 0;
    int found_replacing_var = 0;

    char c;
    char found_datatype_storage[8];
    char current_function_name[256];
    char found_variable_name_storage[256];
    char found_comm_and_func_name_storage[256];
    char found_function_declaration_name_storage[256];    
    char datatypes[6][7] = {"char", "int", "double", "float", "string", "void"};    


    string * line_parts = calloc(1, sizeof(string));
    salloc(&line_parts, 50, 25);
    string * replacing_variable = calloc(1, sizeof(string));
    salloc(&replacing_variable, 25, 15);
    string * temp = calloc(1, sizeof(string));
    salloc(&temp, 25, 15);

    Func_Var_List * variables = NULL;
    Func_Var_List * function_arguments = NULL; 
    Func_Var_List * com_and_func_arguments = NULL;
    
    // Looping through the passed in AScript file.
    while (fgets(source_file_line, sizeof(source_file_line), source_fp)) {
        // For debug messages.
        file_iterator++;
        if (strlen(source_file_line) > 0) {
            for (int i = 0; i < strlen(source_file_line); ++i) {
                c = source_file_line[i];

                // Looking for spaces
                if (c == 32) {
                    // Check to see if we are done grabbing a replacement variable, if so then turn off the grabber. 
                    if (found_replacing_var) {
                        found_replacing_var = 0;
                        Func_Var_List_Node * temp = func_var_search(variables, replacing_variable->array);
                        if (temp) {
                            sadd(&line_parts, "string", ((char**)temp->arguments->array)[0]);
                            sadd(&line_parts, "char", ' ');
                        } else {
                            printf("ERROR: The replacing variable: %s has not been declared.\n", replacing_variable->array);
                            return 1;
                        }
                        sclear(&replacing_variable, 0, 0);
                        continue;
                    }

                    if (!comment_flag && strlen(line_parts->array) && found_datatype) {
                        // This is where we grab the variable name from a given line.
                        if (source_file_line[i + 1] == '=') {
                            if (strcmp(found_datatype_storage, "void")) {
                                // NULL here is a good thing, it signifies that there is not another variable with the given name.
                                if (!func_var_search(variables, line_parts->array)) {
                                    printf("Found a variable name: %s\n", line_parts->array);
                                    func_var_push(&variables, line_parts->array, found_datatype_storage, "NONE", 0);
                                    strcpy(found_variable_name_storage, line_parts->array);
                                    sclear(&line_parts, 0, 0);
                                    memset(found_datatype_storage, 0, sizeof(found_datatype_storage));
                                    found_variable = 1;
                                    found_datatype = 0;
                                    // Move past the iterator past the space after the variable name and equal sign.
                                    i+=2;
                                } else {
                                    printf("ERROR: The given variable is already declared.\n");
                                    return 1;
                                }
                            } else {
                                printf("ERROR: Void can only be the return type of a function, not a variable.\n");
                                return 1;
                            }
                        } else {
                            printf("ERROR: There was no equal sign a space after the variable declaration.\n");
                            return 1;
                        }
                    }

                }
                // Looking for comments that start with the double quote symbol.
                if (c == 34) {
                    if (comment_flag) {
                        if (main_debugger_var) printf("DEBUG: Found the ending comment character.\n");
                        // Check to see if the found replacing var is on and if so turn it off since we have reached the end of a comment.
                        if (found_replacing_var) {
                            found_replacing_var = 0;
                            Func_Var_List_Node * temp = func_var_search(variables, replacing_variable->array);
                            if (temp) {
                                sadd(&line_parts, "string", ((char**)temp->arguments->array)[0]);
                            } else {
                                printf("ERROR: The replacing variable: %s has not been declared.\n", replacing_variable->array);
                                return 1;
                            }
                            sclear(&replacing_variable, 0, 0);
                            comment_flag = 0;
                            continue;
                        }

                        if (found_variable) {
                            comment_flag = 0;
                            continue;
                        }
                        
                        // Check to see if the incoming string is a datatype.
                        if (line_parts->number_of_spaces == 0 && (
                            line_parts->array[0] == 'c' ||
                            line_parts->array[0] == 'd' ||
                            line_parts->array[0] == 'f' ||
                            line_parts->array[0] == 'i' ||
                            line_parts->array[0] == 's' ||
                            line_parts->array[0] == 'v'
                        )) {
                            for (int i = 0; i < number_of_datatypes; ++i) {
                                if (!strcmp(line_parts->array, datatypes[i])) {
                                    printf("Found a datatype: %s\n", line_parts->array);
                                    // If a datatype if found then save it to a buffer and turn on the flag.
                                    // This can be for functions and variables.
                                    found_datatype = 1;
                                    strcpy(found_datatype_storage, line_parts->array);;
                                    break;
                                }
                            }
                        }
                        // If the commented section is not a datatype then ignore and move on. 
                        // Reset the string either way 
                        if (!parenth_flag)
                            sclear(&line_parts, 0, 0);
                        comment_flag = 0;
                    } else {
                        if (main_debugger_var) printf("DEBUG: Found the starting comment character.\n");
                        comment_flag = 1;
                        sclear(&line_parts, 0, 0);
                    }
                    continue;
                }            
                // Looking for the dollar symbol.
                if (c == 36) {
                    found_replacing_var = 1;
                    continue;
                }
                // Looking for parentheses for functions and commands.
                if ((c == 40 || c == 41) && comment_flag == 0) {
                    if (parenth_flag) {
                        if (main_debugger_var) printf("DEBUG: Found the ending paraentheses character.\n");
                        if (found_command || found_old_function) {
                            
                            if (found_replacing_var) {
                                found_replacing_var = 0;
                                Func_Var_List_Node * temp = func_var_search(variables, replacing_variable->array);
                                if (temp) {
                                    sadd(&line_parts, "string", ((char**)temp->arguments->array)[0]);
                                } else {
                                    printf("ERROR: The replacing variable: %s has not been declared.\n", replacing_variable->array);
                                    return 1;
                                }
                                sclear(&replacing_variable, 0, 0);
                            }

                            if (strlen(line_parts->array) > 0) {
                                func_var_argument_push(&com_and_func_arguments, found_comm_and_func_name_storage, line_parts->array);
                            } else
                                func_var_argument_push(&com_and_func_arguments, found_comm_and_func_name_storage, "NONE");


                            printf("Here is the command or function: %s\n", com_and_func_arguments->front->name);
                            printf("Name: %s\n", com_and_func_arguments->front->datatype);
                            printf("Number of arguments: %d\n", com_and_func_arguments->front->number_of_arguments);
                            for (int i = 0; i < com_and_func_arguments->front->number_of_arguments; ++i)
                                printf("Argument-%d: %s\n", i, ((char**)com_and_func_arguments->front->arguments->array)[i]);

                        } else if (found_function_declaration) {
                           if (found_replacing_var) {
                                printf("ERROR: You cannot have a variable in a function declaration.\n");
                                return 1;
                            } 

                            if (strlen(found_datatype_storage) > 0) {
                                func_var_argument_push(&function_arguments, found_function_declaration_name_storage, found_datatype_storage);
                            } else {
                                func_var_argument_push(&function_arguments, found_function_declaration_name_storage, "NONE");
                            }
                            memset(&found_datatype_storage, 0, sizeof(found_datatype_storage));
                            found_datatype = 0;

                            printf("Here is the function declaration: %s\n", function_arguments->front->name);
                            printf("Name: %s\n", function_arguments->front->name);
                            printf("Datatype: %s\n", function_arguments->front->datatype);
                            printf("Number of arguments: %d\n", function_arguments->front->number_of_arguments);
                            for (int i = 0; i < function_arguments->front->number_of_arguments; ++i)
                                printf("Argument-%d: %s\n", i, ((char**)function_arguments->front->arguments->array)[i]);
                        }

                        sclear(&line_parts, 0, 0);
                        parenth_flag = 0;
                    } else {
                        if (main_debugger_var) printf("DEBUG: Found the starting paraentheses character.\n");
                        // There should be no spaces in a function name or command.
                        sremove(&line_parts, "\n ");
                        printf("We have this before the first paraenthese: %s and %lu\n", line_parts->array, strlen(line_parts->array));
                        string * temp = check_available_command(env_path, line_parts->array);
                        if (strlen(temp->array)) { 
                            found_command = 1;
                            func_var_push(&com_and_func_arguments, line_parts->array, "NONE", temp->array, 0);
                            func_var_argument_push(&com_and_func_arguments, line_parts->array, "NULL");
                            printf("A command has been found: %s\n", line_parts->array);
                            if (line_parts->current_num_col < 256) 
                                strcpy(found_comm_and_func_name_storage, line_parts->array);
                            else
                                printf("ERROR: The command name given, is too large for my buffer. \n");
                        } else {
                            if (found_datatype) {
                                found_datatype = 0;
                                found_function_declaration = 1;
                                func_var_push(&function_arguments, line_parts->array, found_datatype_storage, "NONE", file_iterator);
                                printf("We have found a function (%s) declaration.\n", line_parts->array);
                                memset(found_datatype_storage, 0, sizeof(found_datatype_storage));

                                if (line_parts->current_num_col < 256) 
                                    strcpy(found_function_declaration_name_storage, line_parts->array);
                                else
                                    printf("ERROR: The command name given, is too large for my buffer. \n"); 
                            } else {
                                found_old_function = 1;
                                func_var_push(&com_and_func_arguments, line_parts->array, "NONE", "NONE", 0);
                                printf("We have found a function (%s) call.\n", line_parts->array);
                                strcpy(found_function_declaration_name_storage, line_parts->array);
                            }
                        }
                        parenth_flag = 1;
                        sfree(&temp);
                        free(temp);
                        sclear(&line_parts, 0, 0);
                    }
                    continue;
                }            
                // Looking for a comma for functions and commands.
                if (c == 44) {
                    // A comma can only be found in between to paraentheses. 
                    if ((strlen(line_parts->array) || strlen(replacing_variable->array)) && parenth_flag) {
                        // Commands and functions that are being called should enter the following section
                        if (found_command || found_old_function) {
                            if (replacing_variable) {
                                found_replacing_var = 0;
                                Func_Var_List_Node * temp = func_var_search(variables, replacing_variable->array);
                                if (temp) {
                                    sadd(&line_parts, "string", ((char**)temp->arguments->array)[0]);
                                } else {
                                    printf("ERROR: The replacing variable: %s has not been declared.\n", replacing_variable->array);
                                    return 1;
                                }
                                sclear(&replacing_variable, 0, 0);
                            }

                            // If there is a random comma found then error out, or add.
                            if (com_and_func_arguments->list_length) {
                                func_var_argument_push(&com_and_func_arguments, found_comm_and_func_name_storage, line_parts->array);
                            } else {
                                printf("ERROR: Comma found with nothing before it.\n");
                            }
                        } else if (found_function_declaration) {
                           if (found_replacing_var) {
                                printf("ERROR: You cannot have a variable in a function declaration.\n");
                                return 1;
                            } 

                            if (function_arguments->list_length) {
                                func_var_argument_push(&function_arguments, found_function_declaration_name_storage, found_datatype_storage);
                                memset(&found_datatype_storage, 0, sizeof(found_datatype_storage));
                                found_datatype = 0;
                            } else {
                                printf("ERROR: There was no argument for the function until the system thought it found the second one.\n");
                                return 1;
                            }
                        }
                        sclear(&line_parts, 0, 0);
                    } else {
                        printf("ERROR: A comma was found with no context.\n");
                        return 1;
                    }
                    continue;
                }
                // Looking for semicolon.
                if (c == 59) {
                    if (parenth_flag) {
                        printf("ERROR on line-%d: Line ended with a paraentheses still open.\n", file_iterator);
                        exit_flag = 1;
                    } else if (found_function_declaration) {
                        printf("ERROR: The function declaration is incorrect.\n");
                        exit_flag = 1;
                    } else if (found_command) {
                        // There will have to be some feature that finds the correct command. 
                        // Because there can be commands wrapped up in other commands.
                        execute_command(com_and_func_arguments->front->file_location, (char**)com_and_func_arguments->front->arguments->array, envp);
                        func_var_cleanup(&com_and_func_arguments);
                        com_and_func_arguments = NULL;
                        found_command = 0;
                    } else if (found_old_function) {
                        // Call the function (i.e. go to the function in the file) and start procesing the file.
                        memset(&found_function_declaration_name_storage, 0, sizeof(found_function_declaration_name_storage));
                        func_var_cleanup(&com_and_func_arguments);
                        com_and_func_arguments = NULL;
                        found_old_function = 0;
                    } else if (found_variable) {
                        // This is where we check to make sure the given value of a variable matches the declared type.
                        printf("Variable Name: %s and Value: |%s| \n", found_variable_name_storage, line_parts->array);
                        func_var_argument_push(&variables, found_variable_name_storage, line_parts->array);
                        memset(found_variable_name_storage, 0, sizeof(found_variable_name_storage));
                        found_variable = 0;
                        sclear(&line_parts, 0, 0);
                    } else if (strlen(line_parts->array) > 0) {
                        printf("ERROR on line-%d: Line ended with items still being processed.\n", file_iterator);
                        printf("Here: %s and %lu\n", line_parts->array, strlen(line_parts->array));
                        exit_flag = 1;
                    }

                    if (exit_flag) {
                        fclose(source_fp);
                        sfree(&line_parts);
                        free(line_parts);
                        sfree(&replacing_variable);
                        free(replacing_variable);
                        vec_cleanup(&env_path);
                        func_var_cleanup(&function_arguments);
                        func_var_cleanup(&com_and_func_arguments);
                        free(source_file_location);
                        return 1;
                    }
                    continue;
                }
                // Looking for brackets.
                if (c == 123 || c == 125) {
                    if (bracket_flag) {
                        if (main_debugger_var) printf("DEBUG: Found the ending bracket character.\n");
                        bracket_flag = 0;
                    } else {
                        if (main_debugger_var) printf("DEBUG: Found the starting bracket character.\n");
                        bracket_flag = 1;

                        // If we found end of a function declaration 
                        if (found_function_declaration) {
                            strcpy(current_function_name, found_function_declaration_name_storage);
                            memset(found_function_declaration_name_storage, 0, sizeof(found_function_declaration_name_storage));
                            found_function_declaration = 0;
                        }
                    }
                    continue;
                }

                if (found_replacing_var) {
                    sadd(&replacing_variable, "char", c);
                } else {
                    // If we are adding characters for a comment spaces are okay.
                    // If we are not in a comment then please don't add spaces to a possible number.
                    if (comment_flag) {
                        sadd(&line_parts, "char", c);
                    } else {
                        if (c != 32) {
                            sadd(&line_parts, "char", c);
                        }
                    }
                }
            }
        }
    }

    if (main_debugger_var) printf("DEBUG: The number of lines within the file is: %d\n", file_iterator);

    // Clean up the system.
    fclose(source_fp);
    sfree(&line_parts);
    free(line_parts);
    sfree(&replacing_variable);
    free(replacing_variable);
    sfree(&temp);
    free(temp);
    vec_cleanup(&env_path);
    if (function_arguments) func_var_cleanup(&function_arguments);
    if (com_and_func_arguments) func_var_cleanup(&com_and_func_arguments);
    if (variables) func_var_cleanup(&variables);
    free(source_file_location);

    return 0;
}