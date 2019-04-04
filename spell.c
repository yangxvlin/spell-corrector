/* Xulin Yang, 904904,
   time: 2018-5-2 21:26:15,
   description: COMP20007 Assignment 2
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "spell.h"
#include "print.h"
#include "hashtbl.h"
#include "distance.h"
#include "search.h"
#include "strarr.h"

#define TRUE 1
#define FALSE 0
#define EMPTY -1
#define MAX_WORD_LEN 256 /* max length of word */
#define MAX_DISTANCE 3   /* max edit dist=3 */

#define STAGE0 0 /* dist=0 */
#define STAGE1 1 /* dist=1 */
#define STAGE2 2 /* dist=2 */
#define STAGE3 3 /* dist=3 */


/* ******************************************************************** */
/* make dicitonary for lists */
static HashTable* make_dictionary(List *dictionary, char *empty);

/* ******************************************************************** */
/* make dicitonary for lists */
static HashTable* make_dictionary(List *dictionary, char *empty) {
	HashTable *dict = new_hash_table(dictionary->size);
	Node* tmp = dictionary->head;

	/* insert dictionary words into HashTable */
	for (unsigned int i = 0; tmp!=NULL; tmp=tmp->next, i++) {
		hash_table_put(dict, (char*)(tmp->data), i, empty);
	}

	return dict;
}

/* ******************************************************************** */
// see Assignment Task 1: Computing edit distance
void print_edit_distance(char *word1, char *word2) {
	assert(word1 != NULL);
	assert(word2 != NULL);
	
	/* task1 output */
	print_task1(levenshtein(word1, word2, strlen(word1), strlen(word2)));
	return;
}

/* ******************************************************************** */
// see Assignment Task 2: Enumerating all possible edits
void print_all_edits(char *word) {
	assert(word != NULL);
	int n = strlen(word);

	/* estimate number of possible dist=1 words */
	unsigned int dist1_num = possible_one_dist(n);
	
	/* malloc memory to store dist=1 words 
	                                    len + null byte + dist = 1*/ 
	char **dist1 = new_string_arr(dist1_num, MAX_WORD_LEN+1+1);
	
	/* generate words with dist=1 by given string */
	unsigned int arr_cur = 0;
	generate_dist1(word, n, &dist1, &arr_cur);
	
	/* task2 output */
	print_task2(dist1, arr_cur);
	
	/* destroy memory */
	free_string_arr(&dist1, dist1_num);
	return;
}

/* ******************************************************************** */
// see Assignment Task 3: Spell checking
void print_checked(List *dictionary, List *document) {
	char *empty = '\0'; /* empty string for item */

	/* make HashTable based dictionary */
	HashTable *dict = make_dictionary(dictionary, empty); /* dictionary */

	Word_lookup w; /* search result census */

	/* search every word */
	for (Node* tmp = document->head; tmp != NULL; tmp = tmp->next) {
		w.word = (char*)(tmp->data);
		w.searched = FALSE;
		w.finished = FALSE;

		/* no need to store it into search history as HashTable look up in 
		   hist and dist has the same run time */
		search_dist0(dict, &w);

		/* task3 output */
		print_task3(w.word, w.searched);

	}

	free_hash_table(dict);
	free(empty);
	empty = NULL;
	return;
}

/* ******************************************************************** */
// see Assignment Task 4: Spelling correction
void print_corrected(List *dictionary, List *document) {
	char *empty = '\0'; /* empty string for item */
	Word_lookup w;      /* word lookup census */
	
	/* make HashTable based dictionary */
	HashTable *dict = make_dictionary(dictionary, empty), /* dictionary */
			  *hist = new_hash_table(document->size);     /* search history */
	
	/* string array used to store generated word for dist=x */
	unsigned int dist1_num,      dist2_num,      dist3_num,      /* cur */
				 dist1_size = 0, dist2_size = 0, dist3_size = 0; /* max */
	char **dist1 = NULL, **dist2 = NULL, **dist3 = NULL;

	/* loop documents */
	Node* to_look;
	for (to_look = document->head; to_look != NULL; to_look = to_look->next) {
		dist1_num = 0;
		dist2_num = 0;
		dist3_num = 0;
		w.word  = (char*)(to_look->data); /* word to look up */
		w.similar = w.word;               /* matched */
		w.word_len = strlen(w.word);      /* length of word */
		w.dist = EMPTY;                   /* edit distance */
		w.finished = FALSE;               /* nor more search */
		w.searched = FALSE;               /* found matched */

		/* check processed before */
		search_history(hist, &w);
		if (!w.finished) {
			/* calculate for dist from [0, 3] */
			for (w.dist=0;((w.dist<=MAX_DISTANCE)&&(!w.finished));w.dist ++) {
				if (w.dist == STAGE0) {
					search_dist0(dict, &w);
				} else if (w.dist == STAGE1) {
					search_dist1(dict, &w, &dist1, &dist1_size, &dist1_num);
				} else if (w.dist == STAGE2) {
					search_dist_greater(dictionary, dict, &w, &dist2,
					               &dist2_size, &dist2_num, dist1, dist1_num);
				} else if (w.dist == STAGE3) {
					search_dist_greater(dictionary, dict, &w, &dist3, 
					               &dist3_size, &dist3_num, dist2, dist2_num);
				}
			}
			/* store search result */
			hash_table_put(hist, w.word, w.searched, w.similar);
		}
		/* task4 output */
		print_task4(w.similar, w.searched);
	}

	free_string_arr(&dist1, dist1_size);
	free_string_arr(&dist2, dist2_size);
	free_string_arr(&dist3, dist3_size);
	free_hash_table(dict);
	free_hash_table(hist);
	free(empty);
	empty = NULL;
	return;
}

/* design of algorithm is fun! */
