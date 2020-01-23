#include <stdio.h>
#include "queue.h"

void enqueue( Queue * q, Node * n )
{
	// If new node is the only node in the queue, set it as tail and head
	if( q -> tail == NULL )
	{
		q -> tail = n;
		q -> head = n;
	}
	else
	{
		// Set the current tail's next node to point to the new node
		( q -> tail ) -> next = n;

		// Set the new node's previous node to point to the previous tail
		n -> prev = q -> tail;

		// Set the new node's next node to NULL since it is then end
		n -> next = NULL;

		// Sset the new node as the tail
		q -> tail = n;

		// If the new node is the only node in the queue, set it as the head also
		if( q -> head == NULL )
			q -> head = n;
	}

}

Node * dequeue( Queue * q )
{
	// Get node from the head of the queue
	Node * n = q -> head;

	// Set the head to the next node
	q -> head = n -> next;

	// If the queue wasn't empty, set the new head's previous node to point to NULL since it is now the front node
	if( q -> head != NULL )
		( q -> head ) -> prev = NULL;

	return n;
}