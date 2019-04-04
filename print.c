/* Xulin Yang, 904904,
   time: 2018-5-4 12:46:17,
   description: print function used for each task
*/

#include <stdio.h>
#include "print.h"

/* print the results in task1 */
void print_task1(int edit_distance) {
	printf("%d\n", edit_distance);
	return;
}

/* print the results in task2 */
void print_task2(char **arr, unsigned int size) {
    for (unsigned int i=0; i < size; i++) {
        printf("%s\n", arr[i]);
    }
    return;
}

/* print the results in task3 */
void print_task3(char *word, int has) {
    printf("%s", word);

    if (!has) {
        printf("?");
    }
    printf("\n");
    return;
}

/* print the results in task4 */
void print_task4(char *word, int matched) {
    if (matched) {
        printf("%s\n", word);
    } else {
        printf("%s?\n", word);
    }
    return;
}