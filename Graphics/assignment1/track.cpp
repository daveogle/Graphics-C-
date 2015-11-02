#include "track.h"
#define PI  3.14159265358979323846264338327950288   /* pi */
#include <iostream>

track::track()
{
	this->numberOfTracks = 36;
	this->radius = 0.2;
	this->endX = PI * this->radius;
	this->startX = -this->endX;
	this->actualX = startX;
	setPositions();
}


track::~track()
{
}

void track::setPositions()
{
	std::vector<cuboid*> tracks(this->numberOfTracks);
	float n = this->numberOfTracks / 6;
	float spacing = (PI * radius) / n;
	float track_width = spacing * 0.9;

	for (int i = 0; i < this->numberOfTracks; i++)
	{
		cuboid* track = new cuboid(0.02, track_width, 0.3, 0.5, 40.0);
		track->light->setDiffuse(1.0, 0.0, 0.0);

		if (i < n + 1)
		{
			float theta = ((float)i) * PI / n;
			float rotate_value = (180 / PI) * theta;
			float changeX = this->radius * sin(theta);
			track->transform->translate(this->startX - changeX, 'x');
			track->transform->rotate(-rotate_value, 'z');
			float cornerY = radius * cos(theta);
			track->transform->translate(cornerY, 'y');
			tracks[i] = track;
		}

		if (i >= n+1  && i < n*3)
		{
			actualX = actualX + spacing;
			track->transform->translate(actualX, 'x');
			track->transform->translate(-radius, 'y');
			tracks[i] = track;
		}

		if (i >= n*3 && i < n*4+1)
		{
			float theta = (float)(i - (n*3)) * PI / n;
			float cornerX = radius * sin(PI - theta);
			track->transform->translate((endX + cornerX), 'x');
			float rotate_value = (180 / PI) * theta;
			track->transform->rotate(-rotate_value, 'z');
			float cornerY = radius * cos(PI - theta);
			track->transform->translate(cornerY, 'y');
			tracks[i] = track;
			if (i == n*4)
				actualX = endX + cornerX;
		}
		if (i >= n*4 + 1)
		{
			actualX = actualX - spacing;
			track->transform->translate(actualX, 'x');
			track->transform->translate(radius, 'y');
			tracks[i] = track;
		}
	}
	this->tracks = tracks;
}

void track::moveForward(GLfloat increment)
{
	for (int i = 0; i < this->numberOfTracks; i++)
	{
		glm::vec3 coords = this->tracks[i]->transform->getCoords();

		if (coords.x <= startX)
		{
			float old_theta = acos(coords.y / this->radius);
			float new_theta = increment / this->radius;
			float current_rotate = (180 / PI) * new_theta;
			new_theta = new_theta + old_theta;
			float oldX = coords.x;
			float newX = radius* sin(new_theta);
			float change_x = -((oldX - startX) + newX);
			this->tracks[i]->transform->translate((change_x), 'x');
			this->tracks[i]->transform->rotate(-current_rotate, 'z');

			float newY = this->radius * cos(new_theta);
			float changeY = -(coords.y - newY);
			this->tracks[i]->transform->translate(changeY, 'y');
		}

		else if (coords.y < 0 && coords.x >= startX && coords.x < endX)
		{
			this->tracks[i]->transform->translate(increment, 'x');
		}

		else if (coords.x >= endX)
		{
			float old_theta = acos(coords.y / this->radius);
			float new_theta = increment / this->radius;
			float current_rotate = (180 / PI) * new_theta;
			new_theta = old_theta - new_theta;
			float oldX = coords.x;
			float newX = radius * sin(new_theta);
			float change_x = -oldX + endX + newX;
			this->tracks[i]->transform->translate(change_x, 'x');
			this->tracks[i]->transform->rotate(-current_rotate, 'z');

			float newY = this->radius * cos(new_theta);
			float changeY = -(coords.y - newY);
			this->tracks[i]->transform->translate(changeY, 'y');
		}
		else// if (coords.y > 0 && coords.x <= endX && coords.x > startX)
		{
			this->tracks[i]->transform->translate(-increment, 'x');
		}
	}
}

std::vector<cuboid*> track::getTracks()
{
	return this->tracks;
}

