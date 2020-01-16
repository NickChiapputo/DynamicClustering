import os
import sys
import numpy as np
import networkx as nx
import matplotlib.pyplot as plt
from matplotlib.ticker import (MultipleLocator, AutoMinorLocator)

# Get grahp information from command line arguments
file = sys.argv[ 1 ]
numVertices = int( sys.argv[ 2 ] )

# Create graph
G = nx.Graph()

# If file exists, continue. Otherwise inform user and exit
if os.path.exists( file ):
	# Open the file and loop through it
	with open( file ) as f:
		# Loop over each vertex
		for i in range( 0, numVertices ):
			# Get position and adjacent nodes
			nodes = next( f ).split()

			# Add current node with position information
			G.add_node( i, pos = ( int( nodes[ 0 ] ) + 0.5 , int( nodes[ 1 ] ) + 0.5 ) )

			# Add edges between current node and adjacent nodes
			for j in range( 2, np.size( nodes ) ):
				G.add_edge( i, int( nodes[ j ] ) )

	# Get position dictionary
	pos = nx.get_node_attributes( G, 'pos' )

	# Draw the graph
	nx.draw_networkx( G, pos, with_labels = True )


	# Adjust axes so vertices are in between grid lines and the grid is made of squares to match the website
	ax = plt.gca()
	ax.yaxis.set_major_locator( MultipleLocator( 1 ) )
	ax.xaxis.set_major_locator( MultipleLocator( 1 ) )
	ax.set_aspect( "equal" )

	# Adjust graph display
	plt.grid( 'on' )
	plt.axis( 'on' )
	plt.tight_layout()
	plt.show()
else:
	# Inform user file does not exist
	print( "'%s' does not exist." % ( file ) )

