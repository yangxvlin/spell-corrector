/* Xulin Yang, 904904,
   time: 2018-5-20 11:20:51, 
   description: hash function = xor hash
                adapted from lecture
*/
#include <stdio.h>
#include <assert.h>

#include "hash.h"

/* seed used for hash value */
#define SEED 73802

/* xor hash from lectures, with seed 73802 */
unsigned int xor_hash(const char *key, unsigned int size) {
	unsigned int h = SEED;

	int i;
	for (i = 0; key[i] != '\0'; i++) {
		h = h ^ ((h << 5) + key[i] + (h >> 2));
	}

	return h % size;
}

/* design of algorithm is fun! */