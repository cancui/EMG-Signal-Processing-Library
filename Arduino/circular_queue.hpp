#ifndef A2_CIRCULAR_QUEUE_HPP
#define A2_CIRCULAR_QUEUE_HPP

class CircularQueue {
public:
    // Defines the kind of data that the queue will contain.
    typedef int QueueItem;
    // Defines a constant that will be used to indicate an empty queue.
    static const QueueItem EMPTY_QUEUE;

private:
    friend class CircularQueueTest;

	// MEMBER VARIABLES
    // An array of queue items.
    QueueItem *items_;
    
    // Index of the first element in the circular queue.
    int head_;
    // Index of the element after the last item in the circular queue.
	int tail_;
	// Maximum number of items in the queue.
    int capacity_;
    // Current number of items in the queue.
    int size_;

    // Copy constructor. Declared private so we don't use it incorrectly.
    CircularQueue(const CircularQueue& other) {}
    // Assignment operator. Declared private so we don't use it incorrectly.
    CircularQueue operator=(const CircularQueue& other) {}
    
public:
	// CONSTRUCTORS/DESTRUCTOR
    // Default constructor of the class CircularQueue. It uses 16 as the 
	// initial capacity of the array, and allocates the required memory 
	// space for the queue. The function appropriately initializes the 
	// fields of the created empty queue.
    CircularQueue();
    // Parametric constructor of the class CircularQueue. It allocates 
	// the required memory space for the queue of the given capacity. 
	// The function appropriately initializes the fields of the created 
	// empty queue.
    CircularQueue(unsigned int capacity);
  
    // Destructor of the class CircularQueue. It deallocates the memory 
	// space allocated for the queue. 
    ~CircularQueue();

	// ACCESSORS    
    // Returns the number of items in the queue.
    int size() const;

    // Returns true if the queue is empty and false otherwise.
    bool empty() const;
  
    // Returns true if the queue is full and false otherwise.
    bool full() const;

    // Prints the queue items sequentially and in order, from the front 
	// to the rear of the queue.
    void print() const;

    // Returns the value at the front of the queue without removing it 
	// from the queue. If the queue is empty, it returns the EMPTY_QUEUE 
	// constant instead.
    QueueItem peek() const;

	// MUTATORS    
    // Takes as an argument a QueueItem value. If the queue is not at capacity, 
	// it inserts the value at the rear of the queue after the last item, and 
	// returns true. If the insertion fails due to lack of space, it returns false.
    bool enqueue(QueueItem value);
  
    // Removes the item from the front of the queue and returns it. If the queue 
	// is empty, it returns the EMPTY_QUEUE constant instead.
	QueueItem dequeue();
};
#endif
