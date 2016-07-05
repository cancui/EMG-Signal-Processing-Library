#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "moving_average.h"
/*
struct Books {
   char  title[50];
   char  author[50];
   char  subject[100];
   int   book_id;
   	struct Books2{
   		char  title2[50];
   		char  author2[50];
   		char  subject2[100];
   		int   book_id2;
	} book2;
};
*/
int main() {
	puts("starting");

	Moving_Average *m = new_moving_average(4);

	if (m != NULL) {
		/*
		m->data_sum = 5;
		m->data_avg = 6;
		m->cur_length = 7;
		m->max_length = 8; */
		puts("not null");
	}
	//test_print(m);

	for (int i = 1; i < 21; i++) {
		printf("%d\n", get_moving_average(m, i));
	}

	free_moving_average(m);
	puts("ending");
	return 0;
}