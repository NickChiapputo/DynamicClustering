#ifndef QUEUE_H_
#define QUEUE_H_

#include "vertex.h"

typedef struct Node
{
	Vertex * v;			// Vertex contained in this node
	struct Node * next;	// Points to node behind current node
	struct Node * prev; // Points to node in front of current node
} Node;

typedef struct Queue
{
	Node * head;		// Node at front of queue
	Node * tail;		// Node at back of queue
} Queue;

void enqueue( Queue * q, Node * );
Node * dequeue( Queue * q );

#endif