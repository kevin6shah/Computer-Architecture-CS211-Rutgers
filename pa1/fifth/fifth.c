#include <stdio.h>
#include <stdlib.h>
int isMagic(int**, int);
int duplicate(int*, int);

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("Invalid arguments! \n");
		return 0;
	}
	
	FILE *fp = fopen(argv[1], "r");
	int dim, i, j, temp;
	fscanf(fp, "%d\n", &dim);						// Read from file
	int **arr = malloc(sizeof(int *) * dim);
	for (i = 0; i < dim; i++) {
		arr[i] = malloc(sizeof(int) * dim);
	}
	
	int *array = malloc(sizeof(int) * dim * dim);
	int counter = 0;
	int n = 0;
	
	for (i = dim-1; i >= 0; i--) {
		for (j = 0; j < dim; j++) {
			if (j == dim - 1) {
				fscanf(fp, "%d\n", &temp);			// Puts the matrix from file into 2d array
				if ((temp < 1) || (temp > (dim * dim)) || (duplicate(array, temp) == 1)) {
					counter = 1;
				}
				array[n] = temp;
				arr[i][j] = temp;
			} else {
				fscanf(fp, "%d", &temp);
				if ((temp < 1) || (temp > (dim * dim)) || (duplicate(array, temp) == 1)) {
					counter = 1;
				}
				array[n] = temp;
				arr[i][j] = temp;
			}
			n++;
		}
	}
	
	if (counter == 1) {
		printf("not-magic");
		return 0;
	}
	
	int check = isMagic(arr, dim);
	if (check == 0) {
		printf("not-magic");
		return 0;
	} else {
		printf("magic");
	}
	
	
	for (i = 0; i < dim; i++) {
		free(arr[i]);
	}									// Free the 2d array
	free(arr);
	fclose(fp);
	return 0;
}

int isMagic(int** arr, int dim) {		// Checks whether this is a magic square or not
	int counter = 0;
	int temp = 0;
	int Sum;
	int i, j;
	for (i = dim-1; i >= 0; i--) {
		for (j = 0; j < dim; j++) {
			temp = temp + arr[i][j];
		}
		if (i == dim-1) {
			Sum = temp;					// Checks the Sum of each row
		} else {
			if (temp != Sum) {
				counter = 1;
			}
		}
		temp = 0;
	}
	if (counter == 1) {
		return 0;
	}

	
	for (i = 0; i < dim; i++) {
		for (j = dim-1; j >= 0; j--) {
			temp = temp + arr[i][j];
		}
		if (temp != Sum) {
			counter = 1;			// Checks the Sum of each column
		}
		temp = 0;
	}
	if (counter == 1) {
		return 0;
	}

	
	for (i = dim-1; i >= 0; i--) {
		temp = temp + arr[i][i];
	}
	if (temp != Sum) {
		counter = 1;				// Checks the Sum of primary diagonal
	}
	temp = 0;
	
	if (counter == 1) {
		return 0;
	}

	
	int val = 0;
	for (i = dim-1; i >= 0; i--) {
		temp = temp + arr[i][val];
		val++;
	}
	if (temp != Sum) {
		counter = 1;			// Checks the Sum of secondary diagonal
	}
	temp = 0;
	
	if (counter == 1) {
		return 0;
	}


	return 1;
}

int duplicate(int* arr, int i) {		// Checks for duplicates in the array
	int counter = 0;
	int j;
	int n = sizeof(arr)/sizeof(int);
	for (j = 0; j < n; j++) {
		if (arr[j] == i) {
			counter = 1;
		}
	}
	if (counter == 0) {
		return 0;
	}
	return 1;
}