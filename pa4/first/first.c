#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

struct Block {
	size_t tag;
	unsigned short validBit;
	struct Block *next;
};

struct QueueNode {
	size_t tag;
	int blockIndex;
	struct QueueNode* next;
};

int memoryReads = 0, memoryWrites = 0, cacheHits = 0, cacheMisses = 0, 
preMemoryReads = 0, preMemoryWrites = 0, preCacheHits = 0, preCacheMisses = 0;
struct Block *Cache;
struct Block *PreCache;
struct QueueNode *Queue;
struct QueueNode *PreQueue;

void initializeCache(int numOfSets, int associativity, int blockSize); // Initializes the Cache
void initializePreCache(int numOfSets, int associativity, int blockSize); // Initializes the Pre Cache
void ReadFIFO (size_t address, int numOfSets, int associativity, int blockSize); // Reads from the cache
void ReadLRU (size_t address, int numOfSets, int associativity, int blockSize); // Reads from the cache
void WriteFIFO (size_t address, int numOfSets, int associativity, int blockSize); // Writes to the main memory (Write-through)
void WriteLRU (size_t address, int numOfSets, int associativity, int blockSize); // Writes to the main memory (Write-through)
void PreReadFIFO (size_t address, int numOfSets, int associativity, int blockSize); // Reads from the cache for Prefetch
void PreReadLRU (size_t address, int numOfSets, int associativity, int blockSize); // Reads from the cache for Prefetch
void PreWriteFIFO (size_t address, int numOfSets, int associativity, int blockSize); // Writes to the main memory (Write-through) for Prefetch
void PreWriteLRU (size_t address, int numOfSets, int associativity, int blockSize); // Writes to the main memory (Write-through) for Prefetch
void addToPreCacheFIFO(size_t tag, int setIndex); // Adds the tag to the cache
void moveToFrontPRE(size_t tag, int setIndex); // Moves the node to the front for LRU
void enqueue(size_t tag, int blockIndex); // Enqueues the Tag
size_t dequeue(int blockIndex); // Dequeues the Tag
void preEnqueue(size_t tag, int blockIndex); // Enqueues the Tag for Prefetch
size_t preDequeue(int blockIndex); // Dequeues the Tag for Prefetch
void moveToFront(size_t tag, int index); // Moves the node to the front for LRU

void print() {	// Prints the Queue
	struct Block* ptr = &Cache[0];
	printf("*********\n");
	while (ptr != NULL) {
		if (ptr->next == NULL) {
			printf("0x%zx\n", ptr->tag);
			printf("*****END****\n");
			return;
		}
		printf("0x%zx\t", ptr->tag);
		ptr = ptr->next;
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
	if (argc != 6) {
		printf("error");
		return 0;
	}
	int cacheSize, blockSize, n = -1;
	char associativity;
	char cachePolicy;
	if (isdigit(argv[1][0]) && isdigit(argv[4][0]) && (isdigit(argv[2][6]) || strcmp("assoc", argv[2]) == 0 || strcmp("direct", argv[2]) == 0)
	&& !(atoi(argv[1]) <= 0) && !(atoi(argv[4]) <= 0)) {
		if (power2(atoi(argv[1])) && power2(atoi(argv[4])) && (strcmp("direct", argv[2]) == 0 ||
			strcmp("assoc", argv[2]) == 0 || strncmp("assoc:", argv[2], 6) == 0) && (strcmp("fifo", argv[3]) == 0 || strcmp("lru", argv[3]) == 0)) {
			cacheSize = atoi(argv[1]);
			blockSize = atoi(argv[4]);
			if (strncmp("assoc:", argv[2], 6) == 0) {
				sscanf(argv[2], "assoc:%d", &n);
				if (n < 0) {
					printf("error");
					return 0;
				}
				associativity = 'n';
			} else if (strcmp("assoc", argv[2]) == 0){
				associativity = 'a';
			} else {
				associativity = 'd';
			}
			if (strcmp("fifo", argv[3]) == 0) {
				cachePolicy = 'f';
			} else cachePolicy = 'l';
		} else {
			printf("error");
			return 0;
		}
	} else {
		printf("error");
		return 0;
	}
	
// *** End of Checks ***

	FILE *fp = fopen(argv[5], "r");
	if (fp == NULL) {
		printf("error");
		return 0;
	}
	
// *** Initialize the Cache ***
	int numOfSets = 0;
	if (n != -1) {
		numOfSets = cacheSize/(n*blockSize);
		initializeCache(numOfSets, n, blockSize);
		initializePreCache(numOfSets, n, blockSize);
	}
	
	char rw;
	size_t address = 0;
	
	if (associativity == 'a') {					// Takes care of Fully Associative Cache
		numOfSets = 1;
		n = cacheSize/blockSize;
		initializeCache(numOfSets, n, blockSize);
		initializePreCache(numOfSets, n, blockSize);
	}	
	
	if (associativity == 'd') {					// Takes care of Direct Mapped Cache
		n = 1;
		numOfSets = cacheSize/blockSize;
		initializeCache(numOfSets, n, blockSize);
		initializePreCache(numOfSets, n, blockSize);
	}
	
// *** Reads the file and does what is needed ***
	while (fscanf(fp,"%*x: %c %zx\n", &rw, &address) == 2) {
		if (rw == 'R') {
			if (cachePolicy == 'f') {
				ReadFIFO(address, numOfSets, n, blockSize);
				PreReadFIFO(address, numOfSets, n, blockSize);
			} else {
				ReadLRU(address, numOfSets, n, blockSize);
				PreReadLRU(address, numOfSets, n, blockSize);
			}
		} else if (rw == 'W') {
			if (cachePolicy == 'f') {
				WriteFIFO(address, numOfSets, n, blockSize);
				PreWriteFIFO(address, numOfSets, n, blockSize);
			} else {
				WriteLRU(address, numOfSets, n, blockSize);
				PreWriteLRU(address, numOfSets, n, blockSize);
			}
		}
	}
	fclose(fp);
	printf("no-prefetch\n");
	printf("Memory reads: %d\n", memoryReads);
	printf("Memory writes: %d\n", memoryWrites);
	printf("Cache hits: %d\n", cacheHits);
	printf("Cache misses: %d\n", cacheMisses);
	printf("with-prefetch\n");
	printf("Memory reads: %d\n", preMemoryReads);
	printf("Memory writes: %d\n", preMemoryWrites);
	printf("Cache hits: %d\n", preCacheHits);
	printf("Cache misses: %d\n", preCacheMisses);
	return 0;
}

void moveToFront(size_t tag, int index) {
	if (Cache[index].validBit == 0) {
		Cache[index].tag = tag;
		Cache[index].validBit = 1;
		return;
	}
	if (Cache[index].tag == tag) return;
	if (Cache[index].next == NULL) {
		Cache[index].tag = tag;
		return;
	}
	struct Block *ptr = Cache[index].next;
	while (ptr != NULL) {
		if (ptr->tag == tag) {
			ptr->tag = -1;
			ptr->validBit = 0;
			while (Cache[index].validBit != 0) {
				ptr = &Cache[index];
				while (ptr != NULL) {
					if (ptr->next != NULL) {
						if (ptr->next->validBit == 0) {
							ptr->next->validBit = 1;
							ptr->next->tag = ptr->tag;
							ptr->tag = -1;
							ptr->validBit = 0;
							break;
						}
					}
					ptr = ptr->next;
				}
			}
			Cache[index].validBit = 1;
			Cache[index].tag = tag;
			return;
		}
		if (ptr->next == NULL) {
			ptr->tag = -1;
			ptr->validBit = 0;
			ptr->next = NULL;
			break;
		}
		ptr = ptr->next;
	}
	
	while (Cache[index].validBit != 0) {
		ptr = &Cache[index];
		while (ptr != NULL) {
			if (ptr->next != NULL) {
				if (ptr->next->validBit == 0) {
					ptr->next->validBit = 1;
					ptr->next->tag = ptr->tag;
					ptr->tag = -1;
					ptr->validBit = 0;
					break;
				}
			}
			ptr = ptr->next;
		}
	}
	
	Cache[index].validBit = 1;
	Cache[index].tag = tag;
	return;
}

void ReadLRU (size_t address, int numOfSets, int associativity, int blockSize) {
	int offset = (int) (log(blockSize)/log(2));
	int indexBits = (int) (log(numOfSets)/log(2));
	int setIndex = (int) ((address>>offset)% ((int) pow(2, (double) indexBits)));
	size_t tag = address>>(offset+indexBits);
	
	struct Block *ptr = &Cache[setIndex];
	int hit = 0;
	while (ptr != NULL) {
		if (ptr->validBit == 1) {
			if (tag == ptr->tag) {
				hit = 1;
			}
		}
		ptr = ptr->next;
	}
	if (hit == 1) {
		moveToFront(tag, setIndex);
		cacheHits++;
		return;
	} else {
		cacheMisses++;
		memoryReads++;
		moveToFront(tag, setIndex);
		return;
	}
}

void WriteLRU (size_t address, int numOfSets, int associativity, int blockSize) {
	int offset = (int) (log(blockSize)/log(2));
	int indexBits = (int) (log(numOfSets)/log(2));
	int setIndex = (int) ((address>>offset)% ((int) pow(2, (double) indexBits)));
	size_t tag = address>>(offset+indexBits);
	
	struct Block *ptr = &Cache[setIndex];
	int hit = 0;
	while (ptr != NULL) {
		if (ptr->validBit == 1) {
			if (tag == ptr->tag) {
				hit = 1;
			}
		}
		ptr = ptr->next;
	}
	if (hit == 1) {
		moveToFront(tag, setIndex);
		cacheHits++;
		memoryWrites++;
		return;
	} else {
		ReadLRU(address, numOfSets, associativity, blockSize);
		memoryWrites++;
		return;
	}
}

void moveToFrontPRE(size_t tag, int index) {
	if (PreCache[index].validBit == 0) {
		PreCache[index].tag = tag;
		PreCache[index].validBit = 1;
		return;
	}
	if (PreCache[index].tag == tag) return;
	if (PreCache[index].next == NULL) {
		PreCache[index].tag = tag;
		return;
	}
	struct Block *ptr = PreCache[index].next;
	while (ptr != NULL) {
		if (ptr->tag == tag) {
			ptr->tag = -1;
			ptr->validBit = 0;
			while (PreCache[index].validBit != 0) {
				ptr = &PreCache[index];
				while (ptr != NULL) {
					if (ptr->next != NULL) {
						if (ptr->next->validBit == 0) {
							ptr->next->validBit = 1;
							ptr->next->tag = ptr->tag;
							ptr->tag = -1;
							ptr->validBit = 0;
							break;
						}
					}
					ptr = ptr->next;
				}
			}
			PreCache[index].validBit = 1;
			PreCache[index].tag = tag;
			return;
		}
		if (ptr->next == NULL) {
			ptr->tag = -1;
			ptr->validBit = 0;
			ptr->next = NULL;
			break;
		}
		ptr = ptr->next;
	}
	
	while (PreCache[index].validBit != 0) {
		ptr = &PreCache[index];
		while (ptr != NULL) {
			if (ptr->next != NULL) {
				if (ptr->next->validBit == 0) {
					ptr->next->validBit = 1;
					ptr->next->tag = ptr->tag;
					ptr->tag = -1;
					ptr->validBit = 0;
					break;
				}
			}
			ptr = ptr->next;
		}
	}
	
	PreCache[index].validBit = 1;
	PreCache[index].tag = tag;
	return;
}

void PreReadLRU (size_t address, int numOfSets, int associativity, int blockSize) {
	int offset = (int) (log(blockSize)/log(2));
	int indexBits = (int) (log(numOfSets)/log(2));
	int setIndex = (int) ((address>>offset)% ((int) pow(2, (double) indexBits)));
	size_t tag = address>>(offset+indexBits);
	
	struct Block *ptr = &PreCache[setIndex];
	int hit = 0;
	while (ptr != NULL) {
		if (ptr->validBit == 1) {
			if (tag == ptr->tag) hit = 1;
		}
		ptr = ptr->next;
	}
	if (hit == 1) {
		moveToFrontPRE(tag, setIndex);
		preCacheHits++;
		return;
	} else {
		preCacheMisses++;
		moveToFrontPRE(tag, setIndex);
		preMemoryReads++;
		address+=blockSize;
		offset = (int) (log(blockSize)/log(2));
		indexBits = (int) (log(numOfSets)/log(2));
		setIndex = (int) ((address>>offset)% ((int) pow(2, (double) indexBits)));
		tag = address>>(offset+indexBits);
		int present = 0;
		ptr = &PreCache[setIndex];
		while (ptr != NULL) {
			if (ptr->tag == tag) present = 1;
			ptr = ptr->next;
		}
		if (present != 1) {
			moveToFrontPRE(tag, setIndex);
			preMemoryReads++;
		}
		return;
	}
}

void PreWriteLRU (size_t address, int numOfSets, int associativity, int blockSize) {
	int offset = (int) (log(blockSize)/log(2));
	int indexBits = (int) (log(numOfSets)/log(2));
	int setIndex = (int) ((address>>offset)% ((int) pow(2, (double) indexBits)));
	size_t tag = address>>(offset+indexBits);
	
	struct Block *ptr = &PreCache[setIndex];
	int hit = 0;
	while (ptr != NULL) {
		if (ptr->validBit == 1) {
			if (tag == ptr->tag) {
				hit = 1;
			}
		}
		ptr = ptr->next;
	}
	if (hit == 1) {
		preCacheHits++;
		moveToFrontPRE(tag, setIndex);
		preMemoryWrites++;
		return;
	} else {
		PreReadLRU(address, numOfSets, associativity, blockSize);
		preMemoryWrites++;
		return;
	}
}

void ReadFIFO (size_t address, int numOfSets, int associativity, int blockSize) {
	int offset = (int) (log(blockSize)/log(2));
	int indexBits = (int) (log(numOfSets)/log(2));
	int setIndex = (int) ((address>>offset)% ((int) pow(2, (double) indexBits)));
	size_t tag = address>>(offset+indexBits);
	
	struct Block *ptr = &Cache[setIndex];
	int hit = 0;
	while (ptr != NULL) {
		if (ptr->validBit == 1) {
			if (tag == ptr->tag) {
				hit = 1;
			}
		}
		ptr = ptr->next;
	}
	if (hit == 1) {
		cacheHits++;
		return;
	} else {
		cacheMisses++;
		ptr = &Cache[setIndex];
		while (ptr != NULL) {
			if (ptr->validBit == 0) {
				ptr->tag = tag;
				ptr->validBit = 1;
				enqueue(ptr->tag, setIndex);
				memoryReads++;
				return;
			}
			ptr = ptr->next;
		}
	}
	// *** All Bits are Valid and the program reaches Here ***
	// *** Implement FIFO policy to kick out first block ***
	// Should Kick the first one out and memoryReads++ and enqueue
	size_t Kick = dequeue(setIndex);
	ptr = &Cache[setIndex];
	if (Kick != -1) {
		while (ptr != NULL) {
			if (ptr->tag == Kick) {
				ptr->tag = tag;
				ptr->validBit = 1;
				enqueue(ptr->tag, setIndex);
				memoryReads++;
				return;
			}
			ptr = ptr->next;
		}
	}
}

void WriteFIFO (size_t address, int numOfSets, int associativity, int blockSize) {
	int offset = (int) (log(blockSize)/log(2));
	int indexBits = (int) (log(numOfSets)/log(2));
	int setIndex = (int) ((address>>offset)% ((int) pow(2, (double) indexBits)));
	size_t tag = address>>(offset+indexBits);
	
	struct Block *ptr = &Cache[setIndex];
	int hit = 0;
	while (ptr != NULL) {
		if (ptr->validBit == 1) {
			if (tag == ptr->tag) {
				hit = 1;
			}
		}
		ptr = ptr->next;
	}
	if (hit == 1) {
		cacheHits++;
		memoryWrites++;
		return;
	} else {
		ReadFIFO(address, numOfSets, associativity, blockSize);
		memoryWrites++;
		return;
	}
}

void addToPreCacheFIFO(size_t tag, int setIndex) {
	struct Block *ptr = &PreCache[setIndex];
	// Checks whether the tag is already present in the Cache or not
	while (ptr != NULL) {
		if (ptr->tag == tag) return;
		ptr = ptr->next;
	}
	ptr = &PreCache[setIndex];
	// Adds the block into the cache if there is an open spot
	while (ptr != NULL) {
		if (ptr->validBit == 0) {
			ptr->tag = tag;
			ptr->validBit = 1;
			preEnqueue(ptr->tag, setIndex);
			preMemoryReads++;
			return;
		}
		ptr = ptr->next;
	}
	
	// *** FIFO Policy Implemented ***
	// *** Should Kick out the first block ***
	size_t Kick = preDequeue(setIndex);
	ptr = &PreCache[setIndex];
	if (Kick != -1) {
		while (ptr != NULL) {
			if (ptr->tag == Kick) {
				ptr->tag = tag;
				ptr->validBit = 1;
				preEnqueue(ptr->tag, setIndex);
				preMemoryReads++;
				return;
			}
			ptr = ptr->next;
		}
	}
}

void PreReadFIFO (size_t address, int numOfSets, int associativity, int blockSize) {
	int offset = (int) (log(blockSize)/log(2));
	int indexBits = (int) (log(numOfSets)/log(2));
	int setIndex = (int) ((address>>offset)% ((int) pow(2, (double) indexBits)));
	size_t tag = address>>(offset+indexBits);
	
	struct Block *ptr = &PreCache[setIndex];
	int hit = 0;
	while (ptr != NULL) {
		if (ptr->validBit == 1) {
			if (tag == ptr->tag) hit = 1;
		}
		ptr = ptr->next;
	}
	if (hit == 1) {
		preCacheHits++;
		return;
	} else {
		preCacheMisses++;
		addToPreCacheFIFO(tag, setIndex);
		address+=blockSize;
		offset = (int) (log(blockSize)/log(2));
		indexBits = (int) (log(numOfSets)/log(2));
		setIndex = (int) ((address>>offset)% ((int) pow(2, (double) indexBits)));
		tag = address>>(offset+indexBits);
		addToPreCacheFIFO(tag, setIndex);
		return;
	}
}

void PreWriteFIFO (size_t address, int numOfSets, int associativity, int blockSize) {
	int offset = (int) (log(blockSize)/log(2));
	int indexBits = (int) (log(numOfSets)/log(2));
	int setIndex = (int) ((address>>offset)% ((int) pow(2, (double) indexBits)));
	size_t tag = address>>(offset+indexBits);
	
	struct Block *ptr = &PreCache[setIndex];
	int hit = 0;
	while (ptr != NULL) {
		if (ptr->validBit == 1) {
			if (tag == ptr->tag) {
				hit = 1;
			}
		}
		ptr = ptr->next;
	}
	if (hit == 1) {
		preCacheHits++;
		preMemoryWrites++;
		return;
	} else {
		PreReadFIFO(address, numOfSets, associativity, blockSize);
		preMemoryWrites++;
		return;
	}
}

void initializeCache(int numOfSets, int associativity, int blockSize) { // Initializes the Cache
	Cache = malloc(sizeof(struct Block) * numOfSets);
	int i, j;
	for (i = 0; i < numOfSets; i++) {
		struct Block *ptr = malloc(sizeof(struct Block));
		ptr->tag = -1;
		ptr->validBit = 0;
		ptr->next = NULL;
		for (j = 1; j < associativity; j++) {
			struct Block *temp = malloc(sizeof(struct Block));
			temp->tag = -1;
			temp->validBit = 0;
			temp->next = ptr;
			ptr = temp;
		}
		Cache[i] = *ptr;
	}
}

void initializePreCache(int numOfSets, int associativity, int blockSize) { // Initializes the Cache
	PreCache = malloc(sizeof(struct Block) * numOfSets);
	int i, j;
	for (i = 0; i < numOfSets; i++) {
		struct Block *ptr = malloc(sizeof(struct Block));
		ptr->tag = -1;
		ptr->validBit = 0;
		ptr->next = NULL;
		for (j = 1; j < associativity; j++) {
			struct Block *temp = malloc(sizeof(struct Block));
			temp->tag = -1;
			temp->validBit = 0;
			temp->next = ptr;
			ptr = temp;
		}
		PreCache[i] = *ptr;
	}
}

void enqueue(size_t tag, int blockIndex) {	// Enqueues in a way such that it stores the index of the block as well in the node
	struct QueueNode* temp = malloc(sizeof(struct QueueNode));
	temp->tag = tag;
	temp->blockIndex = blockIndex;
	temp->next = NULL;
	if (Queue == NULL) {
		 Queue = temp;
		 return;
	}
	struct QueueNode* ptr = Queue;
	while (ptr != NULL) {
		if (ptr->next == NULL) {
			ptr->next = temp;
			return;
		}
		ptr = ptr->next;
	}
}

size_t dequeue(int blockIndex) {	// Smart Dequeueing so that it only dequeues if it matches the blockIndex
	if (Queue == NULL) return -1;
	if (Queue->blockIndex == blockIndex) {
		size_t tag = Queue->tag;
		Queue = Queue->next;
		return tag;
	}
	struct QueueNode* ptr = Queue->next;
	struct QueueNode* prev = Queue;
	while (ptr != NULL && prev != NULL) {
		if (ptr->blockIndex == blockIndex) {
			size_t tag = ptr->tag;
			prev->next = ptr->next;
			return tag;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return -1;
}

void preEnqueue(size_t tag, int blockIndex) {	// Enqueues in a way such that it stores the index of the block as well in the node
	struct QueueNode* temp = malloc(sizeof(struct QueueNode));
	temp->tag = tag;
	temp->blockIndex = blockIndex;
	temp->next = NULL;
	if (PreQueue == NULL) {
		 PreQueue = temp;
		 return;
	}
	struct QueueNode* ptr = PreQueue;
	while (ptr != NULL) {
		if (ptr->next == NULL) {
			ptr->next = temp;
			return;
		}
		ptr = ptr->next;
	}
}

size_t preDequeue(int blockIndex) {	// Smart Dequeueing so that it only dequeues if it matches the blockIndex
	if (PreQueue == NULL) return -1;
	if (PreQueue->blockIndex == blockIndex) {
		size_t tag = PreQueue->tag;
		PreQueue = PreQueue->next;
		return tag;
	}
	struct QueueNode* ptr = PreQueue->next;
	struct QueueNode* prev = PreQueue;
	while (ptr != NULL && prev != NULL) {
		if (ptr->blockIndex == blockIndex) {
			size_t tag = ptr->tag;
			prev->next = ptr->next;
			return tag;
		}
		prev = ptr;
		ptr = ptr->next;
	}
	return -1;
}