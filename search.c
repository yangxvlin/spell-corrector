/* Xulin Yang, 904904,
   time: 2018-5-20 10:48:51,
   description: 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "search.h"
#include "distance.h"
#include "strarr.h"

/* ******************************************************************** */
#define MAX_WORD_LEN 257 /* max length of word + null byte*/
#define MAX_DISTANCE 3   /* max edit dist=3 */
#define EMPTY -1
#define TRUE 1
#define FALSE 0

/* ******************************************************************** */
/* possible combinairon of words with dist=1 */
unsigned int possible_one_dist(int word_len) {
	/* word_len +      delete
	   26*word_len +   replace
	   26*(word_len+1) insert*/
	return 53 * word_len + 26;
}

/* ******************************************************************** */
/* linear search dictionary stop when dist=lower found */
int linear_search_dictionary(List *dict, Word_lookup *lookup, int lower) {
	Node *tmp = dict->head;
	char *word = NULL;
	unsigned int i = 0;
    long freq = EMPTY;
	int dist, word_len;

	while (tmp != NULL) {
		word = (char*)(tmp->data);
        /* word length diff > 3 implies dist > 3 */
		if (abs(lookup->word_len - strlen(word)) <= MAX_DISTANCE) {
            word_len = strlen(word);

            /* put longer word first */
            if (word_len > lookup->word_len) {
			    dist = levenshtein_in_range(word, lookup->word, word_len,
                            lookup->word_len, MAX_DISTANCE);
            } else {
                dist = levenshtein_in_range(lookup->word, word,
                            lookup->word_len, word_len, MAX_DISTANCE);
            }

            /* some word found */
			if (dist != MAX_WORD_LEN) {
                /* return if its dist = lower */
				if (dist == lower) {
					lookup->similar = word;
					freq = i;
					return TRUE;
                /* store current found and look for lower dist one */
				} else if ((freq == EMPTY) && ((dist > lower) &&
                                  (dist <= MAX_DISTANCE))) {
					lookup->similar = word;
					freq = i;
				}
			}
		}

		tmp = tmp->next;
		i++;
	}
	return (freq != EMPTY);
}

/* ******************************************************************** */
/* search word in the history search */
void search_history(HashTable *hist, Word_lookup *w) {
    if (hash_table_has(hist, w->word)) {
        Bucket *got = hash_table_get(hist, w->word);
        w->similar = get_bucket_item(got);
        w->searched = get_bucket_int(got);
        w->finished = TRUE;
    }
    return;
}

/* search mathced word with dist=0 */
void search_dist0(HashTable *dict, Word_lookup *w) {
    /* search when dictionary is not empty */
    if (get_hashtable_size(dict)) {
        w->searched = hash_table_has(dict, w->word);
        w->similar = w->word;
        w->finished = w->searched;
    /* if dicitonary is empty, no matching */
    } else {
        w->searched = FALSE;
        w->finished = TRUE;
        w->similar = w->word;
    }

    return;
}

/* search mathced word with dist=1 */
void search_dist1(HashTable *dict, Word_lookup *w, char ***dist1,
                  unsigned int *dist1_size, unsigned int *dist1_num) {
    /* generate possible dist=1 */
    w->possible = possible_one_dist(w->word_len);
    (*dist1) = realloc_string_arr((*dist1), w->possible, MAX_WORD_LEN + w->dist, dist1_size);
    generate_dist1(w->word, w->word_len, dist1, dist1_num);
    w->searched = search_hash_table((*dist1), (*dist1_num), dict, w);

    /* found and finished */
    w->finished = w->searched;
    return;
}

/* generate dist=1 words from given array of words */
void traverse_string_array(char **original, char ***arr,
                           unsigned int arr_cur, unsigned int *cur) {
	assert(original != NULL);
	assert(arr != NULL);

	for (unsigned int i = 0; i < arr_cur; i++) {
		generate_dist1(original[i], strlen(original[i]), arr, cur);
	}

	return;
}

/* search mathced word with dist>1
   dist for current string array dist=x
   pre_dist for previous string array pf generated words with dist=x-1*/
void search_dist_greater(List *dictionary, HashTable *dict, Word_lookup *w,
                         char ***dist, unsigned int *dist_size,
                         unsigned int *dist_num, char **pre_dist,
                         unsigned int pre_dist_size) {
    /* next = cur * max(possible_word->len) */
    w->possible = w->possible * possible_one_dist(w->word_len + w->dist);

    /* not too much words will be generated and lookup them in dict */
    if (w->possible < get_hashtable_size(dict)) {
        /* generate possible words */	
        (*dist) = realloc_string_arr((*dist), w->possible, MAX_WORD_LEN + w->dist, dist_size);
        traverse_string_array(pre_dist, dist, pre_dist_size, dist_num);
        w->searched = search_hash_table((*dist), (*dist_num), dict, w);

         /* found and finished */
        w->finished = w->searched;
    /* too much generated and switch to linear search */
    } else {
        w->searched = linear_search_dictionary(dictionary, w, w->dist);
        w->finished = TRUE;
    }
    return;
}

/* design of algorithm is fun! */