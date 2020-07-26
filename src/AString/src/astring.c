/**
* @file
* @author Aaron Valoroso <Aaron.A.Valoroso@erdc.dren.mil>
* @date June 15th, 2020
*
* @section Description
*
* This file contains all the library functions for a string implementation in C.
*/
#include "astring.h"

int string_debugger_flag = 0;

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take in a string structure that will realloc the char pointer that is holding the 
 * string. The function will copy the contents of the char pointer in the string structure for safe keeping
 * because realloc cannot be expected to not change the memory / destroy the data inside of the pointer.
 * Once the contents are copied, the char pointer in the structure is re-sized and check for errors. From
 * there the char pointer in the string struct is "cleaned" and then the temporary char pointer contents 
 * are transfered back to the string structure. In the case that the re-size is to make the string smaller,
 * then only the amount of the resize minus one (for null terminator) is copied to the char pointer in 
 * the string structure.
 *
 * \param[in] array: is a string structure.
 * \param[in] new_col_size: is an integer that tells the function how many elements to resize the char pointer too. 
 * \param[out] return (on success): will be an integer value of zero.
 * \param[out] return (on failure): will be an integer value of one.
 */
int column_reallocation(string ** array, int new_col_size) {

	if (string_debugger_flag) printf("Entering the column_reallocation function.\n");

    char * copy = calloc(strlen((*array)->array) + 1, sizeof(char));
    strcpy(copy, (*array)->array);

    (*array)->array = realloc((*array)->array, new_col_size * sizeof(char));
    if (!(char*)(*array)->array) {
        printf("ERROR: There was an issue with the reallocation of your pointer.\n");
        free((char*)(*array)->array);
        return 1;
    }
    memset((*array)->array, 0, new_col_size * sizeof(char));
    strncpy((*array)->array, copy, ((new_col_size - 1) * sizeof(char)));
    free(copy);

	if (string_debugger_flag) printf("Leaving the column_reallocation function.\n");

	return 0;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take two integers that will tell the function how many characters the user wants
 * the string structure char pointer to have and how many characters to increase the char pointer by
 * when space runs out. If either integer is less than zero then a default value of zero is assigned to
 * either of them in the string structure.
 *
 * \param[in] array: is a string structure.
 * \param[in] number_of_columns: is an integer that holds how many elements need to be in the char pointer.
 * \param[in] col_incrementation: is a integer that tells the library how many elements need to be incremented to the pointer when the pointer is filled with data. 
 * \param[out] return (on success): will be a string structure pointer.
 * \param[out] return (on failure): will be a NULL value.
 */
string * salloc(int number_of_columns, int col_incrementation) {

	if (string_debugger_flag) printf("Entering the salloc.\n");

    string * ptr = calloc(1, sizeof(string));

    if (number_of_columns <= 0)
        ptr->total_num_cols = 20;
    else
        ptr->total_num_cols = number_of_columns;

    if (col_incrementation < 0)
        ptr->col_incrementation = 0;
    else 
        ptr->col_incrementation = col_incrementation;

    ptr->array = calloc(ptr->total_num_cols, sizeof(char));
    if (!ptr->array) {
        printf("ERROR: Calloc has failed...\n");
        if (string_debugger_flag) printf("Leaving the salloc function.\n");
        return NULL;
    }

    ptr->tokens = NULL;
    ptr->total_num_tokens = 0;

    if (string_debugger_flag) printf("Leaving the salloc function.\n");
    return ptr;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take a string structure char pointer and add either a char or a char pointer to
 * the data within the structure. A void pointer is used due to the unknown type of data, and only
 * a char or char pointer is accepted. The current length of the char pointer, the incoming length of
 * the data and null terminator is checked against the amount of memory that is allocated for the char
 * pointer in the string structure. If there is no room for the incoming data then the function will
 * Take the combination of lenghts and add the col_incrementation to it, and resize the char pointer in
 * the string structure. I've debated on keeping the col_incremntation and left it in to allow the user
 * to control how well this library runs. If the user gives a small to zero number, then the library will
 * constantly have to keep allocating for more memory to the pointer, else if they give a big number
 * then less allocates will have to be made (saving some time).
 *
 * \param[in] array: is a string structure.
 * \param[in] format: is a const char pointer that stores the datatype for the incoming data.
 * \param[in] ...: is either a char or a char pointer of data, nothing else is accpeted.
 * \param[out] return (on success): will be an integer value of zero.
 * \param[out] return (on failure): will be an integer value of one.
 */
int sadd(string ** array, const char * format, ...) {

	if (string_debugger_flag) printf("Entering the sadd function.\n");

    if ((*array)) {
        void * vptr;
        va_list arguments;
        va_start(arguments, format);

        // Grab the characters that are to be added to the end of a string.
        if (!strcmp(format, "char")) {
            char * var = calloc(2, sizeof(char));
            var[0] = va_arg(arguments, int);
            vptr = var;
        } else if (!strcmp(format, "string")) {
            char * string = va_arg(arguments, char*);
            char * string2 = calloc(strlen(string) + 1, sizeof(char));
            strncpy(string2, string, strlen(string));
            vptr = string2;
        } else {
            printf("ERROR: The given data type is not supported...\n");
            if (string_debugger_flag) printf("Leaving the sadd funtion.\n");
            va_end(arguments);
            return 1;
        }

        // At this point vptr should all be a char pointer
        int temp_length = (*array)->current_num_col + strlen((char*)vptr) + 1;
        if ((*array)->total_num_cols < temp_length) {
            (*array)->total_num_cols = temp_length + (*array)->col_incrementation;
            column_reallocation(array, (*array)->total_num_cols);
        }
        // Need to subtract one from the temp length due to its addition of the null terminator.
        (*array)->current_num_col = (temp_length - 1);
        
        strcat((*array)->array, (char*)vptr);

        va_end(arguments);
        free(vptr);
    } else {
        if (string_debugger_flag) printf("WARNING: You sent in a NULL pointer...\n");
    }
    
	if (string_debugger_flag) printf("Leaving the sadd funtion.\n");

	return 0;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take a string structure char pointer and insert either a char or a char pointer to
 * the data within the structure. This function was created because of the sadd function not being able to
 * add characters in the beginning of the string.A void pointer is used due to the unknown type of data, 
 * and only a char or char pointer is accepted. The current length of the char pointer, the incoming length 
 * of the data and null terminator is checked against the amount of memory that is allocated for the char
 * pointer in the string structure. If there is no room for the incoming data then the function will
 * Take the combination of lenghts and add the col_incrementation to it, and resize the char pointer in
 * the string structure. I've debated on keeping the col_incremntation and left it in to allow the user
 * to control how well this library runs. If the user gives a small to zero number, then the library will
 * constantly have to keep allocating for more memory to the pointer, else if they give a big number
 * then less allocates will have to be made (saving some time).
 *
 * \param[in] array: is a string structure.
 * \param[in] position: is an integer that tells the function where to insert the incoming data.
 * \param[in] format: is a const char pointer that stores the datatype for the incoming data.
 * \param[in] ...: is either a char or a char pointer of data, nothing else is accpeted.
 * \param[out] return (on success): will be an integer value of zero.
 * \param[out] return (on failure): will be an integer value of one.
 */
int sinsert(string ** array, int position, const char * format, ...) {

	if (string_debugger_flag) printf("Entering the sinsert funtion.\n");

    if ((*array)) {
        void * vptr;
        va_list arguments;
        va_start(arguments, format);

        // Grab the characters that are to be added to the end of a string.
        if (!strcmp(format, "char")) {
            char * var = calloc(2, sizeof(char));
            var[0] = va_arg(arguments, int);
            vptr = var;
        } else if (!strcmp(format, "string")) {
            char * string = va_arg(arguments, char*);
            char * string2 = calloc(strlen(string) + 1, sizeof(char));
            strncpy(string2, string, strlen(string));
            vptr = string2;
        } else {
            printf("ERROR: The given data type is not supported...\n");
            if (string_debugger_flag) printf("Leaving the sadd funtion.\n");
            va_end(arguments);
            return 1;
        }

        // At this point vptr should all be a char pointer
        int temp_length = (*array)->current_num_col + strlen((char*)vptr) + 1;
        if ((*array)->total_num_cols < temp_length) {
            (*array)->total_num_cols = temp_length + (*array)->col_incrementation;
            column_reallocation(array, (*array)->total_num_cols);
        }
        // Need to subtract one from the temp length due to its addition of the null terminator.
        (*array)->current_num_col = (temp_length - 1);

        char * temp = calloc((*array)->current_num_col + 1, sizeof(char));
        strncpy(temp, (*array)->array, position * sizeof(char));
        strcat(temp, (char*)vptr);
        strncat(temp, &(*array)->array[position], ((*array)->current_num_col - (position + strlen((char*)vptr))) * sizeof(char));
        memset((*array)->array, 0, (*array)->current_num_col * sizeof(char));
        strcpy((*array)->array, temp);

        free(temp);
        va_end(arguments);
        free(vptr);
    } else {
        if (string_debugger_flag) printf("WARNING: You sent in a NULL pointer...\n");
    }

    if (string_debugger_flag) printf("Leaving the sinsert funtion.\n");
    
    return 0;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will loop through the char pointer within the string structure and find how many times
 * a character is found within the string. I'm not a big fan of this function because of how simple it
 * is. In the future I wouldn't mind seeing this function also being able to return the position of the
 * first occurence of the char to be found.
 *
 * \param[in] array: is a string structure.
 * \param[in] find_char: is a char that holds a character to be found within a char pointer.
 * \param[out] return (on success): will be an integer of the number of occurences the char has been found in the string.
 */
int soccurences(string * array, char find_char) {

	if (string_debugger_flag) printf("Entering the soccurences function.\n");

    int total_occurences = 0;
    if (array) {
        for (int i = 0; i < array->current_num_col; ++i) {
            if (array->array[i] == find_char)
                total_occurences++;
        }
    } else {
        if (string_debugger_flag) printf("WARNING: You sent in a NULL pointer...\n");
    }

	if (string_debugger_flag) printf("Leaving the soccurences function.\n");

    return total_occurences;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take the string structure char pointer and tokenize it. The tokens are stored in 
 * a two dimensional char array in the string strucutre, as well as how many tokens are found. This
 * function will check to see how many times the tokenizer character can be found in the string and 
 * saved. The returned value will be used to allocate memory to the tokens variable in the string
 * structure. A copy is taken of the char pointer in the string structure due to strtok function being
 * mean and editing the original version. Once a token is found a row in the tokens 2D array is 
 * allocated and the contents of the token are saved to the row. This will loop through all tokens and
 * repeating the same process. The strtok function is not nice with memory or how it interacts with
 * a given char pointer. In the future I want to create my own version of strtok and return the array
 * myself. This function will be updated with said new version of strtok. 
 *
 * \param[in] array: is a string structure.
 * \param[in] token_char: is a char to be used to split the char pointer with.
 * \param[out] return (on success): will be an integer value of zero.
 * \param[out] return (on failure): will be an integer value of one.
 */
int stokenize(string ** array, char token_char) {

	if (string_debugger_flag) printf("Entering the stokenize function.\n");

    if ((*array)) {
        int character_occurences = soccurences((*array), token_char);
        if (character_occurences) {

            if ((*array)->tokens) {
                for (int i = 0; i < (*array)->total_num_tokens; ++i)
                    free((*array)->tokens[i]);
                free((*array)->tokens);
                (*array)->tokens = NULL;
                (*array)->total_num_tokens = 0;
            }

            // Have to add another one to the character_occurences because of the extra token at the end.
            (*array)->tokens = calloc(character_occurences + 1, sizeof(char*));
            if (!(*array)->tokens) {
                printf("ERROR: There was an issue with the first calloc call.\n");
                return 1;
            }

            int token_iterator = 0;
            char * token = calloc(256, sizeof(char));
            for (int i = 0; i < (*array)->current_num_col; i++) {
                // Have to check to see if we are at the end of the string to grab the last token.
                if (((*array)->array[i] == token_char) || (i == ((*array)->current_num_col - 1))) {
                    // Add the last character to the token since we are at the end of the string.
                    if (i == (*array)->current_num_col -1)
                        token[token_iterator] = (*array)->array[i];
                    (*array)->tokens[(*array)->total_num_tokens] = calloc(strlen(token) + 1, sizeof(char));
                    if (!(*array)->tokens[(*array)->total_num_tokens]) {
                        printf("ERROR: There was an issue with the second calloc call.\n");
                        return 1;
                    }

                    strcpy((*array)->tokens[(*array)->total_num_tokens], token);
                    (*array)->total_num_tokens++;  
                    memset(token, 0, 256 * sizeof(char));          
                    token_iterator = 0;
                } else {
                    token[token_iterator] = (*array)->array[i];
                    token_iterator++;
                }
            }
            free(token);
        } else {
            printf("ERROR: There were no characters found like that in the following string: %s\n", (*array)->array);
            return 1;
        }
    } else {
        if (string_debugger_flag) printf("WARNING: You sent in a NULL pointer...\n");
    }

	if (string_debugger_flag) printf("Leaving the stokenize function.\n");

    return 0;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take the string structure and do one of two things with the char pointer within 
 * the structure. If the position is given then the function will stick a NULL terminator in the char
 * pointer of the string structure. If the character is given then the function will loop through 
 * the char pointer in the string structure for the first occurence of said character and stick a NULL
 * terminator there.
 *
 * \param[in] array: is a string structure.
 * \param[in] position: is an integer that tells the function where a null terminator needs to be added.
 * \param[in] character: is a char character that tells the function on where a null terminator needs to be added.
 * \param[out] return (on success): will be an integer value of zero.
 * \param[out] return (on failure): will be an integer value of one.
 */
void strunc(string ** array, int position, char character) {

	if (string_debugger_flag) printf("Entering the strunc function.\n");

    if ((*array)) {
        // If the position_or_char is bigger than the length of the string.
        if (character) {
            // Look of the first occurence of a letter and put a null terminator there.
            for (int i = 0; i < (*array)->current_num_col; ++i) {
                if ((*array)->array[i] == character) {
                    (*array)->array[i] = '\0';
                    (*array)->current_num_col = i;
                    break;
                }
            }
        } else {
            // Put a null terminator at a specific place in the string. 
            (*array)->array[position] = '\0';
            (*array)->current_num_col = position;
        }
    } else {
        if (string_debugger_flag) printf("WARNING: You sent in a NULL pointer...\n");
    }

	if (string_debugger_flag) printf("Leaving the strunc funtion.\n");

    return;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take the string structure and loop through its char pointer ignoring the leading
 * spaces and taking the last account of character in the pointer. This finds where the string actually
 * starts and ends. I grab everything of the string minus leading and trailing spaces, and move the 
 * contents of the string to the front of the pointer. I then adjust the parameters of the string 
 * structure and then NULL terminate the ending porition of the char pointer to remove the trailing spaces.
 *
 * \param[in] array: is a string structure..
 */
void sremove_leading_and_trailing_spaces(string ** array) {

	if (string_debugger_flag) printf("Entering the sremove_leading_and_trailing_spaces function.\n");

    if ((*array)) {
        int starting_point = -1, ending_point = 0;
        for (int i = 0; i < (*array)->current_num_col; ++i) {
            // This if statment is just suppose to grab the first character and then stop grabbing anymore after. 
            if (starting_point == -1 && ((*array)->array[i] > 32 && (*array)->array[i] < 127))
                starting_point = i;
            if ((*array)->array[i] > 32 && (*array)->array[i] < 127)
                ending_point = i;
        }
        
        // I tried the case of just using strncpy to move the content of the string to the beginning of the string but
        // - this causes an issue in valgrind / low level memory. The error is: __strcpy_sse2_unaligned
        char * temp = calloc(((*array)->current_num_col - starting_point) + 1, sizeof(char));
        strncpy(temp, &(*array)->array[starting_point], (*array)->current_num_col - starting_point);
        sclear(array);
        sadd(array, "string", temp);
        free(temp);

        ending_point -= starting_point;
        (*array)->current_num_col -= starting_point;                       
        (*array)->array[ending_point + 1] = '\0';
        // Have to add a one to the ending point since it is zero based, and have to convert it to one base.
        (*array)->current_num_col -= ((*array)->current_num_col - (ending_point + 1));
    } else {
        if (string_debugger_flag) printf("WARNING: You sent in a NULL pointer...\n");
    }

	if (string_debugger_flag) printf("Leaving the sremove_leading_and_trailing_spaces funtion.\n");

    return;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take in a string structure and remove all characters that were passed into this function
 * from the char pointer in the structure. The function will create a temporary pointer with the same amount of
 * memory and loop through all of the characters in the char pointer of the string structure. Each characters is
 * checked against the remove characters pointer. All characters that do not consist of any characters from the 
 * remove characters pointer are added to the temporary pointer. Once finished, the system will clear the string
 * structure, and add the temporary pointer to the string structure.
 *
 * \param[in] array: is a string structure.
 * \param[in] remove_characters: is a char pointer that holds characters to be removed from said string.
 */
void sremove(string ** array, char * remove_characters) {

	if (string_debugger_flag) printf("Entering the sremove function.\n");

    if ((*array)) {
        int temp_iterator = 0, add_flag = 0, number_of_characters = strlen(remove_characters);
        char * ptr = calloc(((*array)->total_num_cols + 1), sizeof(char));
        for (int i = 0; i < (*array)->current_num_col; ++i) {
            for (int j = 0; j < number_of_characters; ++j) {
                if ((*array)->array[i] == remove_characters[j])
                    add_flag = 1;
            }
            if (add_flag)
                add_flag = 0;
            else  {
                ptr[temp_iterator] = (*array)->array[i];
                temp_iterator++;
            }  
        }

        sclear(array);
        sadd(array, "string", ptr);
        free(ptr);
    } else {
        if (string_debugger_flag) printf("WARNING: You sent in a NULL pointer...\n");
    }

	if (string_debugger_flag) printf("Leaving the sremove funtion.\n");

    return;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take a string structure and clean all the data feilds. If there are any tokens
 * for the current string, then they are free'd and the iterator set to zero. The char pointer is
 * then cleaned and the total number of elements in the array is set to zero.
 *
 * \param[in] array: is a string structure.
 */
void sclear(string ** array) {
    
    if (string_debugger_flag) printf("Entering the sclear function.\n");

    if ((*array)) {
        if ((*array)->tokens) {
            for (int i = 0; i < (*array)->total_num_tokens; ++i)
                free((*array)->tokens[i]);
            free((*array)->tokens);
            (*array)->tokens = NULL;
            (*array)->total_num_tokens = 0;
        }

        memset((*array)->array, 0, (*array)->total_num_cols * sizeof(char));
        
        (*array)->current_num_col = 0;
    } else {
        if (string_debugger_flag) printf("WARNING: You sent in a NULL pointer...\n");
    }

    if (string_debugger_flag) printf("Leaving the sclear function.\n");

    return;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take in a string structure and reset the entire structure. This function will check
 * if either the number of elements for the char pointer or how many has to be increased with every add, has
 * been passed. If so then their fields are updated. If there are tokens for the current string, then they
 * are free'd and the iterator set back to zero. The char pointer is then cleaned and the number of elements
 * iterator is set back to zero.
 *
 * \param[in] array: is a string structure.
 * \param[in] number_of_columns: is an integer that holds how many elements need to be in the char pointer.
 * \param[in] col_incrementation: is a integer that tells the library how many elements need to be incremented to the pointer when the pointer is filled with data. 
 * \param[out] return (on success): will be an integer value of zero.
 * \param[out] return (on failure): will be an integer value of one.
 */
int sreset(string ** array, int number_of_columns, int col_incrementation) {

    if (string_debugger_flag) printf("Entering the sreset function.\n");
    
    if ((*array)) {
        if (col_incrementation)
            (*array)->col_incrementation = col_incrementation;

        if (number_of_columns) {
            (*array)->array = realloc((*array)->array, number_of_columns * sizeof(char));
            if (!(*array)->array) {
                printf("ERROR: There was an issue with reallocating the given pointer.\n");
                if (string_debugger_flag) printf("Leaving the sclear function.\n");
                return 1;
            }
            (*array)->total_num_cols = number_of_columns;
        }
        
        if ((*array)->tokens) {
            for (int i = 0; i < (*array)->total_num_tokens; ++i)
                free((*array)->tokens[i]);
            free((*array)->tokens);
            (*array)->tokens = NULL;
            (*array)->total_num_tokens = 0;
        }

        memset((*array)->array, 0, (*array)->total_num_cols * sizeof(char));
        
        (*array)->current_num_col = 0;
    } else {
        if (string_debugger_flag) printf("WARNING: You sent in a NULL pointer...\n");
    }

    if (string_debugger_flag) printf("Leaving the sreset function.\n");
    
    return 0;
}

//-----------------------------------------------------------------------------------------------------
/**
 * This function will take in a string structure and free all tokens (if there are any), the char pointer,
 * that holds the string data, and the string structure pointer itself. 
 *
 * \param[in] array: is a string structure.
 */
void sfree(string ** array) {

	if (string_debugger_flag) printf("Entering the sfree function.\n");

    if ((*array)) {
        if ((*array)->tokens) {
            for (int i = 0; i < (*array)->total_num_tokens; ++i)
                free((*array)->tokens[i]);
            free((*array)->tokens);
        }

        free((*array)->array);
        free((*array));
    } else {
        if (string_debugger_flag) printf("WARNING: You sent in a NULL pointer...\n");
    }

	if (string_debugger_flag) printf("Leaving the sfree funtion\n");

	return;
}