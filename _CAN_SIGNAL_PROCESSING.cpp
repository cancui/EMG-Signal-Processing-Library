#include "_CAN_SIGNAL_PROCESSING.hpp"

using namespace std;

EMG::EMG() {

	pin = 0; 
	type = "EMG";
	averagingSize = 15; //subject to adjustment
	dataLocalSum = 0;

	averagingData = new LinkedList();

	for (int i = 0; i < averagingSize; i++) {
		int temp = analogRead(pin);
		averagingData->insert_front(temp);
		dataLocalSum += temp;
		delay(1);
	}

	rollingData = new LinkedList();
	rollingDataSum = 0;
}

EMG::EMG(int pin0) {

	pin = pin0; 
	type = "EMG";
	averagingSize = 15; //subject to adjustment
	dataLocalSum = 0;

	averagingData = new LinkedList();

	for (int i = 0; i < averagingSize; i++) {
		int temp = analogRead(pin);
		averagingData->insert_front(temp);
		dataLocalSum += temp;
		delay(1);
	}

	rollingData = new LinkedList();
	rollingDataSum = 0;
}

EMG::EMG(int pin, int averagingSize0) {
	
	pin = pin0; 
	type = "EMG";
	averagingSize = averagingSize0; //subject to adjustment
	dataLocalSum = 0;

	averagingData = new LinkedList();

	for (int i = 0; i < averagingSize; i++) {
		int temp = analogRead(pin);
		averagingData->insert_front(temp);
		dataLocalSum += temp;
		delay(1);
	}

	rollingData = new LinkedList();
	rollingDataSum = 0;
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

	if (dataSize < scope) {
		return -1;
	} else if (dataSize > scope) {

		for (int i = dataSize - 1; i >= scope; i--) {
			//FINISH THIS FUNCTION
			
		}


	} else {
		sum -= rollingData->select(scope - 1);
		rollingData->remove_back();
		return rollingDataSum/scope;
	}
}

