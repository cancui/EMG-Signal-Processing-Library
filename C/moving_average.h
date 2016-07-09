#ifndef MOVING_AVERAGE_H
#define MOVING_AVERAGE_H

#include <stdint.h>
#include <stdbool.h>

#include "queue.h"

typedef struct Moving_Average_ Moving_Average;

Moving_Average *new_moving_average(uint16_t max_length_);
void 			free_moving_average(Moving_Average *self);
int 			get_moving_average(Moving_Average *self, int data_entry_);

int				newest_entry_moving_average(Moving_Average *self);
int 			oldest_entry_moving_average(Moving_Average *self);
bool			is_empty_moving_average(Moving_Average *self);

void			test_print_MA(Moving_Average *self);

#endif