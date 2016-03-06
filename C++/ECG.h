#ifndef ECG_H
#define ECG_H

#include "circular_queue.hpp"
#include "core.h"

class ECG : public Data {
public:
	ECG();

	void calibrate(); //finds max, min, and average
	int detectHR();

private:

	int min, max, avg; //maximum and minimum analog readings
	int HR; //heart rate

	const int CALIBRATIONLENGTH = 2000;

};

#endif
