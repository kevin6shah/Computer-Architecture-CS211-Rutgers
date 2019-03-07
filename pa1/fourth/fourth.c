#include <stdio.h>
#include <stdlib.h>

void printMatrix(int**, int, int);
void multiply(int**, int, int, int**, int, int, int**, int, int);

int main(int argc, char** argv) {
	if (argc != 2) {
		printf("not enough arguments!/n");
		return 0;
	}
	FILE *fp;								// Creates a File type and open the arg file
	int m, n, i, temp;
	fp = fopen(argv[1], "r");
	
	if (fp == NULL) return 0;
	
	fscanf(fp, "%d %d\n", &m, &n);
	int **arr = malloc(sizeof(int *) * m);
	for (i = 0; i < m; i++) {
		arr[i] = malloc(sizeof(int) * n);
	}
	int indexm = m-1;
	int indexn = 0;
	
	while (indexm >= 0) {
		if ((m == 1) && (n == 1)) {
			fscanf(fp, "%d\n", &temp);
			arr[0][0] = temp;							// Stores value if it is a 1 x 1 matrix
			break;
		} else if (n == 1) {
			fscanf(fp, "%d\n", &temp);					// Stores value if it is a m x 1 matrix
			arr[indexm][indexn] = temp;
		} else if (n != 1) {
			while(fscanf(fp, "%d", &temp) == 1) {
				if (indexn == n-2) {
					arr[indexm][indexn] = temp;
					indexn++;
					fscanf(fp, "%d\n", &temp);			// Stores the scanned numbers into a 2d array
					arr[indexm][indexn] = temp;
					break;
				}
				arr[indexm][indexn] = temp;
				indexn++;
			}
		}
		indexn = 0;
		indexm--;
	}
	
	int m1, n1;
	fscanf(fp, "%d %d\n", &m1, &n1);
	
	if (m1 != n) {
		printf("bad-matrices");						// Checks if both matrices are compatible to multiply
		return 0;
	}
	
	int **arr1 = malloc(sizeof(int *) * m1);
	for (i = 0; i < m1; i++) {
		arr1[i] = malloc(sizeof(int) * n1);
	}
	int indexm1 = m1-1;
	int indexn1 = 0;
	
	while (indexm1 >= 0) {
		if ((m1 == 1) && (n1 == 1)) {
			fscanf(fp, "%d\n", &temp);					// Stores value if it is a 1 x 1 matrix
			arr1[0][0] = temp;
			break;
		} else if (n1 == 1) {
			fscanf(fp, "%d\n", &temp);
			arr1[indexm1][indexn1] = temp;				// Stores value if it is a m x 1 matrix
		} else if (n1 != 1) {
			while(fscanf(fp, "%d", &temp) == 1) {
				if (indexn1 == n1-2) {
					arr1[indexm1][indexn1] = temp;			// Stores the scanned numbers into another 2d array
					indexn1++;
					fscanf(fp, "%d\n", &temp);
					arr1[indexm1][indexn1] = temp;
					break;
				}
				arr1[indexm1][indexn1] = temp;
				indexn1++;
			}
		}
		indexn1 = 0;
		indexm1--;
	}
	
	int **Final = malloc(m * sizeof(int*));
	for (i = 0; i < m; i++) {
		Final[i] = malloc(n1 * sizeof(int));
	}
	
	multiply(Final, m, n1, arr, m, n, arr1, m1, n1);					// MULTIPLY MATRIX
	printMatrix(Final, m, n1);									// PRINT MATRIX
	
	for (i = 0; i < m; i++) {
		free(arr[i]);						
	}
	free(arr);									// Free all Dynamically Allocated Memory
	for (i = 0; i < m1; i++) {
		free(arr1[i]);
	}
	free(arr1);
	for (i = 0; i < m; i++) {
		free(Final[i]);
	}
	free(Final);
	fclose(fp);									// Close the file
	return 0;
}

void printMatrix(int **arr, int m, int n) {		// Prints the matrix
	int i, j;
	for (i = m-1; i >= 0; i--) {
		for (j = 0; j <= n-1; j++) {
			if (j == n-1) {
				printf("%d\n",arr[i][j]);
				break;
			}
			printf("%d	",arr[i][j]);
		}
	}
}

void multiply(int** Final, int M, int N, int** mat1, int m, int n, int** mat2, int m1, int n1) {  // Algorithm to Multiply
	int i, j, c;
	int Sum = 0;
	int cm = m1-1;
	for (i = m-1; i >= 0; i--) {
		for (j = 0; j < n1; j++) {
			for (c = 0; c < n; c++) {
				Sum = Sum + (mat1[i][c] * mat2[cm][j]);
				cm--;
			}
			Final[i][j] = Sum;
			cm = m1-1;
			Sum = 0;
		}
	}
}