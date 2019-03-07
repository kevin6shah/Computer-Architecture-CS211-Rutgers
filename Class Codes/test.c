#include <stdio.h>
#include <stdlib.h>

char* mystrcpy(char* dest, char* src) {
	char* temp = dest;
	while(*src != '\0') {
		*dest = *src;
		dest++;
		src++;
	}
	*dest = '\0';
	return temp;
}

int main(int argc, char** argv) {
	int len;
	char c;
	char* ptr;
	
	printf("Enter the number of characters: ");
	scanf("%d\n", &len);
	ptr = malloc(sizeof(char) * len+1);
	int i;
	for (i = 0; i < len; i++) {
		scanf("%c", &c);
		ptr[i] = c;
	}
	ptr[len] = '\0';
	printf("entered string is %s\n", ptr);
	
	char *mynew_string = malloc(sizeof(char)*100);
	mystrcpy(mynew_string, ptr);
	
	printf("the copied string is %s\n", mynew_string);
	
	return 0;
}