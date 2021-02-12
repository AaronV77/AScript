#include "ascript_core.h"

int ascript_core_debugger_var = 1;

Env_Linked_List * global_environment;

void setup_core_environment(Env_Linked_List * env_path) {
    global_environment = env_path;
    return;
}

void evaluate_variable_value(Var_Linked_List ** variables, Func_Linked_List * functions, char * data_type, string * incoming_line) {

        char character;                             // Used to store the character being processed.
        char operator = '\0';                       // Used to store the addition operator if found.

        int user_string_flag = 0;                   // Used to turn on and off the parsing mechanism when quotes are found.
        int user_string_variable_found_flag = 0;                // Used to turn on and off the parsing mechanism when both quotes and a variable have been found.
        int left_value_capture_flag = 1;            // Used to turn on and off which char buffer to store values into.
        int decimal_found = 0;
        int number_of_digit_characters = 0;
        int number_of_string_characters = 0;
        int single_quote_flag = 0;
        int number_of_tokens = 0;

        string * user_string_variable = salloc(10, 5);    // Used to store the variable if found in either a quoted section or by itself.
        string * left_value = salloc(10, 5);        // Used to store values on the left hand side of operators.
        string * right_value = salloc(10, 5);       // Used to store values on the right hand side of operators.
        string * value_state = salloc(10, 5);

        // Set the default for the state of the value.
        sadd(&value_state, "string", "empty");

        sremove_leading_and_trailing_spaces(&incoming_line);

        printf("Incoming Value: |%s|\n", incoming_line->array);
        // Loop through value of the incoming variable.
        for (int i = 0; i < incoming_line->current_num_col; ++i) {
            // Grab the character so that I don't have to type this long line every time.
            character = incoming_line->array[i];

            // Check to see if the comment flag is turned off or if a variable has been found.
            if (!user_string_flag || user_string_variable_found_flag) {
                // If the grabbed character is a space.
                if (character == 32) {
                    // Check to see if the parser is not in a comment section.
                    if (!user_string_variable_found_flag) {

                        Var_Linked_List_Node * temp = NULL;
                        
                        if ((number_of_string_characters >= 1 && number_of_digit_characters >= 1) || number_of_string_characters >= 1) {    
                            // Loop through the linked list to see if the variable been found previously.
                            if (left_value_capture_flag)
                               temp = var_ll_search((*variables), left_value->array);
                            else
                                temp = var_ll_search((*variables), right_value->array);
                            
                            // Check to see if the pointer has data associated to it.
                            if (temp) {
                                if (ascript_core_debugger_var) printf("DEBUG: A previously found variables has been spotted in the value.\n");
                                // Determine what buffer to store the data into.
                                if (left_value_capture_flag) {
                                    sclear(&left_value);
                                    sadd(&left_value, "string", temp->data);

                                    // Check to see if the value state is empty on the first parameter.
                                    if (!strcmp(value_state->array, "empty")) {
                                        // Clear the state for the new state.
                                        sclear(&value_state);
                                        // Set the state of the value. The first parameter determines what the line will be doing.
                                        if (number_of_string_characters)
                                            sadd(&value_state, "string", "string");
                                        else if (!decimal_found)
                                            sadd(&value_state, "string", "int");
                                        else if (decimal_found) 
                                            sadd(&value_state, "string", "double");
                                    }

                                } else {
                                    sclear(&right_value);
                                    sadd(&right_value, "string", temp->data);
                                } 

                            } else {
                                printf("ERROR: There was no variable found with that given name (1): %s\n", right_value->array);
                                sfree(&left_value);
                                sfree(&right_value);
                                sfree(&value_state);
                                sfree(&user_string_variable);
                                return;
                            }
                        } else if (number_of_digit_characters) {
                            if (!strcmp(value_state->array, "empty")) {
                                sclear(&value_state);
                                if (decimal_found)
                                    sadd(&value_state, "string", "double");
                                else
                                    sadd(&value_state, "string", "int");
                            }
                        }

                        // Check to see if both buffers are filled with data and if there is an operator.
                        if (left_value->current_num_col && right_value->current_num_col && operator) {
                            if (!strcmp(value_state->array, "string")) {
                                // Check to see if the state of the value already is a number.
                                if (!strcmp(value_state->array, "int") || !strcmp(value_state->array, "float") || !strcmp(value_state->array, "double")) {
                                    // Check to see if the right buffer is a of type string.
                                    if (number_of_string_characters > 1 && number_of_digit_characters > 1) {
                                        if (!strcmp(temp->variable_data_type, "string")) {
                                            printf("ERROR: Trying to add a string to a digit is not acceptable.\n");
                                            sclear(&left_value);
                                            sclear(&right_value);
                                            break;
                                        }
                                    } else if (number_of_string_characters) {
                                        printf("ERROR: Trying to add a string to a digit is not acceptable.\n");
                                        sclear(&left_value);
                                        sclear(&right_value);
                                        break;
                                    }
                                }

                                if (ascript_core_debugger_var) printf("DEBUG: Adding the strings together.\n");
                                // Since this is string operations, just add the data from the right value to the left buffer.
                                sadd(&left_value, "string", right_value->array);
                                // Reset the operator.
                                operator = '\0';
                                // Clear the right buffer of its contents.
                                sclear(&right_value);
                                // Make sure the capture flag is set to fill the right side once more.
                                left_value_capture_flag = 0;
                            } else {
                                sadd(&left_value, "char", '+');
                                sadd(&left_value, "string", right_value->array);
                                sclear(&right_value);
                            }
                        }

                        number_of_string_characters = 0;
                        number_of_digit_characters = 0;
                    } else {
                        // Loop through the linked list to see if the variable been found previously.
                        Var_Linked_List_Node * temp = var_ll_search((*variables), user_string_variable->array);
                        // Check to see if the pointer has data associated to it.
                        if (temp) {
                            if (ascript_core_debugger_var) printf("DEBUG: A previously found variables has been spotted in the value.\n");
                            // Determine what buffer to store the data into.
                            if (left_value_capture_flag)
                                sadd(&left_value, "string", temp->data);
                            else
                                sadd(&right_value, "string", temp->data);
                        } else {
                            printf("ERROR: There was no variable found with that given name (2): %s\n", right_value->array);
                            sfree(&left_value);
                            sfree(&right_value);
                            sfree(&value_state);
                            sfree(&user_string_variable);
                            return;
                        }
                        sclear(&user_string_variable);
                        user_string_variable_found_flag = 0;
                    }

                    // Increase the number of tokens found in the value.
                    number_of_tokens++;

                    // Only add the space character if the comment flag is on.
                    if (!user_string_flag)
                        continue;

                // Check to see if the character is a double quote symbol.
                } else if (character == 34) {
                    // Turn the comment flag on.
                    user_string_flag = (user_string_flag) ? 0 : 1;
                    if (!strcmp(value_state->array, "empty")) {
                        sclear(&value_state);
                        sadd(&value_state, "string", "string");
                    }

                    // Don't add the double quote to the value of the string.
                    continue;
                // Look for single quotes.
                } else if (character == 39) {
                    // Set the flag to one when the first single quote is found and zero again when the second single quote is found.
                    single_quote_flag = (single_quote_flag) ? 0 : 1;

                    // Check to see if this is the closing single quote by checking for zero. (1 == opening & 0 == closing)
                    if (!single_quote_flag) {
                        int total = number_of_string_characters + number_of_digit_characters;
                        // Check to see if there is more or than one character or digit.
                        if (total > 1) {
                            printf("ERROR: There is more than one character / digit for the given single quote token.");
                            sfree(&left_value);
                            sfree(&right_value);
                            sfree(&value_state);
                            sfree(&user_string_variable);
                            return;
                        }
                    }
                    // Don't add the single quote to the value of the character
                    continue;
                // Check to see if the character is an addition symbol for string concatenation.
                } else if (character == 43) {
                    
                     if (ascript_core_debugger_var) printf("DEBUG: Found the addition operator.\n");
                    // Set / save the operator character.
                    operator = 43;

                    // Switch the capture flag to whatever buffer is empty
                    left_value_capture_flag = (left_value_capture_flag) ? 0 : 1;
                    
                    // Don't add the operator to the string.
                    continue;
                // Check to see if the character is a decimal symbol.
                } else if (character == 46) {
                    decimal_found = 1;
                // CHeck to see if the character is a semicolon;
                } else if (character == 59) {
                    // Increase the number of tokens in the value.
                    number_of_tokens++;
                    continue;
                }

                // Check for characters or digits so that they match the given token in the line.
                // We do not need the numbers for a given user string line.
                if ((character >= 32 && character <= 47) || (character >= 58 && character <= 126)) {
                    // Don't accept the following as characters: ; . + - / ^ * ( )
                    if (character != 46 && character != 59 && character != 43 &&
                        character != 47 && character != 40 && character != 41 &&
                        character != 92 && character != 94 && character != 42)
                        number_of_string_characters++;
                } else if (character >= 48 || character <= 57)
                    number_of_digit_characters++;

            // Check to see if the character is a double quote symbol.
            } else if (character == 34) {
                // Turn the comment flag off 
                user_string_flag = (user_string_flag) ? 0 : 1;
                if (!strcmp(value_state->array, "empty")) {
                    sclear(&value_state);
                    sadd(&value_state, "string", "string");
                }
                // Signal to the rest of the algorithm that there are characters coming through.
                number_of_string_characters++;
                // Don't add the double quote to the value of the string.
                continue;
            // Check to see if the user string flag is on and if the dollar symbol is found.
            } else if (user_string_flag && character == 36) {
                // Turn on the variable found flag.
                user_string_variable_found_flag = 1;
                continue;
            }

            // Check to see if the variable found flag is off.
            if (!user_string_variable_found_flag) {
                // Check to see which position the capture flag is in to see what buffer to store the data into.
                if (left_value_capture_flag)
                    sadd(&left_value, "char", character);
                else
                    sadd(&right_value, "char", character);
            } else
                sadd(&user_string_variable, "char", character);
        }

        // Check to see if there is anything in the left side buffer that didn't get processed.
        if (number_of_tokens == 1 && strcmp(value_state->array, "string")) {
            sclear(&value_state);
            Var_Linked_List_Node * temp = var_ll_search((*variables), left_value->array);
            // Check to see if the pointer has data associated to it.
            if (temp) {
                // Check to see if there was an operator found previous to this right side buffer token.
                if (ascript_core_debugger_var) printf("DEBUG: A previously found variables has been spotted in the value.\n");
                sclear(&left_value);
                sadd(&value_state, "string", temp->variable_data_type);
                sadd(&left_value, "string", temp->data);
            } else {
                if (number_of_digit_characters >= 1 && number_of_string_characters >= 1) {
                    printf("ERROR: There was no variable found with that given name (3): %s\n", right_value->array);
                    sfree(&left_value);
                    sfree(&right_value);
                    sfree(&value_state);
                    sfree(&user_string_variable);
                    return;
                } else if (number_of_string_characters >= 1) {
                    sadd(&value_state, "string", "string");
                } else if (number_of_digit_characters >= 1) {
                    if (decimal_found)
                        sadd(&value_state, "string", "double");
                    else
                        sadd(&value_state, "string", "int");
                }
            }
            // Check for strings and chars.
            // Check for digits.
        }

        // Check to see if there is anything in the right side buffer after parsing.
        if (right_value->current_num_col) {
            printf("CHECKING THE RIGHT SIDE BUFFER\n");
            printf("Char Check: %d\n", number_of_string_characters);
            printf("Digit Check: %d\n", number_of_digit_characters);
            if ((number_of_digit_characters >= 1 && number_of_string_characters >= 1) || number_of_string_characters >= 1) {
                // Loop through the linked list to see if the variable been found previously.
                Var_Linked_List_Node * temp = var_ll_search((*variables), right_value->array);
                printf("Looking for: %s\n", right_value->array);
                // Check to see if the pointer has data associated to it.
                if (temp) {
                    // Check to see if there was an operator found previous to this right side buffer token.
                    if (operator == 43) {
                        if (strcmp(value_state->array, "string")) {
                            if (ascript_core_debugger_var) printf("DEBUG: A previously found variables has been spotted in the value.\n");
                            // Check to see if the user is trying to add a string / char to a number.
                            if ((!strcmp(temp->variable_data_type, "string") || !strcmp(temp->variable_data_type, "char"))&& 
                                (!strcmp(value_state->array, "int") || !strcmp(value_state->array, "float") || !strcmp(value_state->array, "double"))) {
                                printf("ERROR: You can not add a string to a number.\n");
                                sfree(&left_value);
                                sfree(&right_value);
                                sfree(&value_state);
                                sfree(&user_string_variable);
                                return;
                            } else {
                                // Store the data into the left buffer by default since there is only one item in the value.
                                if (ascript_core_debugger_var) printf("DEBUG: Storing data (%s) into the left buffer from the %s variable.\n", temp->data, temp->variable_name);
                                sadd(&left_value, "string", temp->data);
                            }
                        } else {
                            sadd(&left_value, "char", '+');
                            sadd(&left_value, "string", right_value->array);
                            sclear(&right_value);
                        }
                    } else {
                        printf("ERROR: There are two buffers filled but no operator in between.\n");
                        sfree(&left_value);
                        sfree(&right_value);
                        sfree(&value_state);
                        sfree(&user_string_variable);
                        return;
                    }
                } else {
                    if (number_of_digit_characters) {
                        printf("ERROR: There was no variable with the given name: %s\n", right_value->array);
                        sfree(&left_value);
                        sfree(&right_value);
                        sfree(&value_state);
                        sfree(&user_string_variable);
                        return;
                    } else {
                        if (operator == 43) {
                            if (!strcmp(value_state->array, "int") || !strcmp(value_state->array, "float") || !strcmp(value_state->array, "double")) {
                                printf("ERROR: You can not add a string to a number.\n");
                                sfree(&left_value);
                                sfree(&right_value);
                                sfree(&value_state);
                                sfree(&user_string_variable);
                                return;
                            } else {
                                // Store the data into the left buffer by default since there is only one item in the value.
                                if (ascript_core_debugger_var) printf("DEBUG: Storing data (%s) into the left buffer from the right buffer.\n", right_value->array);
                                sadd(&left_value, "string", right_value->array);
                            }
                        } else {
                            printf("ERROR: There are two buffers filled but no operator in between.\n");
                            sfree(&left_value);
                            sfree(&right_value);
                            sfree(&value_state);
                            sfree(&user_string_variable);
                            return;
                        }
                    }
                }
            // If there is not a combination of both characters and numbers then it is either one.
            } else {
                if (!operator) {
                    if (ascript_core_debugger_var) printf("DEBUG: Storing data (%s) into the left buffer\n", right_value->array);
                    // The string value for sadd should work for both string and chars in the array of the right side.
                    sadd(&left_value, "string", right_value->array);
                } else {
                    sadd(&left_value, "char", '+');
                    // The string value for sadd should work for both string and chars in the array of the right side.
                    sadd(&left_value, "string", right_value->array);
                    sclear(&right_value);
                }
            }
        }

        // Clear the incoming line for the new data value.
        sclear(&incoming_line);

        // Check to see if there is anything in the left buffer.
        if (left_value->current_num_col) {

            printf("The Value State: %s\n", value_state->array);
            if (strcmp(value_state->array, "string") && strcmp(value_state->array, "char")) {
                double return_value = calculator_and_if_comparison(left_value, 0);
                if (ascript_core_debugger_var) printf("DEBUG: The returned value from the calc function is: %f\n", return_value);
                
                if (!strcmp(value_state->array, "int")) {
                    // Create a temp string to store the returned value.
                    string * temp = salloc(10, 5);
                    // Add the returned value to the string.
                    sadd(&temp, "double", return_value);
                    // Get the strings that lie before and after the decimal.
                    stokenize(&temp, '.');
                    // Clear the temp value string to make room of the final value.
                    sclear(&left_value);
                    // Check to see if there are tokens
                    if (temp->total_num_tokens) {
                        // Add the first string to the temp value.
                        sadd(&left_value, "string", temp->tokens[0]);
                    } else {
                        // Add the entire array if the value is just one digit.
                        sadd(&left_value, "string", temp->array);
                    }
                    // Free the temp string.
                    sfree(&temp);
                }              
            }

            if (ascript_core_debugger_var) printf("DEBUG: The value for the incoming variable is: %s\n", left_value->array);
            sadd(&incoming_line, "string", left_value->array);
            
            // Check to make sure the correct data is going into the correct data type.
            if (strcmp(value_state->array, data_type)) {
                printf("Value State: %s and Variable Declaration: %s\n", value_state->array, data_type);
                // In AScript, doubles and floats are treated the same as well as char and strings (For now). Make sure to check.
                if ((strcmp(value_state->array, "double") && strcmp(data_type, "float")) &&
                    (strcmp(value_state->array, "string") && strcmp(data_type, "char"))) {
                    printf("ERROR: The datatypes do not match one another.\n");
                    sclear(&left_value);
                    sclear(&right_value);
                    sclear(&incoming_line);
                }
            }
        } else {
            printf("ERROR: The value for the incoming variable is empty.\n");
        }

        // Free the strings.
        sfree(&left_value);
        sfree(&right_value);
        sfree(&value_state);
        sfree(&user_string_variable);
\
        return;

}

int parsing_algorithm(FILE * source_fp, Var_Linked_List * variables, Func_Linked_List * functions, int curly_brace_index, char * function_purpose) {

    int comment_flag = 0;                           // Used as a flag to tell the algoirthm to ignore everything in a comment section, minus $variables.
    int datatype_flag = 0;
    int keyword_flag = 0;
    int incrementor_flag = 0;
    int decrementor_flag = 0;
    int current_line_number = 0;                    // Used for debugging to indicate where the error happend on the given line.
    int curly_brace_iterator = 0;
    int number_of_variables = 0;

    char current_character;                         // Used to hold each character of every every line being processed so that I don't have to type this every time "incoming_line->array".
    char source_file_line[256];                     // Used to capture each line looped through the given AScript file.
    
    char datatypes[5][7] = {"char", "double", "float", "int", "string"};
    char keywords[9][9] = {"and", "break", "return", "for", "while", "if", "else", "or", "continue"};

    string * current_word = salloc(15, 10);         // Used to hold a complete word so that the algorithm can guess at what the current line is about the execute.
    string * current_line = salloc(256, 10);        // Used to store and process each captured line within the AScript file.
    string * data_type_buffer = salloc(10, 5);             // Used to store the data type.
    string * function_or_variable_name = salloc(20, 5);         // Used to store the vairable name.

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
 
                // Check to see if the coment flag is off, if not then ignore everything until one is found.
                if (!comment_flag) {
                    // Check to see if the current character is a space or opening parenthese.
                    if (current_character == 32) {

                        // Grab to see what the letter is of the captured word and overwrite the space (not needed).
                        current_character = current_word->array[0];

                        // Check for data types 'char', 'double', 'float, string, int
                        if (current_character == 'c' || current_character == 'd' || current_character == 'f' ||
                            current_character == 's' || current_character == 'i') {
                            for (int j = 0; j < 5; ++j) {
                                if (!strcmp(current_word->array, datatypes[j])) {
                                    if (ascript_core_debugger_var) printf("Debug: Found a datatype: %s\n", datatypes[j]);
                                    sadd(&data_type_buffer, "string", datatypes[j]);
                                    break;
                                }
                            }

                            if (data_type_buffer->current_num_col) {
                                sclear(&current_word);
                                continue;
                            }
                        }

                        // Check for AScript Keywords.
                        if (current_character == 'e' || current_character == 'f' || 
                            current_character == 'i' || current_character == 'r' || current_character == 'w' ||
                            current_character == 'c' || current_character == 'a') {
                            for (int j = 0; j < 9; ++j) {
                                // Check to see if any of the data types match the one passed in.
                                if (!strcmp(current_word->array, keywords[j])) {
                                    if (ascript_core_debugger_var) printf("Debug: Found a keyword: %s\n", keywords[j]);
                                    // Set the found data type flag if one was found.
                                    keyword_flag = 1;
                                    sclear(&current_word);
                                    break;
                                }
                            }
                            // Need to do something about the found given keyword, etc.
                            // Since there is nothing else to do, go back to the top.
                            if (keyword_flag) {
                                sclear(&current_word);
                                continue;
                            }
                        }

                        // Check to see if the current line is trying to increase or decrease the interger value.
                        if (incrementor_flag || decrementor_flag) {
                            // See / Grab the variable out of the linked list of known variables.
                            Var_Linked_List_Node * temp = var_ll_search(variables, current_word->array);
                            if (temp) {
                                // Check to see if the variable is of type int.
                                if (!strcmp(temp->variable_data_type, "int")) {
                                    // Convert the char pointer to an int.
                                    int value = atoi(temp->variable_data_type);
                                    if (incrementor_flag) 
                                        value++;
                                    else 
                                        value--;
                                    // Reset the char pointer for the new number.
                                    memset(temp->data, 0, strlen(temp->data));
                                    // Repopulate said char pointer with new number.
                                    sprintf(temp->data, "%d", value);
                                } else {
                                    // *** Need to prpoerly clean up and do shit ***
                                    if (ascript_core_debugger_var) printf("ERROR: Trying to increment variable that is not of type int.");
                                    return 1;
                                }
                            }
                        // Check to see if the current line is updating a variable.
                        } else {
                            // Save the function or variable name to the buffer.
                            // This will be used for either a function or variable.
                            sadd(&function_or_variable_name, "string", current_word->array);
                        }

                        // Reset the current word on each space.
                        sclear(&current_word);

                        // Since there is nothing else to do, go back to the top.
                        continue;
                    // Check to see if the current character is a double quote aka comment.
                    } else if (current_character == 34) {
                        if (ascript_core_debugger_var) printf("DEBUG: Found a double quote, turning the comment flag on.\n");
                        // Turn the comment flag on.
                        comment_flag = 1;
                    // Check to see if the current character is an opening parenthese.
                    } else if (current_character == 40) {

                        continue;
                    // Check to see if the current character is an addition symbol.
                    } else if (current_character == 43) {

                        continue;
                    // Check to see if the current character is a minus symbol.
                    } else if (current_character == 45) {

                        continue;
                    // Check to see if the current character is a semicolon.
                    } else if (current_character == 59) {
                        if (ascript_core_debugger_var) printf("DEBUG: Found a semicolon, reseting / checking variables.\n");
                        // Need to check to see what was not wrapped up in the current line.
                        // Check flags to make sure that nothing was left open on the current line.
                        // Reset variables to process the next line.
                        continue;
                    // Check to see if the current character is an equal symbol.
                    } else if (current_character == 61) {
                        // Cut the rest of the line from the char pointer.
                        schar_group_delete(&current_line, 0, i + 1);
                        // Remove the beginning and ending spaces, if there are any.
                        sremove_leading_and_trailing_spaces(&current_line);
                        // Check to see if the variable is already in our list of variables.
                        printf("Looking for the following variable: %s\n", function_or_variable_name->array);
                        Var_Linked_List_Node * tmp = var_ll_search(variables, function_or_variable_name->array);
                        if (tmp) {
                            if (ascript_core_debugger_var) printf("Updating an existing variable.\n");
                            // Send the rest of the current line pointer to the evaluate variable value.
                            evaluate_variable_value(&variables, functions, tmp->variable_data_type, current_line);
                            printf("HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEE: %s and %d\n", current_line->array, current_line->current_num_col);
                            if (current_line->current_num_col) {
                                // Clear out the old data in the variable.
                                memset(tmp->data, 0, strlen(tmp->data));
                                // Over write the data to the old variable.
                                strcpy(tmp->data, current_line->array);
                            } else {
                                return 1;
                            }
                        } else {
                            if (ascript_core_debugger_var) printf("Adding a new variable.\n");
                            // Send the rest of the current line pointer to the evaluate variable value.
                            evaluate_variable_value(&variables, functions, data_type_buffer->array, current_line);
                            printf("HEREEEEEEEEEEEEEEEEEEEEEEEEEEEEEE: %s and %d\n", current_line->array, current_line->current_num_col);
                            if (current_line->current_num_col) {
                                // Push to the linked list with the new variable data.
                                var_ll_push(&variables, function_or_variable_name->array, data_type_buffer->array, current_line->array);
                                sclear(&data_type_buffer);
                            } else {
                                return 1;
                            }
                        }   

                        sclear(&function_or_variable_name);
                        break;
                    // Check to see if the current character is an opening curly braces.
                    } else if (current_character == 123) {
                        continue;
                    // If there are no special characters found or have meaning then add the character to the buffer.
                    } else {
                        sadd(&current_word, "char", current_character);
                    }
                // Check to see if the current character is a dobule quote aka comment.
                } else if (current_character == 34) {
                    if (ascript_core_debugger_var) printf("DEBUG: Found a double quote, turning the comment flag off.\n");
                    // If so, turn the comment flag off.
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
    sfree(&data_type_buffer);
    sfree(&function_or_variable_name);

    func_ll_cleanup(&functions);
    printf("Aaron\n");
    var_ll_cleanup(&variables);
    printf("Valoroso\n");

    if (ascript_core_debugger_var) printf("DEBUG: Leaving the AScript main function.\n");

    return 0;
}
