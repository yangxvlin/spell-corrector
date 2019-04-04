/* Xulin Yang, 904904,
   time: 2018-5-20 14:12:32, 
   description: 2-D string array
*/

#ifndef STRARR_H
#define STRARR_H

/* ******************************************************************** */
/* create new 2-D string array */
char **new_string_arr(unsigned int size, int word_len);

/* free 2-D string array */
void free_string_arr(char ***arr, unsigned int size);

/* free 2-D string array if not enough and return a new one*/
char **realloc_string_arr(char **arr, unsigned int size, int word_len,
                          unsigned int *arr_size);

#endif