#pragma once
#include <vector>
#include "cuboid.h"

class track
{
	std::vector<cuboid*> tracks;

public:
	track();
	~track();
	void setPositions();
	void moveForward(GLfloat increment);
	void moveBack();
	std::vector<cuboid*> getTracks();
};

