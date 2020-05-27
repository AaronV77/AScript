#include "string.h"

#include <stdio.h>

int main() {
	
	
	string * ptr = calloc(1, sizeof(string));

	salloc(&ptr, 20, 25);

	char * sentence = calloc(100, sizeof(char));
	strcpy(sentence, "The Dog");

	// Test-1
	sadd(&ptr, "string", sentence);
	sadd(&ptr, "double", 10.0);
	printf("Here: %s\n", ptr->array);

	sclear(&ptr, 0, 0);
	printf("Here: %s\n", ptr->array);

	sadd(&ptr, "string", "I love coding");
	sadd(&ptr, "char", '!');
	sadd(&ptr, "float", 20.0);
	sadd(&ptr, "char", '-');
	sadd(&ptr, "int",  7);

	// Test-2
	// for (int i = 0; i < strlen(sentence); ++i)
	// 	sadd(&ptr, "char", sentence[i]);

	printf("Here: %s\n", ptr->array);

	sfree(&ptr);
	free(ptr);
	free(sentence);

	return 0;
}
