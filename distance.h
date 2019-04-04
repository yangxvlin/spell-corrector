/* Xulin Yang, 904904,
   time: 2018-5-7 21:03:37,
   description: edit distance = levenshtein distance
                calculate edit dist
				generate possible dist=1 words
				calculate edit dist for range in upper bound
*/

#ifndef DISTANCE_H
#define DISTANCE_H

/* generate posssible words with dist=1 to a given string */
void generate_dist1(char *word, int len, char ***arr, unsigned int *arr_cur);

/* calculate levenshtein distance for givn two words */
int levenshtein(const char *word1, const char *word2, int n, int m);

/* return 257 if not in range or
   the actual levenshtein distance for given two words*/
int levenshtein_in_range(const char *word1, const char *word2, int n, int m,
                         int upper_bound);

#endif