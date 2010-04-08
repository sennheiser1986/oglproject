#pragma once

class FlightPath
{
private: 
	int * waypoints;
	int numWaypoints;
	int currWaypoint;
	bool loop;
public:
	FlightPath(void);
	~FlightPath(void);
	FlightPath(int * inWaypoints, int inNumWaypoints, bool inLoop);
	bool  FlightPath::next();
	bool FlightPath::hasWaypoints();
	int FlightPath::getX();
	int FlightPath::getY();
	int FlightPath::getZ();
	void clear();
};
