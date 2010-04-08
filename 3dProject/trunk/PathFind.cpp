// raytrace_test.cpp : Defines the entry point for the console application.
//

#include <cmath>
#include <iostream>
#include "Map.h"
#include "stlastar.h"
#include "MapSearchNode.h"
#include "PathFind.h"

using namespace std;

PathFind::PathFind() {

}

PathFind::~PathFind() {

}

bool PathFind::raytrace(const int x0, const int y0, const int x1, const int y1, const bool exitOnObstruction)
{
	path.empty();
	//init
	int inacc = Map::INACCESSIBLE_FIELD_VALUE;
	Map * instance = Map::getInstance();
	int startVal = instance->getValueAt(x0,y0);
	int endVal = instance->getValueAt(x1,y1);
	if(startVal == inacc || endVal == inacc) {
		int * tempCoords = 	instance->convertMapCoordToWorldCoord(x1, y1);
		cout << tempCoords[0] << " " << tempCoords[1] << endl;
		instance->debugMark(x0,y0,7);
		instance->writeToFile("debugpath.txt");
		return false;
	}

    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    int x = x0;
    int y = y0;
    int n = 1 + dx + dy;
    int x_inc = (x1 > x0) ? 1 : -1;
    int y_inc = (y1 > y0) ? 1 : -1;
    int error = dx - dy;
    dx *= 2;
    dy *= 2;

	int oldX = x;
	int oldY = y;

	int astarX1 = 0; 
	int astarY1 = 0;
	int	astarX2 = 0;
	int astarY2 = 0;

	bool obstructed = false;

	//end init
	
	int * coords = new int[2];
	coords[0] = x0;
	coords[1] = y0;
	cout << "before push: " << path.size() << "row: " << coords[0] << "col: " << coords[1] << endl;
	path.push_back(coords);
	cout << "after push: " << path.size() << endl;

    while(!((x == x1) && (y == y1)))
    {
        int val = instance->getValueAt(x,y);
		cout << val << endl;
		if(!obstructed) {
			if(val == 9) {
				if(exitOnObstruction) {
					return false;
				}
				obstructed = true;

				astarX1 = oldX;
				astarY1 = oldY;

				if(!(oldX == x0 && oldY == y0)) {
					int * coords = new int[2];
					coords[0] = oldX;
					coords[1] = oldY;
					cout << "before push: " << path.size() << "row: " << coords[0] << "col: " << coords[1] << endl;
					path.push_back(coords);
					cout << "after push: " << path.size() << endl;
				}
			} else {
				
			}
		} else {
			if(val != 9) {
				obstructed = false;

				astarX2 = x;
				astarY2 = y;
				list<int *> astarNodes;
				astarSearch(astarX1, astarY1, astarX2, astarY2, astarNodes);
				cout << "before insert: " << path.size() << endl;
				path.insert(path.end(), astarNodes.begin(), astarNodes.end());
				cout << "after insert: " << path.size() << endl;
				astarX1 = 0; 
				astarY1 = 0;
				astarX2 = 0;
				astarY2 = 0;
			}
		}
		
        
		oldX = x;
		oldY = y;

		if (error > 0)
        {
            x += x_inc;
            error -= dy;
        }
        else
        {
            y += y_inc;
            error += dx;
        }
		//cout << "x: "<< x << " y: " << y << endl;
    }

	coords = new int[2];
	coords[0] = x1;
	coords[1] = y1;
	cout << "before push: " << path.size() << "row: " << coords[0] << "col: " << coords[1] << endl;
	path.push_back(coords);
	cout << "after push: " << path.size() << endl;
	
	cout << "wait" << endl;
	
		instance->writeToFile("debugpath.txt");
	return true;
}

bool PathFind::existStraightPath(int x0, int y0, int x1, int y1) {
	bool exitOnObstruction = true;
	return raytrace(x0,y0,x1,y1,exitOnObstruction);
}

bool PathFind::calculatePath(int x0, int y0, int x1, int y1) {
	bool exitOnObstruction = false;
	return raytrace(x0,y0,x1,y1,exitOnObstruction);
}
	
void PathFind::getCalculatedPath(std::list<int*>& out) {
	list<int*> copyList(path);
	out = copyList;
}

void PathFind::astarSearch(int x0, int y0, int x1, int y1, list<int *>& out) {
	Map * instance = Map::getInstance();
	cout << "enter astar search" << endl;
	AStarSearch<MapSearchNode> astarsearch;

	unsigned int SearchCount = 0;

	const unsigned int NumSearches = 1;

	while(SearchCount < NumSearches)
	{

		// Create a start state
		MapSearchNode nodeStart;
		nodeStart.y = y0;
		nodeStart.x = x0; 

		// Define the goal state
		MapSearchNode nodeEnd;
		nodeEnd.y = y1;					
		nodeEnd.x = x1; 
		
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
					coords[0] = node->x;
					coords[1] = node->y;
					out.push_back(coords);
					cout << instance->getValueAt(coords[0],coords[1]) << endl;;
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
	cout << "exit astar search" << endl;
}



