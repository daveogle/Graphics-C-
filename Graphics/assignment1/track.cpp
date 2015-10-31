#include "track.h"
#define PI  3.14159265358979323846264338327950288   /* pi */
#include <iostream>

track::track()
{
	setPositions();
}


track::~track()
{
}

//float offset = -1.5;
//for (int i = 0; i < tracks.size(); i++)
//{
//	tracks[i]->light->setDiffuse(1.0, 0.0, 0.0);
//	tracks[i]->transform->spin(0.2, 'x');
//	tracks[i]->transform->translate(offset, 'x');
//	offset = offset + 0.3;
//}

void track::setPositions()
{
	unsigned int numberOfTracks = 22;
	std::vector<cuboid*> tracks(numberOfTracks);

	float startX = -0.75;
	float actualX = startX;
	float endX = 0.75;
	float radius = 0.2;
	float rotate_amount [6] = {0.0, 30.0, 40.0, 40.0, 40.0, 30.0};
	float current_rotate = 0;

	for (int i = 0; i < numberOfTracks; i++)
	{
		cuboid* track = new cuboid(0.02, 0.2, 0.3, 0.5, 40.0);
		track->light->setDiffuse(1.0, 0.0, 0.0);

		if (i < 6)
		{
			current_rotate = current_rotate - rotate_amount[i];
			float theta = ((float)i) * PI / 5;
			float cornerX = radius * sin(theta);
			track->transform->translate((startX - cornerX), 'x');
			track->transform->rotate(current_rotate, 'z');

			float cornerY = radius * cos(theta);
			track->transform->translate(cornerY, 'y');

			tracks[i] = track;
			std::cout << "track" << i + 1 << " = x:" << startX - cornerX << ", y:" << cornerY << std::endl;
		}
		if (i >= 6  && i < 11)
		{
			actualX = actualX + 0.25;
			track->transform->translate(actualX, 'x');
			track->transform->translate(-radius, 'y');
			tracks[i] = track;
			std::cout << "track" << i + 1 << " = x:" << actualX << ", y:" << -radius << std::endl;
		}

		if (i >= 11 && i < 17)
		{
			current_rotate = current_rotate - rotate_amount[i - 11];
			float theta = ((float)(i - 11)) * PI / 5;
			float cornerX = radius * sin(PI - theta);
			track->transform->translate((endX + cornerX), 'x');
			track->transform->rotate(current_rotate, 'z');
			float cornerY = radius * cos(PI - theta);
			track->transform->translate(cornerY, 'y');
			tracks[i] = track;
			std::cout << "track" << i + 1 << " = x:" << endX + cornerX << ", y:" << cornerY << std::endl;
			if (i == 16)
				actualX = endX + cornerX;
		}
		if (i >= 17)
		{
			actualX = actualX - 0.25;
			track->transform->translate(actualX, 'x');
			track->transform->translate(radius, 'y');
			tracks[i] = track;
			std::cout << "track" << i + 1 << " = x:" << actualX << ", y:" << radius << std::endl;
		}
	}
	this->tracks = tracks;
}

void track::moveForward(GLfloat increment)
{
	unsigned int numberOfTracks = 22;

	float startX = -0.75;
	float steps = 180 / increment;
	float actualX = startX;
	float endX = 0.75;
	float radius = 0.2;
	float rotate_amount[6] = { 0.0, 30.0, 40.0, 40.0, 40.0, 30.0 };
	float current_rotate = 0;

	for (int i = 0; i < numberOfTracks; i++)
	{
		glm::vec3 coords = this->tracks[i]->transform->getCoords();

		if (coords.x <= startX && coords.y != -radius)
		{
			//current_rotate = current_rotate - rotate_amount[i];
			float theta = (float)PI / steps;
			float cornerX = radius * sin(theta);
			this->tracks[i]->transform->translate((startX - cornerX), 'x');
			//this->tracks[i]->transform->rotate(current_rotate, 'z');

			float cornerY = radius * cos(theta);
			this->tracks[i]->transform->translate(cornerY, 'y');
			//std::cout << "track" << i + 1 << " = x:" << startX - cornerX << ", y:" << cornerY << std::endl;
		}
		if (coords.x > startX && coords.y == -radius && coords.x < endX)
		{
			this->tracks[i]->transform->translate(increment, 'x');
			//std::cout << "track" << i + 1 << " = x:" << actualX << ", y:" << -radius << std::endl;
		}
		/*
		if (i >= 11 && i < 17)
		{
			current_rotate = current_rotate - rotate_amount[i - 11];
			float theta = ((float)(i - 11)) * PI / 5;
			float cornerX = radius * sin(PI - theta);
			track->transform->translate((endX + cornerX), 'x');
			track->transform->rotate(current_rotate, 'z');
			float cornerY = radius * cos(PI - theta);
			track->transform->translate(cornerY, 'y');
			tracks[i] = track;
			std::cout << "track" << i + 1 << " = x:" << endX + cornerX << ", y:" << cornerY << std::endl;
			if (i == 16)
				actualX = endX + cornerX;
		}
		if (i >= 17)
		{
			actualX = actualX - 0.25;
			track->transform->translate(actualX, 'x');
			track->transform->translate(radius, 'y');
			tracks[i] = track;
			std::cout << "track" << i + 1 << " = x:" << actualX << ", y:" << radius << std::endl;
		}*/
	}
	this->tracks = tracks;
}

std::vector<cuboid*> track::getTracks()
{
	return this->tracks;
}

