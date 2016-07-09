#ifndef EMG_H
#define EMG_H

#include "peak_to_peak.h"
#include "moving_average.h"

typedef struct EMG_ EMG;

typedef enum {
	REFERENCE_AVAILABLE,
	REFERENCE_UNAVAILABLE,
	HIGH_PASS_FILTER_ON,
	HIGH_PASS_FILTER_OFF
} EMG_OPTIONS;

EMG *new_EMG(uint16_t sample_frequency_, float range_, uint16_t min_EMG_frequency_, uint16_t max_EMG_frequency_, EMG_OPTIONS remove_low_frequency_, EMG_OPTIONS reference_available_);
void free_EMG(EMG *self);

int filter_EMG(EMG *self, int data);
int filter_EMG_r(EMG *self, int data, int reference_data);


#endif