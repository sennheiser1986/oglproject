#include "Hunter.h"
#include "Player.h"
#include "Map.h"
#include "stlastar.h"
#include "MapSearchNode.h"
#include <iostream>
#include <math.h>
#include <list>

Hunter::Hunter(void)
{
}

Hunter::~Hunter(void)
{
}

Hunter::Hunter(float xIn, float yIn, float zIn) {
	x = xIn;
	y = yIn;
	z = zIn;
}

void Hunter::draw() {

}

void Hunter::move() {
	Player * player = Player::getInstance();
	int * playerCoords = player->getGridCoords();

	AStarSearch<MapSearchNode> astarsearch;
	
	Map * instance = Map::getInstance();
	int * hunterCoords = instance->convertWorldCoordToMapCoord(x,z);

	int width = instance->getWidth();
	int height = instance->getHeight();

	unsigned int SearchCount = 0;

	const unsigned int NumSearches = 1;

	list<int *> nodesList;

	while(SearchCount < NumSearches)
	{

		// Create a start state
		MapSearchNode nodeStart;
		nodeStart.x = hunterCoords[0];
		nodeStart.y = hunterCoords[1]; 

		// Define the goal state
		MapSearchNode nodeEnd;
		nodeEnd.x = playerCoords[0];						
		nodeEnd.y = playerCoords[1]; 
		
		// Set Start and goal states
		
		astarsearch.SetStartAndGoalStates( nodeStart, nodeEnd );

		unsigned int SearchState;
		unsigned int SearchSteps = 0;

		do
		{
			SearchState = astarsearch.SearchStep();

			SearchSteps++;

	#if DEBUG_LISTS

			cout << "Steps:" << SearchSteps << "\n";

			int len = 0;

			cout << "Open:\n";
			MapSearchNode *p = astarsearch.GetOpenListStart();
			while( p )
			{
				len++;
	#if !DEBUG_LIST_LENGTHS_ONLY			
				((MapSearchNode *)p)->PrintNodeInfo();
	#endif
				p = astarsearch.GetOpenListNext();
				
			}

			cout << "Open list has " << len << " nodes\n";

			len = 0;

			cout << "Closed:\n";
			p = astarsearch.GetClosedListStart();
			while( p )
			{
				len++;
	#if !DEBUG_LIST_LENGTHS_ONLY			
				p->PrintNodeInfo();
	#endif			
				p = astarsearch.GetClosedListNext();
			}

			cout << "Closed list has " << len << " nodes\n";
	#endif

		}
		while( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SEARCHING );

		if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_SUCCEEDED )
		{
			cout << "Search found goal state\n";

				MapSearchNode *node = astarsearch.GetSolutionStart();

	#if DISPLAY_SOLUTION
				cout << "Displaying solution\n";
	#endif
				int steps = 0;

				node->PrintNodeInfo();
				for( ;; )
				{
					node = astarsearch.GetSolutionNext();

					if( !node )
					{
						break;
					}

					node->PrintNodeInfo();
					int * coords = new int[2];
					coords[0] = node->y;
					coords[1] = node->x;
					nodesList.push_back(coords);
					
					steps ++;
				
				};

				cout << "Solution steps " << steps << endl;

				// Once you're done with the solution you can free the nodes up
				astarsearch.FreeSolutionNodes();

	
		}
		else if( SearchState == AStarSearch<MapSearchNode>::SEARCH_STATE_FAILED ) 
		{
			cout << "Search terminated. Did not find goal state\n";
		
		}

		// Display the number of loops the search went through
		cout << "SearchSteps : " << SearchSteps << "\n";

		SearchCount ++;

		astarsearch.EnsureMemoryFreed();
	}

	list<int *>::iterator it;		
	int i = 0;
	for (it = nodesList.begin() ; it != nodesList.end(); it++ ) {
		if(i == 10) {
			i = 0;
		}
		int * coords = new int[2];
		coords = *it;
		cout << coords[0] << " " << coords[1] << " " << instance->getValueAt(coords[0],coords[1]) << endl;
		int valAt = instance->getValueAt(coords[0],coords[1]);
		instance->mark(coords[1], coords[0], i);
		i++;
	}
	instance->writeToFile("hunter.txt");
}
