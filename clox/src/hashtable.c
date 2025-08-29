// for header

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hashtable.h"

int collision_count;
// for c file
int keyToInt(KEY_T key);
int hash(size_t capacity, KEY_T key);
void HashTable_resize(HashTable_t* ht);
bool isOccupied(HashTable_t ht, int index);

int hash(size_t capacity,  KEY_T key) {
	// should be perfect hash IFF no two tokens share the same first two and last two chars.
	size_t len = strlen(key);
	char index[4];
	index[0] = key[0];
	index[1] = key[1];
	index[2] = key[len - 1];
	index[3] = key[len - 2];
	return *((int*)index)  % capacity;
}

HashTable_t HashTable_create() {
	resolved_collision_count = 0;
	collision_count = 0;
	HashTable_t ht = (HashTable_t) {
		.size = 0,
		.capacity = INITIAL_CAPACITY,
		.pairs = calloc(INITIAL_CAPACITY, sizeof(Pair_t)),
	};
	for (size_t i = 0; i < ht.capacity; i++) {
		ht.pairs[i] = (Pair_t) {
			.value = INT_MIN,
		};
	}
	return ht;
}
int HashTable_linearProbe(HashTable_t ht, int index) {
	// finds the next one through probing
	size_t probeCount = 0;
	while (isOccupied(ht, index)) {
		if (probeCount > ht.capacity) {
			fprintf(stderr, "HASHMAP ERROR: probed %zu times! (cap=%zu)\n", probeCount, ht.capacity);
			return -1;
			break;
		}
		index = (index + 1) % ht.capacity;
		probeCount++;
	}
	return index;

}
void HashTable_put(HashTable_t *ht, KEY_T key, VAL_T value) {
	KEY_T keycopy = malloc(strlen(key) + 1);
	strcpy(keycopy, key);
	Pair_t pair = (Pair_t) {
		keycopy, value,
	};
	float loadFactor = (float)ht->size / (float)ht->capacity ;
//	printf("loadfactor: %f\n", loadFactor);
	if (loadFactor >= THRESHOLD_LOAD_FACTOR) {
		HashTable_resize(ht);
	}

	int index = hash(ht->capacity, key);
//	printf("hash('%s')\t=%x %% %zu --> ", key, index, ht->capacity);
	index = index % ht->capacity;
//	printf("%d\n", index);

	if (isOccupied(*ht, index)) {
		printPair(ht->pairs[index]);
		fprintf(stderr, "will be replaced by Pair:\n");
		printPair(pair);
		collision_count++;
		index = HashTable_linearProbe(*ht, index);
		fprintf(stderr, "HTABLE ERROR: COLLISION DETECTED\n");
		if (index < 0) {
			fprintf(stderr, "HTABLE ERROR: COLLISION DETECTED, PROBE FAILED (%d)!\n ", index);
		} else {
			resolved_collision_count++;
		}
	}
	ht->pairs[index] = pair;
	ht->size++;
}

VAL_T HashTable_get(HashTable_t *ht, KEY_T key) {
	int index = hash(ht->capacity, key);
	if ((int)ht->pairs[index].value == INT_MIN) return HMAP_NO_ENTRY;
	return ht->pairs[index].value;
}

int keyToInt(KEY_T key) {
	// KEY_T SPECIFIC
	int sum = 0;
	int len = strlen(key);
	for (int i = 0; i < len; i++) {
		sum += key[i];
	}
	return sum;
}

void HashTable_resize(HashTable_t* ht) {
	//printf("BEFORE RESIZE: \n");
	//HashTable_print(*ht);
	size_t newCapacity = ht->capacity * 2;
	Pair_t *newPairs = calloc(newCapacity, sizeof(Pair_t));
	for (size_t i = 0; i < newCapacity; i++) {
		newPairs[i] = (Pair_t) {
			.value = INT_MIN,
		};
	}
	// rehash everything
	for (size_t i = 0; i < ht->capacity; i++) {
		Pair_t p = ht->pairs[i];
		if (p.key != NULL && (int)p.value != INT_MIN) {
			newPairs[hash(newCapacity, p.key)] = (Pair_t) {
				p.key, p.value
			};
		}
	}
	free(ht->pairs);
	ht->pairs = newPairs;
	ht->capacity = newCapacity;
	//printf("AFTER RESIZE: \n");
	//HashTable_print(*ht);
}

bool isOccupied(HashTable_t ht, int index) {
	return (int)ht.pairs[index].value != INT_MIN;
}

int getCollisions() {
	return collision_count;
}

void printPair(Pair_t p) {
	printf("'%s'-->%d\n", p.key, p.value);
}

void HashTable_print(HashTable_t ht) {
	printf("PRINTING HASHTABLE: cap=%zu, size=%zu\n", ht.capacity, ht.size);
	for (size_t i = 0; i < ht.capacity; i++) {
		printf("\t[%02zu]:", i);
		printPair(ht.pairs[i]);
	}
}
