#include <stdio.h>
#include <stdlib.h>

void printMatrix(double **arr, int m, int n);
void multiply(double** Final, double** mat1, int m, int n, double** mat2, int m1, int n1);
void transpose(double** Final, double **mat, int m, int n);
void inverse(double** Final, double** mat, int m);

int main(int argc, char** argv) {
	if (argc != 3) {
		printf("Not enough arguments!\n");
		return 0;
	}
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) return 0;
	int n, m, i;
	fscanf(fp, "%d\n", &n);
	n = n + 1;
	fscanf(fp, "%d\n", &m);
	double temp;
	
	double **X = malloc(sizeof(double*) * m);			// Allocate memory for first double array X
	for (i = 0; i < m; i++) {
		X[i] = malloc(sizeof(double) * n);
	}
	
	double **Y = malloc(sizeof(double*) * m);			// Allocate memory for first double array Y
	for (i = 0; i < m; i++) {
		Y[i] = malloc(sizeof(double));
	}
	
	for (i = 0; i < m; i++) {
		X[i][0] = 1;
	}
	
	int j;
	for (i = m-1; i >= 0; i--) {				// Fills Matrix X and Y at the same time
		for (j = 0; j < n; j++) {
			if (j != n-1) {
				fscanf(fp, "%lf,", &temp);
				X[i][j+1] = temp;
			} else {
				fscanf(fp, "%lf\n", &temp);
				Y[i][0] = temp;
			}
		}
	}
	fclose(fp);
	
	FILE *fp1 = fopen(argv[2], "r");
	if (fp1 == NULL) return 0;
	int m1;
	fscanf(fp1, "%d\n", &m1);
	
	double **X_test = malloc(sizeof(double*) * m1);			// Allocate memory for first double array X_test
	for (i = 0; i < m1; i++) {
		X_test[i] = malloc(sizeof(double) * n);
	}
	
	for (i = 0; i < m1; i++) {
		X_test[i][0] = 1;
	}
	
	for (i = m1-1; i >= 0; i--) {			// Fills Matrix X_test
		for (j = 0; j < n-1; j++) {
			if (j != n-2) {
				fscanf(fp1, "%lf,", &temp);
				X_test[i][j+1] = temp;
			} else {
				fscanf(fp1, "%lf\n", &temp);
				X_test[i][j+1] = temp;
			}
		}
	}
	fclose(fp1);
	
	
					// To solve, need to use (Xt*X)inverse * Xt * Y
					
	double **Xt = malloc(sizeof(double*) * n);			// Allocate memory for first double array Xt
	for (i = 0; i < n; i++) {
		Xt[i] = malloc(sizeof(double) * m);
	}
	
	// Step 1: Transpose of X
	transpose(Xt, X, m, n);
	
	double **XtX = malloc(sizeof(double*) * n);			// Allocate memory for first double array Xt * X Product
	for (i = 0; i < n; i++) {
		XtX[i] = malloc(sizeof(double) * n);
	}
	
	// Step 2: Multiply X Transpose times X
	multiply(XtX, Xt, n, m, X, m, n);
	
	double **inv = malloc(sizeof(double*) * n);			// Allocate memory for first double array Xt * X inverse Product
	for (i = 0; i < n; i++) {
		inv[i] = malloc(sizeof(double) * n);
	}
	
	// Step 3: Find the Inverse of Xt * X
	inverse(inv, XtX, n);
	
	double **invXt = malloc(sizeof(double*) * n);			// Allocate memory for first double array inverse * Xt Product
	for (i = 0; i < n; i++) {
		invXt[i] = malloc(sizeof(double) * m);
	}
	
	// Step 4: Multiply the inverse * Xt
	multiply(invXt, inv, n, n, Xt, n, m);
	
	double **W = malloc(sizeof(double*) * m);			// Allocate memory for first double array invXt * Y Product
	for (i = 0; i < m; i++) {
		W[i] = malloc(sizeof(double));
	}
	
	// Step 5: Build W, multiply invXt * Y
	multiply(W, invXt, n, m, Y, m, 1);
	
						// Finally need to Solve X_test * W = YFinal
						
	double **YFinal = malloc(sizeof(double*) * m1);			// Allocate memory for first double array YFinal
	for (i = 0; i < m1; i++) {
		YFinal[i] = malloc(sizeof(double));
	}
	
	// Final Step: Multiply X_test * W
	multiply(YFinal, X_test, m1, n, W, n, 1);
	printMatrix(YFinal, m1, 1);
	
	for (i = 0; i < m; i++) {
		free(X[i]);
	}
	free(X);
	for (i = 0; i < n; i++) {
		free(Xt[i]);
	}
	free(Xt);
	for (i = 0; i < n; i++) {
		free(XtX[i]);
	}
	free(XtX);
	for (i = 0; i < n; i++) {
		free(inv[i]);
	}
	free(inv);
	for (i = 0; i < n; i++) {
		free(invXt[i]);
	}
	free(invXt);
	for (i = 0; i < m; i++) {
		free(Y[i]);
	}
	free(Y);
	for (i = 0; i < m; i++) {
		free(W[i]);
	}
	free(W);
	for (i = 0; i < m1; i++) {
		free(X_test[i]);
	}
	free(X_test);
	for (i = 0; i < m1; i++) {
		free(YFinal[i]);
	}
	free(YFinal);
	return 0;
}

void printMatrix(double **arr, int m, int n) {		// Prints the matrix
	int i, j;
	for (i = m-1; i >= 0; i--) {
		for (j = 0; j < n; j++) {
			if (j == n-1) {
				printf("%0.0lf\n",arr[i][j]);
				break;
			}
			printf("%0.0lf	",arr[i][j]);
		}
	}
}

void multiply(double** Final, double** mat1, int m, int n, double** mat2, int m1, int n1) {  // Algorithm to Multiply
	int i, j, c;
	double Sum = 0;
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

void transpose(double** Final, double **mat, int m, int n) {	// Finds the Transpose of a Matrix
	int i, j;
	int k = 0;
	int l = 0;
	for (i = m-1; i >= 0; i--) {
		for (j = n-1; j >= 0; j--) {
			Final[k][l] = mat[i][j];
			k++;
		}
		l++;
		k = 0;
	}
}

// Finds the Inverse of a Square matrix using Gauss-Jordan Elimination
// And stores the inverse in the Final matrix
void inverse(double** Final, double** mat, int m) {
	double** aug = malloc(sizeof(double*) * m);
	int i, j;
	for (i = 0; i < m; i++) {
		aug[i] = malloc(sizeof(double) * m * 2);
	}
	int k = (m*2)-1;
	int c = 0;
	for (i = 0; i < m; i++) {				// Creates the Augmented Matrix
		for (j = 0; j < m*2; j++) {
			if (j < m) {
				aug[i][j] = mat[i][j];
			} else {
				if (j != k) {
					aug[i][j] = 0;
				} else {
					aug[i][j] = 1;
					c = 1;
				}
			}
		}
		if (c == 1) {
			k--;
		}
	}
	
	c = 0;
	// Step 1: Put all 1's on diagonal
	for (i = m-1; i >= 0; i--) {
		if (i < m-1) {
			int counter = 0;
			for (j = 0; j < c; j++) {
				if (aug[i][j] != 0) {
					counter = 1;
				}
			}
			if (counter == 1) {
				for (j = 0; j < c; j++) {
					int x;
					if (aug[i][j] != 0) {
						int loc = m-1-j;
						if (aug[loc][j] == aug[i][j]) {
							for (x = j; x < m*2; x++) {
								aug[i][x] = aug[i][x] - aug[loc][x];
							}
						} else {
							double temp = aug[i][j];
							for (x = j; x < m*2; x++) {
								aug[i][x] = aug[i][x] - (temp*aug[loc][x]);
							}
						}
					}
				}
			}
		}
		if (aug[i][c] != 1) {
			double div = aug[i][c];
			for (j = 0; j < m*2; j++) {
				if (aug[i][j] != 0) {
					aug[i][j] = aug[i][j]/div;
				}
			}
		}
		c++;
	}
	
	c = m-2;
	// Step 2: Put all 0's around the diagonal
	for (i = 1; i < m; i++) {
		int counter = 0;
		for (j = m-1; j > c; j--) {
			if (aug[i][j] != 0) {
				counter = 1;
			}
		}
		if (counter == 1) {
			for (j = m-1; j > c; j--) {
				int x;
				if (aug[i][j] != 0) {
					int loc = m-1-j;
					if (aug[loc][j] == aug[i][j]) {
						for (x = j; x < m*2; x++) {
							aug[i][x] = aug[i][x] - aug[loc][x];
						}
					} else {
						double temp = aug[i][j];
						for (x = j; x < m*2; x++) {
							aug[i][x] = aug[i][x] - (temp*aug[loc][x]);
						}
					}
				}
			}
		}
		c--;
	}
	
	// Step 3: Fill the Final Matrix with the inverse values
	for (i = 0; i < m; i++) {
		for (j = m; j < m*2; j++) {
			Final[i][j-m] = aug[i][j];
		}
	}
}