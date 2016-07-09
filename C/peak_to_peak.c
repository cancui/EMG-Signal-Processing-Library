#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "peak_to_peak.h"
//#include "queue.h"

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
		puts("Not enough memory for PkPk internal data structures");
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
	self = NULL;
}

PkPk_data *get_pkpk(PkPk *self, int data_entry_ /*, PkPk_data *output*/) {
	
	//printf("contents of data at beginning of function call: ");
	//queue_print(self->data);
	//puts(" ");
	
	if (self->cur_length > (self->max_pk_gap * 2)) {
		int *popped = (int *)queue_pop_tail(self->data);
		//puts("popping from data");


		if ((queue_peek_tail(self->max_values)) && *popped == *((int *)queue_peek_tail(self->max_values))) {
			//puts("popping from max values");
			int *to_delete = queue_pop_tail(self->max_values);
			free(to_delete);
		}  
		if ((queue_peek_tail(self->min_values)) && *popped == *((int *)queue_peek_tail(self->min_values))) {
			//puts("popping from min values");
			int *to_delete = queue_pop_tail(self->min_values);
			free(to_delete);
		} //puts("debug");


		//printf("popped: %d\n", *popped);
		free(popped);
		self->cur_length -= 1;

		//printf("contents AFTER popping data: ");
		//queue_print(self->data);
		//puts(" ");

		if (queue_is_empty(self->max_values) || queue_is_empty(self->min_values)) {
			//scan for new min and max, and push to their queues
			//puts("min or max queue empty, look for new min max");
			empty_queue(self->max_values);
			empty_queue(self->min_values);

			get_queue_max_min(self->data, &(self->cur_max), &(self->cur_min));

			int *cur_max_ = (int *)malloc(sizeof(int));
			*cur_max_ = self->cur_max;
			queue_push_head(self->max_values, cur_max_);

			int *cur_min_ = (int *)malloc(sizeof(int));
			*cur_min_ = self->cur_min;
			queue_push_head(self->min_values, cur_min_);
		}
	}


	int *data_entry = (int *)malloc(sizeof(int)); 	//puts("allocated memory");
	*data_entry = data_entry_; 					//printf("Set to %d", *data_entry);
	//printf("pushed: %d\n", *data_entry);
	queue_push_head(self->data, data_entry); 	//puts("push to head successful");
	self->cur_length += 1;

	if (*data_entry > self->cur_max) {
		//puts("found new unique max, emptying max queue and pushing new value");
		int *for_max_entry = (int *)malloc(sizeof(int));
		*for_max_entry = *data_entry;

		self->cur_max = *for_max_entry;
		empty_queue(self->max_values);
		queue_push_head(self->max_values, for_max_entry);
	} else if (*data_entry == self->cur_max) {
		//puts("found new non unique max value");
		int *for_max_entry = (int *)malloc(sizeof(int));
		*for_max_entry = *data_entry;
		queue_push_head(self->max_values, for_max_entry);
	}

	if (*data_entry < self->cur_min) {
		//puts("found new unique min, emptying min queue and pushing new value");
		int *for_min_entry = (int *)malloc(sizeof(int));
		*for_min_entry = *data_entry;

		self->cur_min = *for_min_entry;
		empty_queue(self->min_values);
		queue_push_head(self->min_values, for_min_entry);
	} else if (*data_entry == self->cur_min) {
		//puts("found new non unique min value");
		int *for_min_entry = (int *)malloc(sizeof(int));
		*for_min_entry = *data_entry;
		queue_push_head(self->min_values, for_min_entry);
	}


	


	//RETURN POINTER TO STRUCT IN STACK
	
	PkPk_data *to_return = (PkPk_data *)malloc(sizeof(PkPk_data));
	to_return->max = self->cur_max;
	to_return->min = self->cur_min;
	to_return->pkpk = to_return->max - to_return->min;
	to_return->neutral = to_return->pkpk/2 + to_return->min;
	//puts("done init return struct \n");
	return to_return;


}

int	unpack_data(PkPk_data *package, PKPK_Stat attribute/*uint8_t attribute*/) {
	switch(attribute) {
		case PKPK_MAX:
			return package->max;

		case PKPK_MIN:
			return package->min;

		case PKPK_PKPK:
			return package->pkpk;

		case PKPK_NEUTRAL:
			return package->neutral;

		default:
			puts("default mode triggered, error in switch");
			return -1;
	}
}

/*
bool empty_queue(Queue *self) {
	if (!self) {
		return false;
	}
	while (!queue_is_empty(self)) {
		int *popped = queue_pop_tail(self);
		printf("popped %d from max values\n", *popped);
		free(popped);
	}
	return true;
} */
/*
bool get_queue_max_min(Queue *self, int *max, int *min) {
	QueueEntry *iterator = self->data.head;
			self->cur_max = -10000;
			self->cur_min = 10000;
			while(iterator) {
				if (iterator->data > self->cur_max) {
					self->cur_max = iterator->data;
				}
				if (iterator->data < self->cur_min) {
					self->cur_min = iterator->data;
				}
				iterator = iterator->next;
			}

			int *cur_max_ = (int *)malloc(sizeof(int));
			*cur_max_ = self->cur_max;
			queue_push_head(self->max_values, cur_max_);

			int *cur_min_ = (int *)malloc(sizeof(int));
			*cur_min_ = self->cur_min;
			queue_push_head(self->min_values, cur_min_);
}*/

void test_print_pkpk(PkPk *self) {
	puts("test print");
	printf("%d %d\n", self->min_pk_gap, self->max_pk_gap);
	puts("end test print");
}

