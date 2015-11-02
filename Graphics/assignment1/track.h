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
	std::vector<cuboid*> tracks;

public:
	track();
	~track();
	void setPositions();
	void moveForward(GLfloat increment);
	void moveBack();
	std::vector<cuboid*> getTracks();
};

