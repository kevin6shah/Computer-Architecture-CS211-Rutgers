#include <stdio.h>
#include <stdlib.h>

struct hash {
	int key;
	int value;								// Create the hash table structure
	struct hash *next;
};

struct hash *hash_table[10000];				// Creating a global hash table array ptr
void insert(int b, int *duplicate);
void insertNeg(int b, int *duplicate);
int search(int b);
int searchNeg(int b);

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Insufficient Arguments!");
	}
	
	FILE *fp = fopen(argv[1], "r");
	char a;
	int b;
	
	if (fp == NULL) {
		printf("error");
		return 0;
	}
	
	while (fscanf(fp, "%c %d\n", &a, &b) == 2) {
		if (a == 'i') {
			int duplicate = 0;
			if (b >= 0) {
				insert(b, &duplicate);		// If b is positive and inserts the value into the hash
			} else insertNeg(b, &duplicate);	// If b is negative and inserts the value into the hash
			if (duplicate == 0) {
				printf("inserted\n");
			} else printf("duplicate\n");
		} else if (a == 's') {
			if (b >= 0) {				// If b is positive and searches the value from the hash
				if (search(b) == 1) {
					printf("present\n");
				} else {
					printf("absent\n");
				}
			} else {
				if (searchNeg(b) == 1) {
					printf("present\n"); // If b is negative and searches the value from the hash
				} else {
					printf("absent\n");
				}
			}
		}
	}
	
	fclose(fp);
	return 0;
}

void insert(int b, int *duplicate) {
	struct hash *list = hash_table[b % 10000];
	while (list != NULL) {
		if (list->value == b) {
			*duplicate = 1;
			return;
		}
		list = list->next;
	}
	list = hash_table[b % 10000];
	struct hash *hashkv = malloc(sizeof(struct hash));
	hashkv->key = b % 10000;
	hashkv->value = b;
	hashkv->next = list;
	list = hashkv;
	hash_table[b % 10000] = list;
}

void insertNeg(int b, int *duplicate) {
	int key = (b % 10000) * -1;
	struct hash *list = hash_table[key];
	while (list != NULL) {
		if (list->value == b) {
			*duplicate = 1;
			return;
		}
		list = list->next;
	}
	list = hash_table[key];
	struct hash *hashkv = malloc(sizeof(struct hash));
	hashkv->key = key;
	hashkv->value = b;
	hashkv->next = list;
	list = hashkv;
	hash_table[key] = list;
}

int search(int b) {
	struct hash *list = hash_table[b % 10000];
	while (list != NULL) {
		if (list->value == b) {
			return 1;
		}
		list = list->next;
	}
	return 0;
}

int searchNeg(int b) {
	struct hash *list = hash_table[(b % 10000) * -1];
	while (list != NULL) {
		if (list->value == b) {
			return 1;
		}
		list = list->next;
	}
	return 0;
}