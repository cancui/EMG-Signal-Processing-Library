#ifndef ECG_H
#define ECG_H

#include "peak_to_peak.h"
#include "moving_average.h"

typedef struct ECG_ ECG;
/*
typedef enum {
	REFERENCE_AVAILABLE,
	REFERENCE_UNAVAILABLE
} ECG_Reference_Availability;

typedef enum {
	AUTODETECT_THRESHOLD_ON,
	AUTODETECT_THRESHOLD_OFF
} Autodetect_Threshold;
*/
typedef enum {
	REFERENCE_AVAILABLE,
	REFERENCE_UNAVAILABLE,
	AUTODETECT_THRESHOLD_ON,
	AUTODETECT_THRESHOLD_OFF
} ECG_OPTIONS;

ECG *new_ECG(uint16_t sample_frequency_, float pkpk_threshold_ratio_, ECG_OPTIONS reference_availability_, ECG_OPTIONS autodetect_);
void free_ECG(ECG *self);

void initialize_ECG(ECG *self, int data);
int get_BPM(ECG *self, int data);

//void initialize_ECG_r(ECG *self, int data, int reference);
int get_BPM_r(ECG *self, int data, int reference);

#endif