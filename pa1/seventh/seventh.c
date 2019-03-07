#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char** argv) {
	if (argc == 1) {
		printf("Not enough Arguments!\n");
		return 0;
	}
	int i;
	char *word;
	for (i = 1; i < argc; i++) {
		word = argv[i];
		int index = strlen(word)-1;
		printf("%c", word[index]);
	}
	return 0;
}