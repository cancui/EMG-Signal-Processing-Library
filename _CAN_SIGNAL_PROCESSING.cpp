#include "_CAN_SIGNAL_PROCESSING.hpp"

using namespace std;

EMG::EMG() : pin(0), type("EMG"), averagingSize(15), dataLocalSum(0), rollingDataSum(0) {

	averagingData = new LinkedList();

	for (int i = 0; i < averagingSize; i++) {
		int temp = analogRead(pin);
		averagingData->insert_front(temp);
		dataLocalSum += temp;
		delay(1);
	}

	rollingData = new LinkedList();
}

EMG::EMG(int pin0) : pin(pin0), type("EMG"), averagingSize(15), dataLocalSum(0), rollingDataSum(0) {

	averagingData = new LinkedList();

	for (int i = 0; i < averagingSize; i++) {
		int temp = analogRead(pin);
		averagingData->insert_front(temp);
		dataLocalSum += temp;
		delay(1);
	}

	rollingData = new LinkedList();
}

EMG::EMG(int pin, int averagingSize0) : pin(pin0), type("EMG"), averagingSize(averagingSize0), dataLocalSum(0), rollingDataSum(0)  {

	averagingData = new LinkedList();

	for (int i = 0; i < averagingSize; i++) {
		int temp = analogRead(pin);
		averagingData->insert_front(temp);
		dataLocalSum += temp;
		delay(1);
	}

	rollingData = new LinkedList();
}

EMG::~EMG() {

	delete averagingData
	delete rollingData;

}


//I FORGOT THAT THESE FUNCTIONS ARE CALLED ON OBJECTS. FIX NOW
int EMG::localAvg() {
	int temp = analogRead(pin);

	dataLocalSum -= averagingData->select(averagingSize-1);
	dataLocalSum += temp;

	averagingData->remove_back();
	averagingData->insert_front(temp);

	dataLocalAvg = dataLocalSum/averagingSize;

	return dataLocalAvg;
}

int EMG::rectify() {
	return abs(analogRead(pin) - this->localAvg());
}

int EMG::highPass(float degree) {
	if (degree <= 1 || degree > 10) // extent must be (1, 10]
		return -1;

	return (log(this->rectify() + 1) / log(degree));
} 

int EMG::rollingAvg(int scope, int degreeHighPass) {
	int dataSize = rollingData->size();

	int temp = this->highPass(degreeHighPass);
	rollingDataSum += temp;
	rollingData->insert_front(temp);

	if (dataSize < scope) {
		
		return -1;

	} else if (dataSize > scope) {
		for (int i = dataSize - 1; i >= scope; i--) { // "i >= scope" may be "i >= scope - 1" ///// look out for data coming out at "-1" for one iteration
			rollingDataSum -= rollingData->select(i);
			rollingData->remove_back();
		}

		return rollingDataSum/scope;
	} else {
		rollingDataSum -= rollingData->select(scope - 1);
		rollingData->remove_back();

		return rollingDataSum/scope;
	}
}







/*

AnalogAcc::AnalogAcc() : pin(0), pinY(1), pinZ(2) {

}

AnalogAcc::AnalogAcc(int pinX0, int pinY0, int pinZ0) : pin(pinX0), pinY(pinY0), pinZ(pinZ0) {

}

AnalogAcc::Vector::Vector(int x0, int y0, int z0) : x(x0), y(y0), z(z0)  {

}

Vector* AnalogAcc::Vector::dot(Vector* other) {
	Vector* newVector = new Vector(0, 0, 0);

	newVector->x = this->x * 
}

Vector* AnalogAcc::Vector::cross(Vector* other) {

}

Vector* AnalogAcc::Vector::unitVector() {

}

//finds average for each axis
void AnalogAcc::calibrate() {
	int sumX = 0, sumY = 0, sumZ = 0;

	for (int i = 0; i < CALIBRATIONLENGTH; i++) {
		sumX += analogRead(pin);
		sumY += analogRead(pinY);
		sumZ += analogRead(pinZ);
	}

	xAvg = sumX / CALIBRATIONLENGTH;
	yAvg = sumY / CALIBRATIONLENGTH;
	zAvg = sumZ / CALIBRATIONLENGTH;
} 

int AnalogAcc::get_x() {
	return analogRead(pin) - xAvg;
}

int AnalogAcc::get_y() {
	return analogRead(pinY) - yAvg;
}

int AnalogAcc::get_z() {
	return analogRead(pinZ) - zAvg;
}

*/