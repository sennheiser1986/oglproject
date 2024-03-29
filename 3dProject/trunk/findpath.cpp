////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// STL A* Search implementation
// (C)2001 Justin Heyes-Jones
//
// Finding a path on a simple grid maze
// This shows how to do shortest path finding using A*

////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#include "stlastar.h" // See header for copyright and usage information

#include "Map.h"

#include <iostream>
#include <math.h>

#define DEBUG_LISTS 0
#define DEBUG_LIST_LENGTHS_ONLY 0

using namespace std;





// Definitions


// Main

//int main( int argc, char *argv[] )
//{
//
//	cout << "STL A* Search implementation\n(C)2001 Justin Heyes-Jones\n";
//
//	// Our sample problem defines the world as a 2d array representing a terrain
//	// Each element contains an integer from 0 to 5 which indicates the cost 
//	// of travel across the terrain. Zero means the least possible difficulty 
//	// in travelling (think ice rink if you can skate) whilst 5 represents the 
//	// most difficult. 9 indicates that we cannot pass.
//
//	// Create an instance of the search class...
//
//	AStarSearch<MapSearchNode> astarsearch;
//	Map * instance = Map::getInstance();
//	int width = instance->getWidth();
//	int height = instance->getHeight();
//
//	unsigned int SearchCount = 0;
//
//	const unsigned int NumSearches = 1;
//
//	while(SearchCount < NumSearches)
//	{
//
//		// Create a start state
//		MapSearchNode nodeStart;
//		nodeStart.x = rand()%width;
//		nodeStart.y = rand()%height; 
//
//		// Define the goal state
//		MapSearchNode nodeEnd;
//		nodeEnd.x = rand()%width;						
//		nodeEnd.y = rand()%height; 
//		
//		// Set Start and goal states
//		
//		astarsearch.SetStartAndGoalStates( nodeStart, nodeEnd );
//
//		unsigned int SearchState;
//		unsigned int SearchSteps = 0;
//
//		do
//		{
//			SearchState = astarsearch.SearchStep();
//
//			SearchSteps++;
//
//	#if DEBUG_LISTS
//
//			cout << "Steps:" << SearchSteps << "\n";
//
//			int len = 0;
//
//			cout << "Open:\n";
//			MapSearchNode *p = astarsearch.GetOpenListStart();
//			while( p )
//			{
//				len++;
//	#if !DEBUG_LIST_LENGTHS_ONLY			
//				((MapSearchNode *)p)->PrintNodeInfo();
//	#endif
//				p = astarsearch.GetOpenListNext();
//				
//			}
//
//			cout << "Open list has " << len << " nodes\n";
//
//			len = 0;
//
//			cout << "Closed:\n";
//			p = astarsearch.GetClosedListStart();
//			while( p )
//			{
//				len++;
//	#if !DEBUG_LIST_LENGTHS_ONLY			
//				p->PrintNodeInfo();
//	#endif			
//				p = astarsearch.GetClosedListNext();
//			}
//
//			cout << "Closed list has " << len << " nodes\n";
//	#endif
//
//		}
//		while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING );
//
//		if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED )
//		{
//			cout << "Search found goal state\n";
//
//				MapSearchNode *node = astarsearch.GetSolutionStart();
//
//	#if DISPLAY_SOLUTION
//				cout << "Displaying solution\n";
//	#endif
//				int steps = 0;
//
//				node->PrintNodeInfo();
//				for( ;; )
//				{
//					node = astarsearch.GetSolutionNext();
//
//					if( !node )
//					{
//						break;
//					}
//
//					node->PrintNodeInfo();
//					steps ++;
//				
//				};
//
//				cout << "Solution steps " << steps << endl;
//
//				// Once you're done with the solution you can free the nodes up
//				astarsearch.FreeSolutionNodes();
//
//	
//		}
//		else if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED ) 
//		{
//			cout << "Search terminated. Did not find goal state\n";
//		
//		}
//
//		// Display the number of loops the search went through
//		cout << "SearchSteps : " << SearchSteps << "\n";
//
//		SearchCount ++;
//
//		astarsearch.EnsureMemoryFreed();
//	}
//	
//	return 0;
//}
//
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
