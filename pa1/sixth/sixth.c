#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int isVowel(char);
void pig(char*);
void case1(char*);
void case2(char*);
void piglast(char*);
void case1last(char*);
void case2last(char*);

int main(int argc, char** argv) {
	if (argc == 1) {
		printf("Not enough Arguments! \n");
		return 0;
	}
	int i;
	for (i = 1; i < argc; i++) {
		if (i == argc-1) {
			piglast(argv[i]);			// Send the word to pig in case its the last word
			break;
		}
		pig(argv[i]);				// Send the word to pig-latin converter
	}
	return 0;
}

int isVowel(char a) {					// Checks if this character is a vowel
	if ((a == 'a')||(a == 'e')||(a == 'i')||(a == 'o')||(a == 'u')||
		(a == 'A')||(a == 'E')||(a == 'I')||(a == 'O')||(a == 'U')) {
			return 1;
	}
	return 0;
}

void pig(char *word) {
	int start = isVowel(word[0]);
	if (start == 1) {
		case1(word);					// Sends it to case 1 or case 2 depending on the rules
	} else {							// of conversion
		case2(word);
	}
}

void case1(char *word) {
	printf("%syay ", word);				// Prints out the case1 word
}

void case2(char *word) {				// Prints out the case2 word
	int i;
	int index = -1;
	for (i = 0; i < strlen(word); i++) {
		if (isVowel(word[i]) == 1) {
			index = i;
			break;
		}
	}
	if (index != -1) {
		int k;
		for (k = index; k < strlen(word); k++) {
			printf("%c", word[k]);
		}
		for (k = 0; k < index; k++) {
			printf("%c", word[k]);
		}
		printf("ay ");
	} else {
		printf("%say ", word);
	}
}

void piglast(char *word) {
	int start = isVowel(word[0]);
	if (start == 1) {
		case1last(word);
	} else {
		case2last(word);
	}
}
												// Same as above but for last word
void case1last(char *word) {
	printf("%syay", word);
}

void case2last(char *word) {
	int i;
	int index = -1;
	for (i = 0; i < strlen(word); i++) {
		if (isVowel(word[i]) == 1) {
			index = i;
			break;
		}
	}
	if (index != -1) {
		int k;
		for (k = index; k < strlen(word); k++) {
			printf("%c", word[k]);
		}
		for (k = 0; k < index; k++) {
			printf("%c", word[k]);
		}
		printf("ay");
	} else {
		printf("%say", word);
	}
}