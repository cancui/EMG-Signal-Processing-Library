#include "ecg.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct ECG_ {
	PkPk *signal_tracker;
	Moving_Average *samples_between_beats;

	uint16_t sample_frequency;
	ECG_OPTIONS reference_availability;

	uint16_t initialization_period;
	uint16_t init_counter;
	int init_sum;
	int average_pkpk;
	int BPM;

	float pkpk_threshold_ratio;
	float autodetect_ratio;
	uint16_t data_samples_since_beat;
	bool first_beat;

	ECG_OPTIONS autodetect;
	Moving_Average *init_maxs;
	int init_maxs_average;
	//Moving_Average *init_mins;
	//int init_mins_average;
};

ECG *new_ECG(	uint16_t sample_frequency_, 
				float pkpk_threshold_ratio_, 
				ECG_OPTIONS reference_availability_, 
				ECG_OPTIONS autodetect_) 
{
	ECG *to_return = (ECG *)malloc(sizeof(ECG));
	
	if (to_return == NULL) {
		puts("Not enough memory to allocate for ECG");
		return NULL;
	}

	to_return->signal_tracker = new_pkpk(sample_frequency_, sample_frequency_ / 20, sample_frequency_ / 8);
	to_return->samples_between_beats = new_moving_average(2); //used to be 3

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

	to_return->pkpk_threshold_ratio = pkpk_threshold_ratio_; //2.0 is a good value for this
	to_return->autodetect_ratio = 0.4;
	to_return->data_samples_since_beat = 0;
	to_return->first_beat = true;

	to_return->autodetect = autodetect_;
	if (to_return->autodetect == AUTODETECT_THRESHOLD_ON) {
		to_return->init_maxs = new_moving_average(sample_frequency_ / 40);
		//to_return->init_mins = new_moving_average(sample_frequency_ / 40);
	} else {
		to_return->init_maxs = NULL;
		//to_return->init_mins = NULL;
	}

	to_return->init_maxs_average = 0;
	//to_return->init_mins_average = 0;

	return to_return;
}

void free_ECG( ECG *self ) 
{
	puts("Freeing ECG");
	free_pkpk(self->signal_tracker);
	free_moving_average(self->samples_between_beats);

	if (self->init_maxs != NULL) {
		free_moving_average(self->init_maxs);
	}
	/*
	if (self->init_mins != NULL) {
		free_moving_average(self->init_mins);
	}*/

	free(self);
	self = NULL;
}

void initialize_ECG( ECG *self, 
					 int data ) 
{
	if (self->init_counter >= self->initialization_period) {
		return;
	} 
	
	int current_pkpk = unpack_data(get_pkpk(self->signal_tracker, data), PKPK_PKPK);
	//printf("current pkpk during init: %d\n", current_pkpk);
	self->init_sum += current_pkpk;

	self->init_counter++;
	self->average_pkpk = self->init_sum / self->init_counter;

	if (self->autodetect == AUTODETECT_THRESHOLD_ON) {
		if (is_empty_moving_average(self->init_maxs) || current_pkpk >= oldest_entry_moving_average(self->init_maxs)) {
			self->init_maxs_average = get_moving_average(self->init_maxs, current_pkpk);
		}
		/*
		if (true || is_empty_moving_average(self->init_mins) || current_pkpk <= oldest_entry_moving_average(self->init_mins)) {
			printf("latest min: %d\n", current_pkpk);
			self->init_mins_average = get_moving_average(self->init_mins, current_pkpk);
		}*/

		if (self->init_counter == self->initialization_period - 1) {
			self->pkpk_threshold_ratio = self->autodetect_ratio * self->init_maxs_average / self->average_pkpk; 
			//printf("INIT MAX AVERAGE: %d \n", self->init_maxs_average /*, self->init_mins_average*/);
			//printf("THE CALCULATED THRESHOLD IS %f\n", self->pkpk_threshold_ratio);
			//free_moving_average(self->init_maxs); //causes seg fault after init period
			//self->init_maxs = NULL;
		} 
	}
}

int get_BPM(ECG *self, int data) 
{
	int average_delay = 0; //this variable will eventually hold the BPM value and be returned

	if (self->init_counter < self->initialization_period) {
		initialize_ECG(self, data);
		return 0;
	}
	//return self->average_pkpk;
	int current_pkpk = unpack_data(get_pkpk(self->signal_tracker, data), PKPK_PKPK);
	self->data_samples_since_beat++;

	if ((current_pkpk > self->average_pkpk * self->pkpk_threshold_ratio) && 
		(self->first_beat == true || self->data_samples_since_beat > 0.75 * newest_entry_moving_average(self->samples_between_beats))) {
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

int get_BPM_r(ECG *self, int data, int reference) {
	if (self->reference_availability == REFERENCE_UNAVAILABLE) {
		return -1;
	}
	data = data - reference;
	return get_BPM(self, data);
}
