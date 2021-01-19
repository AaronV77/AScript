  /**  
    * @file
    * @author Aaron Valoroso <Aaron.A.Valoroso@erdc.dren.mil>
    * @date August 27th, 2020
    *
    * @section Description
    *
    * This file contains source code for a calculator that follows the PEMDAS protocol / operatorn.

    Helpful URL's:
    - https://www.tutorialspoint.com/data_structures_algorithms/tree_traversal.htm#:~:text=values%20it%20contains.-,In%2Dorder%20Traversal,values%20in%20an%20ascending%20order.
    - https://www.geeksforgeeks.org/tree-traversals-inorder-preorder-and-postorder/
*/

#include "calc_and_if.h"

int calc_debugger_value = 0;

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take the two buffers and operator to perform a calculation that needs 
 * to be accomplished. There are only five acceptable operators (^,*,/,+,-). The passed in
 * number buffers are converted to doubles.
 *
 * \param[in] left_number: is a string structure that holds the left side number of the operator.
 * \param[in] right_number: is a string structure that holds the right side number of the operator.
 * \param[in] operator: is a char value that stores the operator for the arithmetic.
 * \param[out] return: will be a double value of the end result of the arithmetic that was done.
 */
double arithmetic(string * left_number, string * right_number, char operator) {

    double return_value = 0.0;
    double current_left_number = atof(left_number->array);
    double current_right_number = atof(right_number->array);

    // Get the priority or weight of the current operator found.
    switch(operator) {
        case 42 :
            // printf("Found the (42) multiplication operator.\n");
            return_value = current_left_number * current_right_number;
            break;
        case 43 :
            // printf("Found the (43) addition operator.\n");
            return_value = current_left_number + current_right_number;
            break;
        case 45 :
            // printf("Found the (45) subtraction operator.\n");
            return_value = current_left_number - current_right_number;
            break;
        case 47 :
            // printf("Found the (47) division operator.\n");
            return_value = current_left_number / current_right_number;
            break;
        case 94 :
            // printf("Found the (94) exponent operator.\n");
            return_value = powf(current_left_number, current_right_number);
            break;
    };

    return return_value;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will convert the passed in number buffers and convert them to a double value and perform
 * a comparison operation on them depending on the passed in operator. This function is pretty straight 
 * forward, because some type of comparison is accomplished for a given comparison operator or logic
 * statement. The return value for this function is either a one or zero. 
 *
 * \param[in] left_number: is a string structure that holds the left side number of the operator.
 * \param[in] right_number: is a string structure that holds the right side number of the operator.
 * \param[in] operator: is a char value that stores the comparison operator or logic statement for the comparison.
 * \param[out] return: will be a double value of the end result of the arithmetic that was done.
 */
int comparison(string * left_number, string * right_number, string * operator) {

    int return_value = 0;
    double current_left_number = atof(left_number->array);
    double current_right_number = atof(right_number->array);

    if (!strcmp(operator->array, "==")) {
        if (current_left_number == current_right_number)
            return_value = 1;
        else
            return_value = 0;
    } else if (!strcmp(operator->array, "!=")) {
        if (current_left_number != current_right_number)
            return_value = 1;
        else
            return_value = 0;
    } else if (!strcmp(operator->array, ">")) {
        if (current_left_number > current_right_number)
            return_value = 1;
        else
            return_value = 0;
    } else if (!strcmp(operator->array, "<")) {
        if (current_left_number < current_right_number)
            return_value = 1;
        else
            return_value = 0;
    } else if (!strcmp(operator->array, ">=")) {
        if (current_left_number >= current_right_number)
            return_value = 1;
        else
            return_value = 0;
    } else if (!strcmp(operator->array, "<=")) {
        if (current_left_number <= current_right_number)
            return_value = 1;
        else
            return_value = 0;
    } else if (!strcmp(operator->array, "or")) {
        if (current_left_number != 0 || current_right_number != 0) {
            return 1;
        } else {
            return 0;
        }
    } else if (!strcmp(operator->array, "and")) {
        if (current_left_number != 0 && current_right_number != 0) {
            return 1;
        } else {
            return 0;
        }
    }

    return return_value;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will loop through the incoming line and generate a double value result of either an if
 * statement line or a calculator line. How this algorithm works is by loop the incoming sequence of 
 * passed in characters and parsing through each one individually. For lines that contain a normal 
 * calculator sequence, the algorithm will loop through three priority levels. These levels consist of 
 * '(), ^', '*,/', and '+,-'. For lines that contain a normal if statementt sequence the line will only
 * have to loop once, due to everything being evaluated when it is found. If there is a combination of 
 * the two lines then both, then the algorithm will loop for the max of three times. The algorithm parses
 * the incoming line from left to right, and saving numbers into left and right buffers. When both are full
 * the algorithm performs either a calculation, comparison, or logic statement. If an operator is found
 * that is not of the current priority then the left number buffer and operator are saved to a temporary 
 * buffer that stores all the ignored operators, numbers and stored results. When the algorithm is on 
 * the last character of the loop, if there are no more operators found then the loop exits, else the 
 * for loop iterator is reset, the temporary line is copied back to the processing line, and the temporary
 * line is reset. The biggest hurdle of the algorithm is opening and closing parentheses and when they are
 * nested within each other. When the opening parenthese character is found the algorithm will become 
 * recurisve and process everything within just the pair of parentheses. This is accomplished by finding 
 * a closing parenthese character and truncating everything else in the passed in line. Due to the algoirthm
 * editting the passed in line, a copy of the line is needed to be save back to the parsed line. To ignore 
 * the characters within the parentheses the algorithm will ignore everything until the next closing parenthese
 * character is found. For nested parenthese pairs, the previous algorithm has a hitch. For the first pair of
 * parentheses the saved line will contain all the nested pairs of parentheses, which when the algorithm
 * ignores everything, the wrong first closing parenthese will be found. To overcome this fault, I've made
 * it so that the closing parenthese flag gets incremented with ever opening parenthese character that gets
 * found when the algorithm is ignore characters, and for every closing parenthese character that gets found
 * the algorithm will decrement this iterator. This will make sure that the algorithm ignores everything within
 * the original pair of paremtheses and starts processing the incoming line at the correct location. This is the
 * general overview of how this function works.
 *
 * \param[in] incoming_line: is a string structure from the user filled with parentheses.
 * \param[in] string_position: is an integer value that will hold the position of the incoming_line.
 * \param[out] return (on success): will be a double value of the final value.
 * \param[out] return (on failure): will be a 0 value. **Careful** I know that 0 is a bad retun value, but did it for simplicity.
 */
double calculator_and_if_comparison(string * incoming_line, int string_position) {

    char operator = '\0';                                   // Used for storing the current operator that is found within the line.
    int last_char_flag = 0;                                 // Used to signify throughout the code that the algorithm is on the last character.
    int operator_flag = 0;                                  // Used to tell if there are any operators left in the cycled line.
    int operator_weight = 0;                                // Used for storing the current operator weight.
    int exclamation_point_flag = 0;                         // Used to tell the algorithm when an exclamation point has been found.
    int closing_parenthese_flag = 0;                        // Used to signal when the algorithm has to loop through the current line setting all the data to a space before it can start processing data again.
    int left_number_capture_flag = 1;                       // Used to switch between storing numbers into two buffers.
    int operator_priority_iteator = 0;                      // Used to loop through the operator priority levels.
    int operator_priority_level_two_flag = 0;               // After the first loop through the incoming line, if any operators of this nature were found in the first scan.
    int operator_weight_priority_array[3] = {3, 2, 1};      // Used to store the operator priorty levels to loop through.
    double return_value = 0.0;                              // Used to store the return value if an operning and closing parethense was found.

    // Store numbers for the left side of the operator.
    string * left_number = salloc(10, 5);
    // Store numbers for the right side of the operator.
    string * right_number = salloc(10, 5);
    // Store the logic statments, which are (and, or).
    string * logic_statment = salloc(10, 5);
    // Store the comparison operators (>, <, =).
    string * comparison_operator = salloc(10, 5);
    // Create a string structure for holding the results and ingored arithmetic.
    string * temporary_line = salloc(incoming_line->current_num_col + 1, 5);

    // Lopp through the current string.
    for (int j = string_position; j < incoming_line->current_num_col; ++j) {

        if (calc_debugger_value) printf("The incoming line is: %s\n", incoming_line->array);
        if (calc_debugger_value) printf("The operator priortity is: %d\n", operator_weight_priority_array[operator_priority_iteator]);

        // Determine if this loop is on the last character in the string, if so turn the flag on.
        if ((j + 1) == incoming_line->current_num_col) last_char_flag = 1;

        if (!closing_parenthese_flag) {
            // Check to see if the closing parenthese flag is turned off. If the flag is on then the
            // - algorithm is looping through the current line clearing out the data until the closing 
            // - parenthese has been found
            if ((incoming_line->array[j] >= 48 && incoming_line->array[j] <= 57) || incoming_line->array[j] == 46) {
                // Look for Numbers and Decimals

                if (left_number_capture_flag) {
                    // Check to see if we are filling the left side of the operator.
                    if (calc_debugger_value) printf("Storing a number to the left array: %c\n", incoming_line->array[j]);
                    sadd(&left_number, "char", incoming_line->array[j]);
                } else {
                    // Check to see if we are filling the right side of the operator.
                    if (calc_debugger_value) printf("Storing a number to the right array: %c\n", incoming_line->array[j]);
                    sadd(&right_number, "char", incoming_line->array[j]);

                    if (last_char_flag) {
                        // Check to see if we are on the last character in the string.                }

                        if (exclamation_point_flag) {
                            // Check to see an exclamation flag was found.
                            if (calc_debugger_value) printf("Found an exclamation point (1).\n");
                            // If the right number string contains digitis.
                            // Convert the string of digits to a double value.
                            return_value = atof(right_number->array);
                            // Reset the right number string.
                            sclear(&right_number);
                            // If the string contains digits greater than or less than zero, store a zero in the left number
                            // - else store a value of one.
                            if (return_value) sadd(&right_number, "int", 0);
                            else sadd(&right_number, "int", 1);
                            // Reset the exclamation point flag back to false.
                            exclamation_point_flag = 0;
                        }
                        
                        if (left_number->current_num_col && right_number->current_num_col) {
                            // Check to see if both number buffers contain digits.
                            // Do arithmetic on the saved buffers and (operator or logic).
                            if (logic_statment->current_num_col) {
                                // Check to see if a logic statement was found.
                                // Send the logic statement and two numbers to get a one or zero back. (1 == true, 0 == false).
                                return_value = comparison(left_number, right_number, logic_statment);
                                // Clear the logic statementout.
                                sclear(&logic_statment);
                                if (calc_debugger_value) printf("Doing logic comparison (1) and the result is: %f\n", return_value);
                            } else if (comparison_operator->current_num_col) {
                                // Check to see if a comparison operator was found.
                                // Send the logic statement and two numbers to get a one or zero back. (1 == true, 0 == false).
                                return_value = comparison(left_number, right_number, comparison_operator);
                                sclear(&comparison_operator);
                                if (calc_debugger_value) printf("Doing extended operator arithmetic (1) and the result is: %f\n", return_value);
                            } else {
                                // Send the operator and two numbers to get a double value back. 
                                return_value = arithmetic(left_number, right_number, operator);
                                // Decrement the operator flag because the algorithm has consumed an operator.
                                operator_flag--;
                                // Reset the operator back to empty.
                                operator = '\0';
                                if (calc_debugger_value) printf("Doing arithmetic (1) and the result is: %f\n", return_value);
                            }

                            // Clear out the buffers for new incoming numbers.
                            sclear(&left_number);
                            sclear(&right_number);

                            // Save the results of the arithmetic to the left side for the next arithmetic.
                            sadd(&left_number, "double", return_value);

                            // Set the flag for storing digits in the left side on, this will be turned of
                            // - at the end of the previous if statment.
                            left_number_capture_flag = 1;
                        }
                    }
                }
            } else if (incoming_line->array[j] == 40) {
                // Looking for an opening parentheses
                if (calc_debugger_value) printf("Found an opening parenthese\n");

                if (last_char_flag) {
                    // Check to see if this is the last character, which means that there is no closing parenthese, which is an error.
                    printf("ERROR: There was an opening parenth at the end of the incoming line..\n");
                    sfree(&left_number);
                    sfree(&right_number);
                    sfree(&logic_statment);
                    sfree(&temporary_line);
                    sfree(&comparison_operator);
                    return 0;
                }

                // Create a temporary string structure to save the current line being looped through.
                // - The algorithm will wipe out the passed in line, due to the algorithm.
                string * temp_incoming_line = salloc(incoming_line->current_num_col + 1, 5);
                sadd(&temp_incoming_line, "string", incoming_line->array);

                // Pass the iterator to the function with a plus one, so that the function is not looking at the open parenthese character.
                return_value = calculator_and_if_comparison(incoming_line, j + 1);
                if (calc_debugger_value) printf("The return value from the recursive call is: %f\n", return_value);
                if (calc_debugger_value) printf("The incoming line is: %s\n", incoming_line->array);

                // When an opening parenthese is found then set the closing parenthese flag to true.
                closing_parenthese_flag = 1;

                if (exclamation_point_flag) {
                    // Check to see if an exclamation point was found.
                    if (calc_debugger_value) printf("Found an exclamation point (2).\n");
                    if (!left_number->current_num_col) {
                        // Check to see if the left number is empty to store the return value.
                        // If the return value is greater than or less than zero, store a zero in the left number
                        // - else store a value of one.
                        if (return_value) sadd(&left_number, "int", 0);
                        else sadd(&left_number, "int", 1);
                    } else if (!right_number->current_num_col) {
                        // Check to see if the right number is empty to store the return value.
                        // If the return value is greater than or less than zero, store a zero in the left number
                        // - else store a value of one.
                        if (return_value) sadd(&right_number, "int", 0);
                        else sadd(&right_number, "int", 1);
                    }
                    // Reset the exclamation point flag back to false.
                    exclamation_point_flag = 0;
                }

                // Check to see if the left buffer is empty for saving the result, else try the right buffer. If all else fails, error out.
                if (!left_number->current_num_col)
                    sadd(&left_number, "double", return_value);
                else if (!right_number->current_num_col)
                    sadd(&right_number, "double", return_value);
                else {
                    printf("ERROR: There is no room to stick the return value into one of the numbers.\n");
                    sfree(&left_number);
                    sfree(&right_number);
                    sfree(&logic_statment);
                    sfree(&temporary_line);
                    sfree(&comparison_operator);
                    sfree(&temp_incoming_line);
                    return 0;
                }

                // Clear out the new filled incoming line string structure.
                sclear(&incoming_line);
                // Add the saved backup of the current looped line back to the string structure.
                sadd(&incoming_line, "string", temp_incoming_line->array);
                // Free away the temporary string structure.
                sfree(&temp_incoming_line);
            } else if (incoming_line->array[j] == 41) {
                // Closing parentheses
                if (calc_debugger_value) printf("Found a closing parenthese\n");
                // If a closing parenthese has been found in the recurisve call then set the last_char_flag
                // - to one so that the string is cut here and what is inside of the parentheses can only 
                // - be processed by the algorithm.
                last_char_flag = 1;
            } else if (incoming_line->array[j] == 33) {
                // Check for incoming exclamation points.
                if (calc_debugger_value) printf("Found an exclamation point (3).\n");
                // Set the exclamation point flag to true.
                exclamation_point_flag = 1;
            } else if (incoming_line->array[j] == 42 ||
                    incoming_line->array[j] == 43 ||
                    incoming_line->array[j] == 45 ||
                    incoming_line->array[j] == 47 ||
                    incoming_line->array[j] == 94) {           
                // Looking for the operators: +,-,/,*,^
                if (calc_debugger_value) printf("Found an operator: %c\n", incoming_line->array[j]);

                if (comparison_operator->current_num_col || comparison_operator->current_num_col) {
                    // Check to make sure that a comparsion operator or logic statement has been found.
                    printf("ERROR: There was an comparison operator or a logic statment found.\n");
                    sfree(&left_number);
                    sfree(&right_number);
                    sfree(&logic_statment);
                    sfree(&temporary_line);
                    sfree(&comparison_operator);
                    return 0;
                }

                if (left_number->current_num_col && right_number->current_num_col) {
                    // Check to see if both buffers contain digits. This if statment will run when both buffers 
                    // - are full and an operator was found, before the algorithm can process the next operator.

                    // Do arithmetic on the saved buffers and operator.
                    return_value = arithmetic(left_number, right_number, operator);
                    if (calc_debugger_value) printf("Doing arithmetic (2) and the result is: %f\n", return_value);

                    // Clear out the buffers for new incoming numbers.
                    sclear(&left_number);
                    sclear(&right_number);
                    // Save the results of the arithmetic to the left side for the next arithmetic.
                    sadd(&left_number, "double", return_value);
                    // Decrement the operator flag because the algorithm has consumed an operator.
                    operator_flag--;
                    // Reset the operator back to empty.
                    operator = '\0';
                    // Set the flag for storing digits in the left side on, this will be turned of
                    // - at the end of the previous if statment.
                    left_number_capture_flag = 1;
                }

                if ((incoming_line->array[j]) == 45 && ((operator != '\0') || !left_number->current_num_col)) {
                    // Check to see if the incoming operator is '-' and determine if a previous operator has been
                    // - found. This if statment is used to catch negative numbers. The check to see if an operator
                    // - has already been found is for when a negative number is next to an operator in the incoming
                    // - line. The check to see if the left buffer is empty, indicates a negative number was the first
                    // - character found in the incoming line.
                    if (calc_debugger_value) printf("Found a negative symbol.\n");

                    // Check to see if the algorithm is storing numbers for either the left or right buffer.
                    if (left_number_capture_flag)
                        sadd(&left_number, "char", incoming_line->array[j]);
                    else {
                        sadd(&right_number, "char", incoming_line->array[j]);
                    }
                } else {
                    // Increment the operator flag to tell the algorithm that there operators still in the string.
                    operator_flag++;

                    // Get the priority or weight of the current operator found.
                    switch(incoming_line->array[j]) {
                        case 42 :
                            // printf("Found the (42) multiplication operator.\n");
                            operator_weight = 2;
                            break;
                        case 43 :
                            // printf("Found the (43) addition operator.\n");
                            operator_weight = 1;
                            break;
                        case 45 :
                            // printf("Found the (45) subtraction operator.\n");
                            operator_weight = 1;
                            break;
                        case 47 :
                            // printf("Found the (47) division operator.\n");
                            operator_weight = 2;
                            break;
                        case 94 :
                            // printf("Found the (94) exponent operator.\n");
                            operator_weight = 3;
                            break;
                    };

                    if (operator_weight == operator_weight_priority_array[operator_priority_iteator]) {
                        // Check to see if the current operator weight matches the current priority level. The 
                        // - algorithm cannot process all the operators at once. The algorithm has to start with
                        // - the highest level first '() and ^', then '*,/', and then '+,-';
                        if (calc_debugger_value) printf("Found the correct operator...\n");
                        // Save the foudnd operator.
                        operator = incoming_line->array[j];
                    } else {
                        // If the found operator is not of the current priority

                        // Save the left side buffer to the temporary line file.
                        sadd(&temporary_line, "string", left_number->array);
                        // Save the operator that was just found
                        sadd(&temporary_line, "char", incoming_line->array[j]);
                        // Clear the left buffer out.
                        sclear(&left_number);
                        // Set the left buffer off, so that the next if statment will turn it back on again.
                        left_number_capture_flag = 0;
                    }

                    // Check to see if the left buffer flag is off, if so turn it on and vice versa.
                    if (!left_number_capture_flag)
                        left_number_capture_flag = 1;
                    else
                        left_number_capture_flag = 0;
                }

                // Check to see if there are any (*,/) operators.
                if (operator_weight == 2)
                    operator_priority_level_two_flag++;;

            } else if (incoming_line->array[j] == 60 ||
                       incoming_line->array[j] == 61 ||
                       incoming_line->array[j] == 62) {           
                // Looking for the operators: <, >, =, !, -
                if (calc_debugger_value) printf("Found an extended operator: %c\n", incoming_line->array[j]);

                if (logic_statment->current_num_col || operator != '\0') {
                    // Check to make sure that a logic statement or operator has been found.
                    printf("ERROR: There was an operator or a logic statment found.\n");
                    sfree(&left_number);
                    sfree(&right_number);
                    sfree(&logic_statment);
                    sfree(&temporary_line);
                    sfree(&comparison_operator);
                    return 0;
                }

                if (exclamation_point_flag) {
                    // Check to see an exclamation point has been found.
                    if (calc_debugger_value) printf("Found an exclamation point (5).\n");
                    if (left_number_capture_flag) {
                        // Check to see if the left number stored the last found number.
                        // Convert the string of digits to a double value.
                        return_value = atof(left_number->array);
                        // Reset the left number string.
                        sclear(&left_number);
                        // If the string contains digits greater than or less than zero, store a zero in the left number
                        // - else store a value of one.
                        if (return_value) sadd(&left_number, "int", 0);
                        else sadd(&left_number, "int", 1);
                    } else {
                        // Check to see if the right number stored the last found number.
                        // Convert the string of digits to a double value.
                        return_value = atof(right_number->array);
                        // Reset the right number string.
                        sclear(&right_number);
                        // If the string contains digits greater than or less than zero, store a zero in the left number
                        // - else store a value of one.
                        if (return_value) sadd(&right_number, "int", 0);
                        else sadd(&right_number, "int", 1);
                    }
                    // Reset the exclamation flag to false.
                    exclamation_point_flag = 0;
                }

                if (left_number->current_num_col && right_number->current_num_col) {
                    // Check to see if both buffers contain digits. This if statment will run when both buffers 
                    // - are full and an operator was found, before the algorithm can process the next operator.

                    // Do arithmetic on the saved buffers and (operator or logic).
                    if (logic_statment->current_num_col) {
                        // Check to see if a logic statement was found.
                        // Send the logic statement and two numbers to get a one or zero back. (1 == true, 0 == false).
                        return_value = comparison(left_number, right_number, logic_statment);
                        // Clear the logic statementout.
                        sclear(&logic_statment);
                        if (calc_debugger_value) printf("Doing logic comparison (1) and the result is: %f\n", return_value);
                    } else if (comparison_operator->current_num_col) {
                        // Check to see if a comparison operator was found.
                        // Send the logic statement and two numbers to get a one or zero back. (1 == true, 0 == false).
                        return_value = comparison(left_number, right_number, comparison_operator);
                        sclear(&comparison_operator);
                        if (calc_debugger_value) printf("Doing extended operator arithmetic (1) and the result is: %f\n", return_value);
                    } else {
                        // Send the operator and two numbers to get a double value back. 
                        return_value = arithmetic(left_number, right_number, operator);
                        // Decrement the operator flag because the algorithm has consumed an operator.
                        operator_flag--;
                        // Reset the operator back to empty.
                        operator = '\0';
                        if (calc_debugger_value) printf("Doing arithmetic (1) and the result is: %f\n", return_value);
                    }

                    // Clear out the buffers for new incoming numbers.
                    sclear(&left_number);
                    sclear(&right_number);

                    // Save the results of the arithmetic to the left side for the next arithmetic.
                    sadd(&left_number, "double", return_value);

                    // Set the flag for storing digits in the left side on, this will be turned of
                    // - at the end of the previous if statment.
                    left_number_capture_flag = 1;
                }

                if (!comparison_operator->current_num_col) {
                    // Check to see if any comparison operators have been found. This if statement 
                    // - should only activate once for each comparison operator.
                    // Check to see if the left buffer flag is off, if so turn it on and vice versa.
                    if (!left_number_capture_flag)
                        left_number_capture_flag = 1;
                    else
                        left_number_capture_flag = 0;
                }

                sadd(&comparison_operator, "char", incoming_line->array[j]);

                if (comparison_operator->current_num_col > 2) {
                    // Check to see if the number of operators that have been collected is greater thatn two.
                    printf("ERROR: There are one to many operators that have been found.\n");
                    sfree(&left_number);
                    sfree(&right_number);
                    sfree(&logic_statment);
                    sfree(&temporary_line);
                    sfree(&comparison_operator);
                    return 0;
                }
            } else if (incoming_line->array[j] == 97 ||
                       incoming_line->array[j] == 100 ||
                       incoming_line->array[j] == 110 ||
                       incoming_line->array[j] == 111 ||
                       incoming_line->array[j] == 114) {
                // Looking for these characters a, n, d, o, and r. 
                if (calc_debugger_value) printf("Found part of a logic statement: %c\n", incoming_line->array[j]);

                if (comparison_operator->current_num_col || operator != '\0') {
                    // Check to make sure that a comparsion operator or operator has been found.
                    printf("ERROR: There was an operator or a comparison operator found.\n");
                    sfree(&left_number);
                    sfree(&right_number);
                    sfree(&logic_statment);
                    sfree(&temporary_line);
                    sfree(&comparison_operator);
                    return 0;
                }

                if (exclamation_point_flag) {
                    // Check to see an exclamation point has been found.
                    if (calc_debugger_value) printf("Found an exclamation point (5).\n");
                    if (left_number_capture_flag) {
                        // Check to see if the left number stored the last found number.
                        // Convert the string of digits to a double value.
                        return_value = atof(left_number->array);
                        // Reset the left number string.
                        sclear(&left_number);
                        // If the string contains digits greater than or less than zero, store a zero in the left number
                        // - else store a value of one.
                        if (return_value) sadd(&left_number, "int", 0);
                        else sadd(&left_number, "int", 1);
                    } else {
                        // Check to see if the right number stored the last found number.
                        // Convert the string of digits to a double value.
                        return_value = atof(right_number->array);
                        // Reset the right number string.
                        sclear(&right_number);
                        // If the string contains digits greater than or less than zero, store a zero in the left number
                        // - else store a value of one.
                        if (return_value) sadd(&right_number, "int", 0);
                        else sadd(&right_number, "int", 1);
                    }
                    // Reset the exclamation flag to false.
                    exclamation_point_flag = 0;
                }

                if (left_number->current_num_col && right_number->current_num_col) {
                    // Do arithmetic on the saved buffers and (operator or logic).
                    if (logic_statment->current_num_col) {
                        // Check to see if a logic statement was found.
                        // Send the logic statement and two numbers to get a one or zero back. (1 == true, 0 == false).
                        return_value = comparison(left_number, right_number, logic_statment);
                        // Clear the logic statementout.
                        sclear(&logic_statment);
                        if (calc_debugger_value) printf("Doing logic comparison (1) and the result is: %f\n", return_value);
                    } else if (comparison_operator->current_num_col) {
                        // Check to see if a comparison operator was found.
                        // Send the logic statement and two numbers to get a one or zero back. (1 == true, 0 == false).
                        return_value = comparison(left_number, right_number, comparison_operator);
                        sclear(&comparison_operator);
                        if (calc_debugger_value) printf("Doing extended operator arithmetic (1) and the result is: %f\n", return_value);
                    } else {
                        // Send the operator and two numbers to get a double value back. 
                        return_value = arithmetic(left_number, right_number, operator);
                        // Decrement the operator flag because the algorithm has consumed an operator.
                        operator_flag--;
                        // Reset the operator back to empty.
                        operator = '\0';
                        if (calc_debugger_value) printf("Doing arithmetic (1) and the result is: %f\n", return_value);
                    }
                     // Clear out the buffers for new incoming numbers.
                    sclear(&left_number);
                    sclear(&right_number);

                    // Save the results of the arithmetic to the left side for the next arithmetic.
                    sadd(&left_number, "double", return_value);

                    // Set the flag for storing digits in the left side on, this will be turned of
                    // - at the end of the previous if statment.
                    left_number_capture_flag = 1;
                }

                if (!logic_statment->current_num_col) {
                    // Check to see if any comparison operators have been found. This if statement 
                    // - should only activate once for each comparison operator.
                    // Check to see if the left buffer flag is off, if so turn it on and vice versa.
                    if (!left_number_capture_flag)
                        left_number_capture_flag = 1;
                    else
                        left_number_capture_flag = 0;
                }

                if (logic_statment->current_num_col > 3) {
                    printf("ERROR: There is one to many letters of the given logic statement: %s\n", logic_statment->array);
                    sfree(&left_number);
                    sfree(&right_number);
                    sfree(&logic_statment);
                    sfree(&temporary_line);
                    sfree(&comparison_operator);
                    return 0;
                }

                sadd(&logic_statment, "char", incoming_line->array[j]); 
            } else {
                // If there is anything other than a decimal, number, logic statement, comparison operator, or an operator then exit.
                printf("ERROR: There was a character or symbol found that does not belong in this calculator: %d\n", incoming_line->array[j]);
                sfree(&left_number);
                sfree(&right_number);
                sfree(&logic_statment);
                sfree(&temporary_line);
                sfree(&comparison_operator);
                return 0;
            }
        } else if (incoming_line->array[j] == 40) {
            // Check for an open parenthese when the closing parenthese flags is turned on. 
            // The reason for this if statement is due to nested parentheses. The algorithm will save the original
            // - line which on nested pairs of parentheses, the first pair will save the original line which will 
            // - contain all the other nested pairs of parentheses. On just one pair, the copy of the line is fine,
            // - but with nested pairs the algoirthm will find the wrong first closing parenthese character.
            // - Incrementing the closing parenthese flag further will keep the algorithm from finding the wrong
            // - closing parenthese character and stopping the ingore sequence too soon. 
            closing_parenthese_flag++;
        } else if (incoming_line->array[j] == 41) {
            // Check to see if the incoming character of the current line is a closing parenthese.
            // - If it is then decrement the flag, on nested pairs of parentheses this if statement
            // - will keep decrementing the flag until all paris are found.
            closing_parenthese_flag--;
        }

        // Clear the already processed data to aid in debugging statements for readability.
        incoming_line->array[j] = 32;

        if (last_char_flag) {
            // If the algorithm loop is on the last character in the given string then determine if we can go onto the next 
            // - priority level or if the algorithm should return. 

            if (exclamation_point_flag) {
                // Check to see an exclamation point has been found.
                if (calc_debugger_value) printf("Found an exclamation point (5).\n");
                if (left_number_capture_flag) {
                    // Check to see if the left number stored the last found number.
                    // Convert the string of digits to a double value.
                    return_value = atof(left_number->array);
                    // Reset the left number string.
                    sclear(&left_number);
                    // If the string contains digits greater than or less than zero, store a zero in the left number
                    // - else store a value of one.
                    if (return_value) sadd(&left_number, "int", 0);
                    else sadd(&left_number, "int", 1);
                } else {
                    // Check to see if the right number stored the last found number.
                    // Convert the string of digits to a double value.
                    return_value = atof(right_number->array);
                    // Reset the right number string.
                    sclear(&right_number);
                    // If the string contains digits greater than or less than zero, store a zero in the left number
                    // - else store a value of one.
                    if (return_value) sadd(&right_number, "int", 0);
                    else sadd(&right_number, "int", 1);
                }
                // Reset the exclamation flag to false.
                exclamation_point_flag = 0;
            }

            if (left_number->current_num_col && right_number->current_num_col) {
                // Check to see if both number buffers contain digits.
                // Do arithmetic on the saved buffers and (operator or logic).
                if (logic_statment->current_num_col) {
                    // Check to see if a logic statement was found.
                    // Send the logic statement and two numbers to get a one or zero back. (1 == true, 0 == false).
                    return_value = comparison(left_number, right_number, logic_statment);
                    // Clear the logic statementout.
                    sclear(&logic_statment);
                    if (calc_debugger_value) printf("Doing logic comparison (1) and the result is: %f\n", return_value);
                } else if (comparison_operator->current_num_col) {
                    // Check to see if a comparison operator was found.
                    // Send the logic statement and two numbers to get a one or zero back. (1 == true, 0 == false).
                    return_value = comparison(left_number, right_number, comparison_operator);
                    sclear(&comparison_operator);
                    if (calc_debugger_value) printf("Doing extended operator arithmetic (1) and the result is: %f\n", return_value);
                } else {
                    // Send the operator and two numbers to get a double value back. 
                    return_value = arithmetic(left_number, right_number, operator);
                    // Decrement the operator flag because the algorithm has consumed an operator.
                    operator_flag--;
                    // Reset the operator back to empty.
                    operator = '\0';
                    if (calc_debugger_value) printf("Doing arithmetic (1) and the result is: %f\n", return_value);
                }
                // Clear out the buffers for new incoming numbers.
                sclear(&left_number);
                sclear(&right_number);

                // Save the results of the arithmetic to the left side for the next arithmetic.
                sadd(&left_number, "double", return_value);

                // Set the flag for storing digits in the left side on, this will be turned of
                // - at the end of the previous if statment.
                left_number_capture_flag = 1;
            }

            if (left_number->current_num_col) {
                // Check to see if there are digits in the left buffer. This if statment is for catching
                // - stray numbers and to store them into the temporary line for the next sequence of processing.
                if (calc_debugger_value) printf("Restoring the left number\n");
                // Add the numbers to the temporary line.
                sadd(&temporary_line, "string", left_number->array);
                // Clear out the left buffer.
                sclear(&left_number);
            }

            // Clear the current looped line to start a blank slate.
            sclear(&incoming_line);
            // Take all the results and saved arithmetic to the incoming line once more. This will hold any arithmetic
            // - that was done or anything that has not been processed. This will only have the operators that are left
            // - and the numbers needed on either side of a given number.
            sadd(&incoming_line, "string", temporary_line->array);
            // Clear out the temporary line for the next lopp of processing.
            sclear(&temporary_line);
            
            // If there are no more operators left in the string, then break from the last loop. No need to keep checking
            // - for operators when there is nothing left in the incoming line to be processed.
            if (!operator_flag)
                break; 
            
            // If the algorithm has exceeded the number of priority levels, then exit the for loop.
            if (operator_priority_iteator == 3)
                break;

            last_char_flag = 0;             // Reset the last character flag
            j = -1;                         // The loop iterator has to restart to -1 so that the incrementation will put it at zero in the upcoming iteration.
            left_number_capture_flag = 1;   // Turn the capture flag back on so that the left number gets populated first.
            operator_priority_iteator++;    // Increment the priority flag to process the next priority level.

            if (operator_priority_iteator == 0 && !operator_priority_level_two_flag) {
                // Check to see if the algorithm is on the first loop and to see if there are no (*,/) operators. This keeps 
                // - the algorithm from having to do extra processing than it has to. 
                if (calc_debugger_value) printf("There were no multiplication or division operators found, moving ahead.\n");
                // Increment the first loop iterator forward to the next priority.
                operator_priority_iteator++;
            }
        }
    }

    if (calc_debugger_value) printf("Calculating a result: %s...\n", incoming_line->array);

    // Free all the buffers.
    sfree(&left_number);
    sfree(&right_number);
    sfree(&temporary_line);
    sfree(&logic_statment);
    sfree(&comparison_operator);

    return atof(incoming_line->array);
}
 
//-----------------------------------------------------------------------------------------------------
/**
 * This function willjust turn on the library debugger for helpful output.
 */
void calculator_debugger() {
    calc_debugger_value = 1;
}
