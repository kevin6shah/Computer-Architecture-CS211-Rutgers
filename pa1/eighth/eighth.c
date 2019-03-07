#include <stdio.h>
#include <stdlib.h>

struct treeNode {
	int data;
	struct treeNode *left;
	struct treeNode *right;		// Define tree node structure
};

struct treeNode* insert(struct treeNode *tree, int a, int *d);
struct treeNode* createNode(int a);
int search (struct treeNode *tree, int a);
int findHeight(struct treeNode *tree, int a);

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Insufficient Arguments");
		return 0;
	}
	
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("error");
		return 0;
	}
	
	char a;
	int b;
	
	struct treeNode *tree = NULL;
	
	while (fscanf(fp, "%c %d\n", &a, &b) == 2) {
		if (a == 'i') {
			int d = 0;
			tree = insert(tree, b, &d);
			if (d == 0) {
				printf("inserted %d\n", findHeight(tree, b));
			} else printf("duplicate\n");
		} else if (a == 's') {
			if (search(tree, b) == 1) {
				printf("present %d\n", findHeight(tree, b));
			} else printf("absent\n");
		}
	}
	fclose(fp);
	return 0;
}

// Creates a node with dynamic memory allocation
struct treeNode* createNode(int a) {
	struct treeNode *node = malloc(sizeof(struct treeNode));
	node->data = a;
	node->left = NULL;
	node->right = NULL;
	return node;
}

struct treeNode* insert(struct treeNode *tree, int a, int *d) {		// Recursively inserts the node into the tree
	if (tree == NULL) return createNode(a);
	
	if (a == tree->data) {
		*d = 1;
		return tree;
	}
	if (a < tree->data) {
		tree->left = insert(tree->left, a, d);
	} else tree->right = insert(tree->right, a, d);
	
	return tree;
}

int search (struct treeNode *tree, int a) {			// Recursively searches if the node exists
	if (tree == NULL) return 0;
	if (tree->data == a) {
		return 1;
	}
	if ((search(tree->left, a) == 1) || (search(tree->right, a) == 1)) return 1;
	else return 0;
}

int findHeight(struct treeNode *tree, int a) {
	if (tree == NULL) return 0;
	if (tree->data == a) {
		return 1;
	}
	if (a < tree->data) {
		return 1 + findHeight(tree->left, a);
	} else return 1 + findHeight(tree->right, a);
}