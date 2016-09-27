#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "moving_average.h"

struct Moving_Average_ {
	
	Queue *data;
	int data_sum;
	int data_avg;
	uint16_t cur_length;
	uint16_t max_length;

};

Moving_Average *new_moving_average(uint16_t max_length_) {
	Moving_Average *to_return;
	to_return = (Moving_Average *) malloc(sizeof(Moving_Average));
	if (to_return == NULL) {
		puts("Not enough memory to allocate for MA");
		return NULL;
	}
	to_return->data = queue_new(); //puts("Creating queue");
	if (to_return->data == NULL) {
		puts("Not enough memory for MA internal data structures");
		return NULL;
	}
	to_return->data_sum = 0;       //puts("Initializing members");
	to_return->data_avg = 0;
	to_return->cur_length = 0;
	to_return->max_length = max_length_;
	//printf("%d %d %d %d \n", to_return->data_sum, to_return->data_avg, to_return->cur_length, to_return->max_length);
	//puts("returning constructed MA");
	return to_return;
}

void free_moving_average(Moving_Average *self) {
	puts("freeing MA");
	queue_free(self->data);
	free(self);
	self = NULL;
}

int get_moving_average(Moving_Average *self, int data_entry_) {

	int *data_entry; 							//puts("created variable");
	data_entry = (int *) malloc(sizeof(int)); 	//puts("allocated memory");
	*data_entry = data_entry_; 					//printf("Set to %d", *data_entry);
	queue_push_head(self->data, data_entry); 	//puts("push to head successful");
	self->data_sum += data_entry_;
	self->cur_length += 1;

	if (self->cur_length > self->max_length) {
		int *popped = (int *)queue_pop_tail(self->data);
		self->data_sum -= *popped;
		free(popped);
		self->cur_length -= 1;
	}

	self->data_avg = self->data_sum / self->cur_length;

	if (self->cur_length < (self->max_length / 8)) {
		//printf("%d\n", data_entry_);
		return data_entry_;
	} else {
		//printf("%d\n", self->data_avg);
		return self->data_avg;
	}
}

int newest_entry_moving_average(Moving_Average *self) {
	return *((int *)queue_peek_head(self->data));
}

int oldest_entry_moving_average(Moving_Average *self){
	return *((int *)queue_peek_tail(self->data));
}

bool is_empty_moving_average(Moving_Average *self) {
	return queue_is_empty(self->data);
}

void test_print_MA(Moving_Average *self) {
	puts("test print");
	printf("%d %d %d %d\n", self->data_sum, self->data_avg, self->cur_length, self->max_length);
	puts("end test print");
} 



