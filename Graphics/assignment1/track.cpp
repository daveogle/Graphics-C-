#include "track.h"
#define PI  3.14159265358979323846264338327950288   /* pi */
#include <iostream>

track::track()
{
	this->numberOfTracks = 36;
	this->radius = 0.2;
	this->n = this->numberOfTracks / 6;
	this->spacing = (PI * radius) / this->n;
	this->track_width = spacing * 0.9;
	this->base_track = new tank_track(0.02, track_width, 0.3, 0.5, 40.0);
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
	std::vector<transformation*> track_transformation(this->numberOfTracks);
	float rotate_value = 0;
	for (int i = 0; i < this->numberOfTracks; i++)
	{
		transformation* track = new transformation();
		this->base_track->light->setDiffuse(1.0, 0.0, 0.0);

		if (i < n + 1)
		{
			float theta = ((float)i) * PI / n;
			rotate_value = 180 + (180 / PI) * theta;
			float changeX = this->radius * sin(theta);
			track->translate(this->startX - changeX, 'x');
			track->rotate(-rotate_value, 'z');
			float cornerY = radius * cos(theta);
			track->translate(cornerY, 'y');
			track_transformation[i] = track;
		}

		if (i >= n+1  && i < n*3)
		{
			actualX = actualX + spacing;
			track->translate(actualX, 'x');
			track->translate(-radius, 'y');
			track->rotate(-rotate_value, 'z');
			track_transformation[i] = track;
		}

		if (i >= n*3 && i < n*4+1)
		{
			float theta = (float)(i - (n*3)) * PI / n;
			float cornerX = radius * sin(PI - theta);
			track->translate((endX + cornerX), 'x');
			rotate_value = (180 / PI) * theta;
			track->rotate(-rotate_value, 'z');
			float cornerY = radius * cos(PI - theta);
			track->translate(cornerY, 'y');
			track_transformation[i] = track;
			if (i == n*4)
				actualX = endX + cornerX;
		}
		if (i >= n*4 + 1)
		{
			actualX = actualX - spacing;
			track->translate(actualX, 'x');
			track->translate(radius, 'y');
			track->rotate(-rotate_value, 'z');
			track_transformation[i] = track;
		}
	}
	this->track_transformation = track_transformation;
}

void track::moveForward(int speed)
{
	GLfloat increment = 0.001;
	if (speed < 0)
	{
		increment = increment * -1;
		speed = speed * -1;
	}
	for (int i = 0; i < this->numberOfTracks; i++)
	{
		for (int j = 0; j < speed; j++)
		{
			glm::vec3 coords = this->track_transformation[i]->getCoords();

			if (coords.x <= startX)
			{
				float old_theta = acos(coords.y / this->radius);
				float new_theta = increment / this->radius;
				float current_rotate = (180 / PI) * new_theta;
				new_theta = new_theta + old_theta;
				float oldX = coords.x;
				float newX = radius* sin(new_theta);
				float change_x = -((oldX - startX) + newX);
				this->track_transformation[i]->translate((change_x), 'x');
				this->track_transformation[i]->rotate(-current_rotate, 'z');

				float newY = this->radius * cos(new_theta);
				float changeY = -(coords.y - newY);
				this->track_transformation[i]->translate(changeY, 'y');
			}

			else if (coords.y < 0 && coords.x >= startX && coords.x < endX)
			{
				this->track_transformation[i]->translate(increment, 'x');
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
				this->track_transformation[i]->translate(change_x, 'x');
				this->track_transformation[i]->rotate(-current_rotate, 'z');

				float newY = this->radius * cos(new_theta);
				float changeY = -(coords.y - newY);
				this->track_transformation[i]->translate(changeY, 'y');
			}
			else
			{
				this->track_transformation[i]->translate(-increment, 'x');
			}
		}
	}
}

std::vector<transformation*> track::getTracks()
{
	return this->track_transformation;
}

tank_track* track::getTrack()
{
	return this->base_track;
}
