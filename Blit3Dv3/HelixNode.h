#pragma once

#include "SceneManager.h"
#include "HelicopterNode.h"
#include "Mesh.h"

/**
* This class Represents a HelixNode from the Helicopter.
* @author Dario Urdapilleta
* @version 1.0
* @since 03/13/2023
*/
class HelixNode : public SceneNode
{
public:
	/**
	* Power factor
	*/
	const float POWER_FACTOR = 400.f;
	/**
	* The Mesh Manager.
	*/
	MeshManager* mManager = NULL;
	/**
	* A pointer to the HelicopterNode.
	*/
	HelicopterNode* helicopter;
	/**
	* The main helix mesh file.
	*/
	std::string meshFile;
	/**
	* The required traslation
	*/
	glm::vec3 traslation;
	/**
	* The spinning axis
	*/
	glm::vec3 axis;
	/**
	* The default HelixNode constructor.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	HelixNode();
	/**
	* The HelixNode constructor.
	* @param ParentNode The Parent node the node is instanced from.
	* @param meshFile The node's mesh file.
	* @param traslation The vector with the distance needed to place the node to its proper location.
	* @param axis The spinning axis
	* @param meshManager The Mesh Manager.
	* @param helicopter The helicopter node that owns this helix
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	HelixNode(SceneNode* ParentNode, std::string meshFile, glm::vec3 traslation, glm::vec3 axis, MeshManager* meshManager, HelicopterNode* helicopter);
	/**
	* Updates the Node.
	* @param seconds The amount of time passed since the last update.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void Update(double seconds);
	/**
	* Creates all the Node's children nodes.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	void CreateChildren();
	/**
	* Returns the Helicopter's gas.
	* @return The Helicopter's gas.
	* @author Dario Urdapilleta
	* @since 03/13/2023
	*/
	float GetSpeed();
};