#include <stdio.h>
#include <stdlib.h>

void sort(int *, int);

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Not enough Arguments! \n");    //In case there aren't any arguments given
		return 0;
	}
	FILE *fp; // Creates a file
	int a;
	fp = fopen(argv[1], "r");
	
	if (fp == NULL) return 0;
	
	fscanf(fp, "%d\n", &a);
	int *arr = (int *) malloc(sizeof(int) * a);
	int i = 0;
	int evenNum = 0;
	int oddNum = 0;
	while(fscanf(fp, "%d", &arr[i]) == 1) {			//Scans the Doc and inputs the numbers
		if (arr[i] % 2 == 0) {						//in an array, and counts the amount of
			evenNum++;								//even and odd numbers in the array
		} else if (arr[i] % 2 != 0) {
			oddNum++;
		}
		i++;
	}
	int *even = (int *) malloc(sizeof(int) * evenNum);				//Creates an Odd and Even separate array
	int *odd = (int *) malloc(sizeof(int) * oddNum);
	int size = evenNum + oddNum;
	int c = 0;
	int x, y;
	for (x = 0; x < a; x++) {
		if (arr[x] % 2 == 0) {
			even[c] = arr[x];
			c++;
		}							//Fills the Even and Odd arrays with their
	}								//respected int values
	c = 0;
	for (y = 0; y < a; y++) {
		if (arr[y] % 2 != 0) {
			odd[c] = arr[y];
			c++;
		}
	}
	sort(even, evenNum);		//Sorts odd and even arrays
	sort(odd, oddNum);
	int *result = (int *) malloc(sizeof(int) * size);
	int i1, i2;
	c = 0;
	for (i1 = 0; i1 < evenNum; i1++) {
		result[c] = even[i1];
		c++;
	}										//Combines both arrays in one array
	for (i2 = 0; i2 < oddNum; i2++) {
		result[c] = odd[i2];
		c++;
	}
	int j;
	for (j = 0; j < a; j++) {
		if (j == a-1) {
			printf("%d", result[j]);
			break;
		}
		printf("%d	", result[j]);
	}
	
	fclose(fp);
	free(arr);
	free(even);
	free(odd);
	free(result);
	return 0;
}

void sort(int *arr, int len) {          //Sorting Algorithm used to sort arrays in place
	int i, j, temp;
	for (i = 0; i < len - 1; i++) {
		for (j = 0; j < len - i - 1; j++) {
			if (arr[j] > arr[j+1]) {
				temp = arr[j];
				arr[j] = arr[j+1];
				arr[j+1] = temp;
			}
		}
	}
}