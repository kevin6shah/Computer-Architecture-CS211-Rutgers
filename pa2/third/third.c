#include <stdio.h>
#include <stdlib.h>

int Sudoku[9][9];
void print();
int isAllowed(int i, int j, int num);
int isAllowedForValidity(int i, int j, int num);
int validSudoku();
int solve();

int main(int argc, char** argv) {
	if (argc != 2) {									// In case file is not given, or too many given
		printf("Insufficient Arguments!\n");
		return 0;
	}
	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) return 0;
	
	int i, j;
	char temp;
	for (i = 0; i < 9; i++) {				// Fills the Sudoku Board from the values of the file
		for (j = 0; j < 9; j++) {
			if (j != 8) {
				fscanf(fp, " %c", &temp);
				if (temp == '-') {
					Sudoku[i][j] = 0;
				} else Sudoku[i][j] = temp - '0';
			} else {
				fscanf(fp, " %c\n", &temp);
				if (temp == '-') {
					Sudoku[i][j] = 0;
				} else Sudoku[i][j] = temp - '0';
			}
		}
	}
	
	if (validSudoku() == 0) {			// Checks if the filled board is valid or not
		printf("no-solution");
		return 0;
	}
	
	solve();			// Solves Sudoku
	
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (Sudoku[i][j] == 0) {
				printf("no-solution");			// Checks if the board was incorrect or not
				return 0;
			}
		}
	}
	
	if (validSudoku() == 0) {
		printf("no-solution");
		return 0;
	}
	
	print();
	
	fclose(fp);
	return 0;
}

void print() {				// Prints the Sudoku board
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (j != 8) {
				printf("%d\t", Sudoku[i][j]);
			} else printf("%d\n", Sudoku[i][j]);
		}
	}
}

int validSudoku() {			// Checks if the Sudoku board is valid or not
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (Sudoku[i][j] != 0) {
				if (isAllowedForValidity(i, j, Sudoku[i][j]) == 0) {
					return 0;
				}
			}
		}
	}
	return 1;
}

int isAllowedForValidity(int i, int j, int num) {			// Function to help check validity of Sudoku board
	int x, y;
	
	// Step 1: Checks the Columns
	for (x = 0; x < 9; x++) {
		if (x == j) {
			if (x < 8) {
				x++;
			} else break;
		} else {
			if (Sudoku[i][x] == num) {
				return 0;
			}
		}
	}
	
	// Step 2: Check the Rows
	for (y = 0; y < 9; y++) {
		if (y == i) {
			if (y < 8) {
				y++;
			} else break;
		} else {
			if (Sudoku[y][j] == num) {
				return 0;
			}
		}
	}
	
	// Step 3: Check the box
	int a = i - i % 3;
	int b = j - j % 3;
	for (x = a; x < a + 3; x++) {
		for (y = b; y < b + 3; y++) {
			if (x == i && y == j) {
				if (y < b + 2) {
					y++;
				} else if (x < a + 3 && y == b + 2) {
					x++;
					break;
				} else if (x == a + 2 && y == b + 2) {
					break;
				}
			} else {
				if (Sudoku[x][y] == num) {
					return 0;
				}
			}
		}
	}
	return 1;
}

int isAllowed(int i, int j, int num) {
	int x, y;
	// Step 1: Check the Columns
	for (x = 0; x < 9; x++) {
		if (Sudoku[i][x] == num) {
			return 0;
		}
	}
	// Step 2: Check the Rows
	for (y = 0; y < 9; y++) {
		if (Sudoku[y][j] == num) {
			return 0;
		}
	}
	// Step 3: Check the Box
	int a = i - i % 3;
	int b = j - j % 3;
	for (x = a; x < a + 3; x++) {
		for (y = b; y < b + 3; y++) {
			if (Sudoku[x][y] == num) {
				return 0;
			}
		}
	}
	return 1;
}


int solve() {				// Solves the Sudoku board using backtracking
	int i, j;
	for (i = 0; i < 9; i++) {
		for (j = 0; j < 9; j++) {
			if (Sudoku[i][j] == 0) {
				int n;
				for (n = 1; n <= 9; n++) {
					if (isAllowed(i, j, n) == 1) {
						Sudoku[i][j] = n;
						if (solve() == 1) {
							return 1;
						} else {
							Sudoku[i][j] = 0;
						}
					}
				}
				return 0;
			}
		}
	}
	return 1;
}