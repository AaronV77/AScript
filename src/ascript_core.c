#include "ascript_core.h"

int ascript_core_debugger_var = 1;

Env_Linked_List * global_environment;

void setup_core_environment(Env_Linked_List * env_path) {
    global_environment = env_path;
    return;
}

// string * check_available_command(Env_Linked_List * env_path, char * command) {
    
//     string * ptr = salloc(50, 10);

//     Env_Linked_List_Node * current = env_path->front;

//     while (current) {
//         sadd(&ptr, "stringcharstring", current->path, '/', command);
//         if (is_file(ptr->array))
//             return ptr;
//         sclear(&ptr);
//         current = current->next;
//     }
//     if (ascript_core_debugger_var) printf("DEBUG: Did not find the command on the system.\n");
//     return ptr;
// }

// int execute_command(char * command_file_location, char ** arguments, char ** envp) {
    
//     pid_t  pid;
    
//     pid = fork();
//     if (pid == 0) {
//         execve(command_file_location, arguments, envp);
//     } else {
//         waitpid(pid, NULL, 0);
//     }

//     return 0;
// }

void evaluate_variable_value(Var_Linked_List ** variables, Func_Linked_List * functions, string * data_type, string * incoming_line) {

    // Check to see if the incoming variable data line has any quotes within it. If there are quotes avaialbe,
    // - then there is a good chance that the line is going to contain a new variable or updating a previous
    // - variable value.
    if (soccurences(incoming_line, '"') || soccurences(incoming_line, '\'')) {
        
        // Remove the semicolon at the end of the line.
        if (soccurences(incoming_line, ';')) {
            // Since AScript should only have one purpose per line, remove everything after the semicolon.
            strunc(&incoming_line, 0, ';');
        // All lines in AScript cannot wrap around.
        } else {
            printf("ERROR: There is no semicolon in the passed in line for the variable declaration.\n");
            sclear(&incoming_line);
            return;
        }

        // Check to see if the incoming variable line has a data type.
        if (data_type->current_num_col) {
            // Check to see if the incoming variable data type is not of type string. Any variables that should
            // - be in this section should be of type string.
            if (strcmp(data_type->array, "string") && strcmp(data_type->array, "char")) {
                // Error out.
                printf("ERROR: The value for the assigned variable has quotes but is of not type char or string.\n");
                sclear(&incoming_line);
                return;
            }
        }

        char character;                             // Used to store the character being processed.
        char operator = '\0';                       // Used to store the addition operator if found.

        int comment_flag = 0;                       // Used to turn on and off the parsing mechanism when quotes are found.
        int variable_found_flag = 0;                // Used to turn on and off the parsing mechanism when both quotes and a variable have been found.
        int  left_value_capture_flag = 1;           // Used to turn on and off which char buffer to store values into.

        string * variable_found = salloc(10, 5);    // Used to store the variavble if found in either a quoted section or by itself.
        string * left_value = salloc(10, 5);        // Used to store values on the left hand side of operators.
        string * right_value = salloc(10, 5);       // Used to store values on the right hand side of operators.

        // Loop through value of the incoming variable.
        for (int i = 0; i < incoming_line->current_num_col; ++i) {
            // Grab the character so that I don't have to type this long line every time.
            character = incoming_line->array[i];

            // Check to see if the comment flag is turned off or if a variable has been found.
            if (!comment_flag || variable_found_flag) {
                // If the grabbed character is a string.
                if (character == 32) {
                    // If a variable has been found.
                    if (variable_found_flag) {
                        // Loop through the linked list to see if the variable been found previously.
                        Var_Linked_List_Node * temp = var_ll_search((*variables), variable_found->array);
                        // Check to see if the pointer has data associated to it.
                        if (temp) {
                            if (ascript_core_debugger_var) printf("DEBUG: A previously found variables has been spotted in the value of a string.\n");
                            // Check to make sure that the returned variable is a string.
                            if (strcmp(temp->variable_data_type, "string")) {
                                printf("ERROR: The given variable is not of type string: %s\n", variable_found->array);
                                break;
                            }

                            // Check which buffer to store the data into.
                            if (left_value_capture_flag) {
                                if (ascript_core_debugger_var) printf("DEBUG: Storing data (%s) into the left buffer from the %s variable.\n", temp->data, temp->variable_name);
                                sadd(&left_value, "string", temp->data);
                            } else {
                                if (ascript_core_debugger_var) printf("DEBUG: Storing data (%s) into the right buffer from the %s variable.\n", temp->data, temp->variable_name);
                                sadd(&right_value, "string", temp->data);
                            }
                            
                            // Check to see if both buffers are filled with data and if there is an operator.
                            if (left_value->current_num_col && right_value->current_num_col && operator) {
                                if (ascript_core_debugger_var) printf("DEBUG: Adding the strings together.\n");
                                // Since this is string operations, just add the data from the right value to the left buffer.
                                sadd(&left_value, "string", right_value->array);
                                // Reset the operator.
                                operator = '\0';
                                // Clear the right buffer of its contents.
                                sclear(&right_value);
                                // Make sure the capture flag is set to fill the right side once more.
                                left_value_capture_flag = 0;
                            }
                        } else {
                            printf("ERROR: There was no variable found with that given name: %s\n", variable_found->array);
                            sclear(&left_value);
                            sclear(&right_value);
                            break;
                        }
                        // Reset the variable found flag.
                        variable_found_flag = 0;
                        // Only add the space character if the comment flag is on.
                        if (!comment_flag)
                            continue;
                    }
                // Check to see if the character is a double quote symbol.
                } else if (character == 34) {
                    // Turn the comment flag on.
                    comment_flag = 1;
                    // Don't add the double quote to the value of the string.
                    continue;
                // Look for single quotes.
                } else if (character == 39) {
                    // Don't add the single quote to the value of the character
                    continue;
                // Check to see if the character is an addition symbol.
                } else if (character == 43) {
                     if (ascript_core_debugger_var) printf("DEBUG: Found the addition operator.\n");
                    // Set / save the operator character.
                    operator = 43;
                    // Switch the capture flag to store the opposite side.
                    left_value_capture_flag = 1;
                    // Don't add the operator to the string.
                    continue;
                // Anything else that is found is considered a variable.
                } else {
                    // Turn the variable found flag to on.
                    variable_found_flag = 1;
                }
            // Check to see if the character is a double quote symbol.
            } else if (character == 34) {
                // Turn the comment flag off 
                comment_flag = 0;
                // Don't add the double quote to the value of the string.
                continue;
            // Check to see if the comment flag is on and if the dollar symbol is found.
            } else if (comment_flag && character == 36) {
                // Turn on the variable found flag.
                variable_found_flag = 1;
                continue;
            }

            // Check to make sure that no spaces are added to any string if the comment flag is not on.
            if (!comment_flag && character == 32)
                continue;

            // Check to see if the variable found flag is off.
            if (!variable_found_flag) {
                // Check to see which position the capture flag is in to see what buffer to store the data into.
                if (left_value_capture_flag) {
                    sadd(&left_value, "char", character);
                } else
                    sadd(&right_value, "char", character);
            } else
                sadd(&variable_found, "char", character);
        }

        if (!left_value->current_num_col && variable_found->current_num_col) {
            sadd(&left_value, "string", variable_found->array);
        }

        // Clear the incoming line for the new data value.
        sclear(&incoming_line);
        // Check to see if the left buffer is emtpy
        if (left_value->current_num_col) {
            if (ascript_core_debugger_var) printf("DEBUG: The value for the incoming variable is: %s\n", left_value->array);
            sadd(&incoming_line, "string", left_value->array);
        }

        // Check to see if the data type is char and if the value is greater than 1 character. No 
        // - char data types in AScript can have a value of more than one character.
        if (!strcmp(data_type->array, "char") && incoming_line->current_num_col > 1) {
            printf("ERROR: The value of the char variable is greater than one character for the value.\n");
            sclear(&incoming_line);
            return;
        }

        // Free the strings.
        sfree(&left_value);
        sfree(&right_value);
        sfree(&variable_found);
        return;
    // Anything that doesn't have a quote in it is considered to be a number operation.
    } else {
        char character;                             // Used to store the current character being processed.
        string * temp_value = salloc(10, 5);        // Used to store the value without any variable names, only values.
        string * variable_found = salloc(10, 5);    // Used to store the variable or function name.

        // Loop through the incoming variable value.
        for (int i = 0; i < incoming_line->current_num_col; ++i) {
            // Save the character so that I don't have to type this long line more than once.
            character = incoming_line->array[i];
            // Check for any upper or lower case characters.
            if ((character >= 65 && character <= 90) || (character >= 97 && character <= 122)) {
                // Add the characters to the variable.
                sadd(&variable_found, "char", character);
            // Check for anything that is not a character.
            } else if (character != 32 && character != 59) {
                // See if the variable string has any letters within it.
                if (variable_found->current_num_col) {
                    // Check to see if the variable_name matches any ascript reserved key words.
                    if (strcmp(variable_found->array, "and") || strcmp(variable_found->array, "or")) {
                        // Loop through the linked list of variables to see if any match the passed in name.
                        Var_Linked_List_Node * temp = var_ll_search((*variables), variable_found->array);
                        
                        // Check to see a variable has been found with the given name.
                        if (temp) {
                            // Check to see if the found variable is not of type string.
                            if (strcmp(temp->variable_data_type, "string")) {
                                // Reset the variable string.
                                sclear(&variable_found);
                                // Set the variable found string to the data that was of the found variable in the linked list.
                                sadd(&variable_found, "string", temp->data);
                            // If the value of the variable is of type string then error out.
                            } else {
                                printf("ERROR: You cannot add a string datatype to a number, this isn't Javascript.\n");
                                sclear(&temp_value);
                                break;
                            }
                        // If the a variable was not found with the given name, check the functions linked list.
                        } else {
                            // Loop through the linked list of functions to see if any match the given name.
                            Func_Linked_List_Node * temp = func_ll_search(functions, variable_found->array);

                            //Check to see if a function was found.
                            if (temp) {
                                // Check to see if the function return type is not of type string.
                                if (strcmp(temp->function_return_type, "string")) {
                                    sclear(&variable_found);
                                    // Call the function and grab the return data.
                                    printf("WARNING: This feature is not yet available.\n");                                
                                } else {
                                    printf("ERROR: You cannot add a string datatype to a number, this isn't Javascript.\n");
                                    sclear(&temp_value);
                                    break;
                                }
                            // If no items were found.
                            } else {
                                printf("ERROR: There is not a single variable or function with the given name: %s\n", variable_found->array);
                                sclear(&temp_value);
                                break;
                            }
                        }
                    }

                    // Add the retireved value to the everything else.
                    sadd(&temp_value, "string", variable_found->array);
                    // Reset the variable found string.
                    sclear(&variable_found);
                }
                // Add the character to the temp_value string.
                sadd(&temp_value, "char", character);
            }                
        }

        // Check to see if the final value has any characters.
        if (!temp_value->current_num_col) {
            // If not then clear the string and return.
            sclear(&incoming_line);
            sfree(&temp_value);
            sfree(&variable_found);
            return;
        }

        
        // Pass the string over to the calc / if comp functions.
        double return_value = calculator_and_if_comparison(temp_value, 0);
        if (ascript_core_debugger_var) printf("DEBUG: The returned value from the calc function is: %f\n", return_value);

        // Check to see if there is a data type
        if (data_type->current_num_col) {
            // Check to see if it is an int
            if (!strcmp(data_type->array, "int")) {
                // Create a temp string to store the returned value.
                string * temp = salloc(10, 5);
                // Add the returned value to the string.
                sadd(&temp, "double", return_value);
                // Get the strings that lie before and after the decimal.
                stokenize(&temp, '.');
                // Clear the temp value string to make room of the final value.
                sclear(&temp_value);
                // Check to see if there are tokens
                if (temp->total_num_tokens) {
                    // Add the first string to the temp value.
                    sadd(&temp_value, "string", temp->tokens[0]);
                } else {
                    // Add the entire array if the value is just one digit.
                    sadd(&temp_value, "string", temp->array);
                }
                // Free the temp string.
                sfree(&temp);
            }
        } 
        
        // Clear the incoming line to make room for the value.
        sclear(&incoming_line);
        // Add the value to the incoming line.
        sadd(&incoming_line, "string", temp_value->array);
        // Free the strings.
        sfree(&temp_value);
        sfree(&variable_found);

        return;
    }
}

int evaluate_variables(Var_Linked_List ** variables, Func_Linked_List * functions, string * current_line) {

    // Check to see if the passed in line is complete.
    if (!soccurences(current_line, ';')) {
        printf("ERROR: Variable declaration or assigning a new value, has to all be on one line.\n");
        return 1;
    }

    int found_datatype_flag = 0;                    // Used to signal the algorithm when a data type has been found.
    int error_was_found = 0;                        // Used to signal when the algorithm has encountered an error.
    int updating_variable_value = 0;                // Used to signal when no data type was given and the variable value is being updated.

    string * data_type = salloc(10, 5);             // Used to store the data type.
    string * variable_name = salloc(20, 5);         // Used to store the vairable name.
    string * variable_value = salloc(20, 5);        // Used to store the variable value.

    char datatypes[5][7] = {"char", "int", "double", "float", "string"};    // Used to determine what variable data types are acceptable.

    // Tokenize the string to get the data type and variable in one token and the value into another token.
    stokenize(&current_line, '=');
    // Save the contents of the first token to a temp string.
    string * left_hand_side = salloc(strlen(current_line->tokens[0]) + 1, 5);
    // Add the first token found in to the temp string.
    sadd(&left_hand_side, "string", current_line->tokens[0]);
    // Remove all leading and ending spaces found.
    sremove_leading_and_trailing_spaces(&left_hand_side);
    // Tokenize the string to get the data type and variable name separated.
    stokenize(&left_hand_side, ' ');

    // Check to see if there are not any tokens avaiable.
    if (!left_hand_side->total_num_tokens) {
        if (ascript_core_debugger_var) printf("DEBUG: Updating the found variable value: %s\n", left_hand_side->array);
        // Add the first token to the variable name section.
        sadd(&variable_name, "string", left_hand_side->array);
        // Set the updating variable value flag, indicating that no data type was found.
        updating_variable_value = 1;
    // Check to see if there were two tokens found.
    } else if (left_hand_side->total_num_tokens == 2) {
        // Loop through the data types.
        for (int i = 0; i < 5; ++i) {
            // Check to see if any of the data types match the one passed in.
            if (!strcmp(left_hand_side->tokens[0], datatypes[i])) {
                if (ascript_core_debugger_var) printf("Found a datatype: %s\n", left_hand_side->tokens[0]);
                // Set the found data type flag if one was found.
                found_datatype_flag = 1;
                break;
            }
        }

        // Check to see if a data type was found.
        if (found_datatype_flag) {
            // Add the data type to the string.
            sadd(&data_type, "string", left_hand_side->tokens[0]);
            // Add the variable name to the string.
            sadd(&variable_name, "string", left_hand_side->tokens[1]);
        // If no data types matched the passed in string, error out.
        } else {
            printf("ERROR: The given data type given for the variable is not familiar.\n");
            error_was_found = 1;
        }
    // If there are more than two tokens, error out.
    } else {
        printf("ERROR: There are too many tiems before the equal symbol.\n");
        error_was_found = 1;
    }
    
    // Check to see if the variable_name does not match any ascript reserved key words.
    if (!strcmp(variable_name->array, "and") || !strcmp(variable_name->array, "or") || !strcmp(variable_name->array, "continue")) {
        printf("ERROR: The passed in variable cannot have the same name as a reserved word.\n");
        error_was_found = 1;
    }

    // Check to see if an error was found.
    if (error_was_found) {
        // Free the strings and return.
        sfree(&data_type);
        sfree(&variable_name);
        sfree(&variable_value);
        sfree(&left_hand_side);
        return 1;
    }

    // Add the value of the incoming variable line to the variable value string.S
    sadd(&variable_value, "string", current_line->tokens[1]);
    // Get the final value from the variable value.
    evaluate_variable_value(variables, functions, data_type, variable_value);
    if (ascript_core_debugger_var) printf("The returned value is this: %s\n", variable_value->array);
    // Check to see if the the returned string is empty.
    if (!variable_value->current_num_col) {
        // Free the strings and return.
        sfree(&data_type);
        sfree(&variable_name);
        sfree(&variable_value);
        sfree(&left_hand_side);
        return 1;
    }
    
    // Loop through the linked list of variables to see if any match the passed in name.
    Var_Linked_List_Node * temp = var_ll_search((*variables), variable_name->array);
    // Check to see if a variable was found in the linked list.
    if (temp) {
        // Check to see if the user is trying to re-declare a variable that has already been declared.
        if (data_type->current_num_col) {
            printf("ERROR: You are trying to re-declare a variable that has already been defined.\n");
            sfree(&data_type);
            sfree(&variable_name);
            sfree(&variable_value);
            sfree(&left_hand_side);
            return 1;
        } else {
            // Check to see if there were any quotes in the returned variable value.
            if (soccurences(variable_value, '"')) {
                // Check to make sure the found variable is a of type string.
                if (!strcmp(temp->variable_data_type, "string")) {
                    if (ascript_core_debugger_var) printf("DEBUG: Updating the variable: %s with the value: %s\n", variable_name->array, variable_value->array);
                    // Clear the data of the found variable.
                    memset(temp->data, '\0', sizeof(temp->data));
                    // Copy the value to the found variable.
                    strcpy(temp->data, variable_value->array);
                }
            // Check to see if there are any decimals in the returned variable value.
            } else if (soccurences(variable_value, '.')) {
                if (ascript_core_debugger_var) printf("DEBUG: Updating the variable: %s with the value: %s\n", variable_name->array, variable_value->array);
                // Check to make sure the found variable is of type float or double.
                if (!strcmp(temp->variable_data_type, "float") || !strcmp(temp->variable_data_type, "double")) {
                    // Clear the data of the found variable.
                    memset(temp->data, '\0', sizeof(temp->data));
                    // Copy the value to the found variable.
                    strcpy(temp->data, variable_value->array);
                }
            // Anything else is an int.
            } else {
                if (ascript_core_debugger_var) printf("DEBUG: Updating the variable %s with the value %s\n", variable_name->array, variable_value->array);
                // Grabe everything before the decimal by tokenizing the string.
                stokenize(&variable_value, '.');
                // Clear the data of the found variable.
                memset(temp->data, '\0', sizeof(temp->data));
                // Check to see if there are any tokens.
                if (variable_value->total_num_tokens) {
                    // Copy the value to the found variable.
                    strcpy(temp->data, variable_value->tokens[0]);
                } else {
                    // Copy the value to the found variable.
                    strcpy(temp->data, variable_value->array);
                }
            }
        }
    // If there was no variable found.
    } else {
        // Check to see if a data type was passed in.
        if (!data_type->current_num_col) {
            printf("ERROR: You need a data type for an undeclared variable.\n");
            sfree(&data_type);
            sfree(&variable_name);
            sfree(&variable_value);
            sfree(&left_hand_side);
            return 1;
        } else {
            // Add the variable to the linked list of other variables.
            if (ascript_core_debugger_var) printf("DEBUG: Adding the variable %s to the linked list of variables.\n", variable_name->array);
            var_ll_push(variables, variable_name->array, data_type->array, variable_value->array);
        }
    }

    sfree(&data_type);
    sfree(&variable_name);
    sfree(&variable_value);
    sfree(&left_hand_side);

    return 0;
}

// int skip_ahead_algorithm(FILE * source_fp, int curly_brace_index) {
    
//     if (ascript_core_debugger_var) printf("DEBUG: Skipping not so import items till we hit the good stuff.\n");

//     int exit_flag = 0;
//     int saved_curly_brace_index = curly_brace_index;

//     char current_character;                         // Used to hold each character of every every line being processed so that I don't have to type this every time "incoming_line->array".
//     char source_file_line[256];                     // Used to capture each line looped through the given AScript file.

//     string * current_line = salloc(256, 10);        // Used to store and process each captured line within the AScript file.

//     curly_brace_index++;

//     // Looping through the passed in AScript file.
//     while (fgets(source_file_line, sizeof(source_file_line), source_fp)) {

//         sadd(&current_line, "string", source_file_line);
//         schar_delete(&current_line, "\t\n");
        
//         if (current_line->current_num_col) {
//             for (int i = 0; i < current_line->current_num_col; ++i) {
//                 current_character = current_line->array[i];

//                 if (current_character == 123) {
//                     curly_brace_index++;
//                 } else if (current_character == 125) {
//                     curly_brace_index--;
//                 } else if (saved_curly_brace_index == curly_brace_index) {
//                     printf("Debug: Found the closing curly brace to where the good stuff is.\n");
//                     exit_flag = 1;
//                     break;
//                 }
//             }
//         } else if (exit_flag) {
//             break;
//         }

//         sclear(&current_line);
//     }
    
//     sfree(&current_line);

//     return 0;
// }

int parsing_algorithm(FILE * source_fp, Var_Linked_List * variables, Func_Linked_List * functions, int curly_brace_index, char * function_purpose) {

    int comment_flag = 0;                           // Used as a flag to tell the algoirthm to ignore everything in a comment section, minus $variables.
    int current_line_number = 0;                    // Used for debugging to indicate where the error happend on the given line.
    int curly_brace_iterator = 0;
    int number_of_variables = 0;

    char current_character;                         // Used to hold each character of every every line being processed so that I don't have to type this every time "incoming_line->array".
    char source_file_line[256];                     // Used to capture each line looped through the given AScript file.

    string * current_word = salloc(15, 10);         // Used to hold a complete word so that the algorithm can guess at what the current line is about the execute.
    string * current_line = salloc(256, 10);        // Used to store and process each captured line within the AScript file.

    if (variables)
        number_of_variables = variables->list_length;

    // Looping through the passed in AScript file.
    while (fgets(source_file_line, sizeof(source_file_line), source_fp)) {

        sadd(&current_line, "string", source_file_line);
        schar_delete(&current_line, "\t\n");
        
        if (current_line->current_num_col) {
            if (ascript_core_debugger_var) printf("DEBUG: Here is the incoming line: %s\n", current_line->array);
            for (int i = 0; i < current_line->current_num_col; ++i) {
                current_character = current_line->array[i];

                if (!comment_flag) {
                    // Check to see if the coment flag is off, if not then ignore everything until one is found.
                    if (current_character == 32 || current_character == 40) {
                        // Check to see if the current character is a space or opening parenthese.

                        // There are 9 possibilities that the current line plans on doing
                        
                        if (current_character == 32) {
                            if (soccurences(current_line, '=')) {
                                if (evaluate_variables(&variables, functions, current_line)) {
                                    return 1;
                                }
                                break;
                            } else {
                                continue;
                                // Pass the whole line to a function that accpets the grabbed line.
                                // Pass it the linked list of functions as a double pointer.
                                // Pass in the current line number.
                                // The function should check to see if the function is in the linnked list and if not
                                // - the function will add it to the list by keep track of the return type, the input
                                // - arguments and their types. Lastly, make sure that there is a curly brace at the end.
                            }
                        } else {
                            continue;
                            // Function Call
                            // Command Call
                            // if statement 
                            // For loop: Recursive
                            // While loop: Recursive
                        }

                        // Semicolon
                            // Loop continuation
                            // function return

                        // Other
                            // Continuation of if statement
                    } else if (current_character == 34) {
                        // Check to see if the current character is a double quote aka comment.
                        if (ascript_core_debugger_var) printf("DEBUG: Found a double quote, turning the comment flag on.\n");
                        // Turn the comment flag on.
                        comment_flag = 1;
                    } else if (current_character == 59) {
                        // Check to see if the current character is a semicolon.
                        if (ascript_core_debugger_var) printf("DEBUG: Found a semicolon, reseting / checking variables.\n");

                        // Need to check to see what was not wrapped up in the current line.

                        // Reset variables to process the next line.
                    }
                } else if (current_character == 34) {
                    // Check to see if the current character is a dobule quote aka comment.
                    if (ascript_core_debugger_var) printf("DEBUG: Found a double quote, turning the comment flag off.\n");
                    // If so, turn the comment flag ssoff.
                    comment_flag = 0;
                }
            }
        }

        current_line_number++;

        sclear(&current_line);
        sclear(&current_word);
    }

    if (ascript_core_debugger_var) printf("Here are all the variables for the given section: \n");
    var_ll_listall(variables);

    if (ascript_core_debugger_var) printf("DEBUG: Cleaning up the variables.\n");
    
    sfree(&current_word);
    sfree(&current_line);

    func_ll_cleanup(&functions);
    var_ll_cleanup(&variables);

    if (ascript_core_debugger_var) printf("DEBUG: Leaving the AScript main function.\n");

    return 0;
}