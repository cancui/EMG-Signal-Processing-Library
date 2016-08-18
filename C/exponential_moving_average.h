#ifndef EXPONENTIAL_MOVING_AVERAGE_H
#define EXPONENTIAL_MOVING_AVERAGE_H

#include <stdint.h>
#include <stdbool.h>

#include "queue.h"

typedef struct Exponential_Moving_Average_ Exponential_Moving_Average;

Exponential_Moving_Average *new_exponential_moving_average(uint16_t max_length_);
void 			free_moving_average(Exponential_Moving_Average *self);
int 			get_moving_average(Exponential_Moving_Average *self, int data_entry_);

int				newest_entry_moving_average(Exponential_Moving_Average *self);
int 			oldest_entry_moving_average(Exponential_Moving_Average *self);
bool			is_empty_moving_average(Exponential_Moving_Average *self);

void			test_print_MA(Exponential_Moving_Average *self);

#endif
