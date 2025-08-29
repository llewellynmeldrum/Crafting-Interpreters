#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include "scanner.h"
#include "limits.h"

#define THRESHOLD_LOAD_FACTOR 0.75f
#define INITIAL_CAPACITY 100
#define KEY_T char*
#define VAL_T TokenType

#define HMAP_NO_ENTRY (VAL_T)INT_MIN
typedef struct Pair_t {
	KEY_T key;
	VAL_T value;
} Pair_t;

typedef struct HashTable_t {
	Pair_t *pairs;
	size_t capacity;
	size_t size;
} HashTable_t;

int resolved_collision_count;

void HashTable_put(HashTable_t *ht, KEY_T key, VAL_T value);
VAL_T HashTable_get(HashTable_t *ht, KEY_T key);
HashTable_t HashTable_create();

void printPair(Pair_t p);
int getCollisions();
int getUnresolvedCollisions();
void HashTable_print(HashTable_t ht);
#endif //HASH_TABLE_H
