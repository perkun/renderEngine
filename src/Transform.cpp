/*
 * Transform.cpp
 *
 *  Created on: 16 wrz 2015
 *      Author: grzeslaff
 */

#include "Transform.h"

#define GLM_FORCE_RADIANS

//Transform::Transform() {
//	// TODO Auto-generated constructor stub
//
//}

Transform::~Transform() {
	// TODO Auto-generated destructor stub
}

glm::mat4 Transform::getModelMatrix() {
	glm::mat4 posMatrix = glm::translate(pos);
	glm::mat4 scaleMatrix = glm::scale(scale);
	
	return posMatrix * getRotationMatrix313() * scaleMatrix;
}


void Transform::setPosition(glm::vec3 pos) {
	this->pos = pos;
}

void Transform::setAngles(float alpha, float beta, float gamma, float gamma0)
{
	this->alpha = alpha;
	this->beta = beta;
	this->gamma = gamma;
	this->gamma0 = gamma0;
}

glm::mat4 Transform::getRotationMatrix() {
	glm::mat4 rotXMatrix = glm::rotate(rot.x, glm::vec3(1.0, 0.0, 0.0));
	glm::mat4 rotYMatrix = glm::rotate(rot.y, glm::vec3(0.0, 1.0, 0.0));
	glm::mat4 rotZMatrix = glm::rotate(rot.z, glm::vec3(0.0, 0.0, 1.0));
	
	return rotZMatrix * rotYMatrix * rotXMatrix;
}

glm::mat4 Transform::getRotationMatrix313() {
	return 
		glm::rotate(alpha, glm::vec3(0.0, 0.0, 1.0)) *
		glm::rotate(beta, glm::vec3(1.0, 0.0, 0.0)) *
		glm::rotate(gamma, glm::vec3(0.0, 0.0, 1.0));

}















