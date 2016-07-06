#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "peak_to_peak.h"

#define PKPK_MAX 1
#define PKPK_MIN 2
#define PKPK_PKPK 3
#define PKPK_NEUTRAL 4

struct PkPk_ {
	Queue *data;
	Queue *max_values;
	Queue *min_values;

	uint16_t cur_length;
	//uint16_t cur_state;

	int cur_max;
	int cur_min;

	uint16_t min_pk_gap;
	uint16_t max_pk_gap;
};

struct PkPk_data_ {
	int max;
	int min;
	int pkpk;
	int neutral;
};

PkPk *new_pkpk(uint16_t sample_frequency, uint16_t min_frequency, uint16_t max_frequency) {
	PkPk *to_return = (PkPk *) malloc(sizeof(PkPk));
	if (to_return == NULL) {
		puts("Not enough memory to allocate for PkPk");
		return NULL;
	}

	to_return->data = queue_new();
	to_return->max_values = queue_new();
	to_return->min_values = queue_new();

	if (!to_return->data || !to_return->max_values || !to_return->min_values) {
		puts("Not enough memory for PkPk's internal queues");
		return NULL;
	}

	to_return->cur_length = 0;
	//to_return->cur_state = 0;

	to_return->cur_max = -100000;
	int *cur_max_ = (int *)malloc(sizeof(int));
	*cur_max_ = to_return->cur_max;
	queue_push_head(to_return->max_values, cur_max_);

	to_return->cur_min = 100000;
	int *cur_min_ = (int *)malloc(sizeof(int));
	*cur_min_ = to_return->cur_min;
	queue_push_head(to_return->min_values, cur_min_);


	to_return->min_pk_gap = sample_frequency / max_frequency;
	to_return->max_pk_gap = sample_frequency / min_frequency;

	if (to_return->max_pk_gap < 1 || to_return->min_pk_gap < 0) {
		puts("Max or min peak gaps are of incorrect size - check constructor parameters");
		return NULL;
	}

	return to_return;
}

void free_pkpk(PkPk *self) {
	puts("freeing PkPk");
	queue_free(self->data);
	queue_free(self->max_values);
	queue_free(self->min_values);
	free(self);
}

/*(PkPk_data *)*/ void get_pkpk(PkPk *self, int data_entry_) {
	
	printf("contents of data at beginning of function call: ");
	queue_print(self->data);
	puts(" ");
	
	if (self->cur_length > (self->max_pk_gap * 2)) {
		int *popped = (int *)queue_pop_tail(self->data);
		puts("popping from data");


		if ((queue_peek_tail(self->max_values)) && *popped == *((int *)queue_peek_tail(self->max_values))) {
			puts("popping from max values");
			int *to_delete = queue_pop_tail(self->max_values);
			free(to_delete);
		}  
		if ((queue_peek_tail(self->min_values)) && *popped == *((int *)queue_peek_tail(self->min_values))) {
			puts("popping from min values");
			int *to_delete = queue_pop_tail(self->min_values);
			free(to_delete);
		} //puts("debug");


		printf("popped: %d\n", *popped);
		free(popped);
		self->cur_length -= 1;

		printf("contents AFTER popping data: ");
		queue_print(self->data);
		puts(" ");

		if (queue_is_empty(self->max_values) || queue_is_empty(self->min_values)) {
			//update min and max, and push to their queues
			puts("min or max queue empty, look for new min max");
		}
	}




	int *data_entry = (int *)malloc(sizeof(int)); 	//puts("allocated memory");
	*data_entry = data_entry_; 					//printf("Set to %d", *data_entry);
	printf("pushed: %d\n", *data_entry);
	queue_push_head(self->data, data_entry); 	//puts("push to head successful");
	self->cur_length += 1;

	int *for_max_entry = (int *)malloc(sizeof(int));
	*for_max_entry = *data_entry;

	int *for_min_entry = (int *)malloc(sizeof(int));
	*for_min_entry = *data_entry;
	
	if (*for_max_entry > self->cur_max) {
		puts("found new unique max, emptying max queue and pushing new value");
		self->cur_max = *for_max_entry;
		while (!queue_is_empty(self->max_values)) {
			int *popped = queue_pop_tail(self->max_values);
			printf("popped %d from max values\n", *popped);
			free(popped);
		}
		queue_push_head(self->max_values, for_max_entry);
	} else if (*for_max_entry == self->cur_max) {
		puts("found new non unique max value");
		queue_push_head(self->max_values, for_max_entry);
	}

	if (*for_min_entry < self->cur_min) {
		puts("found new unique min, emptying min queue and pushing new value");
		self->cur_min = *for_min_entry;
		while (!queue_is_empty(self->min_values)) {
			int *popped = queue_pop_tail(self->min_values);
			printf("popped %d from min values\n", *popped);
			free(popped);
		}
		queue_push_head(self->min_values, for_min_entry);
	} else if (*for_min_entry == self->cur_min) {
		puts("found new non unique min value");
		queue_push_head(self->min_values, for_min_entry);
	}


	


	//RETURN POINTER TO STRUCT IN STACK
	/*
	PkPk_data to_return;
	to_return.max = 0;
	to_return.min = 0;
	to_return.pkpk = 0;
	to_return.neutral = 0;
	puts("done init return struct");
	return to_return;*/


}

void test_print(PkPk *self) {
	puts("test print");
	printf("%d %d\n", self->min_pk_gap, self->max_pk_gap);
	puts("end test print");
}