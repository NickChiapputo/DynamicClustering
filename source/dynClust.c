#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "queue.h"
#include "vertex.h"


#define ARRSIZE 2		// Initial size of adjacency lists
#define FILELEN 64		// Maximum file length
#define COMMLEN 128		// Length of plot command
#define BUFSIZE 1028	// Maximum adjacency vector for each vertex


// Function definitions
void graphInput( Vertex**, int, FILE* );	// Get graph input from file pointer
Vertex** layout( Vertex**, int, char* );	// Top-level clustering function
void graphOutput( Vertex**, int, char* );	// Write graph output to file


int main()
{
	system( "ls" );

	// Get file name from user
	char fileName[ FILELEN ];
	printf( "Input File: " );
	scanf( "%[^\n]%*c", fileName );
	FILE * fp = fopen( fileName, "r" );

	if( fp == NULL )
	{
		printf( "Unable to open '%s'.\n", fileName );
		exit( EXIT_FAILURE );
	}

	// String vars to hold line data from file and to parse through line data
	char * buff = malloc( BUFSIZE * sizeof( char ) );
	char * temp;

	// Keep track of the number of vertices
	int numVertices = 0;

	// Loop over each line of file
	// Each line represents a different vertex
	// Count number of vertices
	while( fgets( buff, BUFSIZE, fp ) )
	{
		numVertices++;
	}
	free( buff );

	// Set file pointer back to beginning of file
	fseek( fp, 0, SEEK_SET );

	// Get graph input and hold each vertex (as a pointer) in the 'graph' array
	Vertex ** graph = (Vertex**) malloc( numVertices * sizeof( Vertex * ) );
	graphInput( graph, numVertices, fp );

	// Get output file name
	printf( "Output File: " );
	scanf( "%[^\n]%*c", fileName );


	// Graph layout
	// Place nodes on the grid
	Vertex ** finalGraph = layout( graph, numVertices, fileName );


	// Save output graph
	graphOutput( finalGraph, numVertices, fileName );

	int i;
	for( i = 0; i < numVertices; i++ )
	{
		if( graph[ i ] == finalGraph[ i ] )
			puts( "SameSame" );
		else
		{
			// free( finalGraph[ i ] -> adjList );
			free( finalGraph[ i ] );	
		}

		free( graph[ i ] -> adjList );
		free( graph[ i ] );
	}

	free( graph );
	free( finalGraph );

	char response;
	printf( "Display graph (y/n)? " );
	scanf( "%c", &response );

	if( response == 'y' || response == 'Y' )
	{
		char command[ COMMLEN ];
		snprintf( command, COMMLEN, "make plot FILE=%s NODES=%i", fileName, numVertices );
		system( command );
	}

	return 0;
} 

void graphInput( Vertex ** graph, int numVertices, FILE * fp )
{
	// String vars to hold line data from file and to parse through line data
	char * buff = malloc( BUFSIZE * sizeof( char ) );
	char * temp;

	// Read each line until the end of the file
	// Use i as current node label. [ 0, numVertices - 1 ]
	int i = 0, j;
	while( fgets( buff, BUFSIZE, fp )  )
	{
		// Create instance of vertex and initialize values
		Vertex * v  	= (Vertex*) malloc( sizeof( Vertex ) );
		v -> label 		= i;
		v -> degree 	= 0;
		v -> adjList 	= (int*) malloc( ARRSIZE * sizeof( int ) );

		// Get the x position
		temp = strtok( buff, " " );
		v -> x = atoi( temp );

		// Get the y position
		temp = strtok( NULL, " " );
		v -> y = atoi( temp );

		// Get the first adjacent node
		temp = strtok( NULL, " " );
		int maxAdjSize = ARRSIZE;
		while( temp != NULL && temp[ 0 ] != '\n' && temp[ 0 ] != '\0' )
		{
			// Add adjacent node to adjacency list and increment degree
			v -> adjList[ v -> degree++ ] = atoi( temp );

			// If adjacent vertex array is full, double allocated size
			if( v -> degree == maxAdjSize )
			{
				maxAdjSize <<= 1;
				v -> adjList = realloc( v -> adjList, maxAdjSize * sizeof( int ) );

				if( !( v -> adjList ) )
				{
					puts( "Unable to allocate sufficient memory. Unable to complete." );
					exit( EXIT_FAILURE );
				}
			}

			// Get the next space-delimited adjacent vertex label
			temp = strtok( NULL, " " );
		}

		// Add vertex to graph
		graph[ i++ ] = v;
	}

	free( buff );
	fclose( fp );	
}

Vertex** layout( Vertex ** g, int numVertices, char* fileName )
{
	int i, j;


	// Save initial graph
	char * tempFileName = (char*) malloc( FILELEN * sizeof( char ) );
	strcpy( tempFileName, fileName );
	tempFileName = strtok( tempFileName, "." );
	strcat( tempFileName, "_init.dat" );
	graphOutput( g, numVertices, tempFileName );

	// Create final, efficient graph and allocate space for vertices
	Vertex ** graph = (Vertex**) malloc( numVertices * sizeof( Vertex * ) );

	// Copy data from initial graph to final graph
	for( i = 0; i < numVertices; i++ )
	{
		Vertex * v = (Vertex*) malloc( sizeof( Vertex ) );
		v -> adjList = (int*) malloc( g[ i ] -> degree * sizeof( int ) );

		memcpy( v -> adjList, g[ i ] -> adjList, g[ i ] -> degree * sizeof( int ) );
		memcpy( v, g[ i ], sizeof( Vertex ) );

		graph[ i ] = v;
	}


	// Create maximum matrix for graph. Value represents label of node at the position
	int ** grid = (int**) malloc( numVertices * sizeof( int* ) );

	// Set default values to -1 for empty. Center space is given as 0
	for( i = 0; i < numVertices; i++ )
	{
		grid[ i ] = (int*) malloc( numVertices * sizeof( int ) );
		for( j = 0; j < numVertices; j++ )
		{
			grid[ i ][ j ] = -1;
		}
	}

	// Create queue to visit nodes in order
	Queue * q = malloc( sizeof( Queue ) );
	q -> tail = NULL;
	q -> head = NULL;


	// Search for node with highest degree
	int maxDegree = graph[ 0 ] -> degree;
	int maxIndex = 0;

	for( i = 0; i < numVertices; i++ )
	{
		if( graph[ i ] -> degree > maxDegree )
		{
			maxDegree = graph[ i ] -> degree;
			maxIndex = i;
		}

		// Set all nodes to middle of plot by default
		graph[ i ] -> x = numVertices / 2;
		graph[ i ] -> y = numVertices / 2;
		graph[ i ] -> placed = 0;
	}

	// Place node with highest degree in middle of grid
	Vertex v = *graph[ maxIndex ];
	v.x = numVertices / 2;
	v.y = numVertices / 2;
	grid[ numVertices / 2 ][ numVertices / 2 ] = v.label;

	// Add node with highest degree to queue
	Node * temp = malloc( sizeof( Node ) );
	temp -> next = NULL;
	temp -> prev = NULL;
	temp -> v = graph[ maxIndex ];
	enqueue( q, temp );


	// Loop until queue is empty
	// At each node, attempt to place nodes in four available spots around current node
	// If unable to place spots, break out
	// If able to place spots, save intermediate graph
	int count = 0;
	while( q -> head != NULL )
	{
		Node * n = dequeue( q );
		count++;
		printf( "Count: %i\n", count );


		Vertex v = *( n -> v );

		// Loop through adjacent nodes and attempt to place in four legal spots
		// around current node
		for( i = 0; i < v.degree; i++ )
		{
			if( graph[ v.adjList[ i ] ] -> placed != 1 )
			{
				if( grid[ v.x - 1 ][ v.y ] == -1 )			// Attempt to place to the left
				{
					graph[ v.adjList[ i ] ] -> x = v.x - 1;
					graph[ v.adjList[ i ] ] -> y = v.y;

					grid[ v.x - 1 ][ v.y ] = v.adjList[ i ];
				}
				else if( grid[ v.x ][ v.y + 1 ] == -1 )		// Attempt to place to the top
				{
					graph[ v.adjList[ i ] ] -> x = v.x;
					graph[ v.adjList[ i ] ] -> y = v.y + 1;

					grid[ v.x ][ v.y + 1 ] = v.adjList[ i ];
				}
				else if( grid[ v.x + 1 ][ v.y ] == -1 )		// Attempt to place to the right
				{
					graph[ v.adjList[ i ] ] -> x = v.x + 1;
					graph[ v.adjList[ i ] ] -> y = v.y;

					grid[ v.x + 1 ][ v.y ] = v.adjList[ i ];
				}
				else if( grid[ v.x ][ v.y - 1 ] == -1 )		// Attempt to place to the bottom
				{
					graph[ v.adjList[ i ] ] -> x = v.x;
					graph[ v.adjList[ i ] ] -> y = v.y - 1;

					grid[ v.x ][ v.y - 1 ] = v.adjList[ i ];
				}
				else										// Break out if unable to place node anywhere
				{
					puts( "Unable to place" );
					break;
				}

				printf( "%i: (%i, %i)\n"
						"Place %i: (%i, %i)\n\n",
						v.label, v.x, v.y,
						v.adjList[ i ], graph[ v.adjList[ i ] ] -> x, graph[ v.adjList[ i ] ] -> y );

				// Mark vertex as placed
				printf( "Enqueue %i\n", v.adjList[ i ] );
				graph[ v.adjList[ i ] ] -> placed = 1;

				Node * newNode = malloc( sizeof( Node ) );
				newNode -> next = NULL;
				newNode -> prev = NULL;
				newNode -> v = graph[ v.adjList[ i ] ];
				enqueue( q, newNode );
			}
		}

		free( n );

		if( i != v.degree )
		{
			puts( "i != v.degree" );
			break;
		}
	}

	if( count != numVertices )
	{
		printf( "Unable to place all nodes." );

		// Free extra nodes
		while( q -> head != NULL )
		{
			Node * n = dequeue( q );
			free( n );
		}
	}


	free( q );
	
	puts( "" );
	return graph;
}

void graphOutput( Vertex ** graph, int numVertices, char * fileName )
{
	FILE * fp = fopen( fileName, "w" );

	if( !fp )
	{
		printf( "Unable to open '%s'.\n", fileName );
		exit( EXIT_FAILURE );
	}

	int i, j;
	for( i = 0; i < numVertices; i++ )
	{
		// Print x, y coordinates
		fprintf( fp, "%i %i", graph[ i ] -> x, graph[ i ] -> y );

		// Print adjacent nodes
		for( j = 0; j < graph[ i ] -> degree; j++ )
		{
			fprintf( fp, " %i", graph[ i ] -> adjList[ j ] );
		}
		fprintf( fp, "\n" );
	}

	fclose( fp );
}