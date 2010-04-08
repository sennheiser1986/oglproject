#include "FlightPath.h"
#include <iostream>

using namespace std;

FlightPath::FlightPath(void)
{
	numWaypoints = 0;
}

FlightPath::~FlightPath(void)
{
}

FlightPath::FlightPath(int * inWaypoints, int inNumWaypoints, bool inLoop) {
	numWaypoints = inNumWaypoints;
	waypoints = inWaypoints;
	currWaypoint = 0;
	loop = inLoop;
}

bool FlightPath::next() {
	if(currWaypoint == (numWaypoints - 1)) {
		if(loop) {
			currWaypoint = 0;
		} else {
			return false;
		}
	} else {
		currWaypoint++;
	}	
	cout << "current waypoint++: " << waypoints[currWaypoint * 3 + 0] 
	<< "," << waypoints[currWaypoint * 3 + 1]
	<< "," << waypoints[currWaypoint * 3 + 2]<< endl;
	return true;	
}

int FlightPath::getX() {
	int x = waypoints[currWaypoint * 3 + 0];
	return x;
}

int FlightPath::getY() {
	int y = waypoints[currWaypoint * 3 + 1];
	return y;
}

int FlightPath::getZ() {
	int z = waypoints[currWaypoint * 3 + 2];
	return z;
}

bool FlightPath::hasWaypoints() {
	return (numWaypoints > 0);
}

void FlightPath::clear() {
	numWaypoints = 0;
}

void FlightPath::printWaypoints() {
	for(int i = 0; i < numWaypoints; i++) {
		cout << waypoints[i * 3 + 0] << " ";
		cout << waypoints[i * 3 + 1] << " ";
		cout << waypoints[i * 3 + 2] << endl;
	}
}