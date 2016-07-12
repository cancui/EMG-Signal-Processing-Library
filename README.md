# EMG/ECG Signal Processing Library

- This is a specialized real-time signal processing library for EMG and ECG signals.
- Most of the algorithms used run in constant time, O(c)
- Currently supports the following languages: C, Python

C DOCUMENTATION
-------------------------------------------

### 1) Setting up
Copy the following .c and .h files into your working directory:
- emg
- ecg
- peak_to_peak
- moving_average
- queue

This library was developed using the GCC compiler with the -std=c11 option. It is advisable to also use this option for compiling. 



### 2) EMG PROCESSING
The library provides a filter which removes high and low frequency noise from a signal, rectifies it with respect to the centre point, and takes a moving average.
This filtered signal is indirectly proportional to the effort exherted by the muscle the sensor is attached to.

-------------------------------------------
```c
typedef enum {
	REFERENCE_AVAILABLE,
	REFERENCE_UNAVAILABLE,
	HIGH_PASS_FILTER_ON,
	HIGH_PASS_FILTER_OFF
} EMG_OPTIONS;
```
An enum of the options which must be specified to create a new EMG struct

-------------------------------------------
```c
EMG *new_EMG(	uint16_t sample_frequency_, 
				float range_, 
				uint16_t min_EMG_frequency_, 
				uint16_t max_EMG_frequency_, 
				EMG_OPTIONS remove_low_frequency_, 
				EMG_OPTIONS reference_available_);
```
The constructor. Returns a pointer to the newly created EMG struct
- sample_frequency_
       *	The constant frequency of which data will be given to the EMG data structure (frequency in which filter_EMG will be called).
       *	Specified in Hz

- range_
 	* The number of data points used to calculate moving average, specified in number of seconds (0.2-0.5 is a good value for this).
	* Data points used in moving average = range_ * sample_frequency_
	* Note that the greater the range_ value, thr greater the latency of the filter
- min_EMG_frequency_
	* Components of signals (noise) below this frequency will be removed.
	* This argument is specified in Hz
	* A good value for this is 40
- max_EMG_frequency_ 
	* Components of signals (noise) above this frequnecy will not be accounted for
	* This is also used in the peak to peak calculations which help remove low frequency noise.
	* This argument is specified in Hz
	* A good value for this is 150
- remove_low_frequency_
	* This argument specifies whether or not to remove low frequency noise. 
	* Can be either HIGH_PASS_FILTER_ON (remove low frequency noise) or HIGH_PASS_FILTER_OFF
	* If the EMG signal does not pass through an analog high pass filter, it is highly recommended to enable this feature
	* Disabling this feature decreases latency slightly
- reference_available_
	* This specifies whether a reference signal will be provided (in which case filter_EMG_r will be called instead of filter_EMG)
	* Can be either REFERENCE_AVAILABLE or REFERENCE_UNAVAILABLE

-------------------------------------------
```c
void free_EMG(EMG *self);
```
The destructor. Takes an EMG pointer as argument

-------------------------------------------
```c
int  filter_EMG(EMG *self, int data);
```
- This function is used to input the most recent data point and returns the most recent filtered EMG value
- High and low frequency noise is removed from the signal. The signal is then rectified with respect to it's centre. 
- A moving average filter returns the completely filtered EMG signal.
- The returned signal is indirectly proportional to the effort exherted by the muscle
- This function must be called at the frequency specified in the constructor

- self
	* A A pointer to the EMG struct
- data
	* The most recent raw data point

-------------------------------------------
```c
int  filter_EMG_r(EMG *self, int data, int reference_data);
```
- This is the same as filter_EMG, except a reference data point must also be specified
- This can only be called if REFERENCE_AVAILABLE was specified in the constructor

- reference_data
	* The most recent reference data point

-------------------------------------------


### 3) ECG PROCESSING
This library provides a tool to derive the BPM indicated by an ECG signal.

-------------------------------------------
```c
typedef enum {
	REFERENCE_AVAILABLE,
	REFERENCE_UNAVAILABLE,
	AUTODETECT_THRESHOLD_ON,
	AUTODETECT_THRESHOLD_OFF
} ECG_OPTIONS;
```
An enum for the options that must be specified when creating a new ECG struct

-------------------------------------------
```c
ECG *new_ECG(uint16_t sample_frequency_, 
	 		 float pkpk_threshold_ratio_, 
			 ECG_OPTIONS reference_availability_, 
			 ECG_OPTIONS autodetect_);
```
This is the constructor for the ECG struct. It returns a pointer to the newly created struct
- sample_frequency_ 
	* This is the constant frequency in which data will be given (the frequency which get_BPM or get_BPM_r will be called)
- pkpk_threshold_ratio_ 
 	* This is the ratio of peak to peak voltage during ventricular depolarization and the PR interval. 
	* This value differs slightly between each ECG setup and must be determined by experimentation. 
	* Alternatively, you can use the autodetect_ feature, which allows the software to calculate this ratio itself but does not guarantee perfect consistency		
	* 2.0 is a good value for this
- reference_availability_ 
	* Specifies whether the filter will be used with a reference signal (which means get_BPM_r will be called instead of get_BPM). 
	* The value of this argument can either be REFERENCE_AVAILABLE or REFERENCE_UNAVAILABLE
- autodetect_ 
	* Specifies whether pkpk_threshold_ratio_ will be specified by the user, or calculated automatically. 
	* This argument can either be AUTODETECT_THRESHOLD_ON or AUTODETECT_THRESHOLD_OFF. 
	* Note that autodetect is not perfect, so it's typically more accurate to manually set an experimentally obtained pkpk_threshold_ratio_.

-------------------------------------------
```c
void free_ECG(ECG *self);
```
The destructor for the ECG struct. Takes an ECG struct as its argument

-------------------------------------------
```c
void initialize_ECG(ECG *self, 
					int data);
```
- This function is automatically called every time get_BPM or get_BPM_r is called, for the first 3 seconds of operation. 
- This is necessary to initialize the BPM detection feature. It is not necessary for the user to call this function. 

-------------------------------------------
```c
int get_BPM(ECG *self, 
			int data);
```
- This function returns the BPM indicated by the ECG signal supplied to it. 

- self 
	* A pointer to the ECG struct
- data 
	* The most recent data point of the signal. 

Note that it takes 5-6 seconds of operation to achieve full accuracy. 
For full accuracy, this function must be called at the frequency specified when the ECG struct was created

-------------------------------------------
```c
int get_BPM_r(ECG *self, 
		      int data, 
			  int reference);
```
This function is the alternative to get_BPM, when a reference was specified as available. 
- reference
	* The most recent reference data point of the signal

-------------------------------------------
