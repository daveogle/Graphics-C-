#pragma once
#include <vector>
#include "cuboid.h"
#include "tank_track.h"

class track
{
	int numberOfTracks;
	float startX;
	float actualX;
	float endX;
	float radius;
	std::vector<transformation*> track_transformation;
	tank_track* base_track;
	int n;
	float spacing;
	float track_width;
public:
	track();
	~track();
	void setPositions();
	void moveForward(int speed);
	std::vector<transformation*> getTracks();
	tank_track* getTrack();
};

