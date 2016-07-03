#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <stdint.h>

#include "queue.h"

typedef struct Moving_Average_ Moving_Average;

Moving_Average *new_moving_average(uint16_t max_length_);
void 			free_moving_average(Moving_Average *self);
int 			get_moving_average(Moving_Average *self, int data_entry_);

void			test_print(Moving_Average *self);

#endif