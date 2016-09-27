#include "emg.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

struct EMG_ {

	Moving_Average *MA;

	EMG_OPTIONS remove_low_frequency;
	Moving_Average *For_HPF;
	//PkPk *PkPk;

	EMG_OPTIONS reference_available;
	Moving_Average *LPF_data;
	Moving_Average *LPF_reference;

};

EMG *new_EMG(	uint16_t sample_frequency_, 
				float range_, 
				uint16_t min_EMG_frequency_, 
				uint16_t max_EMG_frequency_, 
				EMG_OPTIONS remove_low_frequency_, 
				EMG_OPTIONS reference_available_) 
{
	EMG *to_return = (EMG *)malloc(sizeof(EMG));
	if (!to_return) {
		puts("Could not allocate memory for EMG");
		return NULL;
	}

	to_return->MA = new_moving_average(sample_frequency_ * range_);
	if (!to_return->MA) {
		puts("Could not allocate memory for internal data structures");
		return NULL;
	}

	to_return->remove_low_frequency = remove_low_frequency_;
	if (to_return->remove_low_frequency == HIGH_PASS_FILTER_ON) {
		to_return->For_HPF = new_moving_average(sample_frequency_ * 2 / min_EMG_frequency_);
		if(!to_return->For_HPF){
			puts("Could not allocate memory for internal data structures");
			return NULL;
		}
		/*
		to_return->PkPk = new_pkpk(sample_frequency_, min_EMG_frequency_, max_EMG_frequency_);
		if (!to_return->PkPk) {
			puts("Could not allocate memory for internal data structures");
			return NULL;
		}
		*/
	} else {
		to_return->For_HPF = NULL;
		//to_return->PkPk = NULL;
	}

	to_return->reference_available = reference_available_;
	if (to_return->reference_available == REFERENCE_AVAILABLE) {
		uint16_t length = (0.125 * sample_frequency_ / max_EMG_frequency_ >= 1)?0.125 * sample_frequency_ / max_EMG_frequency_:1;
		to_return->LPF_data = new_moving_average(length);
		to_return->LPF_reference = new_moving_average(length);
		if (!to_return->LPF_data || !to_return->LPF_reference) {
			puts("Could not allocate memory for internal data structures");
			return NULL;
		}
	} else {
		//to_return->LPF_data = NULL;
		uint16_t length = (0.125 * sample_frequency_ / max_EMG_frequency_ >= 1)?0.125 * sample_frequency_ / max_EMG_frequency_:1;
		to_return->LPF_data = new_moving_average(length);

		if (!to_return->LPF_data) {
			puts("Could not allocate memory for internal data structures");
			return NULL;
		}

		to_return->LPF_reference = NULL;
	}

	return to_return;
}

void free_EMG(EMG *self) 
{
	puts("Freeing EMG");
	free_moving_average(self->MA);
	self->MA = NULL;
	if (self->For_HPF) {
		free_moving_average(self->For_HPF);
		self->For_HPF = NULL;
	}
	/*
	if (self->PkPk) {
		free_pkpk(self->PkPk);
		self->PkPk = NULL;
	}
	*/
	if (self->LPF_data) {
		free_moving_average(self->LPF_data);
		self->LPF_data = NULL;
	}
	if (self->LPF_reference) {
		free_moving_average(self->LPF_reference);
		self->LPF_reference = NULL;
	}
	free(self);
	self = NULL;
}

int filter_EMG(	EMG *self, 
				int data) 
{
	if (self->reference_available != REFERENCE_AVAILABLE) {
		data = get_moving_average(self->LPF_data, data);
	}

	if (self->remove_low_frequency == HIGH_PASS_FILTER_ON) {
		//int neutral_value = unpack_data(get_pkpk(self->PkPk, data), PKPK_NEUTRAL);
		int neutral_value = get_moving_average(self->For_HPF, data);
		return get_moving_average(self->MA, abs(data - neutral_value));
	} else {
		return get_moving_average(self->MA, abs(data));
	}
}

int filter_EMG_r(	EMG *self, 
					int data, 
					int reference_data) 
{
	if (self->reference_available != REFERENCE_AVAILABLE) {
		return -1;
	}

	int clean_data = get_moving_average(self->LPF_data, data);
	int clean_reference = get_moving_average(self->LPF_reference, reference_data);
	data = clean_data - reference_data;

	return filter_EMG(self, data);
}