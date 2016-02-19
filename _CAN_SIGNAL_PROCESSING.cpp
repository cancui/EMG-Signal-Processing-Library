//Copyright stuff
//Designed by Can Cui

#include "_CAN_SIGNAL_PROCESSING.hpp"

using namespace std;

Data::get_type() {
	return type;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////

EMG::EMG() : pin(0), type("EMG"), averagingSize(15), dataLocalSum(0), dataLocalAvg(0), movingDataSum(0) {

	averagingData = new LinkedList();

	for (int i = 0; i < averagingSize; i++) {
		int temp = analogRead(pin);
		averagingData->insert_front(temp);
		dataLocalSum += temp;
		delay(1);
	}

	movingData = new LinkedList();
}

EMG::EMG(int pin0) : pin(pin0), type("EMG"), averagingSize(15), dataLocalSum(0), dataLocalAvg(0), movingDataSum(0) {

	averagingData = new LinkedList();

	for (int i = 0; i < averagingSize; i++) {
		int temp = analogRead(pin);
		averagingData->insert_front(temp);
		dataLocalSum += temp;
		delay(1);
	}

	movingData = new LinkedList();
}

EMG::EMG(int pin, int averagingSize0) : pin(pin0), type("EMG"), averagingSize(averagingSize0), dataLocalSum(0), dataLocalAvg(0), movingDataSum(0)  {

	averagingData = new LinkedList();

	for (int i = 0; i < averagingSize; i++) {
		int temp = analogRead(pin);
		averagingData->insert_front(temp);
		dataLocalSum += temp;
		delay(1);
	}

	movingData = new LinkedList();
}

EMG::~EMG() {

	delete averagingData;
	delete movingData;

}

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
	if (degree <= 1 || degree > 10) 	// extent must be (1, 10]
		return -1;

	return (log(this->rectify() + 1) / log(degree));	// Maybe add a multiplier
} 

int EMG::movingAvg(int scope, int degreeHighPass) {
	int dataSize = movingData->size();

	int temp = this->highPass(degreeHighPass);
	movingDataSum += temp;
	movingData->insert_front(temp);

	if (dataSize < scope) {
		
		return -1;

	} else if (dataSize > scope) {

		for (int i = dataSize - 1; i >= scope; i--) { // "i >= scope" may be "i >= scope - 1" ///// look out for data coming out at "-1" for one iteration
			movingDataSum -= movingData->select(i);
			movingData->remove_back();
		}

		return movingDataSum/scope;

	} else {

		movingDataSum -= movingData->select(scope - 1);
		movingData->remove_back();

		return movingDataSum/scope;
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////

ECG::EMG() : pin(0), min(1025), max(-1), avg(-1) 	{ calibrate(); }

ECG::EMG(int pin0) : pin(pin0), min(1025), max(-1), avg(-1) 	{ calibrate(); }

void ECG::calibrate() {

	int sum = 0;

	for (int i = 0; i < CALIBRATIONLENGTH; i++) {
		int temp = analogRead(pin);
		sum += temp;

		if (temp < min) {
			min = temp;
		} 
		if (temp > max) {
			max = temp;
		}
	}

	avg = sum/CALIBRATIONLENGTH;
}

int ECG::detectHR() { //uses system's timers

}

///////////////////////////////////////////////////////////////////////////////////////////////

AnalogAcc::AnalogAcc() : pin(0), pinY(1), pinZ(2) { calibrate(); }

AnalogAcc::AnalogAcc(int pinX0, int pinY0, int pinZ0) : pin(pinX0), pinY(pinY0), pinZ(pinZ0) { calibrate(); }

AnalogAcc::Vector::Vector(int x0, int y0, int z0) : x(x0), y(y0), z(z0)  { calibrate(); }

int AnalogAcc::Vector::dot(Vector* other) {
	return this->x * other->x + this->y * other->y + this->z * other->z;
}

Vector* AnalogAcc::Vector::cross(Vector* other) {
	Vector* newVector = new Vector(0,0,0);

	newVector->x = this->y * other->z - other->y * this->z;
	newVector->y = other->x * this->z - this->x * other->z;
	newVector->z = this->x * other->y - other->x * this->y; 

	return newVector;
}

int AnalogAcc::Vector::getMagnitude() {
  int a = this->x * this->x;
  int b = this->y * this->y;
  int c = this->z * this->z;
  
  int value = sqrt(a + b + c);

  return value;
}

Vector* AnalogAcc::Vector::unitVector() {
	Vector* newVector = new Vector(0,0,0);
	int mag = this->getMagnitude();

	newVector->x = this->x / mag;
	newVector->y = this->y / mag;
	newVector->z = this->z / mag;

	return newVector;
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

