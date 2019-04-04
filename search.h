#ifndef SEARCH_H
#define SEARCH_H

#include "list.h"
#include "hashtbl.h"

/* ******************************************************************** */
/* possible combinairon of words with dist=1 */
unsigned int possible_one_dist(int word_len);

/* ******************************************************************** */
/* linear search dictionary stop when dist=lower found */
int linear_search_dictionary(List *dict, Word_lookup *lookup, int lower);

/* ******************************************************************** */
/* search word in the history search */
void search_history(HashTable *hist, Word_lookup *w);
/* search mathced word with dist=0 */
void search_dist0(HashTable *dict, Word_lookup *w);
/* search mathced word with dist=1 */
void search_dist1(HashTable *dict, Word_lookup *w, char ***dist1,
                  unsigned int *dist1_size, unsigned int *dist1_num);
/* generate dist=1 words from given array of words */
void traverse_string_array(char **original, char ***arr,
                           unsigned int arr_cur, unsigned int *cur);
/* search mathced word with dist>1 */
void search_dist_greater(List *dictionary, HashTable *dict, Word_lookup *w,
                         char ***dist, unsigned int *dist_size,
                         unsigned int *dist_num, char **pre_dist,
                         unsigned int pre_dist_size);

#endif