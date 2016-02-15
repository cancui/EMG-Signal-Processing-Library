#include "linked_list.hpp"
#include <cstdlib>
#include <iostream>

LinkedList::Node::Node(int data)
{
	value = data;
	next = NULL;
	prev = NULL;
}

LinkedList::LinkedList()
{
	head_ = NULL;
	tail_ = NULL;
	size_ = 0;
}

LinkedList::~LinkedList()
{
	while(head_) 
		remove_front();

	delete head_;
	delete tail_;
}

unsigned int LinkedList::size() const
{
	return size_;
}

unsigned int LinkedList::capacity() const
{
	return CAPACITY;
}

bool LinkedList::empty() const
{
	return !head_;
}

bool LinkedList::full() const
{
	return size_ >= CAPACITY;
}

int LinkedList::select(unsigned int index) const //can be optimized
{
	if(index >= size_)
		return tail_->value;
	
	Node* selected = head_;
	
	for (int i = 0; i < index; i++)
		selected = selected->next;
	
	return selected->value;
}

unsigned int LinkedList::search(int value) const
{
	unsigned int index = 0;
	Node* item = head_;
	
	while (item && index < size_)
	{
		if(value == item->value)
			return index;
		
		
		item = item->next;
		index++;
	}
	
	return size_;
}

void LinkedList::print() const
{
	Node* selected = head_;
	
	for (int i = 0; i < size_; i++)
	{
		std::cout << "[" << selected->value << "] ";
		selected = selected->next;
	}
}

LinkedList::Node* LinkedList::getNode(unsigned int index) const
{
	Node* selected = head_;
	
	for (int i = 0; i < index; i++)
		selected = selected->next;

	return selected;
}

bool LinkedList::insert(int value, unsigned int index)
{
	if (size_ == CAPACITY || index > size_)
		return false;
	
	else if (size_ == 0 )
	{
		Node* newNode = new Node(value);

		head_ = newNode;
		tail_ = newNode;
		
		newNode = NULL;
		size_++;
		return true;
	}
	
	else if (index == 0)
	{
		Node* newNode = new Node(value);
		newNode->next = getNode(index);
		
		head_ = newNode;
		
		Node* nextNode = getNode(1);
		nextNode->prev = newNode;
		nextNode= NULL;
		
		size_++;
		return true;
	}
	
	else if (index == size_)
	{
		Node* newNode = new Node(value);
		newNode->prev = getNode(index - 1);
	
		tail_ = newNode;
	
		Node* prevNode = getNode(index - 1);
		prevNode->next = newNode;
		prevNode = NULL;
		
		size_++;
		return true;
	}
	
	else
	{
		Node* newNode = new Node(value);
		newNode->next = getNode(index);
		newNode->prev = getNode(index - 1);
	
		Node* prevNode = getNode(index - 1);
		prevNode->next = newNode;
		
		Node* nextNode = getNode(index);
		nextNode->prev = newNode;
	
		newNode = NULL;
		prevNode = NULL;
		nextNode = NULL;
		
		size_++;
		return true;
	}
	
}

bool LinkedList::insert_front(int value)
{
	return insert(value, 0);
}

bool LinkedList::insert_back(int value)
{
	return insert(value, size_);
}

bool LinkedList::remove(unsigned int index)
{
	if (index >= size_)
		return false;
	
	else if (size_ == 1)
	{
		delete head_;
		
		head_ = NULL;
		tail_ = NULL; 
		
		size_--;
		return true;
	}
	else if (index == 0)
	{
		Node* temp = getNode(index);
		Node* nextNode = getNode(index +1);
		
		head_ = nextNode;
		nextNode->prev = NULL;
		
		delete temp;
		temp = NULL;
		nextNode = NULL;
		
		size_--;
		return true;
	}
	else if (index == size_ - 1)
	{
		delete tail_;
		
		Node* newLast = getNode(size_ - 2);
		newLast->next = NULL;
		tail_ = newLast;
		
		newLast = NULL;
		
		size_--;
		return true;
	}
	
	else
	{
		Node* temp = getNode(index);
		Node* prevNode = getNode(index - 1);
		Node* nextNode = getNode(index +1);
		
		prevNode->next = nextNode;
		nextNode->prev = prevNode;
		
		delete temp;
		temp = NULL;
		prevNode = NULL;
		nextNode = NULL;
		
		size_--;
		return true;
	}
	
}

bool LinkedList::remove_front()
{
	return remove(0);
}

bool LinkedList::remove_back()
{
	return remove(size_ - 1);
}

bool LinkedList::replace(unsigned int index, int value)
{	
	if (index >= size_)
		return false;
	
	else
	{
		Node* temp = getNode(index);
	
		temp->value = value;
	
		temp = NULL;
	
		return true;
	}
}
