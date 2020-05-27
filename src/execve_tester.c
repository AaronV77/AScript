#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

int main(int argc, char * argv[], char * envp[]) {
    pid_t  pid;
    
    char * command = calloc(50, sizeof(char));
    char ** arguments = calloc(3, sizeof(char*));
    for (int i = 0; i < 3; ++i) 
        arguments[i] = calloc(50, sizeof(char));

    strcpy(command, "/bin/echo");
    strcpy(arguments[0], "/bin/echo");
    strcpy(arguments[1], "Hello Aaron!");
    arguments[2] = NULL;
    
    pid = fork();
    if (pid == 0) {
        execve(command, arguments, envp);
    } else {
        waitpid(pid, NULL, 0);
    }

    for (int i = 0; i < 3; ++i)
        free(arguments[i]);
    free(arguments);
    free(command);

    return 0;
}