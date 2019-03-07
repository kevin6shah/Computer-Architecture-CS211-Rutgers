#include <stdio.h>
#include <stdlib.h>

struct listnode {
	int data;						// Linked List Structure
	struct listnode *next;
};

void traverse(struct listnode*);
void cleanup(struct listnode*);
struct listnode* insert(struct listnode*, struct listnode*);
struct listnode* delete(struct listnode*, int);
int exists(struct listnode*, int);

int size = 0;

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Insufficient Arguments! /n");
		return 0;
	}
	
	FILE *fp;
	char a;							// Creates a file pointer
	int b;
	
	fp = fopen(argv[1], "r");

	if (fp == NULL) {		// Checks whether the file exists
		printf("error");
		return 0;
	}
	
	fseek(fp, 0, SEEK_END);
	if (ftell(fp) == 0) {		// Checks whether the file is null if it does exist
		printf("0");
		return 0;
	}
	fseek(fp, 0, SEEK_SET);
	
	struct listnode *Head = NULL;
	while(fscanf(fp, "%c %d\n", &a, &b) == 2) {
		int exist = exists(Head, b);
		if (a == 'i') {							// Inserts the node if it doesn't already exist
			struct listnode *temp = malloc(sizeof(struct listnode));
			temp->data = b;
			temp->next = NULL;
			if (exist == 0) {
				Head = insert(Head, temp);
				size++;
			}
		} else if (a == 'd') {
			if (Head != NULL) {
				if (exist == 1) {
					Head = delete(Head, b);					// Deletes the node requested
					size--;
				}
			}
		}
	}
	printf("%d\n", size);
	traverse(Head);
	cleanup(Head);
	fclose(fp);
	return 0;
}

void traverse(struct listnode* head) {		// Traverses through the linked lists and prints it out
	struct listnode* Head = head;
	while (Head != NULL) {
		if (Head->next == NULL) {
			printf("%d", Head->data);
			break;
		}
		printf("%d	", Head->data);
		Head = Head->next;
	}
	return;
}

void cleanup(struct listnode* head) {			// Cleanup the allocated memory
	struct listnode* Head = head;
	struct listnode* temp = NULL;
	
	while (Head != NULL) {
		temp = Head;
		Head = Head->next;
		free(temp);
	}
}


struct listnode* insert(struct listnode* Head, struct listnode* node) {
	if (Head == NULL) {
		Head = node;			// If this is the first insertion
		return Head;
	}
	
	if (node->data < Head->data) {
		node->next = Head;		// If the inserting node is less than the starting node
		return node;
	}
	
	struct listnode *temp = Head->next;
	struct listnode *prev = Head;
	while (temp != NULL && temp->data < node->data) {		// Arranges in order
		prev = temp;
		temp = temp->next;
	}
	
	node->next = temp;
	prev->next = node;
							// Places in the right position
	return Head;
}

struct listnode* delete(struct listnode* head, int a) { 	// Deletes a particular node from the list
	if (head->data == a) {
		return head->next;
	}
	
	struct listnode *temp = head->next;
	struct listnode *prev = head;
	while (head != NULL && temp->data != a) {
		prev = temp;
		temp = temp->next;
	}
	
	prev->next = temp->next;
	
	return head;
	
}

int exists(struct listnode* head, int a) {		// Checks whether the node already exists
	int counter = 0;
	while (head != NULL) {
		if (head->data == a) {
			counter = 1;
			break;
		}
		head = head->next;
	}
	return counter;
}