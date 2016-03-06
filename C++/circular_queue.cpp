#include <iostream>
#include "circular_queue.hpp"

typedef CircularQueue::QueueItem QueueItem;
const QueueItem CircularQueue::EMPTY_QUEUE = -999;

CircularQueue::CircularQueue() : head_(0), tail_(0), capacity_(16), size_(0), sum_(0), average_(0)
{
	items_ = new int[16];
}

CircularQueue::CircularQueue(unsigned int capacity) : head_(0), tail_(0), capacity_(capacity), size_(0), sum_(0), average_(0)
{
	items_ = new int[capacity];
}

CircularQueue::~CircularQueue()
{
	delete[] items_;
	items_ = NULL;
}

bool CircularQueue::empty() const
{    
	return size_ == 0;
}

bool CircularQueue::full() const
{
	return size_ >= capacity_;
}

int CircularQueue::size() const
{  
	return size_;
}

bool CircularQueue::enqueue(QueueItem value)
{
	if(full())
		return false;
	items_[tail_] = value;
	tail_ = (tail_ + 1) % capacity_;
	size_++;

	sum_ += value;
	average_ = sum_ / size_;

	return true;
}

QueueItem CircularQueue::dequeue()
{
	if(empty())
		return EMPTY_QUEUE;
	QueueItem returnItem = items_[head_];
	
	head_ = (head_ + 1) % capacity_;
	size_--;
	
	sum_ -= returnItem;
	average_ = sum_ / size_;

	return returnItem;
}

QueueItem CircularQueue::peek() const
{
	if(empty())
		return EMPTY_QUEUE;
	return items_[head_];
}

int CircularQueue::sum() const {
	return sum_;
}

int CircularQueue::average() const {
	return average_;
}

void CircularQueue::print() const
{
}

