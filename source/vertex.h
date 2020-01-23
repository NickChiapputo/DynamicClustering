#ifndef VERTEX_H_
#define VERTEX_H_

typedef struct Vertex
{
	int label;			// Node label (e.g., 1, 2, 3, etc.)
	int degree;			// Number of adjacent nodes
	int * adjList;		// List of adjacent node labels

	int x, y;			// Position of vertex on graph
	int placed;			// Determines if vertex has been placed onto new graph
} Vertex;

#endif