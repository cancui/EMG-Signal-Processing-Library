#ifndef ECG_H
#define ECG_H

#include "peak_to_peak.h"
#include "moving_average.h"

typedef struct ECG_ ECG;

//An enum for the options that must be specified when creating a new ECG struct
typedef enum {
	REFERENCE_AVAILABLE,
	REFERENCE_UNAVAILABLE,
	AUTODETECT_THRESHOLD_ON,
	AUTODETECT_THRESHOLD_OFF
} ECG_OPTIONS;

/*
This is the constructor for the ECG struct. It returns a pointer to the newly created struct
-sample_frequency_ 
	This is the constant frequency in which data will be given (the frequency which get_BPM or get_BPM_r will be called)
-pkpk_threshold_ratio_ 
	This is the ratio of peak to peak voltage during ventricular depolarization and the PR interval. 
	This value differs slightly between each ECG setup and must be determined by experimentation. 
	Alternatively, you can use the autodetect_ feature, 
	which allows the software to calculate this ratio itself but does not guarantee perfect consistency		
	2.0 is a good value for this
-reference_availability_ 
	Specifies whether the filter will be used with a reference signal 
	(which means get_BPM_r will be called instead of get_BPM). 
	The value of this argument can either be REFERENCE_AVAILABLE or REFERENCE_UNAVAILABLE
-autodetect_ 
	Specifies whether pkpk_threshold_ratio_ will be specified by the user, or calculated automatically. 
	This argument can either be AUTODETECT_THRESHOLD_ON or AUTODETECT_THRESHOLD_OFF. 
	Note that autodetect is not perfect, so it's typically more accurate to manually set an experimentally obtained pkpk_threshold_ratio_.
*/
ECG *new_ECG(uint16_t sample_frequency_, 
	 		 float pkpk_threshold_ratio_, 
			 ECG_OPTIONS reference_availability_, 
			 ECG_OPTIONS autodetect_);


//The destructor for the ECG struct. Takes an ECG struct as its argument
void free_ECG(ECG *self);

/*
This function is automatically called every time get_BPM or get_BPM_r is called, for the first 3 seconds of operation. 
This is necessary to initialize the BPM detection feature. It is not necessary for the user to call this function. 
*/
void initialize_ECG(ECG *self, 
					int data);

/*
This function returns the BPM indicated by the ECG signal supplied to it. 
-self 
	A pointer to the ECG struct
-data 
	The most recent data point of the signal. 

Note that it takes 5-6 seconds of operation to achieve full accuracy. 
For full accuracy, this function must be called at the frequency specified when the ECG struct was created
*/
int get_BPM(ECG *self, 
			int data);


/*
This function is the alternative to get_BPM, when a reference was specified as available. 
-reference
	The most recent reference data point of the signal
*/
int get_BPM_r(ECG *self, 
		      int data, 
			  int reference);

#endif