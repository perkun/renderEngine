#include "Camera.h"

#define GLM_FORCE_RADIANS

Camera::Camera(const glm::vec3 pos, glm::vec3 targ, float fov, float aspect,
				float zNear, float zFar)
{
	perspective = glm::perspective(fov, aspect, zNear, zFar);
	position = pos;
// 	forward = glm::vec3(0,0,-1);
	up = glm::vec3(0.,0.,1.);
	target = targ;

	view = glm::lookAt(position, target, up);

}

Camera::Camera(const glm::vec3 pos, glm::vec3 targ, float left, float right,
			   	float bottom, float top, float zNear, float zFar)
{
	perspective = glm::ortho(left, right, bottom, top, zNear, zFar);
	position = pos;
	up = glm::vec3(0.,0.,1.);
	target = targ;

	view = glm::lookAt(position, target, up);

}

void Camera::moveLeft(float step)
{
	/**
	 * przesuniecie kamery w lewo o step
	 */
	forward = glm::normalize(target - position);
	glm::vec3 left_direction = glm::normalize( -glm::cross(forward, up) );

	position += left_direction * step;
	target	 += left_direction * step;
}

void Camera::moveRight(float step)
{
	forward = glm::normalize(target - position);
	glm::vec3 right_direction = glm::normalize( glm::cross(forward, up) );

	position += right_direction * step;
	target	 += right_direction * step;
}

void Camera::moveForward(float step)
{
	forward = glm::normalize(target - position);
	//glm::vec3 left_direction = glm::normalize( -glm::cross(forward, up) );

	position += forward * step;
	target	 += forward * step;
}

void Camera::moveBack(float step)
{
	forward = -glm::normalize(target - position);
	//glm::vec3 left_direction = glm::normalize( -glm::cross(forward, up) );

	position += forward * step;
	target	 += forward * step;
}

void Camera::moveUp(float step)
{
	position += up * step;
	target	 += up * step;
}

void Camera::moveDown(float step)
{
	position -= up * step;
	target	 -= up * step;
}


void Camera::rotateLeft(float step)
{
	forward = glm::normalize(target - position);
	glm::vec3 right_direction = glm::normalize( glm::cross(forward, up) );
	glm::vec3 new_up = glm::normalize(glm::cross(right_direction, forward));

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), step, new_up);

	target += -position;

	glm::vec4 temp = rotation * glm::vec4(target, 1.0);
	target = glm::vec3(temp.x ,temp.y, temp.z);

	target += position;
}

void Camera::rotateRight(float step)
{
	forward = glm::normalize(target - position);
	glm::vec3 right_direction = glm::normalize( glm::cross(forward, up) );
	glm::vec3 new_up = glm::normalize(glm::cross(right_direction, forward));

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), -step, new_up);

	target += -position;

	glm::vec4 temp = rotation * glm::vec4(target, 1.0);
	target = glm::vec3(temp.x ,temp.y, temp.z);

	target += position;
}

void Camera::rotateUp(float step)
{
	forward = glm::normalize(target - position);
	glm::vec3 right_direction = glm::normalize( glm::cross(forward, up) );
// 	glm::vec3 new_up = glm::normalize(glm::cross(right_direction, forward));

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), step, right_direction);

	target += -position;

	glm::vec4 temp = rotation * glm::vec4(target, 1.0);
	target = glm::vec3(temp.x ,temp.y, temp.z);

	target += position;
}

void Camera::rotateDown(float step)
{
	forward = glm::normalize(target - position);
	glm::vec3 right_direction = glm::normalize( glm::cross(forward, up) );
	// 	glm::vec3 new_up = glm::normalize(glm::cross(right_direction, forward));

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), -step, right_direction);

	target += -position;

	glm::vec4 temp = rotation * glm::vec4(target, 1.0);
	target = glm::vec3(temp.x ,temp.y, temp.z);

	target += position;
}

/*void Camera::changeDirection(glm::vec3 cursor_pos)
  {

	float angle;

	angle = glm::length(cursor_pos)/100;

	forward = glm::normalize(target - position);
	glm::vec3 right_direction = glm::normalize( glm::cross(forward, up) );

	glm::vec3 new_up = glm::normalize(glm::cross(right_direction, forward));

	glm::mat4 rotation = glm::rotate(glm::mat4(1.0), angle, new_up);

	target += -position;

	glm::vec4 temp = rotation * glm::vec4(target, 1.0);
	target = glm::vec3(temp.x ,temp.y, temp.z);

	target += position;

}
*/

Camera::~Camera()
{
	//sdasdsad
}

