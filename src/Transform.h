/*
 * Transform.h
 *
 *  Created on: 16 wrz 2015
 *      Author: grzeslaff
 */

#ifndef TRANSFORM_H_
#define TRANSFORM_H_

#define GLM_FORCE_RADIANS

#include <glm/gtx/transform.hpp>

class Transform {
public:
//	Transform();
	Transform(const glm::vec3& pos = glm::vec3(), const glm::vec3& rot = glm::vec3(), const glm::vec3& scale = glm::vec3(1.0, 1.0, 1.0) ) :
		pos(pos),
		rot(rot),
		scale(scale) {}
		
	Transform(const glm::vec3& pos, float alpha, float beta, float gamma, float gamma0 ):
		pos(pos),
		alpha(alpha),
		gamma(gamma),
		gamma0(gamma0) {}
		

	
	glm::mat4 getModelMatrix();

	void setPosition(glm::vec3 pos);
	void setAngles(float alpha, float beta, float gamma, float gamma0);
	
	glm::mat4 getRotationMatrix();
	glm::mat4 getRotationMatrix313();
	
	
	virtual ~Transform();

	float alpha, beta, gamma, gamma0;
	glm::vec3 pos;
	glm::vec3 rot;
	glm::vec3 scale;
	
	glm::mat4 rotX, rotY, rotZ;
	
	
	
};

#endif /* TRANSFORM_H_ */
