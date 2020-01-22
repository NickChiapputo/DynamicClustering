#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRSIZE 2		// Initial size of adjacency lists
#define FILELEN 64		// Maximum file length
#define COMMLEN 128		// Length of plot command
#define BUFSIZE 1028	// Maximum adjacency vector for each vertex

typedef struct Vertex
{
	int label;			// Node label (e.g., 1, 2, 3, etc.)
	int degree;			// Number of adjacent nodes
	int * adjList;		// List of adjacent node labels

	int x, y;			// Position of vertex on graph
} Vertex;


// Function definitions
void graphInput( Vertex**, int, FILE* );	// Get graph input from file pointer
Vertex** layout( Vertex**, int );			// Top-level clustering function
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


	// Graph layout
	// Place nodes on the grid
	Vertex ** finalGraph = layout( graph, numVertices );


	// Save output graph
	printf( "Output File: " );
	scanf( "%[^\n]%*c", fileName );
	graphOutput( finalGraph, numVertices, fileName );

	int i;
	for( i = 0; i < numVertices; i++ )
	{
		free( graph[ i ] -> adjList );
		free( graph[ i ] );
		free( finalGraph[ i ] -> adjList );
		free( finalGraph[ i ] );
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

Vertex** layout( Vertex ** g, int numVertices )
{
	int i, j;


	// Create final, efficient graph and allocate space for vertices
	Vertex ** graph = (Vertex**) malloc( numVertices * sizeof( Vertex * ) );

	// Copy data from initial graph to final graph
	for( i = 0; i < numVertices; i++ )
	{
		Vertex * v = (Vertex*) malloc( sizeof( Vertex ) );
		v -> adjList = (int*) malloc( ARRSIZE * sizeof( int ) );
		*v = *g[ i ];
		graph[ i ] = v;
	}


	// Search for node with highest degree
	int maxDegree = graph[ 0 ] -> degree;
	int maxIndex = 0;

	graph[ 0 ] -> x = 0;
	graph[ 0 ] -> y = 0;

	for( i = 1; i < numVertices; i++ )
	{
		if( graph[ i ] -> degree > maxDegree )
		{
			maxDegree = graph[ i ] -> degree;
			maxIndex = i;
		}
		graph[ i ] -> x = 0;
		graph[ i ] -> y = 0;
	}

	// Place connected nodes in order around starting node
	Vertex currV = *graph[ maxIndex ];
	graph[ currV.adjList[ 0 ] ] -> x = currV.x - 1;
	graph[ currV.adjList[ 0 ] ] -> y = currV.y;

	if( currV.degree > 1 )
	{
		graph[ currV.adjList[ 1 ] ] -> x = currV.x;
		graph[ currV.adjList[ 1 ] ] -> y = currV.y + 1;
	}

	if( currV.degree > 2 )
	{
		graph[ currV.adjList[ 2 ] ] -> x = currV.x + 1;
		graph[ currV.adjList[ 2 ] ] -> y = currV.y;
	}

	if( currV.degree > 3 )
	{
		graph[ currV.adjList[ 3 ] ] -> x = currV.x;
		graph[ currV.adjList[ 3 ] ] -> y = currV.y - 1;
	}
	
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