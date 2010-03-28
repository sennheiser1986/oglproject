#pragma once

class FlightPath
{
private: 
	int * waypoints;
	int numWaypoints;
	int currWaypoint;
public:
	FlightPath(void);
	~FlightPath(void);
	FlightPath(int * inWaypoints, int inNumWaypoints);
	void  FlightPath::next();
	int FlightPath::getX();
	int FlightPath::getY();
	int FlightPath::getZ();
};
