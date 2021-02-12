#include <stdio.h>
#include <string.h>

#include "ascript_core.h"
#include "dir_utility.h"

int main_debugger_var = 1;

void setup_environment(Env_Linked_List ** env_path, char ** envp) {
	
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
                    env_ll_push(env_path, token);
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

    FILE * source_fp;                               // Used to store the AScript file pointer.

    char * source_file_location;                    // Used to store the location of where the AScript file is located within the filesystem.

    if (argc > 1) {
        // Read the incoming arguments from the source
        for (int i = 1; i < argc; ++i) {
            // Loop through all the passed in arguments.
            if (!strcmp(argv[i], "-d") || !strcmp(argv[i], "--debug")) {
                main_debugger_var = 1;
            } else {
                // This will be the file name of the given file that we are going to be parsing.
                // Need to check to make sure that the file is not a directory and available.
                if (!is_dir(argv[i]) && is_file(argv[i])) {
                    char * last_dot = strrchr(argv[i], '.');
                    // Check to make sure that the file has an extension and the correct one if it does.
                    if (!last_dot || strcmp(last_dot, ".a")) {
                        printf("ERROR: Can you at least give me a file with the proper extension '.a'.\n");
                        return 0;
                    }
                    if (main_debugger_var) printf("Success: We have found the source file for parsing: %s\n", argv[i]);
                    source_file_location = strdup(argv[i]);
                } 
            }
        }
    } else {
        printf("ERROR: You have to at least pass in the file you wanted processed ya goober...\n");
        return 0;
    }

    // Open the file for reading.
    source_fp = fopen(source_file_location, "r");
    if (!source_fp) {
        printf("ERROR: SOmething is fishy about this file '%s', couldn't be opened. Do you have the secret code?\n", source_file_location);
        return 0;
    }

    // Grab the PATH environment variable and save the sections to a linked list data structure.
    Env_Linked_List * env_path = NULL;
    setup_environment(&env_path, envp); 

    // parsing_algorithm(source_fp, NULL, NULL, -1, 1, "file");
    parsing_algorithm(source_fp, NULL, NULL, -1, "file");

    free(source_file_location);
    env_ll_cleanup(&env_path);
    fclose(source_fp);

    return 0;
}