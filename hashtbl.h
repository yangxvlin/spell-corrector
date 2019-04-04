/* Xulin Yang, 904904,
   time: 2018-5-20 10:48:51,
   description: HashTable Module
                hash method        = xor hash
				collision solution = separating chaing and move to front
				bucket             = {key : [data, item]}
				bucket structure   = link list
	some code are adapted from workshop
*/

#ifndef HASHTBL_H
#define HASHTBL_H

typedef struct table HashTable;     /* hash table data structure */
typedef struct bucket Bucket;       /* bucket with {key : [data, item]}*/
typedef struct look_up Word_lookup; /* searching word census */

struct look_up {
	char *word;            /* search word */
	int word_len;          /* length of search word */
	long dist;             /* current edit distance */
    unsigned int possible; /* possible number of dist = x */
	char *similar;         /* similar match */
	int searched;          /* found natched word */
	int finished;          /* searching terminated */
};

/* ******************************************************************** */
/* create new HashTable */
HashTable *new_hash_table(unsigned int size);
/* free memory malloced for HashTable and Buckets */
void free_hash_table(HashTable *table);

/* ******************************************************************** */
/* insert data into HashTable */
void hash_table_put(HashTable *t, char *key, unsigned int freq, char *item);
/* return searched bucket assume key must in HashTable */
Bucket* hash_table_get(HashTable *table, char *key);
/* search key in HasTable and return 1:searched; 0:not searched */
int hash_table_has(HashTable *table, char *key);

/* ******************************************************************** */
/* return integer stored in bucket */
int get_bucket_int(Bucket *bucket);
/* return string stored in bucket */
char *get_bucket_item(Bucket *bucket);
/* return the size of the HashTable */
unsigned int get_hashtable_size(HashTable *t);

/* ******************************************************************** */
/* search HashTable for every word in string array
   return 1:searched; 0:not searched */
int search_hash_table(char **arr, unsigned int size, HashTable *dict,
                      Word_lookup *w);

#endif