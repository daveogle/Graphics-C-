#pragma once
#include <vector>
#include "cuboid.h"
#include "tank_track.h"
#include "cylinder.h"

class track
{
	int numberOfTracks;
	float startX;
	float actualX;
	float endX;
	float radius;
	std::vector<transformation*> track_transformation;
	std::vector<transformation*> wheel_transformation;
	tank_track* base_track;
	cylinder* base_wheel;
	int n;
	float spacing;
	float track_width;
public:
	track();
	~track();
	void setPositions();
	void move(int speed);
	std::vector<transformation*> getTracks();
	std::vector<transformation*> getWheels();
	tank_track* getTrack();
	cylinder* getBaseWheel();
};

