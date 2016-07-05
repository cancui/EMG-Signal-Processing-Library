#ifndef PEAK_TO_PEAK_H
#define PEAK_TO_PEAK_H

#include <stdint.h>

#include "queue.h"

typedef struct PkPk_ PkPk;
typedef struct PkPk_data_ PkPk_data;

PkPk  	   *new_pkpk(uint16_t sample_frequency, uint16_t min_frequency, uint16_t max_frequency);
void 		free_pkpk(PkPk *self);
PkPk_data	get_pkpk(PkPk *self, int data_entry_);

void		test_print(PkPk *self);

int			get_data(PkPk_data return_struct, uint8_t attribute);

#endif