#ifndef DU_H
#define DU_H

#include <stdio.h>      // printf(), fprintf(), fclose(), fopen(), FILE
#include <stdlib.h>     // strtol()
#include <string.h>     // strlen()
#include <unistd.h>     // access(), X_OK
#include <sys/stat.h>   // struct stat, lstat(), chmod()

int is_dir(char * path);

int is_file(char * path);

int change_permissions(char * path, char * permissions);

int create_file(char * path, char * contents, char * permissions, char * mode);

void directory_utility_debugger();

#endif