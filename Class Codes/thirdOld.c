#include <stdio.h>
#include <stdlib.h>

int b; //Global Variable

/* A pointer is just an address */

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int main(int argc, char** argv) {
	
	int p, q;
	printf("enter the value of p: ");
	scanf("%d", &p);
	printf("enter the value of q: ");
	scanf("%d", &q);
	printf("[before swap] p = %d, q = %d\n", p, q);
	swap(&p, &q);
	printf("[after swap] p = %d, q = %d\n", p, q);
	return 0;

}