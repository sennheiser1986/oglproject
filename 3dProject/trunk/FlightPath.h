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
	bool next();
	bool hasWaypoints();
	int getX();
	int getY();
	int getZ();
	void printWaypoints();
	void clear();
};
