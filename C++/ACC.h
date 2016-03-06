#ifndef ACC_H
#define ACC_H

#include "circular_queue.hpp"
#include "core.h"
#include "linear_vector.h"

class AnalogAcc : public Data { //analog accelerometer
public:
	//constructors (SETS TYPE TO Acc)
	AnalogAcc();

	~AnalogAcc();

	void calibrate(); //finds average for each axis

	int get_x();
	int get_y();
	int get_z();

	int get_mag(); //returns magnitude without direction

	void rawAverage(int x0, int y0, int z0); //this isn't actually the average, this is the function that updates the states of the device

	//retrieve device states
	int shaking() const; //magnitude between 0-100
	bool justSwung() const;
	bool changedDirection() const;
	bool accelerating() const;

private:

	int dataSize;
	CircularQueue* xData;
	CircularQueue* yData;
	CircularQueue* zData;

	Vector* latestVectors; //array storing the most recent vectors
	
	//all initialized to 0 before calibration
	int xAvg, yAvg, zAvg;

	//These will be with reference to their respective averages (they are absolute before calibration, however)
	int x, y, z;

	//calibration length
	const int CALIBRATIONLENGTH = 200;

	//possible accelerometer states
	int shaking; //magnitude between 0-100
	bool justSwung;
	bool changedDirection;
	bool accelerating;

};

#endif
