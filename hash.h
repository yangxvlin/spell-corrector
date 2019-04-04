/* Xulin Yang, 904904,
   time: 2018-5-20 11:20:51, 
   description: hash function = xor hash
                adapted from lecture
*/

#ifndef HASH_H
#define HASH_H

/* xor hash from lectures, with seed 73802 */
unsigned int xor_hash(const char *key, unsigned int size);

#endif