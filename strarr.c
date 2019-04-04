/* Xulin Yang, 904904,
   time: 2018-5-20 14:12:32, 
   description: 2-D string array
*/

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "strarr.h"

/* ******************************************************************** */
/* create new 2-D string array */
char **new_string_arr(unsigned int size, int word_len) {
	char **new = malloc(sizeof(char*) * size);
	assert(new != NULL);

	for (unsigned int i = 0; i < size; i++) {
		new[i] = malloc(word_len);
		assert(new[i] != NULL);
	}
	return new;
}

/* free 2-D string array */
void free_string_arr(char ***arr, unsigned int size) {
	if (size) {
		for (unsigned int i = 0; i < size; i++) {
			free((*arr)[i]);
			(*arr)[i] = NULL;
		}
		free((*arr));
		(*arr) = NULL;
	}
	return;
}

/* free 2-D string array if not enough and return a new one*/
char **realloc_string_arr(char **arr, unsigned int size, int word_len,
                          unsigned int *arr_size) {
	/* increase array size */
	if ((*arr_size) < size) {
		/* free array */
		free_string_arr(&arr, (*arr_size));
		/* new array */
		(*arr_size) = size;
		return new_string_arr(size, word_len);
	/* array has enough size */
	} else {
		return arr;
	}
}

/* design of algorithm is fun! */
