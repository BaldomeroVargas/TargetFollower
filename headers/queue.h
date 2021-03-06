
// Permission to copy is granted provided that this header remains intact. 
// This software is provided with no warranties.

////////////////////////////////////////////////////////////////////////////////

//modifying code to take in Point struct instead of unsigned char. Credit to original owner

#ifndef QUEUE_H
#define QUEUE_H

#include <stdlib.h> // required for malloc

//Circular Queue
//Data Structure to implement circular queue MAX_SIZE 255
// ** IMPORTANT ** : Global definitions work, but initalization must occur inside main()
typedef struct _Queue
{
	Point * buffer; 		// Queue of characters
	unsigned char front; 		// Queue front index
	unsigned char back; 		// Queue back index
	unsigned char num_objects;	// Number of objects in queue
	unsigned char capacity;		// Size of the queue
} *Queue;

////////////////////////////////////////////////////////////////////////////////
//Functionality - Allocates space and initializes a queue
//Parameter: Positive integer for the size of queue (0~255)
//Returns: Reference to a queue
Queue QueueInit(unsigned char size) 
{
	Queue Q 		= malloc (sizeof(struct _Queue));
	Q->buffer 		= malloc (size * sizeof(Point));
	Q->front 		= 0;
	Q->back			= 0;
	Q->num_objects	= 0;
	Q->capacity		= size;
	
	return Q;
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - Checks if queue is empty
//Parameter: None
//Returns: 1 if empty else 0
unsigned char QueueIsEmpty(Queue Q) 
{
	return (Q->num_objects == 0);
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - Checks if queue is full
//Parameter: None
//Returns: 1 if full else 0
unsigned char QueueIsFull(Queue Q) 
{
	return (Q->num_objects == Q->capacity);
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - Flushes the queue
//Parameter: None
//Returns: None
void QueueMakeEmpty(Queue Q)
{
	Q->front 		= 0;
	Q->back 		= 0;
	Q->num_objects	= 0;
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - Push a character onto back of queue
//Parameter: Takes a single unsigned char value
//Returns: 1 if full else 0
unsigned char QueueEnqueue(Queue Q, Point value) 
{
	if(Q->num_objects < Q->capacity)	// If queue is not full
	{
		Q->buffer[Q->back] = value;		// Place data into correct location
		Q->back++;						// Increment back counter
		Q->back %= Q->capacity;			// Modulate according to queue capacity
		Q->num_objects++;				// Increment number of objects
		return 0;						// Return queue is not full (0)
	}
	return 1;							// Else return queue is full (1)
}
////////////////////////////////////////////////////////////////////////////////
//Functionality - Pop first character from front of queue
//Parameter: None
//Returns: Unsigned char from queue else null character
Point QueueDequeue(Queue Q) 
{
	//empty queue
	Point empty;
	empty.x = -5;
	empty.y = -5;
	//empty queue
	if(Q->num_objects > 0)				// If queue is not empty
	{									// Retrieve data from correct location
		Point tmp = Q->buffer[Q->front];	
		Q->buffer[Q->front] = empty;	// Clear location with null character
		Q->front++;						// Increment front counter
		Q->front %= Q->capacity;		// Modulate according to queue capacity
		Q->num_objects--;				// Decrement number of objects
		return tmp;						// Return data
	}
	return empty;						// Else return null to indicate empty
}

#endif //QUEUE_H