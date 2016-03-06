#ifndef EMG_H
#define EMG_H

#include "circular_queue.hpp"
#include "core.h"

class EMG : public Data {

	int averagingSize;
	CircularQueue* averagingData;
	int rawAverage, rawSum; //for the raw average

	CircularQueue* movingAvg;
	int movingAvg, movingSum; //for moving average

public:

	EMG();
	EMG(int averagingSize0);
	~EMG();

	//copy constructor
	//class assignment operator

	void rawAverage(int dataEntry);
	int rectify(int dataEntry);
	int highPass();
	int movingAvg();

};

#endif
