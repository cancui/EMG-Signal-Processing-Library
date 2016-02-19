//Copyright stuff
//Designed by Can Cui

#ifndef _CAN_SIGNAL_PROCESSING
#define _CAN_SIGNAL_PROCESSING

#include "linked_list.hpp"
//use a "stack" class later instead of linked list


class Data {
public:
	string get_type(); //returns what type of sensor/data

protected:
	string type; //what type of data this is
	int pin; //analog or digital pin to receive signals from
	
};






class EMG: public Data {
public:
	//constructors (SETS TYPE TO EMG)
	EMG();
	EMG(int pin); 
	EMG(int pin, int averagingSize0); //create a dynamic array for rollingData
	~EMG(); //destructor 

	int localAvg(); //To be called on a separate and looping thread during initialization
	int highPass(float degree); //Extent is 1-10, 10 being the strongest. Logarithmic high-pass filter reducing the difference of spikes from the local mean. 
	int rectify(); //takes the absolute value of (reading - localAvg) 
	
	int rollingAvg(int scope, int degreeHighPass); //Originally what I believed to be a high-pass. returns filtered data. size is the width to average. 


	//int lowPass(float extent); //removes broad waves in data. Relatively sophistocated, challenging to implement

private:

	int averagingSize; //length of rollingAvg
	LinkedList* averagingData; 

	int dataLocalSum;
	int dataLocalAvg; //average of the data over the last [rollingSize] points
	
	LinkedList* rollingData;//pointer for rollingData
	int rollingDataSum; 
};











class ECG: public Data {
public:
	ECG();
	ECG(int pin);

	~ECG();

	void calibrate(); //finds max and min
	void detectHR();


private:

	int min, max; //maximum and minimum analog readings
	int HR; //heart rate

};




class AnalogAcc: public Data { //analog accelerometer
public:
	//constructors (SETS TYPE TO Acc)
	AnalogAcc();
	AnalogAcc(int pinX0, int pinY0, int pinZ0);

	~AnalogAcc();

	void calibrate(); //finds average for each axis

	int get_x();
	int get_y();
	int get_z();

	//background functions for protothreads -- can be optimized to share threads (sometimes device can only be in one state and not the others)
	bool changedDirection(); //returns true if accelerometer changed movement directions
	bool accelerating(); //returns true if accelerometer is accelerating in a general direction

	//feature functions
	int detectShake(); //returns a magnitude, 0 being no shake, 100 being very strong shake



private:

	//consider putting this in a separate file and importing
	struct Vector {
		Vector(int x0, int y0, int z0);


		//NOT QUITE THE FUNCTIONS I NEED - CHECK STATICS NOTES******************
		int dot(Vector* other);
		Vector* cross(Vector* other);

		int getMagnitude();
		Vector* unitVector();

		//int ... cos(pheta) = A.B/AB
		//int ... F.U_ab ... gives component of vector F in the direction of vector U

		int x, y, z;
	};

	//inherited "pin" will be the x axis
	int pinY;
	int pinZ;

	//all initialized to 0 before calibration
	int xAvg;
	int yAvg;
	int zAvg;

	//These will be with reference to their respective averages (they are absolute before calibration, however)
	int x; 
	int y;
	int z;

	//calibration length
	const int CALIBRATIONLENGTH = 100;

	bool changedDirection;
	bool accelerating;

};



#endif