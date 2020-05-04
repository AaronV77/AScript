#include "dir_utility.h"

int du_debugger_flag = 0;

int is_dir(char * path) {

    if (du_debugger_flag) printf("Entering is_dir function.\n");

	struct stat fileStruct;
	lstat(path, &fileStruct);
	if ((fileStruct.st_mode & S_IFMT) == S_IFDIR) {
        if (du_debugger_flag) printf("Leaving is_dir function.\n");
		return 1;
    }

    if (du_debugger_flag) printf("Leaving is_dir function.\n");
	
    return 0;
}

int is_file(char * path) {

    if (du_debugger_flag) printf("Entering is_file function.\n");
	
    struct stat fileStruct;
	lstat(path, &fileStruct);
	if (((fileStruct.st_mode & S_IFMT) == S_IFLNK) ||
        ((fileStruct.st_mode & S_IFMT) == S_IFREG) ||
        ((!access(path, X_OK)) && ((fileStruct.st_mode & S_IFMT) == S_IFREG))) {
        if (du_debugger_flag) printf("Leaving is_file function.\n"); 
		return 1;
    }

    if (du_debugger_flag) printf("Leaving is_file function.\n");

	return 0;
}

int change_permissions(char * path, char * permissions) {
    
    if (du_debugger_flag) printf("Entering change_permissions function.\n");
    
    int i = strtol(permissions, 0, 8);
    if (chmod(path,i) < 0) {
        printf("ERROR: There was an issue with changing '%s' permissions...\n", path);
        if (du_debugger_flag) printf("Leaving change_permissions function.\n");
        return 1;
    }

    if (du_debugger_flag) printf("Leaving change_permissions function.\n");

    return 0;
}

int create_file(char * path, char * contents, char * permissions, char * mode) {
    
    if (du_debugger_flag) printf("Entering create_file function.\n");
    
    if (is_file(path) || is_dir(path)) {
        printf("ERROR: The file to be created '%s', already exists...\n", path);
        if (du_debugger_flag) printf("Leaving create_file function.\n");
        return 1;
    }

    FILE *fp;
    fp  = fopen (path, mode);
    if (!fp) {
        printf("ERROR: The file '%s' could not be created...\n", path);
        if (du_debugger_flag) printf("Leaving create_file function.\n");
        return 1;
    }

    if (contents) {
        if (strlen(contents) > 0)
            fprintf(fp, "%s", contents);
    }
    fclose (fp);

    if (strlen(permissions) > 0 && strlen(permissions) < 5)
        change_permissions(path, permissions);

    if (du_debugger_flag) printf("Leaving create_file function.\n");
    
    return 0;
}

int create_dir() {


    return 0;
}

void directory_utility_debugger() {
	du_debugger_flag = 1;
	return;
}