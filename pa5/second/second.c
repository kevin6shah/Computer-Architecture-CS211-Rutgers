#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

struct HashNode {
	char Variable[100];
	short *Array;
};

struct HashNode *Hash;	// Initialize the Hash Map
int Cap;
int Capacity;

void greyCode(int iterator, int numOfInputs, int index);	// Creates Grey Code truth table
void greyCodeTable(int **table, int iterator, int numOfInputs, int index);		// Creates Grey Code truth table
void Operate(char Input1[100], char Input2[100], char temp[100], int MAX, char OP);
void Decoder(int nInputs, int nOutputs, char **inputs, char **outputs, int MAX);
void Multiplexer(int nInputs, int nSelectors, char **inputs, char **selectors, char output[500], int MAX);
int isValid(char input[500]);

void print(int MAX, int numOfInputs) {	// Prints the table
	int i, j;
	for (i = 0; i < MAX; i++) {
		for (j = 0; j < numOfInputs; j++) {
			printf((j == numOfInputs - 1) ? "%d\n" : "%d ", Hash[j].Array[i]);
		}
	}
}

int power2(int num) {	// Checks if a given number is a power of 2
	while (num != 1) {
		if (num % 2 != 0) return 0;
		num = num/2;
	}
	return 1;
}

int main(int argc, char** argv) {

// *** All are just checks to see if the input is correct or not ***
	if (argc != 2) {
		printf("error");
		return 0;
	}
	
// *** End of Checks ***

	FILE *fp = fopen(argv[1], "r");
	if (fp == NULL) {
		printf("error");
		return 0;
	}

// *** Scans for the # of INPUTSSSS and allocates memory for it ***
	int numOfInputs, numOfOutputs, MAX, i, j;
	fscanf(fp, "INPUTVAR %d ", &numOfInputs);
	if (numOfInputs == 0) {
		printf("error");
		return 0;
	}
	MAX = pow(2, numOfInputs);
	Hash = malloc(sizeof(struct HashNode) * numOfInputs);
	
	for (i = 0; i < numOfInputs; i++) {
		(i == numOfInputs - 1) ? fscanf(fp, "%s\n", Hash[i].Variable) :  fscanf(fp, "%s ", Hash[i].Variable);
		Hash[i].Array = malloc(sizeof(short) * MAX);
		for (j = 0; j < MAX; j++) Hash[i].Array[j] = 0;
	}
	
// *** Scans for the # of OUTPUTSSSS and allocates memory for it ***
	fscanf(fp, "OUTPUTVAR %d ", &numOfOutputs);
	if (numOfOutputs == 0) {
		printf("error");
		return 0;
	}
	
// *** Capacity is used as a "placeholder index" for the print function to tell it where the Outputs end ***
	Capacity = numOfInputs + numOfOutputs;

	Hash = realloc(Hash, sizeof(struct HashNode) * (Capacity));
	for (i = numOfInputs; i < (Capacity); i++) {
		(i == (Capacity) - 1) ? fscanf(fp, "%s\n", Hash[i].Variable) :  fscanf(fp, "%s ", Hash[i].Variable);
		Hash[i].Array = malloc(sizeof(short) * MAX);
	}
	
	greyCode(0, numOfInputs, numOfInputs-1);	// Performs Grey Code and creates a truth table
	
	int counter = 0;
	char * NA = malloc(sizeof(char) * 10000);
	while (fgets(NA,10000,fp)) {
		counter++;
	}
	
// *** Cap is used to keep track of the amount of ACTUAL variables there are including temps ***
	Cap = Capacity;
	int DONE = 0;
	short *validLine = malloc(sizeof(short) * counter);
	for (i = 0; i < counter; i++) validLine[i] = 0;
	do {
		fclose(fp);
		fp = fopen(argv[1], "r");
		fgets(NA,10000,fp);
		fgets(NA,10000,fp);
		j = 0;
		while (!feof(fp)) {
			char tmp[100];
			if (fscanf(fp, "%s ", tmp) != EOF) {
				if (strcmp(tmp, "AND") == 0) {		// ========================== AND Case ==============================
					if (validLine[j] == 0) {
						char temp[100];
						char Input1[100];
						char Input2[100];
						fscanf(fp, "%s %s %s\n", Input1, Input2, temp);
						if (isValid(Input1) && isValid(Input2)) {
							Operate(Input1, Input2, temp, MAX, 'A');
							validLine[j] = 1;
						}
					} else fgets(NA, 10000, fp);
				} else if (strcmp(tmp, "OR") == 0) {	// ========================== OR Case ==============================
					if (validLine[j] == 0) {
						char temp[100];
						char Input1[100];
						char Input2[100];
						fscanf(fp, "%s %s %s\n", Input1, Input2, temp);
						if (isValid(Input1) && isValid(Input2)) {
							Operate(Input1, Input2, temp, MAX, 'O');
							validLine[j] = 1;
						}
					} else fgets(NA, 10000, fp);
				} else if (strcmp(tmp, "NOT") == 0) {	// ========================== NOT Case ==============================
					if (validLine[j] == 0) {
						char temp[100];
						char Input1[100];
						fscanf(fp, "%s %s\n", Input1, temp);
						if (isValid(Input1)) {
							Operate(Input1, NULL, temp, MAX, 'T');
							validLine[j] = 1;
						}
					} else fgets(NA, 10000, fp);
				} else if (strcmp(tmp, "XOR") == 0) {	// ========================== XOR Case ==============================
					if (validLine[j] == 0) {
						char temp[100];
						char Input1[100];
						char Input2[100];
						fscanf(fp, "%s %s %s\n", Input1, Input2, temp);
						if (isValid(Input1) && isValid(Input2)) {
							Operate(Input1, Input2, temp, MAX, 'X');
							validLine[j] = 1;
						}
					} else fgets(NA, 10000, fp);
				} else if (strcmp(tmp, "NAND") == 0) {	// ========================== NAND Case ==============================
					if (validLine[j] == 0) {
						char temp[100];
						char Input1[100];
						char Input2[100];
						fscanf(fp, "%s %s %s\n", Input1, Input2, temp);
						if (isValid(Input1) && isValid(Input2)) {
							Operate(Input1, Input2, temp, MAX, 'D');
							validLine[j] = 1;
						}
					} else fgets(NA, 10000, fp);
				} else if (strcmp(tmp, "NOR") == 0) {	// ========================== NOR Case ==============================
					if (validLine[j] == 0) {
						char temp[100];
						char Input1[100];
						char Input2[100];
						fscanf(fp, "%s %s %s\n", Input1, Input2, temp);
						if (isValid(Input1) && isValid(Input2)) {
							Operate(Input1, Input2, temp, MAX, 'R');
							validLine[j] = 1;
						}
					} else fgets(NA, 10000, fp);
				} else if (strcmp(tmp, "DECODER") == 0) {	// ========================== Decoder Case ==============================
					if (validLine[j] == 0) {
						int nInputs, VALID = 0;
						fscanf(fp, "%d ", &nInputs);
						int nOutputs = pow(2, nInputs);
						char **inputs = malloc(sizeof(char*) * 5000);
						for (i = 0; i < 5000; i++) inputs[i] = malloc(sizeof(char) * 500);
						char **outputs = malloc(sizeof(char*) * 10000);
						for (i = 0; i < 5000; i++) outputs[i] = malloc(sizeof(char) * 500);
						for (i = 0; i < nInputs; i++) fscanf(fp, "%s ", &inputs[i][0]);
						for (i = 0; i < nOutputs; i++) (i == nOutputs-1) ? fscanf(fp, "%s\n", &outputs[i][0]) : fscanf(fp, "%s ", &outputs[i][0]);
						for (i = 0; i < nInputs; i++) {
							if (isValid(inputs[i])) {
								VALID = 1;
							} else {
								VALID = 0;
								break;
							}
						}
						if (VALID == 1) {
							Decoder(nInputs, nOutputs, inputs, outputs, MAX);
							validLine[j] = 1;
						}
					} else fgets(NA, 10000, fp);
				} else if (strcmp(tmp, "MULTIPLEXER") == 0) {	// ======================= Multiplexer Case ==============================
					if (validLine[j] == 0) {
						int nInputs, nSelectors, VALID = 0;
						fscanf(fp, "%d ", &nInputs);
						if (power2(nInputs) == 0) {
							printf("error");
							return 0;
						}
						nSelectors = log(nInputs)/log(2);
						char **inputs = malloc(sizeof(char*) * 5000);
						for (i = 0; i < 5000; i++) inputs[i] = malloc(sizeof(char) * 500);
						char **selectors = malloc(sizeof(char*) * 5000);
						for (i = 0; i < 5000; i++) selectors[i] = malloc(sizeof(char) * 500);
						char output[500];
						for (i = 0; i < nInputs; i++) fscanf(fp, "%s ", &inputs[i][0]);
						for (i = 0; i < nSelectors; i++) fscanf(fp, "%s ", &selectors[i][0]);
						fscanf(fp, "%s\n", output);
						for (i = 0; i < nInputs; i++) {
							if (isValid(inputs[i])) {
								VALID = 1;
							} else {
								VALID = 0;
								break;
							}
						}
						if (VALID == 1) {
							for (i = 0; i < nSelectors; i++) {
								if (isValid(selectors[i])) {
									VALID = 1;
								} else {
									VALID = 0;
									break;
								}
							}
						}
						if (VALID == 1) {
							Multiplexer(nInputs, nSelectors, inputs, selectors, output, MAX);
							validLine[j] = 1;
						}
					} else fgets(NA, 10000, fp);
				} else {
					printf("error");
					return 0;
				}
			} else break;
			j++;
		}
		for (i = 0; i < counter; i++) {
			if (validLine[i] == 1) {
				DONE = 1;
			} else {
				DONE = 0;
				break;
			}
		}
	} while (DONE != 1);
	print(MAX, Capacity);
	
	fclose(fp);
	return 0;
}

int isValid(char input[500]) {
	if (input[0] == '0' || input[0] == '1') return 1;
	int i;
	for (i = 0; i < Cap; i++) {
		if (strcmp(input, Hash[i].Variable) == 0) return 1;
	}
	return 0;
}

void Multiplexer(int nInputs, int nSelectors, char **inputs, char **selectors, char output[500], int MAX) {
	int i, j, k, index = -1, OutputIndex = -1;
	for (i = 0; i < Cap; i++) {
		if (strcmp(Hash[i].Variable, output) == 0) {
			OutputIndex = i;
			break;
		}
	}
	if (OutputIndex == -1) {
		Cap++;
		Hash = realloc(Hash, sizeof(struct HashNode) * Cap);
		strcpy(Hash[Cap-1].Variable, output);
		Hash[Cap-1].Array = malloc(sizeof(short) * MAX);
	}
	for (i = 0; i < MAX; i++) {
		int flag = 0, num, store = 0;
		for (j = 0; j < nSelectors; j++) {
			for (k = 0; k < Cap; k++) if (strcmp(selectors[j], Hash[k].Variable) == 0) index = k;
			if (index == -1) {
				flag = 1;
				num = atoi(selectors[j]);
			}
			if (flag == 0) {
				store <<= 1;
				store += Hash[index].Array[i];
			} else if (flag == 1) {
				store <<= 1;
				store += num;
			}
			flag = 0, index = -1;
		}
		int binary = 0;
		for (; store; store = store >> 1) {
			binary ^= store;
		}
		store = binary;
		for (k = 0; k < Cap; k++) if (strcmp(inputs[store], Hash[k].Variable) == 0) index = k;
		if (index == -1) {
			flag = 1;
			num = atoi(inputs[store]);
		}
		if (OutputIndex == -1) {
			if (flag == 0) Hash[Cap-1].Array[i] = Hash[index].Array[i];
			if (flag == 1) Hash[Cap-1].Array[i] = num;
		} else {
			if (flag == 0) Hash[OutputIndex].Array[i] = Hash[index].Array[i];
			if (flag == 1) Hash[OutputIndex].Array[i] = num;
		}
		index = -1;
	}
}

void Decoder(int nInputs, int nOutputs, char **inputs, char **outputs, int MAX) {
	int i, j = 0, k, l, index = -1, counter = 0, c = 0, x = 0;
	int **table = malloc(sizeof(int *) * nInputs);
	for (i = 0; i < nInputs; i++) table[i] = malloc(sizeof(int) * pow(2, nInputs));
	greyCodeTable(table, 0, nInputs, nInputs-1);
	int NUM = Cap;
	int *VALIDITY = malloc(sizeof(int) * nOutputs);
	for (i = 0; i < nOutputs; i++) {
		VALIDITY[i] = -1;
		for (j = 0; j < Cap; j++) {
			if (strcmp(outputs[i], Hash[j].Variable) == 0) {
				VALIDITY[i] = j;
				break;
			}
		}
		if (VALIDITY[i] == -1) counter++;
	}
	Cap+=counter;
	Hash = realloc(Hash, sizeof(struct HashNode) * Cap);
	j = 0;
	for (i = 0; i < nOutputs; i++) {
		if (VALIDITY[i] == -1) {
			strcpy(Hash[NUM+j].Variable, outputs[i]);
			Hash[NUM+j].Array = malloc(sizeof(short) * MAX);
			j++;
		}
	}
	for (i = 0; i < nOutputs; i++) {
		for (k = 0; k < MAX; k++) {
			int store, flag = 0, num;
			for (j = 0; j < nInputs; j++) {
				for (l = 0; l < NUM; l++) if (strcmp(Hash[l].Variable, inputs[j]) == 0) index = l;
				if (index == -1) {
					flag = 1;
					num = atoi(inputs[j]);
				}
				if (j == 0) {
					if (table[j][c] == 1) {
						if (flag == 0) store = Hash[index].Array[k];
						if (flag == 1) store = num;
					} else {
						if (flag == 0) store = !Hash[index].Array[k];
						if (flag == 1) store = !num;
					}
				} else {
					if (table[j][c] == 1) {
						if (flag == 0) store &= Hash[index].Array[k];
						if (flag == 1) store &= num;
					} else {
						int complement;
						if (flag == 0) complement = !Hash[index].Array[k];
						if (flag == 1) complement = !num;
						store &= complement;
					}
				}
				flag = 0;
				index = -1;
			}
			if (VALIDITY[i] == -1) {
				Hash[NUM+x].Array[k] = store;
			} else {
				Hash[VALIDITY[i]].Array[k] = store;
			}
		}
		if (VALIDITY[i] == -1) x++;
		c++;
	}
}

void Operate(char Input1[100], char Input2[100], char temp[100], int MAX, char OP) {
	int First, Second, duplicate = 0, index, i;
	for (i = 0; i < Cap; i++) {
		if (strcmp(Hash[i].Variable, Input1) == 0) {
			First = i;
		} 
		if (OP != 'T') {
			if (strcmp(Hash[i].Variable, Input2) == 0) {
				Second = i;
			} 
		}
		if (strcmp(Hash[i].Variable, temp) == 0) {
			duplicate = 1;
			index = i;
		}
	}
	int flag = 0, num;
	if (OP != 'T') {
		if (Input1[0] == '0' || Input1[0] == '1' || Input2[0] == '0' || Input2[0] == '1') {
			if (Input1[0] == '0') {
				flag = 1;
				num = 0;
			} else if (Input1[0] == '1') {
				flag = 1;
				num = 1;
			} else if (Input2[0] == '0') {
				flag = 2;
				num = 0;
			} else if (Input2[0] == '1') {
				flag = 2;
				num = 1;
			}
		}
	} else if (OP == 'T') {
		if (Input1[0] == '0' || Input1[0] == '1') {
			if (Input1[0] == '0') {
				flag = 1;
				num = 0;
			} else if (Input1[0] == '1') {
				flag = 1;
				num = 1;
			}
		}
	}
	if (duplicate == 0) {
		Cap++;
		Hash = realloc(Hash, sizeof(struct HashNode) * Cap);
		strcpy(Hash[Cap-1].Variable, temp);
		Hash[Cap-1].Array = malloc(sizeof(short) * MAX);
		for (i = 0; i < MAX; i++) {
			if (OP == 'A') {
				if (flag == 0) Hash[Cap-1].Array[i] = Hash[First].Array[i] & Hash[Second].Array[i];
				if (flag == 1) Hash[Cap-1].Array[i] = num & Hash[Second].Array[i];
				if (flag == 2) Hash[Cap-1].Array[i] = Hash[First].Array[i] & num;
			} else if (OP == 'O') {
				if (flag == 0) Hash[Cap-1].Array[i] = Hash[First].Array[i] | Hash[Second].Array[i];
				if (flag == 1) Hash[Cap-1].Array[i] = num | Hash[Second].Array[i];
				if (flag == 2) Hash[Cap-1].Array[i] = Hash[First].Array[i] | num;
			} else if (OP == 'X') {
				if (flag == 0) Hash[Cap-1].Array[i] = Hash[First].Array[i] ^ Hash[Second].Array[i];
				if (flag == 1) Hash[Cap-1].Array[i] = num ^ Hash[Second].Array[i];
				if (flag == 2) Hash[Cap-1].Array[i] = Hash[First].Array[i] ^ num;
			} else if (OP == 'D') {
				if (flag == 0) Hash[Cap-1].Array[i] = !(Hash[First].Array[i] & Hash[Second].Array[i]);
				if (flag == 1) Hash[Cap-1].Array[i] = !(num & Hash[Second].Array[i]);
				if (flag == 2) Hash[Cap-1].Array[i] = !(Hash[First].Array[i] & num);
			} else if (OP == 'R') {
				if (flag == 0) Hash[Cap-1].Array[i] = !(Hash[First].Array[i] | Hash[Second].Array[i]);
				if (flag == 1) Hash[Cap-1].Array[i] = !(num | Hash[Second].Array[i]);
				if (flag == 2) Hash[Cap-1].Array[i] = !(Hash[First].Array[i] | num);
			} else if (OP == 'T') {
				if (flag == 0) Hash[Cap-1].Array[i] = !Hash[First].Array[i];
				if (flag == 1) Hash[Cap-1].Array[i] = !num;
			}
		}
	} else {
		for (i = 0; i < MAX; i++) {
			if (OP == 'A') {
				if (flag == 0) Hash[index].Array[i] = Hash[First].Array[i] & Hash[Second].Array[i];
				if (flag == 1) Hash[index].Array[i] = num & Hash[Second].Array[i];
				if (flag == 2) Hash[index].Array[i] = Hash[First].Array[i] & num;
			} else if (OP == 'O') {
				if (flag == 0) Hash[index].Array[i] = Hash[First].Array[i] | Hash[Second].Array[i];
				if (flag == 1) Hash[index].Array[i] = num | Hash[Second].Array[i];
				if (flag == 2) Hash[index].Array[i] = Hash[First].Array[i] | num;
			} else if (OP == 'X') {
				if (flag == 0) Hash[index].Array[i] = Hash[First].Array[i] ^ Hash[Second].Array[i];
				if (flag == 1) Hash[index].Array[i] = num ^ Hash[Second].Array[i];
				if (flag == 2) Hash[index].Array[i] = Hash[First].Array[i] ^ num;
			} else if (OP == 'D') {
				if (flag == 0) Hash[index].Array[i] = !(Hash[First].Array[i] & Hash[Second].Array[i]);
				if (flag == 1) Hash[index].Array[i] = !(num & Hash[Second].Array[i]);
				if (flag == 2) Hash[index].Array[i] = !(Hash[First].Array[i] & num);
			} else if (OP == 'R') {
				if (flag == 0) Hash[index].Array[i] = !(Hash[First].Array[i] | Hash[Second].Array[i]);
				if (flag == 1) Hash[index].Array[i] = !(num | Hash[Second].Array[i]);
				if (flag == 2) Hash[index].Array[i] = !(Hash[First].Array[i] | num);
			} else if (OP == 'T') {
				if (flag == 0) Hash[index].Array[i] = !Hash[First].Array[i];
				if (flag == 1) Hash[index].Array[i] = !num;
			}
		}
	}
}

void greyCode(int iterator, int numOfInputs, int index) {	// Recursively creates Grey Code truth table
	if (numOfInputs == 1) {
		Hash[index].Array[0] = 0;
		Hash[index].Array[1] = 1;
		return;
	}
	int MAX = pow(2, numOfInputs), i;
	for (i = 0; i < MAX; i++) {
		if (i < MAX/2) {
			Hash[iterator].Array[i] = 0;
		} else Hash[iterator].Array[i] = 1;
	}

	greyCode(iterator+1, numOfInputs-1, index);
	
	int k;
	for (i = iterator+1; i < index+1; i++) {
		int j = (MAX/2) - 1;
		for (k = j+1; k < MAX; k++) {
			Hash[i].Array[k] = Hash[i].Array[j];
			j--;
		}
	}
	return;
}

void greyCodeTable(int **table, int iterator, int numOfInputs, int index) {	// Recursively creates Grey Code truth table
	if (numOfInputs == 1) {
		table[index][0] = 0;
		table[index][1] = 1;
		return;
	}
	int MAX = pow(2, numOfInputs), i;
	for (i = 0; i < MAX; i++) {
		if (i < MAX/2) {
			table[iterator][i] = 0;
		} else table[iterator][i] = 1;
	}

	greyCodeTable(table, iterator+1, numOfInputs-1, index);
	
	int k;
	for (i = iterator+1; i < index+1; i++) {
		int j = (MAX/2) - 1;
		for (k = j+1; k < MAX; k++) {
			table[i][k] = table[i][j];
			j--;
		}
	}
	return;
}