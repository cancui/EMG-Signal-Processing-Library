#include "ecg.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct ECG_ {
	PkPk *signal_tracker;
	Moving_Average *samples_between_beats;

	uint16_t sample_frequency;
	Reference_Availability reference_availability;

	uint16_t initialization_period;
	uint16_t init_counter;
	int init_sum;
	int average_pkpk;
	int BPM;

	float pkpk_threshold_ratio;
	uint16_t data_samples_since_beat;
	bool first_beat;
};

ECG *new_ECG(uint16_t sample_frequency_, Reference_Availability reference_availability_) {
	ECG *to_return = (ECG *)malloc(sizeof(ECG));
	
	if (to_return == NULL) {
		puts("Not enough memory to allocate for ECG");
		return NULL;
	}

	to_return->signal_tracker = new_pkpk(sample_frequency_, sample_frequency_ / 20, sample_frequency_ / 10);
	to_return->samples_between_beats = new_moving_average(3);

	if (to_return->signal_tracker == NULL || to_return->samples_between_beats == NULL) {
		puts("Not enough memory for ECG internal data structures");
		return NULL;
	}

	to_return->sample_frequency = sample_frequency_;
	to_return->reference_availability = reference_availability_;

	to_return->initialization_period = sample_frequency_ * 3;
	to_return->init_counter = 0;
	to_return->init_sum = 0;
	to_return->average_pkpk = -1;
	to_return->BPM = -1;

	to_return->pkpk_threshold_ratio = 2.0;
	to_return->data_samples_since_beat = 0;
	to_return->first_beat = true;

	return to_return;
}

void free_ECG(ECG *self) {
	puts("Freeing ECG");
	free_pkpk(self->signal_tracker);
	free_moving_average(self->samples_between_beats);
	free(self);
}

void initialize_ECG(ECG *self, int data) {
	if (self->init_counter >= self->initialization_period) {
		return;
	} 
	
	int current_pkpk = unpack_data(get_pkpk(self->signal_tracker, data), PKPK_PKPK);
	self->init_sum += current_pkpk;

	self->init_counter++;
	self->average_pkpk = self->init_sum / self->init_counter;
}

int get_BPM(ECG *self, int data) {
	
	int average_delay = 0; //this variable will eventually hold the BPM value and be returned

	if (self->init_counter < self->initialization_period) {
		initialize_ECG(self, data);
		return 0;
	}
	//return self->average_pkpk;
	int current_pkpk = unpack_data(get_pkpk(self->signal_tracker, data), PKPK_PKPK);
	self->data_samples_since_beat++;

	if ((current_pkpk > self->average_pkpk * self->pkpk_threshold_ratio) && 
		(self->first_beat == true || self->data_samples_since_beat > 0.75 * latest_entry_moving_average(self->samples_between_beats))) {
		//if (self->first_beat == true) {
		//	average_delay = get_moving_average(self->samples_between_beats, self->data_samples_since_beat);
		//	self->first_beat = false;
		//}

		average_delay = get_moving_average(self->samples_between_beats, self->data_samples_since_beat);

		if (self->first_beat == true) {
			self->BPM = 60.0 * self->sample_frequency / self->data_samples_since_beat;
			self->first_beat = false;
		} else {
			self->BPM = 60.0 * self->sample_frequency / average_delay;
		}

		self->data_samples_since_beat = 0;

	}
	return self->BPM;
}



/*

void initialize_ECG_r(ECG *self, int data, int reference) {

}

int get_BPM_r(ECG *self, int data, int reference) {
	if (self->reference_availability == REFERENCE_UNAVAILABLE) {

	}
}
*/