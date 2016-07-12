#ifndef EMG_H
#define EMG_H

#include "peak_to_peak.h"
#include "moving_average.h"

typedef struct EMG_ EMG;

//An enum of the options which must be specified to create a new EMG struct
typedef enum {
	REFERENCE_AVAILABLE,
	REFERENCE_UNAVAILABLE,
	HIGH_PASS_FILTER_ON,
	HIGH_PASS_FILTER_OFF
} EMG_OPTIONS;


/*
The constructor. Returns a pointer to the newly created EMG struct
-sample_frequency_
	The constant frequency of which data will be given to the EMG data structure (frequency in which filter_EMG will be called).
	Specified in Hz
-range_
	The number of data points used to calculate moving average, specified in number of seconds (0.2-0.5 is a good value for this).
	Data points used in moving average = range_ * sample_frequency_
	Note that the greater the range_ value, thr greater the latency of the filter
-min_EMG_frequency_
	Components of signals (noise) below this frequency will be removed.
	This argument is specified in Hz
	A good value for this is 40
-max_EMG_frequency_ 
	Components of signals (noise) above this frequnecy will not be accounted for
	This is also used in the peak to peak calculations which help remove low frequency noise.
	This argument is specified in Hz
	A good value for this is 150
-remove_low_frequency_
	This argument specifies whether or not to remove low frequency noise. 
	Can be either HIGH_PASS_FILTER_ON (remove low frequency noise) or HIGH_PASS_FILTER_OFF
	If the EMG signal does not pass through an analog high pass filter, it is highly recommended to enable this feature
	Disabling this feature decreases latency slightly
-reference_available_
	This specifies whether a reference signal will be provided (in which case filter_EMG_r will be called instead of filter_EMG)
	Can be either REFERENCE_AVAILABLE or REFERENCE_UNAVAILABLE
*/
EMG *new_EMG(	uint16_t sample_frequency_, 
				float range_, 
				uint16_t min_EMG_frequency_, 
				uint16_t max_EMG_frequency_, 
				EMG_OPTIONS remove_low_frequency_, 
				EMG_OPTIONS reference_available_);


//The destructor. Takes an EMG pointer as argument
void free_EMG(EMG *self);


/*
This function is used to input the most recent data point and returns the most recent filtered EMG value
High and low frequency noise is removed from the signal. The signal is then rectified with respect to it's centre. 
A moving average filter returns the completely filtered EMG signal.
The returned signal is indirectly proportional to the effort exherted by the muscle
This function must be called at the frequency specified in the constructor
-self
	A A pointer to the EMG struct
-data
	The most recent raw data point
*/
int  filter_EMG(EMG *self, int data);


/*
This is the same as filter_EMG, except a reference data point must also be specified
This can only be called if REFERENCE_AVAILABLE was specified in the constructor
-reference_data
	The most recent reference data point
*/
int  filter_EMG_r(EMG *self, int data, int reference_data);



#endif