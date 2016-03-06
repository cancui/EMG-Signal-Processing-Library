#include <iostream>
#include <cstdlib>

#include "linked_list.hpp"

using namespace std;

int main() {
	
	LinkedList list1;
	
	cout << list1.empty() << endl;
	
	list1.insert_back(1);	
	list1.insert_back(2);
	list1.insert_back(3);
	list1.insert_back(4);
	list1.insert_front(5);
	
	cout << endl;
	
	cout << list1.empty() << list1.full();
	
	list1.print();
	
	cout << endl;
	
	cout << list1.select(0) << list1.select(4);
	
	cout<< endl << "end";
	
	return 0;
}

