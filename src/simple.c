#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
	
	char * ptr = calloc(10, sizeof(char));
	strcpy(ptr, "A A R O N");
	
	char * copy = calloc(strlen(ptr) + 1, sizeof(char));
	
	int size = strlen(ptr);
	int copy_iterator = 0;
	for (int i = 0; i < size; ++i) {
		if (ptr[i] != 32) {
			copy[copy_iterator] = ptr[i];
			copy_iterator++;
		}
	}

	printf("Here: %s\n", copy);

	return 0;
}
