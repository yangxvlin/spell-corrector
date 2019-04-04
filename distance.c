/* Xulin Yang, 904904,
   time: 2018-5-7 21:03:37,
   description: edit distance = levenshtein distance
                calculate edit dist
				generate possible dist=1 words
				calculate edit dist for range in upper bound
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "distance.h"

/* ******************************************************************** */
#define ALPHABET 26  /* number of alphabet */
#define MAX_DIST 257 /* as max len(word) = 256, max edit distance = len()+1*/

/* return min of two values */
#define MIN(x, y) ((x<y) ? x : y)
/* return min of three values */
#define TERNARY_MIN(a, b, c) ((a<b) ? MIN(a, c) : MIN(b, c))
/* convert digit to character */
#define DIGIT_TO_CHR(x) (x + 'a')
/* check two character is the same return 1:same; 0:different */
#define EQUAL(a, b) ((a==b) ? 1 : 0)
/* calculate edit distance for x=i, j=y */
#define CAL(word1, word2, x, y, dist) {\
	dist[x][y] = TERNARY_MIN((!EQUAL(word1[x-1], word2[y-1])) +\
								dist[x-1][y-1], \
							 dist[x-1][y] + 1,\
							 dist[x][y-1] + 1);\
}

/* ******************************************************************** */
/* generate words with 1 element deleted from given word */
static void delete(char *word, int len, char ***arr, unsigned int *arr_cur);
/* generate words with 1 element replaced by a-z from given word */
static void replace(char *word, int len, char ***arr, unsigned int *arr_cur);
/* generate words with 1 element inserted by a-z from given word */
static void insert(char *word, int len, char ***arr, unsigned int *arr_cur);

/* ******************************************************************** */
/* generate posssible words with dist=1 to a given string */
void generate_dist1(char *word, int len, char ***arr, unsigned int *arr_cur) {
	delete(word, len, arr, arr_cur);
	replace(word, len, arr, arr_cur);
	insert(word, len, arr, arr_cur);
	return;
}

/* ******************************************************************** */
/* generate words with 1 element deleted from given word */
static void delete(char *word, int len, char ***arr, unsigned int *arr_cur) {
	/* delete element */
	for (int del = 0; del < len; del++) {
		strncpy((*arr)[*arr_cur], word, del);
		strcpy((*arr)[*arr_cur] + del, word+ del+1);
		(*arr_cur) ++;
	}
	return;
}

/* generate words with 1 element replaced by a-z from given word */
static void replace(char *word, int len, char ***arr, unsigned int *arr_cur) {
	/* decide which index to be replaced */
	for (int substitute = 0; substitute < len; substitute++) {
		/* replace element */
		for (int chr = 0; chr < ALPHABET; chr++) {
			strncpy((*arr)[*arr_cur], word, substitute);
			strcpy((*arr)[*arr_cur] + substitute+1, word+ substitute+1);
			(*arr)[*arr_cur][substitute] = DIGIT_TO_CHR(chr);
			(*arr_cur) ++;
		}
	}

	return;
}

/* generate words with 1 element inserted by a-z from given word */
static void insert(char *word, int len, char ***arr, unsigned int *arr_cur) {
	/* decide which index to be inserted
	   from index:0 before to index:-1 after*/
	for (int insert = 0; insert <= len; insert++) {
		/* insert element */
		for (int chr = 0; chr < ALPHABET; chr++) {
			strncpy((*arr)[*arr_cur], word, insert);
			strcpy((*arr)[*arr_cur] + insert+1, word+ insert);
			(*arr)[*arr_cur][insert] = DIGIT_TO_CHR(chr);
			(*arr_cur) ++;
		}
	}
	return;
}

/* ******************************************************************** */
/* calculate levenshtein distance for given two words */
int levenshtein(const char *word1, const char *word2, int n, int m) {
	int arr[n+1][m+1], /* arr[i, j] is the edit distance between */
	    i,             /* the first i characters of word_1 and */
		j;             /* the first j characters of word_2 */

	/* cal for i from 0 to n: arr[i, 0] = i */
	for (i = 0; i < n+1; i++) {
		arr[i][0] = i;
	}
	/* cal for j from 1 to m: arr[0, j] = j */
	for (j = 1; j < m+1; j++) {
		arr[0][j] = j;
	}

	/* cal rest of table */
	for (i = 1; i <= n; i++) {
		for (j = 1; j <= m; j++) {
			/* substitute_cost = 0 if word_1[i] == word_2[j] else 1
			   arr[i, j] = min(
			   arr[i-1, j-1] + substitute_cost, // substitution
			   arr[i-1, j] + 1,                 // deletion
			   arr[i, j-1] + 1)                 // insertion */
			CAL(word1, word2, i, j, arr);
		}
	}

	return arr[n][m];
}

/* return 257 if not in range or
   the actual levenshtein distance for given two words*/
int levenshtein_in_range(const char *word1, const char *word2, int n, int m,
                         int upper_bound) {
	int arr[n+1][m+1], /* arr[i, j] is the edit distance between */
	    i,             /* the first i characters of word_1 and */
		j,             /* the first j characters of word_2 */
	    x,             /* current column to calculate */
		y,             /* current row to calculate */
		cur_dist;      /* current (x, y) calculated */

	/* cal for i from 0 to n: arr[i, 0] = i */
	for (i = 0; i < n+1; i++) {
		arr[i][0] = i;
	}
	/* cal for j from 1 to m: arr[0, j] = j */
	for (j = 1; j < m+1; j++) {
		arr[0][j] = j;
	}

	/* calculate layer by layer from left-top to right bottom */
	for (i=1; i<=n; i++) {
		j = MIN(m, i); /* not get out of range of word2 */
		/* check for rest of table */
		if (i > upper_bound) {
			cur_dist = MAX_DIST;
			/* calculate row below */
			for (y=((i > j) ? i : 1), x=j; y<i; y++) {
				CAL(word1, word2, y, x, arr);
				cur_dist = MIN(arr[y][x], cur_dist);
			}
			/* calculate column at right */
			for (x=1, y=i; x<=j; x++) {
				CAL(word1, word2, y, x, arr);
				cur_dist = MIN(arr[y][x], cur_dist);				
			}

			/* terminate when no possible to get dist <= upper bound */
			if (cur_dist > upper_bound) {
				return MAX_DIST;
			}
		/* no need to check left top 4*4 as its max definitly <= 3*/
		} else {
			/* calculate row below */
			for (y=((i > j) ? i : 1), x=j; y<i; y++) {
				CAL(word1, word2, y, x, arr);
			}
			/* calculate column at right */
			for (x=1, y=i; x<=j; x++) {
				CAL(word1, word2, y, x, arr);
			}
		}
	}
	return arr[n][m];
}

/* design of algorithm is fun! */