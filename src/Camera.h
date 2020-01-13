#ifndef CAMERA_H
#define CAMERA_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <stdio.h>
#define GLM_FORCE_RADIANS

using namespace std;

class Camera
{
public:
	Camera() {}
	Camera(const glm::vec3 pos, glm::vec3 targ, float fov, float aspect,
					float zNear, float zFar, bool earth_tilt);
	Camera(const glm::vec3 pos,  glm::vec3 targ, float left, float right,
		float bottom, float top, float zNear, float zFar, bool earth_tilt);
    ~Camera();


	inline glm::mat4 getProjectionMatrix() const
	{
		return perspective;
	}

	inline glm::mat4 getViewMatrix() const
	{
		return glm::lookAt(position, target, up);
	}

	inline glm::mat4 getNoTranslationViewMatrix() const
	{
		glm::mat4 viewMatrix = getViewMatrix();
		viewMatrix[3] = glm::vec4(0, 0, 0, 1); //  => brak translacji
		return viewMatrix;
	}

	void moveLeft(float step);
	void moveRight(float step);
	void moveForward(float step);
	void moveBack(float step);
	void moveUp(float step);
	void moveDown(float step);
	void rotateLeft(float step);
	void rotateRight(float step);
	void rotateUp(float step);
	void rotateDown(float step);

	void fixedRotateRight(float step);
	void fixedRotateLeft(float step);
	void fixedRotateUp(float step);
	void fixedRotateDown(float step);
	void zoomIn(float step);
	void zoomOut(float step);

	void tiltLeft(float step);
	void tiltRight(float step);
	void setTilt(float tilt);

	void resetView() {
		position = glm::vec3(0, -1, 0);
		target = glm::vec3(0, 0, 0);
		up = glm::vec3(0, 0, 1);
	}

	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;


private:
	glm::mat4 perspective, view;
	glm::vec3 forward;
};

#endif // CAMERA_H
