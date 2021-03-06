#include "Camera.h"
#include "MathHelper.h"


Camera::Camera()
	: position(0, 0, 0),
	direction(0, 0, -1),
	up(0, 1, 0),
	fov(45),
	focalLength(1.0f),
	viewDistance(_INFINITY)
{
}


Camera::~Camera()
{
}

//Calculates camera properties based on the aspect ratio of the image
void Camera::calculate(float aspectRatio)
{
	up = glm::normalize(up);
	direction = glm::normalize(direction);

	//convert fov to radians
	float fovRad = fov*3.1415f / 180.0f;

	float thfov = tan(fovRad / 2);

	right = glm::cross(direction, up);
	right = glm::normalize(right);

	right *= thfov * aspectRatio;
	up *= thfov;
}
