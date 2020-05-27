#include "td_array.h"

void do_something(char ** env) {
    printf("Got the env...\n");
    
    for(int i = 0; i < 5; ++i)
        printf("Here: %s\n", env[i]);

    return;
}

int main() {
    td_debugger();

    char * ptr = calloc(30, sizeof(char));
    char * ptr2 = calloc(30, sizeof(char));
    char * ptr3 = calloc(30, sizeof(char));
    char * ptr4 = calloc(30, sizeof(char));
    strcpy(ptr, "Aaron Valoroso");
    strcpy(ptr2, "Aaron Valoroso");
    strcpy(ptr3, "Aaron Valoroso");
    strcpy(ptr4, "Aaron Valoroso");

    td_array * env = calloc(1, sizeof(td_array));
    env->array = NULL;

    td_allocation(&env, "char", 3, 0, 10);
    td_push(&env, "char", &ptr);
    td_push(&env, "char", &ptr2);
    td_push(&env, "char", &ptr3);
    td_push(&env, "char", &ptr4);

    for (int i = 0; i < env->current_num_rows; ++i)
        printf("%d: %s\n", i, ((char**)env->array)[i]);

    do_something((char**)env->array);

    td_free(&env);
    env->array = NULL;

    td_allocation(&env, "char", 3, 0, 0);
    td_push(&env, "char", &ptr);
    td_free(&env);
    free(env);

    free(ptr);
    free(ptr2);
    free(ptr3);
    free(ptr4);

    return 0;
}