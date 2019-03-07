#include <stdio.h>
#include <stdlib.h>

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("not enough arguments!/n");
		return 0;
	}
	FILE *fp;
	int b;
	char a;
	fp = fopen(argv[1], "r");
	
	if (fp == NULL) return 0;
	
	while(fscanf(fp, "%c %d\n", &a, &b) == 2) {
		printf("%c %d\n", a, b);
	}
	
	fclose(fp);
	return 0;
}