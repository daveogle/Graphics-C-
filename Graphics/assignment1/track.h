#pragma once
#include <vector>
#include "cuboid.h"

class track
{
	int numberOfTracks;
	float startX;
	float actualX;
	float endX;
	float radius;
	std::vector<transformation*> track_transformation;
	cuboid* cube;
	int n;
	float spacing;
	float track_width;
public:
	track();
	~track();
	void setPositions();
	void moveForward(int speed);
	std::vector<transformation*> getTracks();
	cuboid* getCube();
};

