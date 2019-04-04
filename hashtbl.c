/* Xulin Yang, 904904,
   time: 2018-5-20 10:48:51,
   description: HashTable Module
                hash method        = xor hash
				collision solution = separating chaing and move to front
				bucket             = {key : [data, item]}
				bucket structure   = link list
	some code are adapted from workshop
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "hashtbl.h"
#include "hash.h"

/* ******************************************************************** */
#define ZERO 0             /* integer zero */
#define EMPTY -1           /* no word found */
#define MAX_BUCKET 6000000 /* max size of buckets in HashTable */

#define TRUE 1
#define FALSE 0

/* return 1:same strings; 0:different */
#define STR_EQUAL(str1, str2) (strcmp(str1, str2)==0)

/* ******************************************************************** */
/* hash table data structure */
struct table {
	unsigned int size; /* number of buckets */
	Bucket **buckets;  /* separate chaining for collision */
};

/* separate chaining bucket data structure with {key : [data, item]}*/
struct bucket {
	char *key;         /* key of object */
	unsigned int data; /* integer item of object */
	char *item;        /* string item of object */
	Bucket *next;      /* next object with same hash value */
};

/* ******************************************************************** */
/* create new bucket and assign new value into bucket */
static Bucket *new_bucket(char *key, unsigned int integer, char *item);
/* destroy the memory malloced for bucket */
static void free_bucket(Bucket *bucket);

/* ******************************************************************** */
/* create new bucket and assign new value into bucket */
static Bucket *new_bucket(char *key, unsigned int integer, char *item) {
	Bucket *bucket = malloc(sizeof(Bucket));
	assert(bucket != NULL);

	bucket->key = key;
	assert(bucket->key != NULL);
	
	bucket->item = item;
	bucket->data = integer;
	bucket->next = NULL;

	return bucket;
}

/* destroy the memory malloced for bucket */
static void free_bucket(Bucket *bucket) {
	assert(bucket != NULL);
	free(bucket);
	bucket = NULL;
	return;
}

/* ******************************************************************** */
/* create new HashTable */
HashTable *new_hash_table(unsigned int size) {
	HashTable* table = (HashTable*)malloc(sizeof(HashTable));
	assert(table!=NULL);

	/* HashTable with max_size = 8,000,000 */
	table->size = ((size < MAX_BUCKET) ? size : MAX_BUCKET);

	/* prevent empty HashTable creation */
	if (size > ZERO) {
		table->buckets = malloc((sizeof(Bucket*) * table->size));
		assert(table->buckets!=NULL);

		for (unsigned int i = 0; i < table->size; i++) {
			table->buckets[i] = NULL;
		}
	}

	return table;
}

/* free memory malloced for HashTable and Buckets */
void free_hash_table(HashTable *table) {
	assert(table != NULL);
	/* free HashTable when it is not empty */
	if (table->size) {
		/* free buckets */
		for (unsigned int i = 0; i < table->size; i++) {
			Bucket *cur, *next;
			cur = table->buckets[i];
			while (cur) {
				next = cur->next;
				free_bucket(cur);
				cur = next;
			}
		}
		free(table->buckets);
		table->buckets = NULL;
	}
	free(table);
	table = NULL;
	return;
}

/* ******************************************************************** */
/* insert data into HashTable */
void hash_table_put(HashTable *t, char *key, unsigned int freq, char *item) {
	assert(t != NULL);
	assert(key != NULL);

	unsigned int hash_value = xor_hash(key, t->size);

	/* look for existing key */
	Bucket *bucket = t->buckets[hash_value];
	while (bucket) {
		if (STR_EQUAL(key, bucket->key)) {
			return;
		}
		bucket = bucket->next;
	}

	/* if key wasn't found, add it at front of list */
	Bucket *new = new_bucket(key, freq, item);
	new->next = t->buckets[hash_value];
	t->buckets[hash_value] = new;
}

/* return searched bucket assume key must in HashTable */
Bucket* hash_table_get(HashTable *table, char *key) {
	assert(table != NULL);
	assert(key != NULL);

	unsigned int hash_value = xor_hash(key, table->size);

	/* look for existing key */
	Bucket *bucket = table->buckets[hash_value];
	while (bucket) {
		if (STR_EQUAL(key, bucket->key)) {
			return bucket;
		}
		bucket = bucket->next;
	}

	/* key doesn't exist! */
	fprintf(stderr, "error: key \"%s\" not found in table\n", key);
	exit(1);
}

/* search key in HasTable and return 1:searched; 0:not searched */
int hash_table_has(HashTable *table, char *key) {
	assert(table != NULL);
	assert(key != NULL);

	unsigned int hash_value = xor_hash(key, table->size);

	/* look for existing key */
	Bucket *bucket = table->buckets[hash_value];
	while (bucket) {
		/* found */
		if (STR_EQUAL(key, bucket->key)) {
			return TRUE;
		}
		bucket = bucket->next;
	}

	/* not found */
	return FALSE;
}

/* ******************************************************************** */
/* return integer stored in bucket */
int get_bucket_int(Bucket *bucket) {
	assert(bucket != NULL);	
	return bucket->data;
}

/* return string stored in bucket */
char *get_bucket_item(Bucket *bucket) {
	assert(bucket != NULL);
	return bucket->item;
}

/* return the size of the HashTable */
unsigned int get_hashtable_size(HashTable *t) {
	assert(t != NULL);
	return t->size;
}

/* ******************************************************************** */
/* search HashTable for every word in string array
   return 1:searched; 0:not searched */
int search_hash_table(char **arr, unsigned int size, HashTable *dict,
                      Word_lookup *w) {
	assert(arr != NULL);
	assert(dict != NULL);
	assert(w != NULL);
	long freq = EMPTY;

	/* traverse string array */
	for (unsigned int i = 0; i < size; i++) {
		/* has this possible word */
		if (hash_table_has(dict, arr[i])) {
			Bucket *found = hash_table_get(dict, arr[i]);

			/* first matched found || higher frequency word in dict found */
			if ((freq == EMPTY) || (found->data < freq)) {
				freq = found->data;
				w->similar = found->key;
			}
		}
	}

	return (freq != EMPTY);
}

/* design of algorithm is fun! */