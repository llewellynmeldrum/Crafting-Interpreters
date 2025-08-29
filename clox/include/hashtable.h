#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "scanner.h"

#define THRESHOLD_LOAD_FACTOR 0.75f
#define INITIAL_CAPACITY 2

#define LARGE_PRIME 7879
#define KEY_T char*
#define VAL_T TokenType

typedef struct Pair_t {
	KEY_T key;
	VAL_T value;
} Pair_t;

typedef struct HashTable_t {
	Pair_t *pairs;
	size_t capacity;
	size_t size;
} HashTable_t;


void HashTable_put(HashTable_t *ht, KEY_T key, VAL_T value);
TokenType HashTable_get(HashTable_t *ht, KEY_T key);
HashTable_t HashTable_create();

int nCollisions();
void HashTable_print(HashTable_t ht);
#endif //HASH_TABLE_H
