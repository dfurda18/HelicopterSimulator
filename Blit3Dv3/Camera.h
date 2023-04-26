#pragma once

#include <glm/glm.hpp>
#include "HelicopterNode.h"

/**
* This class represents the camera
*/
class Camera
{
public:
	/**
	* The Camera offset
	*/
	glm::vec3 offset;
	/**
	* The Following Entity
	*/
	HelicopterNode* follow;
	/**
	* The camera view mtrix
	*/
	glm::mat4 viewMatrix;
	/**
	* The follow mode
	*/
	bool followMode;
	/**
	* Creates a new instance of the camera
	* @param follow The entity to follow
	*/
	Camera(HelicopterNode* follow, glm::vec3 offset);
	/**
	* Updates the camera
	*/
	void Update(double seconds);
	/**
	* Returns the Camera position
	*/
	glm::vec3 GetPosition();
};