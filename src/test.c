#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {

	char * ptr = calloc(2, sizeof(char));
	ptr = calloc(5, sizeof(char));
	free(ptr);
	return 0;
}
