#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#define GLM_FORCE_RADIANS
class Camera
{
public:
	Camera() {}
	Camera(const glm::vec3 pos, glm::vec3 targ, float fov, float aspect, 
					float zNear, float zFar);
	Camera(const glm::vec3 pos, glm::vec3 targ, float left, float right, 
					float bottom, float top, float zNear, float zFar);

// 	glm::mat4 getProjectionMatrix();
// 	glm::mat4 getViewMatrix();




	inline glm::mat4 getProjectionMatrix() const
	{
		return perspective;
	}

	inline glm::mat4 getViewMatrix() const
	{
		return glm::lookAt(position, target, up);
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

	void resetView() {
		position = glm::vec3(0, -1, 0);
		target = glm::vec3(0, 0, 0);
		up = glm::vec3(0, 0, 1);
	}
// 	void changeDirection(glm::vec3 cursor_pos);


    ~Camera();
	glm::vec3 position;
	glm::vec3 target;
	glm::vec3 up;
private:
	glm::mat4 perspective, view;
	glm::vec3 forward;

};

#endif // CAMERA_H
