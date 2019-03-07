#include <stdio.h>
#include <stdlib.h>

struct node {					//Linked List here using Structures
	int value;
	struct node *next;
};

void traverse(struct node* mylist) {
	struct node* t1 = mylist;
	while (t1 != NULL) {
		printf("%d --> ", t1->value);
		t1 = t1->next;
	}
	return;
}

void cleanup(struct node* mylist) {
	struct node* t1 = mylist;
	struct node* t2 = NULL;
	
	while (t1 != NULL) {
		t2 = t1;
		t1 = t1->next;
		free(t2);
	}
}

int main(int argc, char** argv) {
	int num;
	int a, i, n;
	
	struct node * head = NULL;
		
	printf("How many elements: ");
	scanf("%d", &num);
	
	for (i = 0; i < num; i++) {
		n = i + 1;
		printf("Element[%d]: ", n);
		scanf("%d", &a);
		struct node * temp = (struct node *) malloc(1 * sizeof(struct node));
		temp->value = a;
		temp->next = head;
		head = temp;
	}
	traverse(head);
	cleanup(head);
	return 0;
}