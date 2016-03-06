#ifndef CORE_H
#define CORE_H

#include <string>
#include <thread>

#include "circular_queue.hpp"

using namespace std;

//this goes in the data listener thread in Main
//void initialize(variable # of arguments); 

class Data {

	string type;

public:

	Data();
	~Data();

	string get_type() const;

	virtual void rawAverage();

};

#endif