#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define ARRSIZE 2		// Initial size of adjacency lists
#define FILELEN 64		// Maximum file length
#define COMMLEN 128		// Length of plot command
#define BUFSIZE 1028	// Maximum adjacency vector for each vertex

// Function definitions
Vertex ** layout( Vertex**, int )	// Top-level clustering function

typedef struct Vertex
{
	int label;			// Node label (e.g., 1, 2, 3, etc.)
	int degree;			// Number of adjacent nodes
	int * adjList;		// List of adjacent node labels

	int x, y;			// Position of vertex on graph
} Vertex;

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

	// Set file pointer back to beginning of file
	fseek( fp, 0, SEEK_SET );

	// Array to hold all vertices (graph)
	Vertex ** graph = (Vertex**) malloc( numVertices * sizeof( Vertex * ) );

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

	fclose( fp );

	// Display edges
	// for( i = 0; i < numVertices; i++ )
	// {
	// 	printf( "%2i  (%i, %i)- ", i, graph[ i ] -> x, graph[ i ] -> y );
	// 	for( j = 0; j < graph[ i ] -> degree; j++ )
	// 	{
	// 		// if( graph[ i ] -> adjList[ j ] > graph[ i ] -> label )
	// 		// {
	// 			printf( "{ %2i, %2i }%s", 
	// 					graph[ i ] -> label, 
	// 					graph[ i ] -> adjList[ j ], 
	// 					j == graph[ i ] -> degree - 1 ? "\n\n" : ", " );
	// 		// }
	// 	}
	// }

	// Graph layout
	// Place nodes on the grid
	graph = layout( graph, numVertices );

	// Save output graph
	printf( "Output File: " );
	scanf( "%[^\n]%*c", fileName );
	fp = fopen( fileName, "w" );

	if( !fp )
	{
		printf( "Unable to open '%s'.\n", fileName );
		exit( EXIT_FAILURE );
	}

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

	for( i = 0; i < numVertices; i++ )
	{
		free( graph[ i ] -> adjList );
		free( graph[ i ] );
	}

	free( buff );
	free( graph );

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

Vertex ** layout( Vertex ** graph, int numVertices )
{
	
}