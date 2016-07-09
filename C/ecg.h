#ifndef ECG_H
#define ECG_H

#include "peak_to_peak.h"
#include "moving_average.h"

typedef struct ECG_ ECG;

typedef enum {
	REFERENCE_AVAILABLE,
	REFERENCE_UNAVAILABLE
} Reference_Availability;

ECG *new_ECG(uint16_t sample_frequency_, Reference_Availability reference_availability_);
void free_ECG(ECG *self);

void initialize_ECG(ECG *self, int data);
int get_BPM(ECG *self, int data);

void initialize_ECG_r(ECG *self, int data, int reference);
int get_BPM_r(ECG *self, int data, int reference);

#endif