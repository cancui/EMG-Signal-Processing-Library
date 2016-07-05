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
	PkPk *to_return;
	to_return = (PkPk *) malloc(sizeof(PkPk));
	if (to_return == NULL) {
		puts("PkPk returned NULL");
		return NULL;
	}

	to_return->min_pk_gap = sample_frequency / max_frequency;
	to_return->max_pk_gap = sample_frequency / min_frequency;
}

void free_pkpk(PkPk *self) {
	puts("freeing PkPk");
	queue_free(self->data);
	free(self);
}

PkPk_data get_pkpk(PkPk *self, int data_entry_) {
	int *data_entry = (int *)mallac(sizeof(int));
	*data_entry = data_entry_;
	self->data;
}

void test_print(PkPk *self) {
	puts("test print");
	printf("%d %d\n", self->min_pk_gap, self->max_pk_gap);
	puts("end test print");
}